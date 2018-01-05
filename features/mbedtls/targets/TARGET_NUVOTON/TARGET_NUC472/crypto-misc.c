/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include <limits.h>
#include "nu_modutil.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

/* Track if AES H/W is available */
static uint16_t crypto_aes_avail = 1;
/* Track if DES H/W is available */
static uint16_t crypto_des_avail = 1;
/* Track if SHA H/W is available */
static uint16_t crypto_sha_avail = 1;

/* Crypto (AES, DES, SHA, etc.) init counter. Crypto's keeps active as it is non-zero. */
static uint16_t crypto_init_counter = 0U;

static bool crypto_submodule_acquire(uint16_t *submodule_avail);
static void crypto_submodule_release(uint16_t *submodule_avail);

/* As crypto init counter changes from 0 to 1:
 *
 * 1. Enable crypto clock
 * 2. Enable crypto interrupt
 */
void crypto_init(void)
{
    core_util_critical_section_enter();
    if (crypto_init_counter == USHRT_MAX) {
        core_util_critical_section_exit();
        error("Crypto clock enable counter would overflow (> USHRT_MAX)");
    }
    core_util_atomic_incr_u16(&crypto_init_counter, 1);
    if (crypto_init_counter == 1) {
        SYS_UnlockReg();    // Unlock protected register
        CLK_EnableModuleClock(CRPT_MODULE);
        SYS_LockReg();      // Lock protected register
        
        NVIC_EnableIRQ(CRPT_IRQn);
    }
    core_util_critical_section_exit();
}

/* As crypto init counter changes from 1 to 0:
 *
 * 1. Disable crypto interrupt 
 * 2. Disable crypto clock
 */
void crypto_uninit(void)
{
    core_util_critical_section_enter();
    if (crypto_init_counter == 0) {
        core_util_critical_section_exit();
        error("Crypto clock enable counter would underflow (< 0)");
    }
    core_util_atomic_decr_u16(&crypto_init_counter, 1);
    if (crypto_init_counter == 0) {
        NVIC_DisableIRQ(CRPT_IRQn);
        
        SYS_UnlockReg();    // Unlock protected register
        CLK_DisableModuleClock(CRPT_MODULE);
        SYS_LockReg();      // Lock protected register
    }
    core_util_critical_section_exit();
}

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char*) v;
    while (n--) {
        *p++ = 0;
    }
}

bool crypto_aes_acquire(void)
{
    return crypto_submodule_acquire(&crypto_aes_avail);
}

void crypto_aes_release(void)
{
    crypto_submodule_release(&crypto_aes_avail);
}

bool crypto_des_acquire(void)
{
    return crypto_submodule_acquire(&crypto_des_avail);
}

void crypto_des_release(void)
{
    crypto_submodule_release(&crypto_des_avail);
}

bool crypto_sha_acquire(void)
{
    return crypto_submodule_acquire(&crypto_sha_avail);
}

void crypto_sha_release(void)
{
    crypto_submodule_release(&crypto_sha_avail);
}

static bool crypto_submodule_acquire(uint16_t *submodule_avail)
{
    uint16_t expectedCurrentValue = 1;
    return core_util_atomic_cas_u16(submodule_avail, &expectedCurrentValue, 0);
}

static void crypto_submodule_release(uint16_t *submodule_avail)
{
    uint16_t expectedCurrentValue = 0;
    while (! core_util_atomic_cas_u16(submodule_avail, &expectedCurrentValue, 1));
}

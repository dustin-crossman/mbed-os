/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/***************************************************************************//**
* \copyright
*
* (c) 2018-2019, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
*
* This software is a port of the open source MCUBoot project.
* This file was modified to fit Cypress Secure Bootloader design.
*
* Portions of this software, including source code, documentation and related
* materials ("Software"), are owned by Cypress Semiconductor
* Corporation or one of its subsidiaries ("Cypress") and is protected by
* and subject to worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-
* exclusive, non-transferable license to copy, modify, and compile the
* Software source code solely for use in connection with Cypress's
* integrated circuit products. Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. Cypress reserves the right to make
* changes to the Software without notice. Cypress does not assume any
* liability arising out of the application or use of the Software or any
* product or circuit described in the Software. Cypress does not
* authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*
********************************************************************************/

/*
 * This module provides a thin abstraction over some of the crypto
 * primitives to make it easier to swap out the used crypto library.
 *
 * At this point, there are two choices: MCUBOOT_USE_MBEDCRYPTO, or
 * MCUBOOT_USE_TINYCRYPT.  It is a compile error there is not exactly
 * one of these defined.
 */

#ifndef __BOOTUTIL_CRYPTO_H_
#define __BOOTUTIL_CRYPTO_H_

#include "mcuboot_config/mcuboot_config.h"

#if defined(MCUBOOT_USE_MBEDCRYPTO) && defined(MCUBOOT_USE_TINYCRYPT)
    #error "Cannot define both MBED_TLS and TINYCRYPT"
#endif

#if !defined(MCUBOOT_USE_MBEDCRYPTO) && !defined(MCUBOOT_USE_TINYCRYPT)
    #error "One of MBED_TLS or TINYCRYPT must be defined"
#endif

#ifdef MCUBOOT_USE_MBEDCRYPTO
    #include <mbedtls/sha256.h>
	#include "flashboot_psacrypto.h"
    #include "psa/crypto.h"
#endif /* MCUBOOT_USE_MBEDCRYPTO */

#ifdef MCUBOOT_USE_TINYCRYPT
    #include <tinycrypt/sha256.h>
#endif /* MCUBOOT_USE_TINYCRYPT */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MCUBOOT_USE_MBEDCRYPTO
typedef mbedtls_sha256_context bootutil_sha256_context;

static inline psa_status_t bootutil_sha256_init(fb_psa_hash_operation_t *psa_op)
{
	psa_status_t psa_ret;

    psa_ret = fb_psa_hash_setup (psa_op, PSA_ALG_SHA_256);

    return psa_ret;
}

static inline psa_status_t bootutil_sha256_update(fb_psa_hash_operation_t *psa_op,
                                          const void *data,
                                          uint32_t data_len)
{
	psa_status_t psa_ret;

	psa_ret = fb_psa_hash_update(psa_op, data, data_len);

    return psa_ret;
}

static inline psa_status_t bootutil_sha256_finish(fb_psa_hash_operation_t *psa_op,
                                          uint8_t *output)
{
	psa_status_t psa_ret;
	int hash_length;

	psa_ret = fb_psa_hash_finish(psa_op, output, PSA_HASH_SIZE(PSA_ALG_SHA_256), (size_t*)&hash_length);
    
    return psa_ret;
}
#endif /* MCUBOOT_USE_MBEDCRYPTO */

#ifdef MCUBOOT_USE_TINYCRYPT
typedef struct tc_sha256_state_struct bootutil_sha256_context;
static inline void bootutil_sha256_init(bootutil_sha256_context *ctx)
{
    tc_sha256_init(ctx);
}

static inline void bootutil_sha256_update(bootutil_sha256_context *ctx,
                                          const void *data,
                                          uint32_t data_len)
{
    tc_sha256_update(ctx, data, data_len);
}

static inline void bootutil_sha256_finish(bootutil_sha256_context *ctx,
                                          uint8_t *output)
{
    tc_sha256_final(output, ctx);
}
#endif /* MCUBOOT_USE_TINYCRYPT */

#ifdef __cplusplus
}
#endif

#endif /* __BOOTUTIL_SIGN_KEY_H_ */

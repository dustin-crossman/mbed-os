/*
 * mbed Microcontroller Library
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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

/*  Hardware entropy collector for the PSoC6A families */

#if DEVICE_TRNG

#include "trng_api.h"
#include "cy_crypto_core_trng.h"
#include "cy_crypto_core_hw.h"

/* Initialization polynomial values fro True Random Generator */
#define GARO31_INITSTATE          (0x04c11db7u)
#define FIRO31_INITSTATE          (0x04c11db7u)

#define MAX_TRNG_BIT_SIZE         (32UL)

void trng_init(trng_t *obj)
{
    (void)obj;

    if (!Cy_Crypto_Core_IsEnabled(CRYPTO))
    {
        (void)Cy_Crypto_Core_Enable(CRYPTO);
    }
}

void trng_free(trng_t *obj)
{
    /* Deinitialization is not needed for the driver */
    (void)obj;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret = 0;
    uint32_t random;
    *output_length = 0;

    (void)obj;

    /* Get Random byte */
    while ((*output_length < length) && (ret == 0))
    {
        if (Cy_Crypto_Core_Trng(CRYPTO, GARO31_INITSTATE, FIRO31_INITSTATE, MAX_TRNG_BIT_SIZE, &random) != CY_CRYPTO_SUCCESS) {
            ret = -1;
        }
        else
        {
            for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++)
            {
                *output++ = ((uint8_t*)&random)[i];
                *output_length += 1;
            }
        }
    }

    return (ret);
}

#endif

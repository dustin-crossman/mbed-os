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

/**
 * \file    crypto_common.c
 *
 * \brief   Source file for common mbedtls acceleration functions
 *
 */

#include <string.h>

#include "psoc6_utils.h"

#include "crypto_common.h"
#include "cy_crypto_common.h"
#include "cy_crypto_core_hw.h"

int cy_hw_sha_start(uint32_t *currBlockLen, cy_stc_crypto_sha_state_t *hashState, cy_en_crypto_sha_mode_t shaMode, void *shaBuffers)
{
    *currBlockLen = 0;

    if (cy_reserve_crypto(CY_CRYPTO_COMMON_HW) != 0)
    {
        return (-1);
    }

    Cy_Crypto_Core_Sha_Init (CRYPTO, hashState, shaMode, shaBuffers);
    Cy_Crypto_Core_Sha_Start(CRYPTO, hashState);

    return (0);
}

int cy_hw_sha_update(uint32_t *currBlockLen,
                     cy_stc_crypto_sha_state_t *hashState,
                     const uint8_t *in, uint32_t inlen,
                     uint8_t *shaTempBlock)
{
    uint32_t tmpBlockSize;
	uint32_t shaUpdateLen;
    uint32_t n;

    tmpBlockSize = hashState->blockSize;

    while (inlen > 0)
    {
        if (*currBlockLen == 0 && inlen >= tmpBlockSize)
        {
        	shaUpdateLen = (inlen / tmpBlockSize) * tmpBlockSize;

            Cy_Crypto_Core_Sha_Update(CRYPTO, hashState, in, shaUpdateLen);

            inlen -= shaUpdateLen;
            in += shaUpdateLen;
        } else {
            n = inlen;

            if (n > (tmpBlockSize - *currBlockLen))
            {
                n = (tmpBlockSize - *currBlockLen);
            }

            memcpy(shaTempBlock + *currBlockLen, in, n);

    	    *currBlockLen += n;
            in += n;
            inlen -= n;

            if (*currBlockLen == tmpBlockSize)
            {
                Cy_Crypto_Core_Sha_Update(CRYPTO, hashState, shaTempBlock, tmpBlockSize);

                *currBlockLen = 0;
            }
        }
    }

    return (0);
}

int cy_hw_sha_finish(cy_stc_crypto_sha_state_t *hashState,
                     uint8_t *output,
                     uint8_t *shaTempBlock, uint32_t currBlockLen)
{
    Cy_Crypto_Core_Sha_Update(CRYPTO, hashState, shaTempBlock, currBlockLen);
    Cy_Crypto_Core_Sha_Finish(CRYPTO, hashState, output);
    Cy_Crypto_Core_Sha_Free  (CRYPTO, hashState);

    cy_free_crypto(CY_CRYPTO_COMMON_HW);

    return (0);
}

int cy_hw_sha_process(cy_stc_crypto_sha_state_t *hashState, const uint8_t *in)
{
	Cy_Crypto_Core_Sha_Update(CRYPTO, hashState, in, hashState->blockSize);

    return (0);
}

cy_en_crypto_ecc_curve_id_t get_dp_idx(mbedtls_ecp_group_id gid)
{
    cy_en_crypto_ecc_curve_id_t dp_idx;

    switch( gid )
    {
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP192R1:
            dp_idx = CY_CRYPTO_ECC_ECP_SECP192R1;
            break;
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP224R1:
            dp_idx = CY_CRYPTO_ECC_ECP_SECP224R1;
            break;
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP256R1:
            dp_idx = CY_CRYPTO_ECC_ECP_SECP256R1;
            break;
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP384R1:
            dp_idx = CY_CRYPTO_ECC_ECP_SECP384R1;
            break;
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP521R1:
            dp_idx = CY_CRYPTO_ECC_ECP_SECP521R1;
            break;
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */

        default:
            dp_idx = CY_CRYPTO_ECC_ECP_NONE;
            break;
    }

    return dp_idx;
}


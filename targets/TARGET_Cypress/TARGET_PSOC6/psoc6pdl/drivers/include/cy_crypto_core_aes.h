/***************************************************************************//**
* \file cy_crypto_core_aes.h
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the AES method
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_AES_H)
#define CY_CRYPTO_CORE_AES_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_AES == 1)

#include "cy_crypto_core_aes_v1.h"
#include "cy_crypto_core_aes_v2.h"

typedef cy_en_crypto_status_t (*cy_crypto_aes_init_func_t)(CRYPTO_Type *base,
                                                 uint8_t const *key,
                                                 cy_en_crypto_aes_key_length_t keyLength,
                                                 cy_stc_crypto_aes_state_t *aesState);

typedef cy_en_crypto_status_t (*cy_crypto_aes_ecb_func_t)(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState);

typedef cy_en_crypto_status_t (*cy_crypto_aes_cbc_func_t)(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint32_t srcSize,
                                                uint8_t const *ivPtr,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState);

typedef cy_en_crypto_status_t (*cy_crypto_aes_cfb_func_t)(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint32_t srcSize,
                                                uint8_t const *ivPtr,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState);

typedef cy_en_crypto_status_t (*cy_crypto_aes_ctr_func_t)(CRYPTO_Type *base,
                                                uint32_t srcSize,
                                                uint32_t *srcOffset,
                                                uint8_t *ivPtr,
                                                uint8_t *streamBlock,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState);

/**
* \addtogroup group_crypto_lld_symmetric_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Init
****************************************************************************//**
*
* Initializes AES mode of operation and prepares the inverse key.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param key
* The pointer to the encryption/decryption key.
*
* \param keyLength
* \ref cy_en_crypto_aes_key_length_t
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Aes_Init(CRYPTO_Type *base,
                                                 uint8_t const *key,
                                                 cy_en_crypto_aes_key_length_t keyLength,
                                                 cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t tmpResult;

    if (cy_device->cryptoVersion == 1u)
    {
        tmpResult = Cy_Crypto_Core_V1_Aes_Init(base, key, keyLength, aesState);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Aes_Init(base, key, keyLength, aesState);
    }

    return tmpResult;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Ecb
****************************************************************************//**
*
* Performs AES operation on a single block.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t).
*
* \param dst
* The pointer to a destination cipher block.
*
* \param src
* The pointer to a source block.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ecb(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t tmpResult;

    if (cy_device->cryptoVersion == 1u)
    {
        tmpResult = Cy_Crypto_Core_V1_Aes_Ecb(base, dirMode, dst, src, aesState);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Aes_Ecb(base, dirMode, dst, src, aesState);
    }

    return tmpResult;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Cbc
****************************************************************************//**
*
* Performs AES-CBC operation defined in the dirMode parameter on a plain text
* defined in the src parameter.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cbc(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint32_t srcSize,
                                                uint8_t const *ivPtr,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t tmpResult;

    if (cy_device->cryptoVersion == 1u)
    {
        tmpResult = Cy_Crypto_Core_V1_Aes_Cbc(base, dirMode, srcSize, ivPtr, dst, src, aesState);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Aes_Cbc(base, dirMode, srcSize, ivPtr, dst, src, aesState);
    }

    return tmpResult;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Cfb
****************************************************************************//**
*
* Performs AES-CFB operation defined in the dirMode parameter on a plain text
* defined in the src parameter.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cfb(CRYPTO_Type *base,
                                                cy_en_crypto_dir_mode_t dirMode,
                                                uint32_t srcSize,
                                                uint8_t const *ivPtr,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t tmpResult;

    if (cy_device->cryptoVersion == 1u)
    {
        tmpResult = Cy_Crypto_Core_V1_Aes_Cfb(base, dirMode, srcSize, ivPtr, dst, src, aesState);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Aes_Cfb(base, dirMode, srcSize, ivPtr, dst, src, aesState);
    }

    return tmpResult;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Ctr
****************************************************************************//**
*
* Performs AES-CTR operation on a plain text defined in the src parameter.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param srcSize
* The size of a source plain text.
*
* \param srcOffset
* The size of an offset within the current block stream for resuming within the
* current cipher stream.
*
* \param ivPtr
* The 128-bit nonce and counter.
*
* \param streamBlock
* The saved stream-block for resuming. Is over-written by the function.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text. Must be 4-Byte aligned.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ctr(CRYPTO_Type *base,
                                                uint32_t srcSize,
                                                uint32_t *srcOffset,
                                                uint8_t *ivPtr,
                                                uint8_t *streamBlock,
                                                uint8_t *dst,
                                                uint8_t const *src,
                                                cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t tmpResult;

    if (cy_device->cryptoVersion == 1u)
    {
        tmpResult = Cy_Crypto_Core_V1_Aes_Ctr(base, srcSize, srcOffset, ivPtr, streamBlock, dst, src, aesState);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Aes_Ctr(base, srcSize, srcOffset, ivPtr, streamBlock, dst, src, aesState);
    }

    return tmpResult;
}

/** \} group_crypto_lld_symmetric_functions */

#endif /* #if (CPUSS_CRYPTO_AES == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_AES_H) */


/* [] END OF FILE */


/***************************************************************************//**
* \file fb_cy_crypto_common.h
* \version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* \brief
*  This file provides common constants and parameters
*  for the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(FB_CY_CRYPTO_COMMON_H)
#define FB_CY_CRYPTO_COMMON_H

/** The key length options for the AES method. */
typedef enum
{
    FB_CY_CRYPTO_KEY_AES_128   = 0x00u,   /**< The AES key size is 128 bits */
    FB_CY_CRYPTO_KEY_AES_192   = 0x01u,   /**< The AES key size is 192 bits */
    FB_CY_CRYPTO_KEY_AES_256   = 0x02u    /**< The AES key size is 256 bits */
} fb_cy_en_crypto_aes_key_length_t;

/** Structure for storing the AES state */
typedef struct
{
    /** Pointer to AES key */
    uint8_t *key;
    /** Pointer to AES inversed key */
    uint8_t *invKey;
    /** AES key length */
    fb_cy_en_crypto_aes_key_length_t keyLength;
    /** Pointer to AES work buffers */
    uint32_t *buffers;
    /** AES processed block index (for CMAC, SHA operations) */
    uint32_t blockIdx;
} fb_cy_stc_crypto_aes_state_t;

/* The structure for storing the SHA context */
typedef struct
{
    uint32_t mode;
    uint8_t *block;
    uint32_t blockSize;
    uint8_t *hash;
    uint32_t hashSize;
    uint8_t *roundMem;
    uint32_t roundMemSize;
    uint32_t messageSize;
    uint32_t digestSize;
    uint32_t blockIdx;
    uint8_t  const *initialHash;
} fb_cy_stc_crypto_sha_state_t;

#endif /* #if !defined(FB_CY_CRYPTO_COMMON_H) */

/* [] END OF FILE */

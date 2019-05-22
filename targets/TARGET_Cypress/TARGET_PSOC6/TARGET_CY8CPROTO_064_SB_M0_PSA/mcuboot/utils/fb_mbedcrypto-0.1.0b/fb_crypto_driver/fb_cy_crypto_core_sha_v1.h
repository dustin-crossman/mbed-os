/***************************************************************************//**
* \file fb_cy_crypto_core_sha.h
* \version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the SHA method in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(FB_CY_CRYPTO_CORE_SHA_V1_H)
#define FB_CY_CRYPTO_CORE_SHA_V1_H

#include "cy_crypto_common.h"
#include "cy_syslib.h"

/** \cond INTERNAL */


typedef struct
{
    /* Allocate CRYPTO_MAX_BLOCK_SIZE Bytes for block */
    uint32_t block[CY_CRYPTO_SHA256_BLOCK_SIZE / 4u];

    /* Allocate CRYPTO_MAX_HASH_SIZE Bytes for hash */
    uint32_t hash[CY_CRYPTO_SHA256_HASH_SIZE / 4u];

    /* Allocate CRYPTO_MAX_ROUND_MEM_SIZE Bytes for roundMem */
    uint32_t roundMem[CY_CRYPTO_SHA256_ROUND_MEM_SIZE / 4u];
} fb_cy_stc_crypto_v1_sha256_buffers_t;

#endif /* #if !defined(FB_CY_CRYPTO_CORE_SHA_V1_H) */


/* [] END OF FILE */

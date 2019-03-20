/***************************************************************************//**
* \file fb_cy_crypto_core_aes_v1.h
* \version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* \brief
*  This file provides constant and parameters for the API for the AES method
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(FB_CY_CRYPTO_CORE_AES_V1_H)
#define FB_CY_CRYPTO_CORE_AES_V1_H

#include "cy_crypto_common.h"

/* The structure to define used memory buffers */
typedef struct
{
    uint32_t key[CY_CRYPTO_AES_256_KEY_SIZE_U32];
    uint32_t keyInv[CY_CRYPTO_AES_256_KEY_SIZE_U32];
    uint32_t block0[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    uint32_t block1[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    uint32_t block2[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    uint32_t iv[CY_CRYPTO_AES_BLOCK_SIZE_U32];
} fb_cy_stc_crypto_aes_buffers_t;

#endif /* #if !defined(FB_CY_CRYPTO_CORE_AES_V1_H) */


/* [] END OF FILE */

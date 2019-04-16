/*******************************************************************************
* File Name: cy_hal_crc_common.c
*
* Description:
* Provides a high level interface for interacting with the Cypress CRC. This is
* a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cy_hal_crypto_common.h"

CRYPTO_Type* CY_CRYPTO_BASE_ADDRESSES[CY_CRC_INST_COUNT] =
{
#ifdef CRYPTO
    CRYPTO,
#endif
};
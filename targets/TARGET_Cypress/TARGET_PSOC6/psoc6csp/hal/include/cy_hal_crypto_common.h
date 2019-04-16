/***************************************************************************//**
* \file cy_hal_crypto_common.h
*
* Description:
* This file provides common defines and addresses required by drivers using the 
* Crypto block.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_device.h"
#include "cy_pdl.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Block count for CRYPTO blocks */
#if defined(CY_IP_MXCRYPTO_INSTANCES)
    #define CY_CRC_INST_COUNT      CY_IP_MXCRYPTO_INSTANCES
#elif defined(CPUSS_CRYPTO_PRESENT)
    #define CY_CRC_INST_COUNT      1u
#else
    #define CY_CRC_INST_COUNT      0u
#endif

/** The start address of the CRYPTO blocks */
extern CRYPTO_Type* CY_CRYPTO_BASE_ADDRESSES[CY_CRC_INST_COUNT];

#if defined(__cplusplus)
}
#endif
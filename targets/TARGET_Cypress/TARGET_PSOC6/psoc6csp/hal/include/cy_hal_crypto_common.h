/***************************************************************************//**
* \file cy_hal_crypto_common.h
*
* Description:
* This file provides common defines and addresses required by drivers using the 
* Crypto block.
* 
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

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

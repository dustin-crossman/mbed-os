/***************************************************************************//**
* \file cy_crypto_core_hmac.h
* \version 2.20
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the HMAC method in the Crypto block driver.
*
********************************************************************************
* Copyright 2016-2018 Cypress Semiconductor Corporation
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


#if !defined(CY_CRYPTO_CORE_HMAC_H)
#define CY_CRYPTO_CORE_HMAC_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_SHA == 1)

#include "cy_crypto_core_hmac_v1.h"
#include "cy_crypto_core_hmac_v2.h"

typedef cy_en_crypto_status_t (*cy_crypto_hmac_func_t)(CRYPTO_Type *base,
                                          uint8_t *hmac,
                                          uint8_t  const *message,
                                          uint32_t messageSize,
                                          uint8_t  const *key,
                                          uint32_t keyLength,
                                          cy_en_crypto_sha_mode_t mode);


#endif /* #if (CPUSS_CRYPTO_SHA == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_HMAC_H) */


/* [] END OF FILE */

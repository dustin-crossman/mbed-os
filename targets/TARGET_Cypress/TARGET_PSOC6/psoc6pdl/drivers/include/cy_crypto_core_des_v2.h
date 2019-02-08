/***************************************************************************//**
* \file cy_crypto_core_des_v2.h
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the DES method
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


#if !defined(CY_CRYPTO_CORE_DES_V2_H)
#define CY_CRYPTO_CORE_DES_V2_H

#include "cy_crypto_common.h"
#include "cy_syslib.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_DES == 1)

cy_en_crypto_status_t Cy_Crypto_Core_V2_Des(CRYPTO_Type *base,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint8_t const *key,
                                        uint8_t *dst,
                                        uint8_t const *src);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Tdes(CRYPTO_Type *base,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint8_t const *key,
                                        uint8_t *dst,
                                        uint8_t const *src);


#endif /* #if (CPUSS_CRYPTO_DES == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_DES_V2_H) */


/* [] END OF FILE */


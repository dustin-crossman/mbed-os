/***************************************************************************//**
* \file cy_hal_trng_impl.h
*
* \brief
* Provides an implementation of the Cypress TRNG HAL API.
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

#include "cy_hal_trng.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* Initialization polynomial values for True Random Number Generator */
#define GARO31_INITSTATE          (0x04c11db7UL)
#define FIRO31_INITSTATE          (0x04c11db7UL)

#define MAX_TRNG_BIT_SIZE         (32UL)

__STATIC_INLINE cy_rslt_t cy_trng_generate(const cy_trng_t *obj, uint32_t *value)
{
    if(NULL == obj || NULL == value)
        return CY_RSLT_CSP_ERR_TRNG_BAD_ARGUMENT;

    cy_en_crypto_status_t status = Cy_Crypto_Core_Trng(obj->base, GARO31_INITSTATE, FIRO31_INITSTATE, MAX_TRNG_BIT_SIZE, value);

    return (CY_CRYPTO_SUCCESS == status) ? CY_RSLT_SUCCESS : CY_RSLT_CSP_ERR_TRNG_HW_ERROR;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

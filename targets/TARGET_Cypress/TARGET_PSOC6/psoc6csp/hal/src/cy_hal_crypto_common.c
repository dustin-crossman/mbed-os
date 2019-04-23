/*******************************************************************************
* File Name: cy_hal_crypto_common.c
*
* Description:
* Provides a high level interface for interacting with the Cypress CRC. This is
* a wrapper around the lower level PDL API.
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

#include "cy_hal_hwmgr.h"
#include "cy_crypto_core_crc.h"
#include "cy_hal_crypto_common.h"

CRYPTO_Type* CY_HAL_CRYPTO_BASE_ADDRESSES[CY_HAL_CRYPTO_INST_COUNT] =
{
#ifdef CRYPTO
    CRYPTO,
#endif
};

cy_rslt_t cy_crypto_reserve(CRYPTO_Type** base, cy_resource_inst_t *resource)
{
    cy_rslt_t result = CY_RSLT_ERR_CSP_HWMGR_NONE_FREE;
    for(uint32_t i = 0u; i < CY_HAL_CRYPTO_INST_COUNT; i++)
    {
        resource->block_num = i;
        result = cy_hwmgr_reserve(resource);
        if(result == CY_RSLT_SUCCESS)
        {
            *base = CY_HAL_CRYPTO_BASE_ADDRESSES[i];
            Cy_Crypto_Core_Enable(*base);
            result = cy_hwmgr_set_configured(resource->type, resource->block_num, resource->channel_num);
            break;
        }
    }
    if (result != CY_RSLT_SUCCESS)
    {
        cy_crypto_free(*base, resource);
    }
    return result;
}

cy_rslt_t cy_crypto_free(CRYPTO_Type* base, const cy_resource_inst_t *resource)
{
    cy_rslt_t result = CY_RSLT_SUCCESS; 
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    if(Cy_Crypto_Core_IsEnabled(base))
        Cy_Crypto_Core_Disable(base);
    result = cy_hwmgr_set_unconfigured(resource->type, resource->block_num, resource->channel_num);
    ret = result;
    result = cy_hwmgr_free(resource);
    if(ret == CY_RSLT_SUCCESS)
        ret = result;
    return ret;
}

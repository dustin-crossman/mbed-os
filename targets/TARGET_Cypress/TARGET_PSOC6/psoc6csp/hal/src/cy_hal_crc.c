/*******************************************************************************
* File Name: cy_hal_crc.c
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
#include "cy_hal_crypto_common.h"
#include "cy_hal_crc_impl.h"
#include "cy_crypto_core_crc.h"

/*******************************************************************************
*       Functions
*******************************************************************************/
cy_rslt_t cy_crc_init(cy_crc_t *obj)
{
    if(NULL == obj)
        return CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT;
    
    memset(obj, 0, sizeof(cy_crc_t));
    cy_rslt_t result = CY_RSLT_ERR_CSP_HWMGR_NONE_FREE;
    obj->resource.type = CY_RSC_CRC;
    for(uint32_t i = 0u; i < CY_CRC_INST_COUNT; i++)
    {
        obj->resource.block_num = i;
        result = cy_hwmgr_reserve(&(obj->resource));
        if(result == CY_RSLT_SUCCESS)
        {
            obj->base = CY_CRYPTO_BASE_ADDRESSES[i];
            Cy_Crypto_Core_Enable(obj->base);
            result = cy_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
            break;
        }
    }
    if (result != CY_RSLT_SUCCESS)
    {
        cy_crc_free(obj);
    }
    return result;
}

cy_rslt_t cy_crc_free(cy_crc_t *obj)
{
    if(NULL == obj)
        return CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT;

    cy_rslt_t result = CY_RSLT_SUCCESS; 
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    if (obj->resource.type != CY_RSC_INVALID)
    {
        if(obj->base != NULL)
            Cy_Crypto_Core_Disable(obj->base);
        result = cy_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        ret = result;
        result = cy_hwmgr_free(&(obj->resource));
        if(ret == CY_RSLT_SUCCESS)
            ret = result;
        if(ret == CY_RSLT_SUCCESS)
            memset(obj, 0, sizeof(cy_crc_t));
    }
    return ret;
}

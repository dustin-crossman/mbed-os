/*******************************************************************************
* File Name: cy_hal_crc.c
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
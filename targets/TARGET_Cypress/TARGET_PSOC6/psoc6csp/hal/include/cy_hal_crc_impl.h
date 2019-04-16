/***************************************************************************//**
* \file cy_hal_gpio_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_hal_crc.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

__STATIC_INLINE cy_rslt_t cy_crc_start(cy_crc_t *obj, const crc_algorithm_t *algorithm)
{
    if(NULL == obj ||  NULL == algorithm)
        return CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT;

    Cy_Crypto_Core_Crc_CalcInit(obj->base, 
                                algorithm->width,
                                algorithm->polynomial,
                                algorithm->dataReverse,
                                algorithm->dataXor,
                                algorithm->remReverse,
                                algorithm->remXor,
                                algorithm->lfsrInitState);

    obj->crc_width = algorithm->width;
    
    return CY_RSLT_SUCCESS;
}

__STATIC_INLINE cy_rslt_t cy_crc_compute(const cy_crc_t *obj, const uint8_t *data, size_t length)
{ 
    if(NULL == obj || NULL == data || 0 == length)
        return CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT;

    Cy_Crypto_Core_Crc_CalcPartial(obj->base, data, length);   
    
    return CY_RSLT_SUCCESS;
}

__STATIC_INLINE cy_rslt_t cy_crc_finish(const cy_crc_t *obj, uint32_t *crc)
{
    if(NULL == obj || NULL == crc)
        return CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT;

    Cy_Crypto_Core_Crc_CalcFinish(obj->base, obj->crc_width, crc);
    
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */
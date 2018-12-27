/*******************************************************************************
* File Name: cyresult.h
*
* Description:
* Basic function result handling. Defines a simple type for conveying 
* information about whether something succeeded or details about any issues
* that were detected.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define CY_BIT_MASK(x)                     ((1U << (x)) - 1)

#define CY_RSLT_CODE_POSITION              (0U)
#define CY_RSLT_CODE_WIDTH                 (16U)
#define CY_RSLT_MODULE_POSITION            (16U)
#define CY_RSLT_MODULE_WIDTH               (14U)
#define CY_RSLT_TYPE_POSITION              (30U)
#define CY_RSLT_TYPE_WIDTH                 (2U)

#define CY_RSLT_CODE_MASK                  CY_BIT_MASK(CY_RSLT_CODE_WIDTH)
#define CY_RSLT_MODULE_MASK                CY_BIT_MASK(CY_RSLT_MODULE_WIDTH)
#define CY_RSLT_TYPE_MASK                  CY_BIT_MASK(CY_RSLT_TYPE_WIDTH)

#define CY_RSLT_TYPE_INFO                  (0)
#define CY_RSLT_TYPE_WARNING               (1)
#define CY_RSLT_TYPE_ERROR                 (2)
#define CY_RSLT_TYPE_FATAL                 (3)

#define CY_RSLT_GET_CODE(x)                (((x) >> CY_RSLT_CODE_POSITION) & CY_RSLT_CODE_MASK)
#define CY_RSLT_GET_TYPE(x)                (((x) >> CY_RSLT_TYPE_POSITION) & CY_RSLT_TYPE_MASK)
#define CY_RSLT_GET_MODULE(x)              (((x) >> CY_RSLT_MODULE_POSITION) & CY_RSLT_MODULE_MASK)

#define CY_RSLT_MODULE_CHIP_BASE           (0)
#define CY_RSLT_MODULE_BOARD_BASE          (2048)
#define CY_RSLT_MODULE_RTOS_BASE           (4096)
#define CY_RSLT_MODULE_NETWORK_BASE        (6144)
#define CY_RSLT_MODULE_MIDDLEWARE_BASE     (8192)

typedef uint32_t cy_rslt_t;

#define CY_RSLT_SUCCESS                    ((cy_rslt_t)0x00000000)

#define CY_RSLT_CREATE(type, module, code) \
    ((((module) & CY_RSLT_MODULE_MASK) << CY_RSLT_MODULE_POSITION) | \
    (((code) & CY_RSLT_CODE_MASK) << CY_RSLT_CODE_POSITION) | \
    (((type) & CY_RSLT_TYPE_MASK) << CY_RSLT_TYPE_POSITION))


#ifdef __cplusplus
} ;
#endif

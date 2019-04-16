/***************************************************************************//**
* \file cy_result.h
*
* \brief
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

/** Mask for the bit at position "x" */
#define CY_BIT_MASK(x)                     ((1U << (x)) - 1)

/** Bit position of the result code */
#define CY_RSLT_CODE_POSITION              (0U)
/** Bit width of the result code */
#define CY_RSLT_CODE_WIDTH                 (16U)
/** Bit position of the result type */
#define CY_RSLT_TYPE_POSITION              (16U)
/** Bit width of the result type */
#define CY_RSLT_TYPE_WIDTH                 (2U)
/** Bit position of the module identifier */
#define CY_RSLT_MODULE_POSITION            (18U)
/** Bit width of the module identifier */
#define CY_RSLT_MODULE_WIDTH               (14U)

/** Mask for the result code */
#define CY_RSLT_CODE_MASK                  CY_BIT_MASK(CY_RSLT_CODE_WIDTH)
/** Mask for the module identifier */
#define CY_RSLT_MODULE_MASK                CY_BIT_MASK(CY_RSLT_MODULE_WIDTH)
/** Mask for the result type */
#define CY_RSLT_TYPE_MASK                  CY_BIT_MASK(CY_RSLT_TYPE_WIDTH)

/** Informational-only result status */
#define CY_RSLT_TYPE_INFO                  (0)
/** Warning result */
#define CY_RSLT_TYPE_WARNING               (1)
/** Error result */
#define CY_RSLT_TYPE_ERROR                 (2)
/** Fatal error result */
#define CY_RSLT_TYPE_FATAL                 (3)

/** Get the value of the result code field */
#define CY_RSLT_GET_CODE(x)                (((x) >> CY_RSLT_CODE_POSITION) & CY_RSLT_CODE_MASK)
/** Get the value of the result type field */
#define CY_RSLT_GET_TYPE(x)                (((x) >> CY_RSLT_TYPE_POSITION) & CY_RSLT_TYPE_MASK)
/** Get the value of the module identifier field */
#define CY_RSLT_GET_MODULE(x)              (((x) >> CY_RSLT_MODULE_POSITION) & CY_RSLT_MODULE_MASK)

/** Base identifier for chip support modules */
#define CY_RSLT_MODULE_CHIP_BASE           (0)
/** Base identifier for board support modules */
#define CY_RSLT_MODULE_BOARD_BASE          (2048)
/** Base identifier for RTOS modules */
#define CY_RSLT_MODULE_RTOS_BASE           (4096)
/** Base identifier for network stack modules */
#define CY_RSLT_MODULE_NETWORK_BASE        (6144)
/** Base identifier for middleware modules */
#define CY_RSLT_MODULE_MIDDLEWARE_BASE     (8192)

/** Provides the result of an operation as a structured bitfield */
typedef uint32_t cy_rslt_t;

/** Result value indicating success */
#define CY_RSLT_SUCCESS                    ((cy_rslt_t)0x00000000)

/** Create a result value from the specified type, module, and result code */
#define CY_RSLT_CREATE(type, module, code) \
    ((((module) & CY_RSLT_MODULE_MASK) << CY_RSLT_MODULE_POSITION) | \
    (((code) & CY_RSLT_CODE_MASK) << CY_RSLT_CODE_POSITION) | \
    (((type) & CY_RSLT_TYPE_MASK) << CY_RSLT_TYPE_POSITION))


#ifdef __cplusplus
} ;
#endif

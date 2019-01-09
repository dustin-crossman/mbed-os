/*******************************************************************************
* File Name: cy_hal_adc.h
*
* Description:
* Provides a high level interface for interacting with the Cypress ADC. This is
* a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#define MAX_VALUE 0x0FFF

/** Initialize the adc peripheral
 *
 * Configures the pin used by adc.
 * @param[out] obj The adc object to initialize
 * @param[in]  pin The adc pin name
 * @return The status of the init request
 */
cy_rslt_t adc_init(adc_t *obj, PinName pin);

/** Uninitialize the adc peripheral and adc_t object
 *
 * @param[in,out] obj The adc object
 * @return The status of the free request
 */
cy_rslt_t adc_free(adc_t *obj);

/** Read the value from adc pin, represented as an unsigned 16bit value
 *
 * @param[in] obj The adc object
 * @param[out] value An unsigned 16bit value representing the current input voltage
 * @return The status of the read request
 */
cy_rslt_t adc_read_u16(const adc_t *obj, uint16_t *value);

#if defined(__cplusplus)
}
#endif
/***************************************************************************//**
* \file cy_hal_adc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Analog to 
* Digital Converter. This interface abstracts out the chip specific details. 
* If any chip specific functionality is necessary, or performance is critical 
* the low level functions can be used directly.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Maximum value that the ADC can return */
#define MAX_VALUE 0x0FFF

/** Initialize the adc peripheral
 *
 * Configures the pin used by adc.
 * @param[out] obj The adc object to initialize
 * @param[in]  pin The adc pin name
 * @return The status of the init request
 */
cy_rslt_t cy_adc_init(cy_adc_t *obj, cy_gpio_t pin);

/** Uninitialize the adc peripheral and cy_adc_t object
 *
 * @param[in,out] obj The adc object
 * @return The status of the free request
 */
cy_rslt_t cy_adc_free(cy_adc_t *obj);

/** Read the value from adc pin, represented as an unsigned 16bit value
 *
 * @param[in] obj The adc object
 * @param[out] value An unsigned 16bit value representing the current input voltage
 * @return The status of the read request
 */
cy_rslt_t cy_adc_read_u16(const cy_adc_t *obj, uint16_t *value);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
* File Name: cy_hal_dac.h
*
* Description:
* Provides a high level interface for interacting with the Cypress DAC. This is
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

/** Initialize the dac peripheral
 *
 * Configures the pin used by dac.
 * @param[in] obj The dac object to initialize
 * @param[in] pin The dac pin name
 * @return The status of the init request
 */
cy_rslt_t dac_init(dac_t *obj, PinName pin);

/** Release the dac object
 *
 * @param[in,out] obj The dac object
 * @return The status of the free request
 */
cy_rslt_t dac_free(dac_t *obj);

/** Set the output voltage, specified as unsigned 16-bit
 *
 * @param[in] obj The analogin object
 * @param[in] value The unsigned 16-bit output voltage to be set
 * @return The status of the write request
 */
cy_rslt_t dac_write_u16(const dac_t *obj, uint16_t value);

#if defined(__cplusplus)
}
#endif
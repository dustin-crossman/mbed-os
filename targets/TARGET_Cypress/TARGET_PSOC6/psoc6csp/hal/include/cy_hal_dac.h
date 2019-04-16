/***************************************************************************//**
* \file cy_hal_dac.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Digital to
* Analog Converter. This interface abstracts out the chip specific details. If 
* any chip specific functionality is necessary, or performance is critical the 
* low level functions can be used directly.
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

/** Initialize the dac peripheral
 *
 * Configures the pin used by dac.
 * @param[in] obj The dac object to initialize
 * @param[in] pin The dac pin name
 * @return The status of the init request
 */
cy_rslt_t cy_dac_init(cy_dac_t *obj, cy_gpio_t pin);

/** Release the dac object
 *
 * @param[in,out] obj The dac object
 * @return The status of the free request
 */
cy_rslt_t cy_dac_free(cy_dac_t *obj);

/** Read the current voltage value on the pin, as a normalized unsigned 16bit value
 *
 * @param[in]  obj        The analogin object
 * @param[out] millivolts The maximum output voltage in terms of the number of milivolts
 * @return The status of the get_max request
 */
cy_rslt_t cy_dac_get_max(cy_dac_t *obj, uint16_t *millivolts);

/** Set the output voltage, specified as unsigned 16-bit millivolts
 *
 * @param[in] obj        The analogin object
 * @param[in] millivolts The unsigned 16-bit output voltage to be set
 * @return The status of the write request
 */
cy_rslt_t cy_dac_write(const cy_dac_t *obj, uint16_t millivolts);

/** Read the current voltage value on the pin, as a normalized unsigned 16bit value
 *
 * @param[in]  obj        The analogin object
 * @param[out] millivolts The output voltage in terms of the number of milivolts
 * @return The status of the read request
 */
cy_rslt_t analogout_read(cy_dac_t *obj, uint16_t *millivolts);

#if defined(__cplusplus)
}
#endif

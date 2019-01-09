/*******************************************************************************
* File Name: cy_hal_gpio.h
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
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

/** Initialize the GPIO pin
 *
 * @param[out] obj The GPIO object to initialize
 * @param[in] pin The GPIO pin to initialize
 * @return The status of the init request
 */
cy_rslt_t gpio_init(gpio_t *obj, PinName pin);

/** Uninitialize the gpio peripheral and the gpio_t object
 *
 * @param[in,out] obj The flash object
 * @return The status of the free request
 */
cy_rslt_t gpio_free(gpio_t *obj);

/** Set the input pin mode
 *
 * @param[in] obj  The GPIO object
 * @param[in] mode The pin mode to be set
 * @return The status of the mode request
 */
cy_rslt_t gpio_mode(gpio_t *obj, PinMode mode);

/** Set the pin direction
 *
 * @param[in] obj       The GPIO object
 * @param[in] direction The pin direction to be set
 * @return The status of the dir request
 */
cy_rslt_t gpio_dir(gpio_t *obj, PinDirection direction);

/** Set the output value
 *
 * @param[in] obj   The GPIO object
 * @param[in] value The value to be set (0 or 1)
 * @return The status of the write request
 */
cy_rslt_t gpio_write(gpio_t *obj, uint8_t value);

/** Read the input value
 *
 * @param[in]  obj   The GPIO object
 * @param[out] value The value to set the IO to (0 or 1)
 * @return The status of the read request
 */
cy_rslt_t gpio_read(gpio_t *obj, uint8_t *value);

/** Register/clear an interrupt handler for the pin toggle pin IRQ event
 *
 * @param obj     The GPIO object
 * @param handler The function to call when the specified event happens
 * @param event   The GPIO IRQ event
 * @return The status of the irq_set request
 */
cy_rslt_t gpio_irq_set(gpio_irq_t *obj, gpio_irq_handler handler, gpio_irq_event event);

#if defined(__cplusplus)
}
#endif
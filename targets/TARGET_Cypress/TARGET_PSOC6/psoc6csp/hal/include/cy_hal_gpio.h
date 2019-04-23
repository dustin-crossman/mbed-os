/***************************************************************************//**
* \file cy_hal_gpio.h
*
* \brief
* Provides a high level interface for interacting with the Cypress GPIO.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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

/**
* \addtogroup group_hal_gpio GPIO (General Purpose Input Output)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress GPIO.
*
* \defgroup group_hal_gpio_macros Macros
* \defgroup group_hal_gpio_functions Functions
* \defgroup group_hal_gpio_data_structures Data Structures
* \defgroup group_hal_gpio_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Defines
*******************************************************************************/

/**
* \addtogroup group_hal_gpio_macros
* \{
*/

/** Integer representation of no connect pin (required to exist in all BSPs) */
#define CY_NC_PIN_VALUE ((int)0xFFFFFFFF)

/** \} group_hal_gpio_macros */


/**
* \addtogroup group_hal_gpio_enums
* \{
*/

/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Pin IRQ events */
typedef enum {
    CY_GPIO_IRQ_NONE = 0,   /**< No interrupt */
    CY_GPIO_IRQ_RISE = 1,   /**< Interrupt on rising edge */
    CY_GPIO_IRQ_FALL = 2,   /**< Interrupt on falling edge */
    CY_GPIO_IRQ_BOTH = 3,   /**< Interrupt on both rising and falling edges */
} cy_gpio_irq_event_t;

/** Pin direction */
typedef enum {
    CY_GPIO_DIR_INPUT         = 0,  /**< Input pin */
    CY_GPIO_DIR_OUTPUT        = 1,  /**< Output pin */
    CY_GPIO_DIR_BIDIRECTIONAL = 2,   /**< Input and output pin */
} cy_gpio_direction_t;

/** Pin drive mode */
typedef enum {
    CY_GPIO_DRIVE_ANALOG              = 0, /**< Analog Hi-Z */
    CY_GPIO_DRIVE_PULLUP              = 2, /**< Pull-up resistor */
    CY_GPIO_DRIVE_PULLDOWN            = 3, /**< Pull-down resistor */
    CY_GPIO_DRIVE_OPENDRAINDRIVESLOW  = 4, /**< Open-drain, Drives Low */
    CY_GPIO_DRIVE_OPENDRAINDRIVESHIGH = 5, /**< Open-drain, Drives High */
    CY_GPIO_DRIVE_STRONG              = 6, /**< Strong output */
    CY_GPIO_DRIVE_PULLUPDOWN          = 7, /**< Pull-up and pull-down resistors */
} cy_gpio_drive_mode_t;

/** \} group_hal_gpio_enums */


/**
* \addtogroup group_hal_gpio_data_structures
* \{
*/

/** GPIO callback function type */
typedef void (*cy_gpio_irq_handler)(void *handler_arg, cy_gpio_irq_event_t event);

/** \} group_hal_gpio_data_structures */


/**
* \addtogroup group_hal_gpio_functions
* \{
*/

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the GPIO pin
 *
 * @param[in]  pin The GPIO pin to initialize
 * @param[in]  direction The pin direction (input/output)
 * @param[in]  drvMode The pin drive mode
 * @param[in]  initVal Initial value on the pin
 * 
 * @return The status of the init request
 */
cy_rslt_t cy_gpio_init(cy_gpio_t pin, cy_gpio_direction_t direction, cy_gpio_drive_mode_t drvMode, bool initVal);

/** Uninitialize the gpio peripheral and the cy_gpio_t object
 *
 * @param[in] pin Pin number
 * 
 * @return The status of the free request
 */
cy_rslt_t cy_gpio_free(cy_gpio_t pin);

/** Set the pin direction
 *
 * @param[in] pin       The pin number
 * @param[in] direction The pin direction to be set
 * 
 * @return The status of the dir request
 */
cy_rslt_t cy_gpio_direction(cy_gpio_t pin, cy_gpio_direction_t direction);

/** Set the input pin mode
 *
 * @param[in] pin  The GPIO object
 * @param[in] drvMode The pin mode to be set
 * 
 * @return The status of the mode request
 */
cy_rslt_t cy_gpio_drivemode(cy_gpio_t pin, cy_gpio_drive_mode_t drvMode);

/** Set the output value
 *
 * @param[in] pin   The GPIO object
 * @param[in] value The value to be set (high = true, low = false)
 * 
 * @return The status of the write request
 */
__STATIC_INLINE cy_rslt_t cy_gpio_write(cy_gpio_t pin, bool value);

/** Read the input value
 *
 * @param[in]  pin   The GPIO object
 * @param[out] value The value of the IO (true = high, false = low)
 * 
 * @return The status of the read request
 */
__STATIC_INLINE cy_rslt_t cy_gpio_read(cy_gpio_t pin, bool *value);

/** Toggle the output value
 *
 * @param[in]  pin   The GPIO object
 * 
 * @return The status of the toggle request
 */
__STATIC_INLINE cy_rslt_t cy_gpio_toggle(cy_gpio_t pin);

/** Register/clear an interrupt handler for the pin toggle pin IRQ event
 *
 * @param[in] pin           The pin number
 * @param[in] intrPriority  The NVIC interrupt channel priority
 * @param[in] handler       The function to call when the specified event happens
 * @param[in] handler_arg   Generic argument that will be provided to the handler when called
 * 
 * @return The status of the register_irq request
 */
cy_rslt_t cy_gpio_register_irq(cy_gpio_t pin, uint8_t intrPriority, cy_gpio_irq_handler handler, void *handler_arg);

/** Enable or Disable the GPIO IRQ
 *
 * @param[in] pin    The GPIO object
 * @param[in] event  The GPIO IRQ event
 * @param[in] enable True to turn on interrupts, False to turn off
 * 
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_gpio_irq_enable(cy_gpio_t pin, cy_gpio_irq_event_t event, bool enable);

/** \} group_hal_gpio_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_hal_gpio */

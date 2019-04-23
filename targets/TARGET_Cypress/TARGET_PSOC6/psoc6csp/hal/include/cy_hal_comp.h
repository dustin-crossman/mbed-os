/***************************************************************************//**
* \file cy_hal_comp.h
*
* Provides a high level interface for interacting with the Cypress Comparator.
* This interface abstracts out the chip specific details.
* If any chip specific functionality is necessary, or performance is critical,
* the low level functions can be used directly.
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
* \addtogroup group_hal_comp COMP (Comparator)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress Comparator.
*
* \defgroup group_hal_comp_macros Macros
* \defgroup group_hal_comp_functions Functions
* \defgroup group_hal_comp_data_structures Data Structures
* \defgroup group_hal_comp_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_comp_enums
* \{
*/

/** Comparator interrupt triggers */
typedef enum {
    CY_COMP_IRQ_NONE, //!< Interrupts disabled
    CY_COMP_IRQ_RISE, //!< Rising edge
    CY_COMP_IRQ_FALL,  //!< Falling edge
} cy_comp_irq_event_t;

/** Comparator power options */
typedef enum {
    CY_COMP_POWER_OFF,    //!< Block is turned off
    CY_COMP_POWER_LOW,    //!< Block runs in low power/speed mode
    CY_COMP_POWER_MEDIUM, //!< Block runs in medium power/speed mode
    CY_COMP_POWER_HIGH,   //!< Block runs in high power/speed mode
} cy_comp_power_t;

/** Comparator output modes */
typedef enum {
    CY_COMP_OUTPUT_PULSE,  //!< Pulse output
    CY_COMP_OUTPUT_DIRECT, //!< Level output directly
    CY_COMP_OUTPUT_SYNC,   //!< Level output after synchronous edge detection
} cy_comp_output_t;

/** \} group_hal_comp_enums */


/**
* \addtogroup group_hal_comp_data_structures
* \{
*/

/** Configuration options for the comparator */
typedef struct
{
    cy_comp_power_t power;   //!< Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
    cy_comp_output_t output; //!< Output configuration (0=pulse, 1=direct, 2=sync)
    bool hysteresis;         //!< Should this use hysteresis
    bool deepsleep;          //!< Does this need to operate in deepsleep
} cy_comp_config_t;

/** Handler for comparator interrupts */
typedef void (*cy_comp_irq_handler)(void *handler_arg, cy_comp_irq_event_t event);

/** \} group_hal_comp_data_structures */


/**
* \addtogroup group_hal_comp_functions
* \{
*/

/** Initialize the comparator peripheral.
 *
 * @param[out] obj  The comparator object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t cy_comp_init(cy_comp_t *obj, cy_gpio_t vinp, cy_gpio_t vinm, cy_gpio_t vout);

/** Release the comparator peripheral.
 *
 * @param[in,out] obj The comparator object
 * @return The status of the free request
 */
cy_rslt_t cy_comp_free(cy_comp_t *obj);

/** Reconfigure the comparator object
 *
 * @param[in,out] obj  The comparator object
 * @param[in]     cfg  Configuration to apply to the comparator
 * @return The status of the power request
 */
cy_rslt_t cy_comp_configure(cy_comp_t *obj, const cy_comp_config_t cfg);

/** Gets the result of the comparator object
 *
 * @param[in]  obj    The comparator object
 * @param[out] output The comparator output state
 * @return The value of the comparator output request
 */
cy_rslt_t cy_comp_output(cy_comp_t *obj, bool *output);

/** Register/clear an interrupt handler for the comparator toggle IRQ event
 *
 * @param[in] obj         The comparator object
 * @param[in] handler     The function to call when the specified event happens
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_comp_register_irq(cy_comp_t *obj, cy_comp_irq_handler handler, void *handler_arg);

/** Enable or Disable the comparator IRQ
 *
 * @param[in] obj    The comparator object
 * @param[in] event  The comparator IRQ event
 * @param[in] enable True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_cy_comp_irq_enable(cy_comp_t *obj, cy_comp_irq_event_t event, bool enable);

/** \} group_hal_comp_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_comp */

/***************************************************************************//**
* \file cy_hal_timer.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Timer/Counter. 
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
* \addtogroup group_hal_timer TIMER (Timer/Counter)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress GPIO.
*
* \defgroup group_hal_timer_macros Macros
* \defgroup group_hal_timer_functions Functions
* \defgroup group_hal_timer_data_structures Data Structures
* \defgroup group_hal_timer_enums Enumerated Types
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
* \addtogroup group_hal_timer_enums
* \{
*/

/** Timer directions */
typedef enum
{
    CY_TIMER_DIR_UP, //!< Counts up
    CY_TIMER_DIR_DOWN, //!< Counts down
    CY_TIMER_DIR_UP_DOWN, //!< Counts up and down, terminal count occurs on both overflow and underflow.
} cy_timer_direction_t;

/** Timer interrupt triggers */
typedef enum {
    CY_TIMER_IRQ_NONE,
    CY_TIMER_IRQ_TERMINAL_COUNT,
    CY_TIMER_IRQ_CAPTURE_COMPARE,
    CY_TIMER_IRQ_ALL,
} cy_timer_irq_event_t;

/** \} group_hal_timer_enums */


/**
* \addtogroup group_hal_timer_data_structures
* \{
*/

/** Describes the current configuration of a timer/counter */
typedef struct
{
    /**
      * Whether the timer is set to continously run.
      * If true, the timer will run forever.
      * Otherwise, the timer will run once and stop (one shot).
      */
    bool is_continuous; //!< Whether the timer/counter operates continuous (true) or one shot (false)
    cy_timer_direction_t direction; //!< Direction the timer/counter is running
    bool is_compare; //!< Is it in compare (true) or capture (false) mode 
    uint32_t period; //!< Timer/counter period
    uint32_t compare_value; //!< Timer/counter comparison value
    uint32_t value; //!< Current value of the timer/counter
} cy_timer_cfg_t;

/** Handler for test interrupts */
typedef void (*cy_timer_irq_handler)(void *handler_arg, cy_timer_irq_event_t event);

/** \} group_hal_timer_data_structures */


/**
* \addtogroup group_hal_hwmgr_macros
* \{
*/

/** Bad argument. eg: null pointer */
#define CYCSP_TIMER_RSLT_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TIMER, 0))
/** Failed to initialize Timer clock */
#define CYCSP_TIMER_RSLT_FAILED_CLOCK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TIMER, 1))
/** Failed to initialize Timer */
#define CYCSP_TIMER_RSLT_FAILED_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TIMER, 2))

/** \} group_hal_hwmgr_macros */


/**
* \addtogroup group_hal_timer_functions
* \{
*/

/** Initialize the timer/counter peripheral and configure the pin
 *
 * @param[out] obj The timer/counter object to initialize
 * @param[in]  pin optional - The timer/counter compare/capture pin to initialize
 * @param[in]  clk optional - The shared clock to use, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cy_timer_init(cy_timer_t *obj, cy_gpio_t pin, const cy_clock_divider_t *clk);

/** Deinitialize the timer/counter object
 *
 * @param[in,out] obj The timer/counter object
 * @return The status of the free request
 */
cy_rslt_t cy_timer_free(cy_timer_t *obj);

/** Updates the configuration of the timer/counter object
 *
 * @param[in] obj  The timer/counter object
 * @param[in] cfg  The configuration of the timer/counter
 * @return The status of the info request
 */
cy_rslt_t cy_timer_set_config(cy_timer_t *obj, const cy_timer_cfg_t *cfg);

/** Starts the timer/counter with the pre-set configuration.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the start request
 */
cy_rslt_t cy_timer_start(cy_timer_t *obj);

/** Stops the timer/counter.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the stop request
 */
cy_rslt_t cy_timer_stop(cy_timer_t *obj);

/** The timer/counter interrupt handler registration
 *
 * @param[in] obj         The timer/counter object
 * @param[in] priority    The NVIC interrupt channel priority
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_timer_register_irq(cy_timer_t *obj, uint8_t priority, cy_timer_irq_handler handler, void *handler_arg);

/** Configure timer/counter interrupt enablement.
 *
 * @param[in] obj      The timer/counter object
 * @param[in] event    The timer/counter IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_timer_irq_enable(cy_timer_t *obj, cy_timer_irq_event_t event, bool enable);

/** \} group_hal_timer_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_timer */

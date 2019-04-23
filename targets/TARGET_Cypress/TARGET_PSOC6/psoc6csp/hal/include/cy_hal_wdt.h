/***************************************************************************//**
* \file cy_hal_wdt.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Watchdog Timer.
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
* \addtogroup group_hal_wdt WDT (Watchdog Timer)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress WDT.
*
* \defgroup group_hal_wdt_macros Macros
* \defgroup group_hal_wdt_functions Functions
* \defgroup group_hal_wdt_data_structures Data Structures
* \defgroup group_hal_wdt_enums Enumerated Types
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
* \addtogroup group_hal_wdt_enums
* \{
*/

/** WDT interrupt triggers */
typedef enum {
    /** TODO: Fill in */
    CY_WDT_TBD,
} cy_wdt_irq_event_t;

/** \} group_hal_wdt_enums */


/**
* \addtogroup group_hal_wdt_data_structures
* \{
*/

/** Handler for WDT interrupts */
typedef void (*cy_wdt_irq_handler)(void *handler_arg, cy_wdt_irq_event_t event);

/** \} group_hal_wdt_data_structures */


/**
* \addtogroup group_hal_wdt_functions
* \{
*/

/** Initialize the WDT
 *
 * Initialize or re-initialize the WDT. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * @param[out] obj The WDT object
 * @return The status of the init request
 */
cy_rslt_t cy_wdt_init(cy_wdt_t *obj);

/** Deinitialize the WDT
 *
 * Powerdown the WDT in preparation for sleep, powerdown, or reset.
 *
 * After calling this function no other WDT functions should be called except
 * timer_init(). Calling any function other than init after freeing is
 * undefined.
 *
 * @param[inout] obj The WDT object
 * @return The status of the free request
 */
cy_rslt_t cy_wdt_free(cy_wdt_t *obj);

/**
 * Reload/Reset the watchdog counter.
 *
 * @param[in] obj   The WDT object
 * @return The status of the reload request
 */
cy_rslt_t cy_wdt_reload(cy_wdt_t *obj);

/** Set timeframe between interrupts
 *
 * @param[in] obj   The WDT object
 * @param[in] time  The time in ticks to be set
 *
 * @return The status of the set_time request
 */
cy_rslt_t cy_wdt_set_time(cy_wdt_t *obj, uint32_t time);

/** Read the current tick
 *
 * If no rollover has occurred, the seconds passed since wdt_init() or wdt_set_time()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by timer_get_info.
 *
 * @param[in] obj    The WDT object
 * @param[out] count The timer's counter value in ticks
 * @return The status of the raed request
 */
cy_rslt_t cy_wdt_read(const cy_wdt_t *obj, uint32_t *count);

/** The WDT alarm interrupt handler registration
 *
 * @param[in] obj         The WDT object
 * @param[in] handler     The callback handler which will be invoked when the alarm fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_wdt_register_irq(cy_wdt_t *obj, cy_wdt_irq_handler handler, void *handler_arg);

/** Configure WDT alarm interrupt enablement.
 *
 * @param[in] obj      The WDT object
 * @param[in] event    The WDT IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_wdt_irq_enable(cy_wdt_t *obj, cy_wdt_irq_event_t event, bool enable);

/** \} group_hal_wdt_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_wdt */

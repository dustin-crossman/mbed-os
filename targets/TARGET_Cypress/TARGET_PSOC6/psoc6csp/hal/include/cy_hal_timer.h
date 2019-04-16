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

/** Timer directions */
typedef enum 
{
	UP, //!< Counts up
	DOWN, //!< Counts down
	UP_DOWN1, //!< Counts up and down, terminal count only occurs on underflow.
	UP_DOWN2, //!< Counts up and down, terminal count occurs on both overflow and underflow.
} cy_timer_direction_t;

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

/** Timer interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_TIMER_TBD,
} cy_timer_irq_event_t;

/** Handler for test interrupts */
typedef void (*cy_timer_irq_handler)(void *handler_arg, cy_timer_irq_event_t event);

/** Initialize the timer/counter peripheral and configure the pin
 *
 * @param[out] obj The timer/counter object to initialize
 * @param[in]  pin The timer/counter pin to initialize
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
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
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_timer_register_irq(cy_timer_t *obj, cy_timer_irq_handler handler, void *handler_arg);

/** Configure timer/counter interrupt enablement.
 *
 * @param[in] obj      The timer/counter object
 * @param[in] event    The timer/counter IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_timer_irq_enable(cy_timer_t *obj, cy_timer_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
* File Name: cy_hal_timer.h
*
* Description:
* Provides a high level interface for interacting with the Cypress MCWDT. 
* This is wrapper around the lower level PDL API.
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

/** Initialize the ticker
 *
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 * @return The status of the init request
 */
cy_rslt_t timer_init(void);

/** Deinitialize the ticker
 *
 * Powerdown the ticker in preparation for sleep, powerdown, or reset.
 *
 * After calling this function no other ticker functions should be called except
 * timer_init(). Calling any function other than init after freeing is
 * undefined.
 *
 * @return The status of the free request
 */
cy_rslt_t timer_free(void);

/** Read the current tick
 *
 * If no rollover has occurred, the seconds passed since timer_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by timer_get_info.
 *
 * @param[out] count The timer's counter value in ticks
 * @return The status of the raed request
 */
cy_rslt_t timer_read(uint32_t *count);

/** Set interrupt for specified timestamp
 *
 * @param[in] timestamp The time in ticks to be set
 *
 * @return The status of the set_interrupt request
 */
cy_rslt_t timer_set_interrupt(timestamp_t timestamp);

/** Disable ticker interrupt
 *
 * @return The status of the disable_interrupt request
 */
cy_rslt_t timer_disable_interrupt(void);

/** Clear the ticker interrupt
 *
 * @return The status of the clear_interrupt request
 */
cy_rslt_t timer_clear_interrupt(void);

/** Set pending interrupt that should be fired right away.
 *
 * @return The status of the fire_interrupt request
 */
cy_rslt_t timer_fire_interrupt(void);

/** Get frequency and counter bits of this ticker.
 *
 * @param[in] info status of the ticker
 * @return The status of the get_info request
 */
cy_rslt_t timer_get_info(ticker_info_t *info);

#if defined(__cplusplus)
}
#endif
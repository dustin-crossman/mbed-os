/***************************************************************************//**
* \file cy_hal_rtc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress RealTimeClock. 
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

/** RTC interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_RTC_TBD,
} cy_rtc_irq_event_t;

/** Handler for RTC interrupts */
typedef void (*cy_rtc_irq_handler)(void *handler_arg, cy_rtc_irq_event_t event);

/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @param[out] obj RTC object
 * @return The status of the init request
 */
cy_rslt_t cy_rtc_init(cy_rtc_t *obj);

/** Deinitialize RTC
 *
 * Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
 * affect the CPU domain and not the time keeping logic.
 * After this function is called no other RTC functions should be called
 * except for rtc_init.
 *
 * @param[in,out] obj RTC object
 * @return The status of the free request
 */
cy_rslt_t cy_rtc_free(cy_rtc_t *obj);

/** Check if the RTC has the time set and is counting
 *
 * @param[in] obj RTC object
 * @return The status of the is_enabled request
 */
cy_rslt_t cy_rtc_is_enabled(cy_rtc_t *obj);

/** Get the current time from the RTC peripheral
 *
 * @param[in]  obj RTC object
 * @param[out] time The current time in seconds
 * @return The status of the read request
 */
cy_rslt_t cy_rtc_read(cy_rtc_t *obj, uint64_t *time);

/** Write the current time in seconds to the RTC peripheral
 *
 * @param[in] obj  RTC object
 * @param[in] time The current time to be set in seconds.
 * @return The status of the write request
 */
cy_rslt_t cy_rtc_write(cy_rtc_t *obj, uint64_t time);

/** Set an alarm for the specified time in seconds to the RTC peripheral
 *
 * @param[in] obj  RTC object
 * @param[in] time The alarm time to be set in seconds.
 * @return The status of the alarm request
 */
cy_rslt_t cy_rtc_alarm(cy_rtc_t *obj, uint64_t time);

/** The RTC alarm interrupt handler registration
 *
 * @param[in] obj         The RTC object
 * @param[in] handler     The callback handler which will be invoked when the alarm fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_rtc_register_irq(cy_rtc_t *obj, cy_rtc_irq_handler handler, void *handler_arg);

/** Configure RTC alarm interrupt enablement.
 *
 * @param[in] obj      The RTC object
 * @param[in] event    The RTC IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_rtc_irq_enable(cy_rtc_t *obj, cy_rtc_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

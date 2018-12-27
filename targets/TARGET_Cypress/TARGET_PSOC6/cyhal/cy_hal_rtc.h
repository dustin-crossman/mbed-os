/*******************************************************************************
* File Name: cy_hal_rtc.h
*
* Description:
* Provides a high level interface for interacting with the Cypress RTC. This 
* is wrapper around the lower level PDL API.
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

/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @param[out] obj RTC object
 * @return The status of the init request
 */
cy_rslt_t rtc_init(rtc_t *obj);

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
cy_rslt_t rtc_free(rtc_t *obj);

/** Check if the RTC has the time set and is counting
 *
 * @param[in] obj RTC object
 * @return The status of the is_enabled request
 */
cy_rslt_t rtc_is_enabled(rtc_t *obj);

/** Get the current time from the RTC peripheral
 *
 * @param[in]  obj RTC object
 * @param[out] time The current time in seconds
 * @return The status of the read request
 */
cy_rslt_t rtc_read(rtc_t *obj, uint64_t *time);

/** Write the current time in seconds to the RTC peripheral
 *
 * @param[in] obj  RTC object
 * @param[in] time The current time to be set in seconds.
 * @return The status of the write request
 */
cy_rslt_t rtc_write(rtc_t *obj, uint64_t time);

#if defined(__cplusplus)
}
#endif
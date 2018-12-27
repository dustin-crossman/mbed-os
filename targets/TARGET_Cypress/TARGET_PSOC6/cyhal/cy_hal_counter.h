/*******************************************************************************
* File Name: cy_hal_counter.h
*
* Description:
* Provides a high level interface for interacting with the Cypress TCPWM. This 
* is a wrapper around the lower level PDL API.
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

struct counter_state_t
{
    bool is_continuous;
    counter_direction_t direction;
    uint8_t width;
    uint32_t compare;
    uint32_t period;
    uint32_t value;
}

enum counter_direction_t
{
    UP,
    DOWN,
    UP_DOWN1,
    UP_DOWN2,
}

/** Initialize the Counter peripheral and configure the pin
 *
 * @param[out] obj The Counter object to initialize
 * @param[in]  pin The Counter pin to initialize
 * @return The status of the init request
 */
cy_rslt_t counter_init(counter_t *obj, PinName pin);

/** Deinitialize the Counter object
 *
 * @param[in,out] obj The Counter object
 * @return The status of the free request
 */
cy_rslt_t counter_free(counter_t *obj);

/** Read the current float-point output duty-cycle
 *
 * @param[in] obj     The Counter object
 * @param[out] status A summary of the current configuration
 * @return The status of the info request
 */
cy_rslt_t counter_info(counter_t *obj, counter_state_t *status);

/** Read the current float-point output duty-cycle
 *
 * @param[in] obj        The Counter object
 * @param[in] continuous Whether the counter operates continuous (true) or one shot (false)
 * @param[in] direction  The direction the counter runs
 * @return The status of the info request
 */
cy_rslt_t counter_mode(counter_t *obj, bool continuous, counter_direction_t direction);

/** Starts the Counter with the provided period and pulsewidth
 *
 * @param[in] obj     The Counter object
 * @param[in] period_us  The period in microsecond
 * @param[in] pulsewidth_us  The pulsewidth in microseconds
 * @return The status of the start request
 */
cy_rslt_t counter_start(counter_t *obj, uint32_t period_us, uint32_t pulsewidth_us);

/** Set the Counter period specified in microseconds, keeping the duty cycle the same
 *
 * @param[in] obj The Counter object
 * @param[in] us  The period in microsecond
 * @return The status of the period request
 */
cy_rslt_t counter_period(counter_t *obj, uint32_t us);

/** Set the Counter compare value specified in microseconds, keeping the period the same
 *
 * @param[in] obj The Counter object
 * @param[in] us  The compare value in microseconds
 * @return The status of the compare request
 */
cy_rslt_t counter_compare(counter_t *obj, uint32_t us);

#if defined(__cplusplus)
}
#endif
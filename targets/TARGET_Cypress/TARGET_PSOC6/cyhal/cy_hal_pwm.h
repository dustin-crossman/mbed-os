/*******************************************************************************
* File Name: cy_hal_pwm.h
*
* Description:
* Provides a high level interface for interacting with the Cypress PWM. This is
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

struct pwm_state_t
{
    uint8_t width;
    uint32_t period;
    uint32_t pulsewidth;
}

/** Initialize the PWM out peripheral and configure the pin
 *
 * @param[out] obj The PWM object to initialize
 * @param[in]  pin The PWM pin to initialize
 * @return The status of the init request
 */
cy_rslt_t pwm_init(pwm_t *obj, PinName pin);

/** Deinitialize the PWM object
 *
 * @param[in,out] obj The PWM object
 * @return The status of the free request
 */
cy_rslt_t pwm_free(pwm_t *obj);

/** Read the current state of the PWM
 *
 * @param[in] obj The PWM object
 * @param[out] A summary of the current configuration
 * @return The status of the info request
 */
cy_rslt_t pwm_info(pwm_t *obj, pwm_state_t *status);

/** Starts the PWM with the provided period and pulsewidth
 *
 * @param[in] obj           The PWM object
 * @param[in] period_us     The period in microsecond
 * @param[in] pulsewidth_us The pulsewidth in microseconds
 * @return The status of the start request
 */
cy_rslt_t pwm_start(pwm_t *obj, uint32_t period_us, uint32_t pulsewidth_us);

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * @param[in] obj The PWM object
 * @param[in] us  The period in microsecond
 * @return The status of the period request
 */
cy_rslt_t pwm_period(pwm_t *obj, uint32_t us);

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param[in] obj The PWM object
 * @param[in] us  The pulsewidth in microseconds
 * @return The status of the pulsewidth request
 */
cy_rslt_t pwm_pulsewidth(pwm_t *obj, uint32_t us);

#if defined(__cplusplus)
}
#endif
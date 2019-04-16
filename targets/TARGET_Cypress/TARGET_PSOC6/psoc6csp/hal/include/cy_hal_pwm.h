/***************************************************************************//**
* \file cy_hal_pwm.h
*
* \brief
* Provides a high level interface for interacting with the Cypress PWM. 
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
#include "cy_result.h"
#include "cy_hal_hwmgr.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Bad argument */
#define CYCSP_PWM_RSLT_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_PWM, 0))
/** Failed to initialize PWM clock */
#define CYCSP_PWM_RSLT_FAILED_CLOCK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_PWM, 1))
/** Failed to initialize PWM */
#define CYCSP_PWM_RSLT_FAILED_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_PWM, 2))

/** Initialize the PWM out peripheral and configure the pin
 *
 * @param[out] obj The PWM object to initialize
 * @param[in]  pin The PWM pin to initialize
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cy_pwm_init(cy_pwm_t *obj, cy_gpio_t pin, const cy_clock_divider_t *clk);

/** Deinitialize the PWM object
 *
 * @param[in,out] obj The PWM object
 * @return The status of the free request
 */
cy_rslt_t cy_pwm_free(cy_pwm_t *obj);

/** Set the number of microseconds for the PWM period & pulse width
 *
 * @param[in] obj            The PWM object
 * @param[in] period_us      The period in microseconds
 * @param[in] pulse_width_us The pulse width in microseconds
 * @return The status of the period request
 */
cy_rslt_t cy_pwm_period(cy_pwm_t *obj, uint32_t period_us, uint32_t pulse_width_us);

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param[in] obj          The PWM object
 * @param[in] duty_cycle   The percentage of time the output is high
 * @param[in] frequency_hz The frequency of the PWM
 * @return The status of the pulsewidth request
 */
cy_rslt_t cy_pwm_duty_cycle(cy_pwm_t *obj, float duty_cycle, uint32_t frequency_hz);

/** Starts the PWM with the provided period and pulsewidth
 *
 * @param[in] obj           The PWM object
 * @return The status of the start request
 */
cy_rslt_t cy_pwm_start(cy_pwm_t *obj);

/** Stops the PWM from running
 *
 * @param[in] obj The PWM object
 * @return The status of the stop request
 */
cy_rslt_t cy_pwm_stop(cy_pwm_t *obj);

#if defined(__cplusplus)
}
#endif

/***************************************************************************//**
* \file cy_hal_opamp.h
*
* \brief
* Provides a high level interface for interacting with the Cypress OpAmp. 
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

/** Initialize the opamp peripheral. 
 *  If vinp is NULL, it will initialize in follower mode.
 *
 * @param[out] obj  The opamp object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t cy_opamp_init(cy_opamp_t *obj, cy_gpio_t vinp, cy_gpio_t vinm, cy_gpio_t vout);

/** Release the opamp peripheral.
 *
 * @param[in,out] obj The opamp object
 * @return The status of the free request
 */
cy_rslt_t cy_opamp_free(cy_opamp_t *obj);

/** Reconfigure the opamp object
 *
 * @param[in,out] obj       The opamp object 
 * @param[in]     power     Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
 * @param[in]     deepsleep Does this need to operate in deepsleep
 * @return The status of the power request
 */
cy_rslt_t cy_opamp_power(cy_opamp_t *obj, uint8_t power, bool deepsleep);

#if defined(__cplusplus)
}
#endif

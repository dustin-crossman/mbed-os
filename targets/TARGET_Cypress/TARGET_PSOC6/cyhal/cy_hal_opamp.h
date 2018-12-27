/*******************************************************************************
* File Name: cy_hal_opamp.h
*
* Description:
* Provides a high level interface for interacting with the Cypress OpAmp. This
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

/** Initialize the opamp peripheral. 
 *  If vinp is NULL, it will initialize in follower mode.
 *
 * @param[out] obj  The opamp object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t opamp_init(opamp_t *obj, PinName vinp, PinName vinm, PinName vout);

/** Release the opamp peripheral.
 *
 * @param[in,out] obj The opamp object
 * @return The status of the free request
 */
cy_rslt_t opamp_free(opamp_t *obj);

/** Reconfigure the opamp object
 *
 * @param[in,out] obj       The opamp object 
 * @param[in]     power     Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
 * @param[in]     deepsleep Does this need to operate in deepsleep
 * @return The status of the power request
 */
cy_rslt_t opamp_power(opamp_t *obj, uint8_t power, bool deepsleep);

#if defined(__cplusplus)
}
#endif
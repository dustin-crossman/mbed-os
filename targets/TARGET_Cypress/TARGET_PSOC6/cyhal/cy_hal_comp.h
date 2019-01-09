/*******************************************************************************
* File Name: cy_hal_comp.h
*
* Description:
* Provides a high level interface for interacting with the Cypress Comparator. 
* This is a wrapper around the lower level PDL API.
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

/** Initialize the comparator peripheral. 
 *
 * @param[out] obj  The comparator object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t comp_init(comp_t *obj, PinName vinp, PinName vinm, PinName vout);

/** Release the comparator peripheral.
 *
 * @param[in,out] obj The comparator object
 * @return The status of the free request
 */
cy_rslt_t comp_free(comp_t *obj);

/** Reconfigure the comparator object
 *
 * @param[in,out] obj       The comparator object 
 * @param[in]     power     Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
 * @param[in]     deepsleep Does this need to operate in deepsleep
 * @return The status of the power request
 */
cy_rslt_t comp_power(comp_t *obj, uint8_t power, bool deepsleep);

/** Gets the result of the comparator object
 *
 * @param[in]  obj    The comparator object
 * @param[out] status The comparator output state
 * @return The status of the status request
 */
cy_rslt_t comp_status(comp_t *obj, bool *status);

#if defined(__cplusplus)
}
#endif
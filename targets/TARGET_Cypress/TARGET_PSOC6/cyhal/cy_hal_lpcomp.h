/*******************************************************************************
* File Name: cy_hal_lpcomp.h
*
* Description:
* Provides a high level interface for interacting with the Cypress Low Power 
* Comparator. This is a wrapper around the lower level PDL API.
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

/** Initialize the low power comparator peripheral. 
 *
 * @param[out] obj  The low power comparator object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t lpcomp_init(lpcomp_t *obj, PinName vinp, PinName vinm, PinName vout);

/** Release the low power comparator peripheral.
 *
 * @param[in,out] obj The low power comparator object
 * @return The status of the free request
 */
cy_rslt_t lpcomp_free(comp_t *obj);

/** Reconfigure the low power comparator object
 *
 * @param[in,out] obj        The low power comparator object 
 * @param[in]     power      Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
 * @param[in]     hysteresis enable (true), disable (false)
 * @param[in]     output     Power mode to operate in (0=pulse, 1=direct, 2=sync)
 * @return The status of the configure request
 */
cy_rslt_t lpcomp_configure(lpcomp_t *obj, uint8_t power, bool hysteresis, uint8_t output);

/** Gets the result of the low power comparator object
 *
 * @param[in]  obj    The comparator object
 * @param[out] status The comparator output state
 * @return The status of the status request
 */
cy_rslt_t lpcomp_status(lpcomp_t *obj, bool *status);

#if defined(__cplusplus)
}
#endif
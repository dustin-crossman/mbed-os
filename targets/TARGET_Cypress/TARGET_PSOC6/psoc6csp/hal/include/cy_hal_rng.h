/***************************************************************************//**
* \file cy_hal_rng.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Random 
* Number Generator. This interface abstracts out the chip specific details. If 
* any chip specific functionality is necessary, or performance is critical the 
* low level functions can be used directly.
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

/** Initialize the random number generator.
 *
 * @param[out] obj The random number generator object
 * @return The status of the init request
 */
cy_rslt_t cy_rng_init(cy_rng_t *obj);

/** Release the random number generator.
 *
 * @param[in,out] obj The random number generator object
 * @return The status of the free request
 */
cy_rslt_t cy_rng_free(cy_rng_t *obj);

/** Generate a random number.
 *
 * @param[in]  obj   The random number generator object
 * @param[out] value The random number generated
 * @return The status of the generate request
 */
cy_rslt_t cy_rng_generate(const cy_rng_t *obj, uint32_t *value);

#if defined(__cplusplus)
}
#endif

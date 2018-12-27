/*******************************************************************************
* File Name: cy_hal_rng.h
*
* Description:
* Provides a high level interface for interacting with the Cypress Random Number 
* Generator. This is a wrapper around the lower level PDL API.
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

/** Initialize the random number generator.
 *
 * @param[out] obj The random number generator object
 * @return The status of the init request
 */
cy_rslt_t rng_init(rng_t *obj);

/** Release the random number generator.
 *
 * @param[in,out] obj The random number generator object
 * @return The status of the free request
 */
cy_rslt_t rng_free(rng_t *obj);

/** Generate a random number.
 *
 * @param[in] obj The random number generator object
 * @return The status of the generate request
 */
cy_rslt_t rng_generate(const rng_t *obj);

/** Release the random number generator.
 *
 * @param[in,out] obj  The random number generator object
 * @param[in]     seed The seed to use for the pseudo generator
 * @return The status of the configure_pseudo request
 */
cy_rslt_t rng_configure_pseudo(rng_t *obj, uint32_t seed);

/** generate a pseudo random number.
 *
 * @param[in] obj The random number generator object
 * @return The status of the generate_pseudo request
 */
cy_rslt_t rng_generate_pseudo(const rng_t *obj);

#if defined(__cplusplus)
}
#endif
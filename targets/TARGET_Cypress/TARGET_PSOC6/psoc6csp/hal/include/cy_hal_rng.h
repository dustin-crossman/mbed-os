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
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_rng RNG (Random Number Generator)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress RNG.
*
* \defgroup group_hal_rng_macros Macros
* \defgroup group_hal_rng_functions Functions
* \defgroup group_hal_rng_data_structures Data Structures
* \defgroup group_hal_rng_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**
* \addtogroup group_hal_rng_functions
* \{
*/

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

/** \} group_hal_rng_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_rng */

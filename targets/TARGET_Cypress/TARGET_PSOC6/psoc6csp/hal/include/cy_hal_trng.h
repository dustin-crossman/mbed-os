/***************************************************************************//**
* \file cy_hal_trng.h
*
* \brief
* Provides a high level interface for interacting with the Cypress True Random
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
* \addtogroup group_hal_trng TRNG (True Random Number Generator)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress TRNG.
*
* \defgroup group_hal_trng_macros Macros
* \defgroup group_hal_trng_functions Functions
* \defgroup group_hal_trng_data_structures Data Structures
* \defgroup group_hal_trng_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Arguments passed the function are not valid. */
#define CY_RSLT_CSP_ERR_TRNG_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TRNG, 0))
/** Hardware error in the crypto block. This will only occur if the Ring oscillators in the TRNG generator are explicitly 
 *  disabled during TRNG generation.
 */
#define CY_RSLT_CSP_ERR_TRNG_HW_ERROR (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TRNG, 1))

/**
* \addtogroup group_hal_trng_functions
* \{
*/

/** Initialize the random number generator.
 *
 * @param[out] obj The random number generator object
 * @return The status of the init request
 */
cy_rslt_t cy_trng_init(cy_trng_t *obj);

/** Release the random number generator.
 *
 * @param[in,out] obj The random number generator object
 * @return The status of the free request
 */
cy_rslt_t cy_trng_free(cy_trng_t *obj);

/** Generate a random number.
 *
 * @param[in]  obj   The random number generator object
 * @param[out] value The random number generated
 * @return The status of the generate request
 */
__STATIC_INLINE cy_rslt_t cy_trng_generate(const cy_trng_t *obj, uint32_t *value);

/** \} group_hal_trng_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_trng */

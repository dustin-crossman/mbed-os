/***************************************************************************//**
* \file cy_utils.h
*
* \brief
* Basic utility macros and functions.
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
* \addtogroup group_utils Utilities
* \ingroup group_abstraction
* \{
* Basic utility macros and functions.
*
* \defgroup group_utils_macros Macros
*/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_utils_macros
* \{
*/

/** Simple macro to supress the unused parameter warning by casting to void. */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/** Utility macro when NDEBUG is not declared to check a condition and if false call CY_HALT() */
#if defined(NDEBUG)
    #define CY_ASSERT(x)    do  \
                            {   \
                            } while(0)
#else
    #define CY_ASSERT(x)    do  \
                            {   \
                                if(!(x)) \
                                { \
                                    __asm("    bkpt    1"); \
                                } \
                            } while(0)
#endif  /* defined(NDEBUG) */


/** \} group_utils_macros */

#ifdef __cplusplus
}
#endif

/** \} group_utils */

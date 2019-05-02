/***************************************************************************//**
* \file cy_abstraction_board.h
*
* \brief
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
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
* \addtogroup group_abstraction_board Board abstraction
* \ingroup group_abstraction
* \{
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
*
* \defgroup group_abstraction_board_macros Macros
* \defgroup group_abstraction_board_functions Functions
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_abstraction_board_macros
* \{
*/

/** The specified resource number is not valid */
#define CYBSP_RSLT_INVALID_INDEX (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 0))
/** The specified resource is already enabled */
#define CYBSP_RSLT_ALREADY_ENABLED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 1))
/** The specified resource is not enabled */
#define CYBSP_RSLT_NOT_ENABLED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 2))

/** \} group_abstraction_board_macros */


/**
* \addtogroup group_abstraction_board_functions
* \{
*/

/**
 * \brief Initialize all hardware on the board
 * \returns CY_RSLT_SUCCESS if the board is sucessfully initialized, if there is
 *          a problem initializing any hardware it returns an error code specific
 *          to the hardware module that had a problem.
 */
cy_rslt_t cy_board_init(void);

/**
 * \brief Returns the number of LEDs in the system
 * \returns The number of LEDs physically on the board that the user can use.
 */
uint32_t cy_board_led_count(void);

/**
 * \brief Enables the specified LED, setting the GPIO pin as necessary
 * \param which The specific LED number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBSP_RSLT_ALREADY_ENABLED if the specified LED number is already enabled
 */
cy_rslt_t cy_board_led_enable(uint32_t which);

/**
 * \brief Sets the state of the LED.
 * \param which The specific LED number to set state, must be between 0 and count-1
 * \param on Whether the LED should be turned on (true) or off (false)
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBSP_RSLT_NOT_ENABLED if the specified LED number has not been enabled
 */
cy_rslt_t cy_board_led_set_state(uint32_t which, bool on);

/**
 * \brief Disables an LED making the pin available for other purposes.
 * \param which The specific LED number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBSP_RSLT_NOT_ENABLED if the specified LED number has not been enabled
 */
cy_rslt_t cy_board_led_disable(uint32_t which);

/**
 * \brief Returns the number of switches in the system.
 * \returns The number of switches physically on the board that the user can use.
 */
uint32_t cy_board_switch_count(void);

/**
 * \brief Enables the specified switch, setting the GPIO pin as necessary
 * \param which The specific switch number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the switch was enabled successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBSP_RSLT_ALREADY_ENABLED if the specified switch number is already enabled
 */
cy_rslt_t cy_board_switch_enable(uint32_t which);

/**
 * \brief Sets the state of the switch.
 * \param which The specific switch number to get state from, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the switch information was retrieved successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBSP_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_get_state(uint32_t which);

/**
 * \brief Sets the interrupt to trigger when the switch state is changed.
 * \param which The specific switch number to get state from, must be between 0 and count-1
 * \param type The type sets level vs edge and active high vs active low
 * \param callback The function pointer to call when the switch state changes
 * \returns CY_RSLT_SUCCESS if the switch information was retrieved successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBSP_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_set_interrupt(uint32_t which, uint32_t type, void * callback);

/**
 * \brief Disables a switch making the pin available for other purposes.
 * \param which The specific switch number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the switch was enabled successfully
 *          CYBSP_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBSP_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_disable(uint32_t which);

/** \} group_abstraction_board_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_abstraction_board */

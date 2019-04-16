/*******************************************************************************
* File Name: cy_abstraction_board.h
*
* Description:
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

/**
* \addtogroup group_abstraction_board Board abstraction
* \ingroup group_abstraction
* \{
* TODO: Board abstraction high-level description
* to be set in abstraction/include/cy_abstraction_board.h
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

/** TODO */
#define CYBRD_RSLT_INVALID_INDEX (CY_RSLT_CREATE(CY_RSLT_ERROR, CY_RSLT_MODULE_BOARD_BASE, 0))
/** TODO */
#define CYBRD_RSLT_ALREADY_ENABLED (CY_RSLT_CREATE(CY_RSLT_ERROR, CY_RSLT_MODULE_BOARD_BASE, 1))
/** TODO */
#define CYBRD_RSLT_NOT_ENABLED (CY_RSLT_CREATE(CY_RSLT_ERROR, CY_RSLT_MODULE_BOARD_BASE, 2))

/** \} group_abstraction_board_macros */


/**
* \addtogroup group_abstraction_board_functions
* \{
*/

/**
 * \brief initialize all hardware on the board
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
 *          CYBRD_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBRD_RSLT_ALREADY_ENABLED if the specified LED number is already enabled
 */
cy_rslt_t cy_board_led_enable(uint32_t which);

/**
 * \brief Sets the state of the LED.
 * \param which The specific LED number to set state, must be between 0 and count-1
 * \param on Whether the LED should be turned on (true) or off (false)
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 *          CYBRD_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBRD_RSLT_NOT_ENABLED if the specified LED number has not been enabled
 */
cy_rslt_t cy_board_led_set_state(uint32_t which, bool on);

/**
 * \brief Disables an LED making the pin available for other purposes.
 * \param which The specific LED number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 *          CYBRD_RSLT_INVALID_INDEX if the specified LED number is not valid
 *          CYBRD_RSLT_NOT_ENABLED if the specified LED number has not been enabled
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
 *          CYBRD_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBRD_RSLT_ALREADY_ENABLED if the specified switch number is already enabled
 */
cy_rslt_t cy_board_switch_enable(uint32_t which);

/**
 * \brief Sets the state of the switch.
 * \param which The specific switch number to get state from, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the switch information was retrieved successfully
 *          CYBRD_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBRD_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_get_state(uint32_t which);

/**
 * \brief Sets the interrupt to trigger when the switch state is changed.
 * \param which The specific switch number to get state from, must be between 0 and count-1
 * \param type The type sets level vs edge and active high vs active low
 * \param callback The function pointer to call when the switch state changes
 * \returns CY_RSLT_SUCCESS if the switch information was retrieved successfully
 *          CYBRD_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBRD_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_set_interrupt(uint32_t which, uint32_t type, void * callback);

/**
 * \brief Disables a switch making the pin available for other purposes.
 * \param which The specific switch number to enable, must be between 0 and count-1
 * \returns CY_RSLT_SUCCESS if the switch was enabled successfully
 *          CYBRD_RSLT_INVALID_INDEX if the specified switch number is not valid
 *          CYBRD_RSLT_NOT_ENABLED if the specified switch number has not been enabled
 */
cy_rslt_t cy_board_switch_disable(uint32_t which);

/** \} group_abstraction_board_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_abstraction_board */

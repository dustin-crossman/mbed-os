/***************************************************************************//**
* \file CY8CPROTO-063-BLE/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-063-BLE kit.
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
* \addtogroup group_bsp_cy8cproto_063_ble CY8CPROTO-063-BLE
* \ingroup group_bsp
* \{
* \defgroup group_bsp_cy8cproto_063_ble_macros Macros
* \defgroup group_bsp_cy8cproto_063_ble_enums Enumerated Types
*/

#pragma once

#include "cyhal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_cy8cproto_063_ble_macros
* \{
*/

/* Board components mapping */
/** Pin: LED3 in the CY8CPROTO-063-BLE board */
#define CYBSP_LED3                P6_3
/** Pin: LED4 in the CY8CPROTO-063-BLE board */
#define CYBSP_LED4                P7_1
/** Pin: SW2 in the CY8CPROTO-063-BLE board */
#define CYBSP_SW2                 P0_4

/* Board peripheral count */
#define CYBSP_LED_COUNT     2
#define CYBSP_BTN_COUNT     1

/* Generic signal names */
/** Pin: WCO input */
#define CYBSP_WCO_IN           P0_0
/** Pin: WCO output */
#define CYBSP_WCO_OUT          P0_1

/** Pin: UART RX */
#define CYBSP_UART_RX          P5_0
/** Pin: UART TX */
#define CYBSP_UART_TX          P5_1

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL          P6_4
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA          P6_5

/** Pin: SWDIO */
#define CYBSP_SWDIO            P6_6
/** Pin: SWDCK */
#define CYBSP_SWDCK            P6_7

/** \} group_bsp_cy8cproto_063_ble_macros */

/**
* \addtogroup group_bsp_cy8cproto_063_ble_enums
* \{
*/

/** Enum defining the different states for the LED. */
typedef enum
{
    CYBSP_LED_STATE_ON          = 0,
    CYBSP_LED_STATE_OFF         = 1,
} cybsp_led_state_t;

/** Enum defining the different states for a button. */
typedef enum
{
    CYBSP_BTN_PRESSED           = 0,
    CYBSP_BTN_OFF               = 1,
} cybsp_btn_state_t;

/** Enum defining the different LED pins on the board. */
typedef enum
{
    CYBSP_LED_RED = CYBSP_LED3,
    CYBSP_LED_GREEN = CYBSP_LED4,

    CYBSP_USER_LED = CYBSP_LED_RED,
    CYBSP_USER_LED1 = CYBSP_LED_RED,
    CYBSP_USER_LED2 = CYBSP_LED_GREEN,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_USER_BTN = CYBSP_SW2,
    CYBSP_USER_BTN1 = CYBSP_SW2,
} cybsp_btn_t;

/** \} group_bsp_cy8cproto_063_ble_enums */

/**
* \addtogroup group_bsp_cy8cproto_063_ble_macros
* \{
*/

/* Board peripheral count */
/** Macro: Number of LEDs on CY8CPROTO-063-BLE board */
#define CYBSP_LED_COUNT     2
/** Macro: Number of buttons on CY8CPROTO-063-BLE board */
#define CYBSP_BTN_COUNT     1

/** \} group_bsp_cy8cproto_063_ble_macros */


#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8cproto_062_4343w */

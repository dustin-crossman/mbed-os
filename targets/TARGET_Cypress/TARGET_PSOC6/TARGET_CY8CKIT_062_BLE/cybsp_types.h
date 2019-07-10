/***************************************************************************//**
* \file CY8CKIT-062-BLE/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-BLE pioneer kit.
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
* \addtogroup group_bsp_cy8ckit_062_ble CY8CKIT-062-BLE
* \ingroup group_bsp
* \{
*/

#pragma once

#include "cyhal_pin_package.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_cy8ckit_062_ble_macros
* \{
*/

// Generic signal names
/** Pin: WCO input */
#define CYBSP_WCO_IN           P0_0
/** Pin: WCO output */
#define CYBSP_WCO_OUT          P0_1

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL          P6_0
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA          P6_1

/** Pin: SWO */
#define CYBSP_SWO              P6_4
/** Pin: SWDIO */
#define CYBSP_SWDIO            P6_6
/** Pin: SWDCK */
#define CYBSP_SWDCK            P6_7

/** Pin: CapSesnse TX */
#define CYBSP_CSD_TX           P1_0
/** Pin: CapSesnse CINA */
#define CYBSP_CINA             P7_1
/** Pin: CapSesnse CINB */
#define CYBSP_CINB             P7_2
/** Pin: CapSesnse CMOD */
#define CYBSP_CMOD             P7_7
/** Pin: CapSesnse Button 0 */
#define CYBSP_CSD_BTN0         P8_1
/** Pin: CapSesnse Button 1 */
#define CYBSP_CSD_BTN1         P8_2
/** Pin: CapSesnse Slider 0 */
#define CYBSP_CSD_SLD0         P8_3
/** Pin: CapSesnse Slider 1 */
#define CYBSP_CSD_SLD1         P8_4
/** Pin: CapSesnse Slider 2 */
#define CYBSP_CSD_SLD2         P8_5
/** Pin: CapSesnse Slider 3 */
#define CYBSP_CSD_SLD3         P8_6
/** Pin: CapSesnse Slider 4 */
#define CYBSP_CSD_SLD4         P8_7

/** Pin: QUAD SPI SS */
#define CYBSP_QSPI_SS          P11_2
/** Pin: QUAD SPI D3 */
#define CYBSP_QSPI_D3          P11_3
/** Pin: QUAD SPI D2 */
#define CYBSP_QSPI_D2          P11_4
/** Pin: QUAD SPI D1 */
#define CYBSP_QSPI_D1          P11_5
/** Pin: QUAD SPI D0 */
#define CYBSP_QSPI_D0          P11_6
/** Pin: QUAD SPI SCK */
#define CYBSP_QSPI_SCK         P11_7

/** \} group_bsp_cy8ckit_062_ble_macros */

/**
* \addtogroup group_bsp_cy8ckit_062_ble_enums
* \{
*/

/** Enum defining the different states for the LED. */
typedef enum
{
	CYBSP_LED_STATE_ON        	= 0,
	CYBSP_LED_STATE_OFF        	= 1,
} cybsp_led_state_t;

/** Enum defining the different states for a button. */
typedef enum
{
	CYBSP_BTN_PRESSED  	      	= 0,
	CYBSP_BTN_OFF	          	= 1,
} cybsp_btn_state_t;

/** Enum defining the different LED pins on the board. */
typedef enum
{
    CYBSP_LED9 = P13_7,
    CYBSP_LED8 = P1_5,
    CYBSP_LED_RGB_RED = P0_3,
    CYBSP_LED_RGB_GREEN = P1_1,
    CYBSP_LED_RGB_BLUE = P11_1,

    CYBSP_USER_LED1 = CYBSP_LED8,
    CYBSP_USER_LED2 = CYBSP_LED9,
    CYBSP_USER_LED3 = CYBSP_LED_RGB_RED,
    CYBSP_USER_LED4 = CYBSP_LED_RGB_GREEN,
    CYBSP_USER_LED5 = CYBSP_LED_RGB_BLUE,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_SW2 = P0_4,

    CYBSP_USER_BTN1 = CYBSP_SW2,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;

/** \} group_bsp_cy8ckit_062_ble_enums */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8ckit_062_ble */

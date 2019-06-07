/***************************************************************************//**
* \file CY8CKIT-062S2-43012/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062S2-43012 kit.
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
* \addtogroup group_bsp_cy8ckit_062s2_43012 CY8CKIT-062S2-43012
* \ingroup group_bsp
* \{
* \defgroup group_bsp_cy8ckit_062s2_43012_macros Macros
* \defgroup group_bsp_cy8ckit_062s2_43012_enums Enumerated Types
*/

#pragma once

#include "cyhal.h"
#include "cybsp_cy8ckit_062s2_43012.h"

#if defined(__cplusplus)
extern "C" {
#endif



/**
* \addtogroup group_bsp_cy8ckit_062s2_43012_enums
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

/* Kit specific LED names and mapping */
#define LED9				P11_1 		// TODO: needs to be configurator-generated
#define LED8				P1_5 		// TODO: needs to be configurator-generated
#define LED5_RGB_R			P1_1		// TODO: needs to be configurator-generated
#define LED5_RGB_G			P0_5		// TODO: needs to be configurator-generated
#define LED5_RGB_B			P7_3		// TODO: needs to be configurator-generated

/* BSP LED defines by LED color */
#define CYBSP_LED_RED		LED5_RGB_R
#define CYBSP_LED_GREEN		LED5_RGB_G
#define CYBSP_LED_BLUE		LED5_RGB_B
#define CYBSP_LED_ORANGE	LED8
#define CYBSP_LED_WHITE		NC

typedef enum
{
    CYBSP_USER_LED1 = LED9,
    CYBSP_USER_LED2 = LED8,
	CYBSP_USER_LED3 = LED5_RGB_R,
	CYBSP_USER_LED4 = LED5_RGB_G,
	CYBSP_USER_LED5 = LED5_RGB_B,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */

#define SW2		P0_4		// TODO: needs to be configurator-generated
#define SW4		P1_4		// TODO: needs to be configurator-generated

typedef enum
{
    CYBSP_USER_BTN1 = SW2,
	CYBSP_USER_BTN2 = SW4,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;

/** \} group_bsp_cy8ckit_062s2_43012_enums */

/**
* \addtogroup group_bsp_cy8ckit_062s2_43012_macros
* \{
*/

// Generic signal names
/* These need to be generated from configurator, see CONFIGURATORS-1077 */

/** Pin: WCO input */
#define WCO_IN           P0_0
/** Pin: WCO output */
#define WCO_OUT          P0_1

/** Pin: WIFI SDIO D0 */
#define WIFI_SDIO_DAT0     P2_0
/** Pin: WIFI SDIO D1 */
#define WIFI_SDIO_DAT1     P2_1
/** Pin: WIFI SDIO D2 */
#define WIFI_SDIO_DAT2     P2_2
/** Pin: WIFI SDIO D3 */
#define WIFI_SDIO_DAT3     P2_3
/** Pin: WIFI SDIO CMD */
#define WIFI_SDIO_CMD    	P2_4
/** Pin: WIFI SDIO CLK */
#define WIFI_SDIO_CLK    	P2_5
/** Pin: WIFI ON */
#define WIFI_WL_REG_ON   	P2_6
/** Pin: WIFI Host Wakeup */
#define WIFI_WL_HOST_WAKE   P2_7

/** Pin: BT UART RX */
#define BT_UART_RX       P3_0
/** Pin: BT UART TX */
#define BT_UART_TX       P3_1
/** Pin: BT UART RTS */
#define BT_UART_RTS      P3_2
/** Pin: BT UART CTS */
#define BT_UART_CTS      P3_3
/** Pin: BT Power */
#define BT_POWER         P3_4
/** Pin: BT Host Wakeup */
#define BT_HOST_WAKE     P3_5
/** Pin: BT Device Wakeup */
#define BT_DEVICE_WAKE   P4_0

/** Pin: UART RX */
#define UART_RX          P5_0
/** Pin: UART TX */
#define UART_TX          P5_1

/** Pin: I2C SCL */
#define I2C_SCL          P6_0
/** Pin: I2C SDA */
#define I2C_SDA          P6_1

/** Pin: SWO */
#define SWO              P6_4
/** Pin: SWDIO */
#define SWDIO            P6_6
/** Pin: SWDCK */
#define SWDCK            P6_7

/** Pin: CapSesnse TX */
#define CSD_TX           P1_0
/** Pin: CapSesnse CINA */
#define CINA             P7_1
/** Pin: CapSesnse CINB */
#define CINB             P7_2
/** Pin: CapSesnse CMOD */
#define CMOD             P7_7
/** Pin: CapSesnse Button 0 */
#define CSD_BTN0         P8_1
/** Pin: CapSesnse Button 1 */
#define CSD_BTN1         P8_2
/** Pin: CapSesnse Slider 0 */
#define CSD_SLD0         P8_3
/** Pin: CapSesnse Slider 1 */
#define CSD_SLD1         P8_4
/** Pin: CapSesnse Slider 2 */
#define CSD_SLD2         P8_5
/** Pin: CapSesnse Slider 3 */
#define CSD_SLD3         P8_6
/** Pin: CapSesnse Slider 4 */
#define CSD_SLD4         P8_7

/** Pin: QUAD SPI SS */
#define QSPI_SS          P11_2
/** Pin: QUAD SPI D3 */
#define QSPI_D3          P11_3
/** Pin: QUAD SPI D2 */
#define QSPI_D2          P11_4
/** Pin: QUAD SPI D1 */
#define QSPI_D1          P11_5
/** Pin: QUAD SPI D0 */
#define QSPI_D0          P11_6
/** Pin: QUAD SPI SCK */
#define QSPI_SCK         P11_7

/** \} group_bsp_cy8ckit_062s2_43012_macros */




#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8ckit_062s2_43012 */

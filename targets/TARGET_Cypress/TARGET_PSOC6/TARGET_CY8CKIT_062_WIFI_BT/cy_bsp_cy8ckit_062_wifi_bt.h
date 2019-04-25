/***************************************************************************//**
* \file cy_bsp_cy8ckit_062_wifi_bt.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-WIFI-BT pioneer kit.
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
* \addtogroup group_bsp_cy8kit_062_wifi_bt CY8CKIT-062-WIFI-BT
* \ingroup group_bsp
* \{
* The PSoC 6 WiFi-BT Pioneer Kit is a low-cost hardware platform
* that enables design and debug of the PSoC 62 MCU (CY8C6247BZI-D54)
* and the Murata LBEE5KL1DX Module (CYW4343W WiFi + Bluetooth Combo Chip).
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>BLE v5.0</li>
* <li>Serial memory interface</li>
* <li>PDM-PCM digital microphone interface</li>
* <li>Industry-leading CapSense</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>CY8CKIT-062-WIFI-BT evaluation board</li>
* <li>TFT display shield with a 2.4" TFT display, light sensor, 6-axis motion sensor, and digital microphone</li>
* <li>USB cable</li>
* </ul>
*
* \defgroup group_bsp_cy8kit_062_wifi_bt_macros Macros
* \defgroup group_bsp_cy8kit_062_wifi_bt_functions Functions
* \defgroup group_bsp_cy8kit_062_wifi_bt_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_pdl.h"
#include "cy_abstraction_board.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \cond INTERNAL */
#define CY_RSLT_BSP_ERR_WIFI_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 1))

// Generic signal names
#define I2C_SCL            P6_0
#define I2C_SDA            P6_1

#define UART_RX            P5_0
#define UART_TX            P5_1
#define UART_RTS           P5_2
#define UART_CTS           P5_3

#define BT_UART_RX         P3_0
#define BT_UART_TX         P3_1
#define BT_UART_CTS        P3_3
#define BT_UART_RTS        P3_2

#define BT_PIN_POWER       P3_4
#define BT_PIN_HOST_WAKE   P3_5
#define BT_PIN_DEVICE_WAKE P4_0

// Reset pin unavailable
#define SWITCH2            P0_4
#define LED1               P13_7
#define LED2               LED1
#define LED3               NC
#define LED4               NC
#define LED_RED            LED1

#define USER_BUTTON        SWITCH2
#define BUTTON1            USER_BUTTON

#define PDM_DATA           P10_5
#define PDM_CLK            P10_4
#define THERM_OUT_1        P10_1
#define THERM_OUT_2        P10_2
#define THERM_OUT          THERM_OUT_1
#define THERM_VDD          P10_3
#define THERM_GND          P10_0

#define CARD_DETECT_1      P13_5
#define CARD_DETECT_2      P12_1
#define CARD_DETECT        CARD_DETECT_1
#define SD_CMD             P12_4
#define SD_CLK             P12_5
#define SD_IO_0            P13_0
#define SD_IO_1            P13_1
#define SD_IO_2            P13_2
#define SD_IO_3            P13_3

#define QSPI_CLK           P11_7
#define QSPI_IO_0          P11_6
#define QSPI_IO_1          P11_5
#define QSPI_IO_2          P11_4
#define QSPI_IO_3          P11_3
#define QSPI_SEL           P11_2

// Standardized interfaces names
#define STDIO_UART_TX      UART_TX
#define STDIO_UART_RX      UART_RX
#define STDIO_UART_CTS     UART_CTS
#define STDIO_UART_RTS     UART_RTS

#define CY_STDIO_UART_RX   STDIO_UART_RX
#define CY_STDIO_UART_TX   STDIO_UART_TX
#define CY_STDIO_UART_CTS  STDIO_UART_CTS
#define CY_STDIO_UART_RTS  STDIO_UART_RTS

#define CY_BT_UART_RX      BT_UART_RX
#define CY_BT_UART_TX      BT_UART_TX
#define CY_BT_UART_CTS     BT_UART_CTS
#define CY_BT_UART_RTS     BT_UART_RTS

#define CY_BT_PIN_POWER       BT_PIN_POWER
#define CY_BT_PIN_HOST_WAKE   BT_PIN_HOST_WAKE
#define CY_BT_PIN_DEVICE_WAKE BT_PIN_DEVICE_WAKE

#define USBTX              UART_TX
#define USBRX              UART_RX

#define CY_WIFI_SDIO_DATA_0    P2_0
#define CY_WIFI_SDIO_DATA_1    P2_1
#define CY_WIFI_SDIO_DATA_2    P2_2
#define CY_WIFI_SDIO_DATA_3    P2_3
#define CY_WIFI_SDIO_CMD       P2_4
#define CY_WIFI_SDIO_CLK       P2_5
#define CY_WIFI_WL_REG_ON      P2_6
#define CY_WIFI_HOST_WAKE      P2_7

// Not connected
#define AOUT               NC

/** \endcond */
cy_rslt_t init_cycfg_wlan_hw(void);


#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8kit_062_wifi_bt */

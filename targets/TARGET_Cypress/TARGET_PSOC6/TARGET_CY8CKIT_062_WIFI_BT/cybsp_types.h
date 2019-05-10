/***************************************************************************//**
* \file CY8CKIT-062-WIFI-BT/cybsp_types.h
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
#include "cyhal.h"

#pragma once

#define CYBSP_LED_OFF         1U
#define CYBSP_LED_ON          0U
#define CYBSP_SWITCH_OFF      1U
#define CYBSP_SWITCH_ON       0U

typedef enum
{
    CYBSP_LED_RED = P0_3,
    CYBSP_LED_BLUE = P11_1,
    CYBSP_LED9 = P13_7,
    CYBSP_LED_GREEN = P1_1,
    CYBSP_LED8 = P1_5,
} cybsp_led_t;

typedef enum
{
    CYBSP_BTN2 = P0_4,
} cybsp_switch_t;

// Generic signal names
#define CYBSP_WCO_IN           P0_0
#define CYBSP_WCO_OUT          P0_1

#define CYBSP_WIFI_SDIO_D0     P2_0
#define CYBSP_WIFI_SDIO_D1     P2_1
#define CYBSP_WIFI_SDIO_D2     P2_2
#define CYBSP_WIFI_SDIO_D3     P2_3
#define CYBSP_WIFI_SDIO_CMD    P2_4
#define CYBSP_WIFI_SDIO_CLK    P2_5
#define CYBSP_WIFI_WL_REG_ON   P2_6
#define CYBSP_WIFI_HOST_WAKE   P2_7

#define CYBSP_BT_UART_RX       P3_0
#define CYBSP_BT_UART_TX       P3_1
#define CYBSP_BT_UART_RTS      P3_2
#define CYBSP_BT_UART_CTS      P3_3

#define CYBSP_BT_POWER         P3_4
#define CYBSP_BT_HOST_WAKE     P3_5

#define CYBSP_BT_DEVICE_WAKE   P4_0

#define CYBSP_UART_RX          P5_0
#define CYBSP_UART_TX          P5_1

#define CYBSP_I2C_SCL          P6_0
#define CYBSP_I2C_SDA          P6_1

#define CYBSP_SWO              P6_4
#define CYBSP_SWDIO            P6_6
#define CYBSP_SWDCK            P6_7

#define CYBSP_CSD_TX           P1_0
#define CYBSP_CINA             P7_1
#define CYBSP_CINB             P7_2
#define CYBSP_CMOD             P7_7
#define CYBSP_CSD_BTN0         P8_1
#define CYBSP_CSD_BTN1         P8_2
#define CYBSP_CSD_SLD0         P8_3
#define CYBSP_CSD_SLD1         P8_4
#define CYBSP_CSD_SLD2         P8_5
#define CYBSP_CSD_SLD3         P8_6
#define CYBSP_CSD_SLD4         P8_7

#define CYBSP_QSPI_SS          P11_2
#define CYBSP_QSPI_D3          P11_3
#define CYBSP_QSPI_D2          P11_4
#define CYBSP_QSPI_D1          P11_5
#define CYBSP_QSPI_D0          P11_6
#define CYBSP_QSPI_SCK         P11_7

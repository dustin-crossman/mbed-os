/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "PinNamesTypes.h"
#include "cyhal_pin_package.h"
#include "cybsp_types.h"

// Generic signal names

#define I2C_SCL CYBSP_I2C_SCL
#define I2C_SDA CYBSP_I2C_SDA

#define UART_RX CYBSP_DEBUG_UART_RX
#define UART_TX CYBSP_DEBUG_UART_TX
#define UART_RTS P5_2
#define UART_CTS P5_3

#define LED1 CYBSP_USER_LED1
#define LED2 CYBSP_USER_LED2
#define LED3 CYBSP_USER_LED3
#define LED4 CYBSP_USER_LED4
#define LED5 CYBSP_USER_LED5
#define LED_RED CYBSP_LED_RGB_RED
#define LED_BLUE CYBSP_LED_RGB_BLUE
#define LED_GREEN CYBSP_LED_RGB_GREEN

// Reset pin unavailable
#define SWITCH2 CYBSP_SW2
#define SWITCH2 CYBSP_SW4
#define USER_BUTTON SWITCH2
#define BUTTON1 USER_BUTTON
#define BUTTON2 CYBSP_SW4

#define QSPI_CLK CYBSP_QSPI_SCK
#define QSPI_IO_0 CYBSP_QSPI_D0
#define QSPI_IO_1 CYBSP_QSPI_D1
#define QSPI_IO_2 CYBSP_QSPI_D2
#define QSPI_IO_3 CYBSP_QSPI_D3
#define QSPI_SEL CYBSP_QSPI_SS

#define QSPI_FLASH1_IO0 QSPI_IO_0
#define QSPI_FLASH1_IO1 QSPI_IO_1
#define QSPI_FLASH1_IO2 QSPI_IO_2
#define QSPI_FLASH1_IO3 QSPI_IO_3
#define QSPI_FLASH1_SCK QSPI_CLK
#define QSPI_FLASH1_CSN QSPI_SEL

// Standardized interfaces names
#define STDIO_UART_TX UART_TX
#define STDIO_UART_RX UART_RX
#define STDIO_UART_CTS UART_CTS
#define STDIO_UART_RTS UART_RTS

#define USBTX UART_TX
#define USBRX UART_RX

#define AOUT P9_6

#endif

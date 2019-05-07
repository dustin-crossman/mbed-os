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

#include "PeripheralNames.h"
#include "PeripheralPins.h"
#include "pinmap.h"

#if DEVICE_SERIAL
//*** SERIAL ***
const PinMap PinMap_UART_RX[] = {
    {P0_2, UART_0, CY_PIN_IN_FUNCTION( P0_2_SCB0_UART_RX, PCLK_SCB0_CLOCK)},
    {P1_0, UART_7, CY_PIN_IN_FUNCTION( P1_0_SCB7_UART_RX, PCLK_SCB7_CLOCK)},
    {P2_0, UART_1, CY_PIN_IN_FUNCTION( P2_0_SCB1_UART_RX, PCLK_SCB1_CLOCK)},
    {P2_4, UART_9, CY_PIN_IN_FUNCTION( P2_4_SCB9_UART_RX, PCLK_SCB9_CLOCK)},
    {P3_0, UART_2, CY_PIN_IN_FUNCTION( P3_0_SCB2_UART_RX, PCLK_SCB2_CLOCK)},
    {P4_0, UART_7, CY_PIN_IN_FUNCTION( P4_0_SCB7_UART_RX, PCLK_SCB7_CLOCK)},
    {P5_0, UART_5, CY_PIN_IN_FUNCTION( P5_0_SCB5_UART_RX, PCLK_SCB5_CLOCK)},
    {P5_4, UART_10, CY_PIN_IN_FUNCTION( P5_4_SCB10_UART_RX, PCLK_SCB10_CLOCK)},
    {P6_0, UART_3, CY_PIN_IN_FUNCTION( P6_0_SCB3_UART_RX, PCLK_SCB3_CLOCK)},
    {P6_4, UART_6, CY_PIN_IN_FUNCTION( P6_4_SCB6_UART_RX, PCLK_SCB6_CLOCK)},
    {P7_0, UART_4, CY_PIN_IN_FUNCTION( P7_0_SCB4_UART_RX, PCLK_SCB4_CLOCK)},
    {P8_0, UART_4, CY_PIN_IN_FUNCTION( P8_0_SCB4_UART_RX, PCLK_SCB4_CLOCK)},
    {P8_4, UART_11, CY_PIN_IN_FUNCTION( P8_4_SCB11_UART_RX, PCLK_SCB11_CLOCK)},
    {P9_0, UART_2, CY_PIN_IN_FUNCTION( P9_0_SCB2_UART_RX, PCLK_SCB2_CLOCK)},
    {P10_0, UART_1, CY_PIN_IN_FUNCTION( P10_0_SCB1_UART_RX, PCLK_SCB1_CLOCK)},
    {P11_0, UART_5, CY_PIN_IN_FUNCTION( P11_0_SCB5_UART_RX, PCLK_SCB5_CLOCK)},
    {P12_0, UART_6, CY_PIN_IN_FUNCTION( P12_0_SCB6_UART_RX, PCLK_SCB6_CLOCK)},
    {P13_0, UART_6, CY_PIN_IN_FUNCTION( P13_0_SCB6_UART_RX, PCLK_SCB6_CLOCK)},
    {P13_4, UART_12, CY_PIN_IN_FUNCTION( P13_4_SCB12_UART_RX, PCLK_SCB12_CLOCK)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_TX[] = {
    {P0_3, UART_0, CY_PIN_OUT_FUNCTION( P0_3_SCB0_UART_TX, PCLK_SCB0_CLOCK)},
    {P1_1, UART_7, CY_PIN_OUT_FUNCTION( P1_1_SCB7_UART_TX, PCLK_SCB7_CLOCK)},
    {P2_1, UART_1, CY_PIN_OUT_FUNCTION( P2_1_SCB1_UART_TX, PCLK_SCB1_CLOCK)},
    {P2_5, UART_9, CY_PIN_OUT_FUNCTION( P2_5_SCB9_UART_TX, PCLK_SCB9_CLOCK)},
    {P3_1, UART_2, CY_PIN_OUT_FUNCTION( P3_1_SCB2_UART_TX, PCLK_SCB2_CLOCK)},
    {P4_1, UART_7, CY_PIN_OUT_FUNCTION( P4_1_SCB7_UART_TX, PCLK_SCB7_CLOCK)},
    {P5_1, UART_5, CY_PIN_OUT_FUNCTION( P5_1_SCB5_UART_TX, PCLK_SCB5_CLOCK)},
    {P5_5, UART_10, CY_PIN_OUT_FUNCTION( P5_5_SCB10_UART_TX, PCLK_SCB10_CLOCK)},
    {P6_1, UART_3, CY_PIN_OUT_FUNCTION( P6_1_SCB3_UART_TX, PCLK_SCB3_CLOCK)},
    {P6_5, UART_6, CY_PIN_OUT_FUNCTION( P6_5_SCB6_UART_TX, PCLK_SCB6_CLOCK)},
    {P7_1, UART_4, CY_PIN_OUT_FUNCTION( P7_1_SCB4_UART_TX, PCLK_SCB4_CLOCK)},
    {P8_1, UART_4, CY_PIN_OUT_FUNCTION( P8_1_SCB4_UART_TX, PCLK_SCB4_CLOCK)},
    {P8_5, UART_11, CY_PIN_OUT_FUNCTION( P8_5_SCB11_UART_TX, PCLK_SCB11_CLOCK)},
    {P9_1, UART_2, CY_PIN_OUT_FUNCTION( P9_1_SCB2_UART_TX, PCLK_SCB2_CLOCK)},
    {P10_1, UART_1, CY_PIN_OUT_FUNCTION( P10_1_SCB1_UART_TX, PCLK_SCB1_CLOCK)},
    {P11_1, UART_5, CY_PIN_OUT_FUNCTION( P11_1_SCB5_UART_TX, PCLK_SCB5_CLOCK)},
    {P12_1, UART_6, CY_PIN_OUT_FUNCTION( P12_1_SCB6_UART_TX, PCLK_SCB6_CLOCK)},
    {P13_1, UART_6, CY_PIN_OUT_FUNCTION( P13_1_SCB6_UART_TX, PCLK_SCB6_CLOCK)},
    {P13_5, UART_12, CY_PIN_OUT_FUNCTION( P13_5_SCB12_UART_TX, PCLK_SCB12_CLOCK)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_RTS[] = {
    {P0_4, UART_0, CY_PIN_OUT_FUNCTION( P0_4_SCB0_UART_RTS, PCLK_SCB0_CLOCK)},
    {P1_2, UART_7, CY_PIN_OUT_FUNCTION( P1_2_SCB7_UART_RTS, PCLK_SCB7_CLOCK)},
    {P2_2, UART_1, CY_PIN_OUT_FUNCTION( P2_2_SCB1_UART_RTS, PCLK_SCB1_CLOCK)},
    {P2_6, UART_9, CY_PIN_OUT_FUNCTION( P2_6_SCB9_UART_RTS, PCLK_SCB9_CLOCK)},
    {P3_2, UART_2, CY_PIN_OUT_FUNCTION( P3_2_SCB2_UART_RTS, PCLK_SCB2_CLOCK)},
    {P5_2, UART_5, CY_PIN_OUT_FUNCTION( P5_2_SCB5_UART_RTS, PCLK_SCB5_CLOCK)},
    {P5_6, UART_10, CY_PIN_OUT_FUNCTION( P5_6_SCB10_UART_RTS, PCLK_SCB10_CLOCK)},
    {P6_2, UART_3, CY_PIN_OUT_FUNCTION( P6_2_SCB3_UART_RTS, PCLK_SCB3_CLOCK)},
    {P6_6, UART_6, CY_PIN_OUT_FUNCTION( P6_6_SCB6_UART_RTS, PCLK_SCB6_CLOCK)},
    {P7_2, UART_4, CY_PIN_OUT_FUNCTION( P7_2_SCB4_UART_RTS, PCLK_SCB4_CLOCK)},
    {P8_2, UART_4, CY_PIN_OUT_FUNCTION( P8_2_SCB4_UART_RTS, PCLK_SCB4_CLOCK)},
    {P8_6, UART_11, CY_PIN_OUT_FUNCTION( P8_6_SCB11_UART_RTS, PCLK_SCB11_CLOCK)},
    {P9_2, UART_2, CY_PIN_OUT_FUNCTION( P9_2_SCB2_UART_RTS, PCLK_SCB2_CLOCK)},
    {P10_2, UART_1, CY_PIN_OUT_FUNCTION( P10_2_SCB1_UART_RTS, PCLK_SCB1_CLOCK)},
    {P11_2, UART_5, CY_PIN_OUT_FUNCTION( P11_2_SCB5_UART_RTS, PCLK_SCB5_CLOCK)},
    {P12_2, UART_6, CY_PIN_OUT_FUNCTION( P12_2_SCB6_UART_RTS, PCLK_SCB6_CLOCK)},
    {P13_2, UART_6, CY_PIN_OUT_FUNCTION( P13_2_SCB6_UART_RTS, PCLK_SCB6_CLOCK)},
    {P13_6, UART_12, CY_PIN_OUT_FUNCTION( P13_6_SCB12_UART_RTS, PCLK_SCB12_CLOCK)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_CTS[] = {
    {P0_5, UART_0, CY_PIN_IN_FUNCTION( P0_5_SCB0_UART_CTS, PCLK_SCB0_CLOCK)},
    {P1_3, UART_7, CY_PIN_IN_FUNCTION( P1_3_SCB7_UART_CTS, PCLK_SCB7_CLOCK)},
    {P2_3, UART_1, CY_PIN_IN_FUNCTION( P2_3_SCB1_UART_CTS, PCLK_SCB1_CLOCK)},
    {P2_7, UART_9, CY_PIN_IN_FUNCTION( P2_7_SCB9_UART_CTS, PCLK_SCB9_CLOCK)},
    {P3_3, UART_2, CY_PIN_IN_FUNCTION( P3_3_SCB2_UART_CTS, PCLK_SCB2_CLOCK)},
    {P5_3, UART_5, CY_PIN_IN_FUNCTION( P5_3_SCB5_UART_CTS, PCLK_SCB5_CLOCK)},
    {P5_7, UART_10, CY_PIN_IN_FUNCTION( P5_7_SCB10_UART_CTS, PCLK_SCB10_CLOCK)},
    {P6_3, UART_3, CY_PIN_IN_FUNCTION( P6_3_SCB3_UART_CTS, PCLK_SCB3_CLOCK)},
    {P6_7, UART_6, CY_PIN_IN_FUNCTION( P6_7_SCB6_UART_CTS, PCLK_SCB6_CLOCK)},
    {P7_3, UART_4, CY_PIN_IN_FUNCTION( P7_3_SCB4_UART_CTS, PCLK_SCB4_CLOCK)},
    {P8_3, UART_4, CY_PIN_IN_FUNCTION( P8_3_SCB4_UART_CTS, PCLK_SCB4_CLOCK)},
    {P8_7, UART_11, CY_PIN_IN_FUNCTION( P8_7_SCB11_UART_CTS, PCLK_SCB11_CLOCK)},
    {P9_3, UART_2, CY_PIN_IN_FUNCTION( P9_3_SCB2_UART_CTS, PCLK_SCB2_CLOCK)},
    {P10_3, UART_1, CY_PIN_IN_FUNCTION( P10_3_SCB1_UART_CTS, PCLK_SCB1_CLOCK)},
    {P11_3, UART_5, CY_PIN_IN_FUNCTION( P11_3_SCB5_UART_CTS, PCLK_SCB5_CLOCK)},
    {P12_3, UART_6, CY_PIN_IN_FUNCTION( P12_3_SCB6_UART_CTS, PCLK_SCB6_CLOCK)},
    {P13_3, UART_6, CY_PIN_IN_FUNCTION( P13_3_SCB6_UART_CTS, PCLK_SCB6_CLOCK)},
    {P13_7, UART_12, CY_PIN_IN_FUNCTION( P13_7_SCB12_UART_CTS, PCLK_SCB12_CLOCK)},
    {NC,    NC,     0}
};
#endif // DEVICE_SERIAL


#if DEVICE_I2C
//*** I2C ***
const PinMap PinMap_I2C_SCL[] = {
    {P0_2, I2C_0, CY_PIN_OD_FUNCTION( P0_2_SCB0_I2C_SCL, PCLK_SCB0_CLOCK)},
    {P1_0, I2C_7, CY_PIN_OD_FUNCTION( P1_0_SCB7_I2C_SCL, PCLK_SCB7_CLOCK)},
    {P2_0, I2C_1, CY_PIN_OD_FUNCTION( P2_0_SCB1_I2C_SCL, PCLK_SCB1_CLOCK)},
    {P2_4, I2C_9, CY_PIN_OD_FUNCTION( P2_4_SCB9_I2C_SCL, PCLK_SCB9_CLOCK)},
    {P3_0, I2C_2, CY_PIN_OD_FUNCTION( P3_0_SCB2_I2C_SCL, PCLK_SCB2_CLOCK)},
    {P4_0, I2C_7, CY_PIN_OD_FUNCTION( P4_0_SCB7_I2C_SCL, PCLK_SCB7_CLOCK)},
    {P5_0, I2C_5, CY_PIN_OD_FUNCTION( P5_0_SCB5_I2C_SCL, PCLK_SCB5_CLOCK)},
    {P5_4, I2C_10, CY_PIN_OD_FUNCTION( P5_4_SCB10_I2C_SCL, PCLK_SCB10_CLOCK)},
    {P6_0, I2C_3, CY_PIN_OD_FUNCTION( P6_0_SCB3_I2C_SCL, PCLK_SCB3_CLOCK)},
    {P6_4, I2C_6, CY_PIN_OD_FUNCTION( P6_4_SCB6_I2C_SCL, PCLK_SCB6_CLOCK)},
    {P7_0, I2C_4, CY_PIN_OD_FUNCTION( P7_0_SCB4_I2C_SCL, PCLK_SCB4_CLOCK)},
    {P8_0, I2C_4, CY_PIN_OD_FUNCTION( P8_0_SCB4_I2C_SCL, PCLK_SCB4_CLOCK)},
    {P8_4, I2C_11, CY_PIN_OD_FUNCTION( P8_4_SCB11_I2C_SCL, PCLK_SCB11_CLOCK)},
    {P9_0, I2C_2, CY_PIN_OD_FUNCTION( P9_0_SCB2_I2C_SCL, PCLK_SCB2_CLOCK)},
    {P10_0, I2C_1, CY_PIN_OD_FUNCTION( P10_0_SCB1_I2C_SCL, PCLK_SCB1_CLOCK)},
    {P11_0, I2C_5, CY_PIN_OD_FUNCTION( P11_0_SCB5_I2C_SCL, PCLK_SCB5_CLOCK)},
    {P12_0, I2C_6, CY_PIN_OD_FUNCTION( P12_0_SCB6_I2C_SCL, PCLK_SCB6_CLOCK)},
    {P13_0, I2C_6, CY_PIN_OD_FUNCTION( P13_0_SCB6_I2C_SCL, PCLK_SCB6_CLOCK)},
    {P13_4, I2C_12, CY_PIN_OD_FUNCTION( P13_4_SCB12_I2C_SCL, PCLK_SCB12_CLOCK)},
    {NC,    NC,    0}
};
const PinMap PinMap_I2C_SDA[] = {
    {P0_3, I2C_0, CY_PIN_OD_FUNCTION( P0_3_SCB0_I2C_SDA, PCLK_SCB0_CLOCK)},
    {P1_1, I2C_7, CY_PIN_OD_FUNCTION( P1_1_SCB7_I2C_SDA, PCLK_SCB7_CLOCK)},
    {P2_1, I2C_1, CY_PIN_OD_FUNCTION( P2_1_SCB1_I2C_SDA, PCLK_SCB1_CLOCK)},
    {P2_5, I2C_9, CY_PIN_OD_FUNCTION( P2_5_SCB9_I2C_SDA, PCLK_SCB9_CLOCK)},
    {P3_1, I2C_2, CY_PIN_OD_FUNCTION( P3_1_SCB2_I2C_SDA, PCLK_SCB2_CLOCK)},
    {P4_1, I2C_7, CY_PIN_OD_FUNCTION( P4_1_SCB7_I2C_SDA, PCLK_SCB7_CLOCK)},
    {P5_1, I2C_5, CY_PIN_OD_FUNCTION( P5_1_SCB5_I2C_SDA, PCLK_SCB5_CLOCK)},
    {P5_5, I2C_10, CY_PIN_OD_FUNCTION( P5_5_SCB10_I2C_SDA, PCLK_SCB10_CLOCK)},
    {P6_1, I2C_3, CY_PIN_OD_FUNCTION( P6_1_SCB3_I2C_SDA, PCLK_SCB3_CLOCK)},
    {P6_5, I2C_6, CY_PIN_OD_FUNCTION( P6_5_SCB6_I2C_SDA, PCLK_SCB6_CLOCK)},
    {P7_1, I2C_4, CY_PIN_OD_FUNCTION( P7_1_SCB4_I2C_SDA, PCLK_SCB4_CLOCK)},
    {P8_1, I2C_4, CY_PIN_OD_FUNCTION( P8_1_SCB4_I2C_SDA, PCLK_SCB4_CLOCK)},
    {P8_5, I2C_11, CY_PIN_OD_FUNCTION( P8_5_SCB11_I2C_SDA, PCLK_SCB11_CLOCK)},
    {P9_1, I2C_2, CY_PIN_OD_FUNCTION( P9_1_SCB2_I2C_SDA, PCLK_SCB2_CLOCK)},
    {P10_1, I2C_1, CY_PIN_OD_FUNCTION( P10_1_SCB1_I2C_SDA, PCLK_SCB1_CLOCK)},
    {P11_1, I2C_5, CY_PIN_OD_FUNCTION( P11_1_SCB5_I2C_SDA, PCLK_SCB5_CLOCK)},
    {P12_1, I2C_6, CY_PIN_OD_FUNCTION( P12_1_SCB6_I2C_SDA, PCLK_SCB6_CLOCK)},
    {P13_1, I2C_6, CY_PIN_OD_FUNCTION( P13_1_SCB6_I2C_SDA, PCLK_SCB6_CLOCK)},
    {P13_5, I2C_12, CY_PIN_OD_FUNCTION( P13_5_SCB12_I2C_SDA, PCLK_SCB12_CLOCK)},
    {NC,    NC,    0}
};
#endif // DEVICE_I2C

#if DEVICE_SPI
//*** SPI ***
const PinMap PinMap_SPI_MOSI[] = {
    {P0_2, SPI_0, CY_PIN_OUT_FUNCTION( P0_2_SCB0_SPI_MOSI, PCLK_SCB0_CLOCK)},
    {P1_0, SPI_7, CY_PIN_OUT_FUNCTION( P1_0_SCB7_SPI_MOSI, PCLK_SCB7_CLOCK)},
    {P2_0, SPI_1, CY_PIN_OUT_FUNCTION( P2_0_SCB1_SPI_MOSI, PCLK_SCB1_CLOCK)},
    {P3_0, SPI_2, CY_PIN_OUT_FUNCTION( P3_0_SCB2_SPI_MOSI, PCLK_SCB2_CLOCK)},
    {P4_0, SPI_7, CY_PIN_OUT_FUNCTION( P4_0_SCB7_SPI_MOSI, PCLK_SCB7_CLOCK)},
    {P5_0, SPI_5, CY_PIN_OUT_FUNCTION( P5_0_SCB5_SPI_MOSI, PCLK_SCB5_CLOCK)},
    {P6_0, SPI_3, CY_PIN_OUT_FUNCTION( P6_0_SCB3_SPI_MOSI, PCLK_SCB3_CLOCK)},
    {P6_4, SPI_6, CY_PIN_OUT_FUNCTION( P6_4_SCB6_SPI_MOSI, PCLK_SCB6_CLOCK)},
    {P7_0, SPI_4, CY_PIN_OUT_FUNCTION( P7_0_SCB4_SPI_MOSI, PCLK_SCB4_CLOCK)},
    {P8_0, SPI_4, CY_PIN_OUT_FUNCTION( P8_0_SCB4_SPI_MOSI, PCLK_SCB4_CLOCK)},
    {P9_0, SPI_2, CY_PIN_OUT_FUNCTION( P9_0_SCB2_SPI_MOSI, PCLK_SCB2_CLOCK)},
    {P10_0, SPI_1, CY_PIN_OUT_FUNCTION( P10_0_SCB1_SPI_MOSI, PCLK_SCB1_CLOCK)},
    {P11_0, SPI_5, CY_PIN_OUT_FUNCTION( P11_0_SCB5_SPI_MOSI, PCLK_SCB5_CLOCK)},
    {P12_0, SPI_6, CY_PIN_OUT_FUNCTION( P12_0_SCB6_SPI_MOSI, PCLK_SCB6_CLOCK)},
    {P13_0, SPI_6, CY_PIN_OUT_FUNCTION( P13_0_SCB6_SPI_MOSI, PCLK_SCB6_CLOCK)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_MISO[] = {
    {P0_3, SPI_0, CY_PIN_IN_FUNCTION( P0_3_SCB0_SPI_MISO, PCLK_SCB0_CLOCK)},
    {P1_1, SPI_7, CY_PIN_IN_FUNCTION( P1_1_SCB7_SPI_MISO, PCLK_SCB7_CLOCK)},
    {P2_1, SPI_1, CY_PIN_IN_FUNCTION( P2_1_SCB1_SPI_MISO, PCLK_SCB1_CLOCK)},
    {P3_1, SPI_2, CY_PIN_IN_FUNCTION( P3_1_SCB2_SPI_MISO, PCLK_SCB2_CLOCK)},
    {P4_1, SPI_7, CY_PIN_IN_FUNCTION( P4_1_SCB7_SPI_MISO, PCLK_SCB7_CLOCK)},
    {P5_1, SPI_5, CY_PIN_IN_FUNCTION( P5_1_SCB5_SPI_MISO, PCLK_SCB5_CLOCK)},
    {P6_1, SPI_3, CY_PIN_IN_FUNCTION( P6_1_SCB3_SPI_MISO, PCLK_SCB3_CLOCK)},
    {P6_5, SPI_6, CY_PIN_IN_FUNCTION( P6_5_SCB6_SPI_MISO, PCLK_SCB6_CLOCK)},
    {P7_1, SPI_4, CY_PIN_IN_FUNCTION( P7_1_SCB4_SPI_MISO, PCLK_SCB4_CLOCK)},
    {P8_1, SPI_4, CY_PIN_IN_FUNCTION( P8_1_SCB4_SPI_MISO, PCLK_SCB4_CLOCK)},
    {P9_1, SPI_2, CY_PIN_IN_FUNCTION( P9_1_SCB2_SPI_MISO, PCLK_SCB2_CLOCK)},
    {P10_1, SPI_1, CY_PIN_IN_FUNCTION( P10_1_SCB1_SPI_MISO, PCLK_SCB1_CLOCK)},
    {P11_1, SPI_5, CY_PIN_IN_FUNCTION( P11_1_SCB5_SPI_MISO, PCLK_SCB5_CLOCK)},
    {P12_1, SPI_6, CY_PIN_IN_FUNCTION( P12_1_SCB6_SPI_MISO, PCLK_SCB6_CLOCK)},
    {P13_1, SPI_6, CY_PIN_IN_FUNCTION( P13_1_SCB6_SPI_MISO, PCLK_SCB6_CLOCK)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_SCLK[] = {
    {P0_4, SPI_0, CY_PIN_OUT_FUNCTION( P0_4_SCB0_SPI_CLK, PCLK_SCB0_CLOCK)},
    {P1_2, SPI_7, CY_PIN_OUT_FUNCTION( P1_2_SCB7_SPI_CLK, PCLK_SCB7_CLOCK)},
    {P2_2, SPI_1, CY_PIN_OUT_FUNCTION( P2_2_SCB1_SPI_CLK, PCLK_SCB1_CLOCK)},
    {P3_2, SPI_2, CY_PIN_OUT_FUNCTION( P3_2_SCB2_SPI_CLK, PCLK_SCB2_CLOCK)},
    {P5_2, SPI_5, CY_PIN_OUT_FUNCTION( P5_2_SCB5_SPI_CLK, PCLK_SCB5_CLOCK)},
    {P6_2, SPI_3, CY_PIN_OUT_FUNCTION( P6_2_SCB3_SPI_CLK, PCLK_SCB3_CLOCK)},
    {P6_6, SPI_6, CY_PIN_OUT_FUNCTION( P6_6_SCB6_SPI_CLK, PCLK_SCB6_CLOCK)},
    {P7_2, SPI_4, CY_PIN_OUT_FUNCTION( P7_2_SCB4_SPI_CLK, PCLK_SCB4_CLOCK)},
    {P8_2, SPI_4, CY_PIN_OUT_FUNCTION( P8_2_SCB4_SPI_CLK, PCLK_SCB4_CLOCK)},
    {P9_2, SPI_2, CY_PIN_OUT_FUNCTION( P9_2_SCB2_SPI_CLK, PCLK_SCB2_CLOCK)},
    {P10_2, SPI_1, CY_PIN_OUT_FUNCTION( P10_2_SCB1_SPI_CLK, PCLK_SCB1_CLOCK)},
    {P11_2, SPI_5, CY_PIN_OUT_FUNCTION( P11_2_SCB5_SPI_CLK, PCLK_SCB5_CLOCK)},
    {P12_2, SPI_6, CY_PIN_OUT_FUNCTION( P12_2_SCB6_SPI_CLK, PCLK_SCB6_CLOCK)},
    {P13_2, SPI_6, CY_PIN_OUT_FUNCTION( P13_2_SCB6_SPI_CLK, PCLK_SCB6_CLOCK)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_SSEL[] = {
    {P0_5, SPI_0, CY_PIN_OUT_FUNCTION( P0_5_SCB0_SPI_SELECT0, PCLK_SCB0_CLOCK)},
    {P1_3, SPI_7, CY_PIN_OUT_FUNCTION( P1_3_SCB7_SPI_SELECT0, PCLK_SCB7_CLOCK)},
    {P2_3, SPI_1, CY_PIN_OUT_FUNCTION( P2_3_SCB1_SPI_SELECT0, PCLK_SCB1_CLOCK)},
    {P3_3, SPI_2, CY_PIN_OUT_FUNCTION( P3_3_SCB2_SPI_SELECT0, PCLK_SCB2_CLOCK)},
    {P5_3, SPI_5, CY_PIN_OUT_FUNCTION( P5_3_SCB5_SPI_SELECT0, PCLK_SCB5_CLOCK)},
    {P6_3, SPI_3, CY_PIN_OUT_FUNCTION( P6_3_SCB3_SPI_SELECT0, PCLK_SCB3_CLOCK)},
    {P6_7, SPI_6, CY_PIN_OUT_FUNCTION( P6_7_SCB6_SPI_SELECT0, PCLK_SCB6_CLOCK)},
    {P7_3, SPI_4, CY_PIN_OUT_FUNCTION( P7_3_SCB4_SPI_SELECT0, PCLK_SCB4_CLOCK)},
    {P8_3, SPI_4, CY_PIN_OUT_FUNCTION( P8_3_SCB4_SPI_SELECT0, PCLK_SCB4_CLOCK)},
    {P9_3, SPI_2, CY_PIN_OUT_FUNCTION( P9_3_SCB2_SPI_SELECT0, PCLK_SCB2_CLOCK)},
    {P10_3, SPI_1, CY_PIN_OUT_FUNCTION( P10_3_SCB1_SPI_SELECT0, PCLK_SCB1_CLOCK)},
    {P11_3, SPI_5, CY_PIN_OUT_FUNCTION( P11_3_SCB5_SPI_SELECT0, PCLK_SCB5_CLOCK)},
    {P12_3, SPI_6, CY_PIN_OUT_FUNCTION( P12_3_SCB6_SPI_SELECT0, PCLK_SCB6_CLOCK)},
    {P13_3, SPI_6, CY_PIN_OUT_FUNCTION( P13_3_SCB6_SPI_SELECT0, PCLK_SCB6_CLOCK)},
    {NC,    NC,    0}
};
#endif // DEVICE_SPI

#if DEVICE_PWMOUT
//*** PWM ***
const PinMap PinMap_PWM_OUT[] = {
    // 16-bit PWM outputs
    {P0_0, PWM_16b_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0, PCLK_TCPWM1_CLOCKS0)},
    {P0_2, PWM_16b_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM1_LINE1, PCLK_TCPWM1_CLOCKS1)},
    {P0_4, PWM_16b_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2, PCLK_TCPWM1_CLOCKS2)},
    {P1_0, PWM_16b_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM1_LINE3, PCLK_TCPWM1_CLOCKS3)},
    {P1_2, PWM_16b_12, CY_PIN_OUT_FUNCTION(P1_2_TCPWM1_LINE12, PCLK_TCPWM1_CLOCKS12)},
    {P1_4, PWM_16b_13, CY_PIN_OUT_FUNCTION(P1_4_TCPWM1_LINE13, PCLK_TCPWM1_CLOCKS13)},
    {P2_0, PWM_16b_15, CY_PIN_OUT_FUNCTION(P2_0_TCPWM1_LINE15, PCLK_TCPWM1_CLOCKS15)},
    {P2_2, PWM_16b_16, CY_PIN_OUT_FUNCTION(P2_2_TCPWM1_LINE16, PCLK_TCPWM1_CLOCKS16)},
    {P2_4, PWM_16b_17, CY_PIN_OUT_FUNCTION(P2_4_TCPWM1_LINE17, PCLK_TCPWM1_CLOCKS17)},
    {P2_6, PWM_16b_18, CY_PIN_OUT_FUNCTION(P2_6_TCPWM1_LINE18, PCLK_TCPWM1_CLOCKS18)},
    {P3_0, PWM_16b_19, CY_PIN_OUT_FUNCTION(P3_0_TCPWM1_LINE19, PCLK_TCPWM1_CLOCKS19)},
    {P3_2, PWM_16b_20, CY_PIN_OUT_FUNCTION(P3_2_TCPWM1_LINE20, PCLK_TCPWM1_CLOCKS20)},
    {P3_4, PWM_16b_21, CY_PIN_OUT_FUNCTION(P3_4_TCPWM1_LINE21, PCLK_TCPWM1_CLOCKS21)},
    {P4_0, PWM_16b_22, CY_PIN_OUT_FUNCTION(P4_0_TCPWM1_LINE22, PCLK_TCPWM1_CLOCKS22)},
    {P5_0, PWM_16b_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE4, PCLK_TCPWM1_CLOCKS4)},
    {P5_2, PWM_16b_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM1_LINE5, PCLK_TCPWM1_CLOCKS5)},
    {P5_4, PWM_16b_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM1_LINE6, PCLK_TCPWM1_CLOCKS6)},
    {P5_6, PWM_16b_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE7, PCLK_TCPWM1_CLOCKS7)},
    {P6_0, PWM_16b_8, CY_PIN_OUT_FUNCTION(P6_0_TCPWM1_LINE8, PCLK_TCPWM1_CLOCKS8)},
    {P6_2, PWM_16b_9, CY_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE9, PCLK_TCPWM1_CLOCKS9)},
    {P6_4, PWM_16b_10, CY_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE10, PCLK_TCPWM1_CLOCKS10)},
    {P6_6, PWM_16b_11, CY_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE11, PCLK_TCPWM1_CLOCKS11)},
    {P7_0, PWM_16b_12, CY_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE12, PCLK_TCPWM1_CLOCKS12)},
    {P7_2, PWM_16b_13, CY_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE13, PCLK_TCPWM1_CLOCKS13)},
    {P7_4, PWM_16b_14, CY_PIN_OUT_FUNCTION(P7_4_TCPWM1_LINE14, PCLK_TCPWM1_CLOCKS14)},
    {P7_6, PWM_16b_15, CY_PIN_OUT_FUNCTION(P7_6_TCPWM1_LINE15, PCLK_TCPWM1_CLOCKS15)},
    {P8_0, PWM_16b_16, CY_PIN_OUT_FUNCTION(P8_0_TCPWM1_LINE16, PCLK_TCPWM1_CLOCKS16)},
    {P8_2, PWM_16b_17, CY_PIN_OUT_FUNCTION(P8_2_TCPWM1_LINE17, PCLK_TCPWM1_CLOCKS17)},
    {P8_4, PWM_16b_18, CY_PIN_OUT_FUNCTION(P8_4_TCPWM1_LINE18, PCLK_TCPWM1_CLOCKS18)},
    {P8_6, PWM_16b_19, CY_PIN_OUT_FUNCTION(P8_6_TCPWM1_LINE19, PCLK_TCPWM1_CLOCKS19)},
    {P9_0, PWM_16b_20, CY_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE20, PCLK_TCPWM1_CLOCKS20)},
    {P9_2, PWM_16b_21, CY_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE21, PCLK_TCPWM1_CLOCKS21)},
    {P9_4, PWM_16b_0, CY_PIN_OUT_FUNCTION(P9_4_TCPWM1_LINE0, PCLK_TCPWM1_CLOCKS0)},
    {P9_6, PWM_16b_1, CY_PIN_OUT_FUNCTION(P9_6_TCPWM1_LINE1, PCLK_TCPWM1_CLOCKS1)},
    {P10_0, PWM_16b_22, CY_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE22, PCLK_TCPWM1_CLOCKS22)},
    {P10_2, PWM_16b_23, CY_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE23, PCLK_TCPWM1_CLOCKS23)},
    {P10_4, PWM_16b_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0, PCLK_TCPWM1_CLOCKS0)},
    {P10_6, PWM_16b_2, CY_PIN_OUT_FUNCTION(P10_6_TCPWM1_LINE2, PCLK_TCPWM1_CLOCKS2)},
    {P11_0, PWM_16b_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM1_LINE1, PCLK_TCPWM1_CLOCKS1)},
    {P11_2, PWM_16b_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE2, PCLK_TCPWM1_CLOCKS2)},
    {P11_4, PWM_16b_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE3, PCLK_TCPWM1_CLOCKS3)},
    {P12_0, PWM_16b_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM1_LINE4, PCLK_TCPWM1_CLOCKS4)},
    {P12_2, PWM_16b_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM1_LINE5, PCLK_TCPWM1_CLOCKS5)},
    {P12_4, PWM_16b_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM1_LINE6, PCLK_TCPWM1_CLOCKS6)},
    {P12_6, PWM_16b_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7, PCLK_TCPWM1_CLOCKS7)},
    {P13_0, PWM_16b_8, CY_PIN_OUT_FUNCTION(P13_0_TCPWM1_LINE8, PCLK_TCPWM1_CLOCKS8)},
    {P13_2, PWM_16b_9, CY_PIN_OUT_FUNCTION(P13_2_TCPWM1_LINE9, PCLK_TCPWM1_CLOCKS9)},
    {P13_4, PWM_16b_10, CY_PIN_OUT_FUNCTION(P13_4_TCPWM1_LINE10, PCLK_TCPWM1_CLOCKS10)},
    {P13_6, PWM_16b_11, CY_PIN_OUT_FUNCTION(P13_6_TCPWM1_LINE11, PCLK_TCPWM1_CLOCKS11)},
    // 16-bit PWM inverted outputs
    {P0_1, PWM_16b_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0, PCLK_TCPWM1_CLOCKS0)},
    {P0_3, PWM_16b_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM1_LINE_COMPL1, PCLK_TCPWM1_CLOCKS1)},
    {P0_5, PWM_16b_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2, PCLK_TCPWM1_CLOCKS2)},
    {P1_1, PWM_16b_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM1_LINE_COMPL3, PCLK_TCPWM1_CLOCKS3)},
    {P1_3, PWM_16b_12, CY_PIN_OUT_FUNCTION(P1_3_TCPWM1_LINE_COMPL12, PCLK_TCPWM1_CLOCKS12)},
    {P1_5, PWM_16b_14, CY_PIN_OUT_FUNCTION(P1_5_TCPWM1_LINE_COMPL14, PCLK_TCPWM1_CLOCKS14)},
    {P2_1, PWM_16b_15, CY_PIN_OUT_FUNCTION(P2_1_TCPWM1_LINE_COMPL15, PCLK_TCPWM1_CLOCKS15)},
    {P2_3, PWM_16b_16, CY_PIN_OUT_FUNCTION(P2_3_TCPWM1_LINE_COMPL16, PCLK_TCPWM1_CLOCKS16)},
    {P2_5, PWM_16b_17, CY_PIN_OUT_FUNCTION(P2_5_TCPWM1_LINE_COMPL17, PCLK_TCPWM1_CLOCKS17)},
    {P2_7, PWM_16b_18, CY_PIN_OUT_FUNCTION(P2_7_TCPWM1_LINE_COMPL18, PCLK_TCPWM1_CLOCKS18)},
    {P3_1, PWM_16b_19, CY_PIN_OUT_FUNCTION(P3_1_TCPWM1_LINE_COMPL19, PCLK_TCPWM1_CLOCKS19)},
    {P3_3, PWM_16b_20, CY_PIN_OUT_FUNCTION(P3_3_TCPWM1_LINE_COMPL20, PCLK_TCPWM1_CLOCKS20)},
    {P3_5, PWM_16b_21, CY_PIN_OUT_FUNCTION(P3_5_TCPWM1_LINE_COMPL21, PCLK_TCPWM1_CLOCKS21)},
    {P4_1, PWM_16b_22, CY_PIN_OUT_FUNCTION(P4_1_TCPWM1_LINE_COMPL22, PCLK_TCPWM1_CLOCKS22)},
    {P5_1, PWM_16b_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL4, PCLK_TCPWM1_CLOCKS4)},
    {P5_3, PWM_16b_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM1_LINE_COMPL5, PCLK_TCPWM1_CLOCKS5)},
    {P5_5, PWM_16b_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM1_LINE_COMPL6, PCLK_TCPWM1_CLOCKS6)},
    {P5_7, PWM_16b_7, CY_PIN_OUT_FUNCTION(P5_7_TCPWM1_LINE_COMPL7, PCLK_TCPWM1_CLOCKS7)},
    {P6_1, PWM_16b_8, CY_PIN_OUT_FUNCTION(P6_1_TCPWM1_LINE_COMPL8, PCLK_TCPWM1_CLOCKS8)},
    {P6_3, PWM_16b_9, CY_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL9, PCLK_TCPWM1_CLOCKS9)},
    {P6_5, PWM_16b_10, CY_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL10, PCLK_TCPWM1_CLOCKS10)},
    {P6_7, PWM_16b_11, CY_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL11, PCLK_TCPWM1_CLOCKS11)},
    {P7_1, PWM_16b_12, CY_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL12, PCLK_TCPWM1_CLOCKS12)},
    {P7_3, PWM_16b_13, CY_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL13, PCLK_TCPWM1_CLOCKS13)},
    {P7_5, PWM_16b_14, CY_PIN_OUT_FUNCTION(P7_5_TCPWM1_LINE_COMPL14, PCLK_TCPWM1_CLOCKS14)},
    {P7_7, PWM_16b_15, CY_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL15, PCLK_TCPWM1_CLOCKS15)},
    {P8_1, PWM_16b_16, CY_PIN_OUT_FUNCTION(P8_1_TCPWM1_LINE_COMPL16, PCLK_TCPWM1_CLOCKS16)},
    {P8_3, PWM_16b_17, CY_PIN_OUT_FUNCTION(P8_3_TCPWM1_LINE_COMPL17, PCLK_TCPWM1_CLOCKS17)},
    {P8_5, PWM_16b_18, CY_PIN_OUT_FUNCTION(P8_5_TCPWM1_LINE_COMPL18, PCLK_TCPWM1_CLOCKS18)},
    {P8_7, PWM_16b_19, CY_PIN_OUT_FUNCTION(P8_7_TCPWM1_LINE_COMPL19, PCLK_TCPWM1_CLOCKS19)},
    {P9_1, PWM_16b_20, CY_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL20, PCLK_TCPWM1_CLOCKS20)},
    {P9_3, PWM_16b_21, CY_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL21, PCLK_TCPWM1_CLOCKS21)},
    {P9_5, PWM_16b_0, CY_PIN_OUT_FUNCTION(P9_5_TCPWM1_LINE_COMPL0, PCLK_TCPWM1_CLOCKS0)},
    {P9_7, PWM_16b_1, CY_PIN_OUT_FUNCTION(P9_7_TCPWM1_LINE_COMPL1, PCLK_TCPWM1_CLOCKS1)},
    {P10_1, PWM_16b_22, CY_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL22, PCLK_TCPWM1_CLOCKS22)},
    {P10_3, PWM_16b_23, CY_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL23, PCLK_TCPWM1_CLOCKS23)},
    {P10_5, PWM_16b_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0, PCLK_TCPWM1_CLOCKS0)},
    {P10_7, PWM_16b_2, CY_PIN_OUT_FUNCTION(P10_7_TCPWM1_LINE_COMPL2, PCLK_TCPWM1_CLOCKS2)},
    {P11_1, PWM_16b_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM1_LINE_COMPL1, PCLK_TCPWM1_CLOCKS1)},
    {P11_3, PWM_16b_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL2, PCLK_TCPWM1_CLOCKS2)},
    {P11_5, PWM_16b_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL3, PCLK_TCPWM1_CLOCKS3)},
    {P12_1, PWM_16b_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM1_LINE_COMPL4, PCLK_TCPWM1_CLOCKS4)},
    {P12_3, PWM_16b_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM1_LINE_COMPL5, PCLK_TCPWM1_CLOCKS5)},
    {P12_5, PWM_16b_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM1_LINE_COMPL6, PCLK_TCPWM1_CLOCKS6)},
    {P12_7, PWM_16b_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7, PCLK_TCPWM1_CLOCKS7)},
    {P13_1, PWM_16b_8, CY_PIN_OUT_FUNCTION(P13_1_TCPWM1_LINE_COMPL8, PCLK_TCPWM1_CLOCKS8)},
    {P13_3, PWM_16b_9, CY_PIN_OUT_FUNCTION(P13_3_TCPWM1_LINE_COMPL9, PCLK_TCPWM1_CLOCKS9)},
    {P13_5, PWM_16b_10, CY_PIN_OUT_FUNCTION(P13_5_TCPWM1_LINE_COMPL10, PCLK_TCPWM1_CLOCKS10)},
    {P13_7, PWM_16b_11, CY_PIN_OUT_FUNCTION(P13_7_TCPWM1_LINE_COMPL11, PCLK_TCPWM1_CLOCKS11)},
    // 32-bit PWM outputs
    {PWM32(P0_0), PWM_32b_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P0_2), PWM_32b_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P0_4), PWM_32b_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P1_0), PWM_32b_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P1_2), PWM_32b_4, CY_PIN_OUT_FUNCTION(P1_2_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P1_4), PWM_32b_5, CY_PIN_OUT_FUNCTION(P1_4_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P2_0), PWM_32b_6, CY_PIN_OUT_FUNCTION(P2_0_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P2_2), PWM_32b_7, CY_PIN_OUT_FUNCTION(P2_2_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P2_4), PWM_32b_0, CY_PIN_OUT_FUNCTION(P2_4_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P2_6), PWM_32b_1, CY_PIN_OUT_FUNCTION(P2_6_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P3_0), PWM_32b_2, CY_PIN_OUT_FUNCTION(P3_0_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P3_2), PWM_32b_3, CY_PIN_OUT_FUNCTION(P3_2_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P3_4), PWM_32b_4, CY_PIN_OUT_FUNCTION(P3_4_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P4_0), PWM_32b_5, CY_PIN_OUT_FUNCTION(P4_0_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    //{PWM32(P4_2), PWM_32b_6, CY_PIN_OUT_FUNCTION(P4_2_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P5_0), PWM_32b_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P5_2), PWM_32b_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P5_4), PWM_32b_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P5_6), PWM_32b_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P6_0), PWM_32b_0, CY_PIN_OUT_FUNCTION(P6_0_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P6_2), PWM_32b_1, CY_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P6_4), PWM_32b_2, CY_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P6_6), PWM_32b_3, CY_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P7_0), PWM_32b_4, CY_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P7_2), PWM_32b_5, CY_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P7_4), PWM_32b_6, CY_PIN_OUT_FUNCTION(P7_4_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P7_6), PWM_32b_7, CY_PIN_OUT_FUNCTION(P7_6_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P8_0), PWM_32b_0, CY_PIN_OUT_FUNCTION(P8_0_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P8_2), PWM_32b_1, CY_PIN_OUT_FUNCTION(P8_2_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P8_4), PWM_32b_2, CY_PIN_OUT_FUNCTION(P8_4_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P8_6), PWM_32b_3, CY_PIN_OUT_FUNCTION(P8_6_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P9_0), PWM_32b_4, CY_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P9_2), PWM_32b_5, CY_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P9_4), PWM_32b_7, CY_PIN_OUT_FUNCTION(P9_4_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P9_6), PWM_32b_0, CY_PIN_OUT_FUNCTION(P9_6_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P10_0), PWM_32b_6, CY_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P10_2), PWM_32b_7, CY_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P10_4), PWM_32b_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P10_6), PWM_32b_1, CY_PIN_OUT_FUNCTION(P10_6_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P11_0), PWM_32b_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P11_2), PWM_32b_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P11_4), PWM_32b_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P12_0), PWM_32b_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM0_LINE4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P12_2), PWM_32b_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM0_LINE5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P12_4), PWM_32b_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM0_LINE6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P12_6), PWM_32b_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P13_0), PWM_32b_0, CY_PIN_OUT_FUNCTION(P13_0_TCPWM0_LINE0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P13_2), PWM_32b_1, CY_PIN_OUT_FUNCTION(P13_2_TCPWM0_LINE1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P13_4), PWM_32b_2, CY_PIN_OUT_FUNCTION(P13_4_TCPWM0_LINE2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P13_6), PWM_32b_3, CY_PIN_OUT_FUNCTION(P13_6_TCPWM0_LINE3, PCLK_TCPWM0_CLOCKS3)},
    // 32-bit PWM inverted outputs
    {PWM32(P0_1), PWM_32b_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P0_3), PWM_32b_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P0_5), PWM_32b_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P1_1), PWM_32b_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P1_3), PWM_32b_4, CY_PIN_OUT_FUNCTION(P1_3_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P1_5), PWM_32b_5, CY_PIN_OUT_FUNCTION(P1_5_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P2_1), PWM_32b_6, CY_PIN_OUT_FUNCTION(P2_1_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P2_3), PWM_32b_7, CY_PIN_OUT_FUNCTION(P2_3_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P2_5), PWM_32b_0, CY_PIN_OUT_FUNCTION(P2_5_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P2_7), PWM_32b_1, CY_PIN_OUT_FUNCTION(P2_7_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P3_1), PWM_32b_2, CY_PIN_OUT_FUNCTION(P3_1_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P3_3), PWM_32b_3, CY_PIN_OUT_FUNCTION(P3_3_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P3_5), PWM_32b_4, CY_PIN_OUT_FUNCTION(P3_5_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P4_1), PWM_32b_5, CY_PIN_OUT_FUNCTION(P4_1_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    //{PWM32(P4_3), PWM_32b_6, CY_PIN_OUT_FUNCTION(P4_3_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P5_1), PWM_32b_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P5_3), PWM_32b_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P5_5), PWM_32b_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P5_7), PWM_32b_7, CY_PIN_OUT_FUNCTION(P5_7_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P6_1), PWM_32b_0, CY_PIN_OUT_FUNCTION(P6_1_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P6_3), PWM_32b_1, CY_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P6_5), PWM_32b_2, CY_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P6_7), PWM_32b_3, CY_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P7_1), PWM_32b_4, CY_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P7_3), PWM_32b_5, CY_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P7_5), PWM_32b_6, CY_PIN_OUT_FUNCTION(P7_5_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P7_7), PWM_32b_7, CY_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P8_1), PWM_32b_0, CY_PIN_OUT_FUNCTION(P8_1_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P8_3), PWM_32b_1, CY_PIN_OUT_FUNCTION(P8_3_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P8_5), PWM_32b_2, CY_PIN_OUT_FUNCTION(P8_5_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P8_7), PWM_32b_3, CY_PIN_OUT_FUNCTION(P8_7_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P9_1), PWM_32b_4, CY_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P9_3), PWM_32b_5, CY_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P9_5), PWM_32b_7, CY_PIN_OUT_FUNCTION(P9_5_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P9_7), PWM_32b_0, CY_PIN_OUT_FUNCTION(P9_7_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P10_1), PWM_32b_6, CY_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P10_3), PWM_32b_7, CY_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P10_5), PWM_32b_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P10_7), PWM_32b_1, CY_PIN_OUT_FUNCTION(P10_7_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P11_1), PWM_32b_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P11_3), PWM_32b_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P11_5), PWM_32b_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {PWM32(P12_1), PWM_32b_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM0_LINE_COMPL4, PCLK_TCPWM0_CLOCKS4)},
    {PWM32(P12_3), PWM_32b_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM0_LINE_COMPL5, PCLK_TCPWM0_CLOCKS5)},
    {PWM32(P12_5), PWM_32b_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM0_LINE_COMPL6, PCLK_TCPWM0_CLOCKS6)},
    {PWM32(P12_7), PWM_32b_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL7, PCLK_TCPWM0_CLOCKS7)},
    {PWM32(P13_1), PWM_32b_0, CY_PIN_OUT_FUNCTION(P13_1_TCPWM0_LINE_COMPL0, PCLK_TCPWM0_CLOCKS0)},
    {PWM32(P13_3), PWM_32b_1, CY_PIN_OUT_FUNCTION(P13_3_TCPWM0_LINE_COMPL1, PCLK_TCPWM0_CLOCKS1)},
    {PWM32(P13_5), PWM_32b_2, CY_PIN_OUT_FUNCTION(P13_5_TCPWM0_LINE_COMPL2, PCLK_TCPWM0_CLOCKS2)},
    {PWM32(P13_7), PWM_32b_3, CY_PIN_OUT_FUNCTION(P13_7_TCPWM0_LINE_COMPL3, PCLK_TCPWM0_CLOCKS3)},
    {NC,    NC,     0}
};
#endif // DEVICE_PWMOUT

#if DEVICE_ANALOGIN
const PinMap PinMap_ADC[] = {
    {P9_0, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_0 pin through the AMUXA bus
    {P9_1, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_1 pin through the AMUXA bus
    {P9_2, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_2 pin through the AMUXA bus
    {P9_4, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_4 pin through the AMUXA bus
    {P9_5, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_5 pin through the AMUXA bus
    {P9_6, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)},  // The SAR ADC Vplus input connects to the P9_6 pin through the AMUXA bus
    {P10_0, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_0 pin
    {P10_1, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_1 pin
    {P10_2, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_2 pin
    {P10_3, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_3 pin
    {P10_4, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_4 pin
    {P10_5, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_5 pin
    {P10_6, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_6 pin
    {P10_7, ADC_0, CY_PIN_ANALOG_FUNCTION(PCLK_PASS_CLOCK_SAR)}, // The SAR ADC Vplus input has the direct connection to the P10_7 pin
    {NC,    NC,     0}
};
#endif // DEVICE_ANALOGIN

#if DEVICE_QSPI
const PinMap PinMap_QSPI_SCLK[] = {          // does not use PERI clock, uses HFCLK2
    {P11_7, SMIF_0, CY_PIN_OUT_FUNCTION(P11_7_SMIF_SPI_CLK, 0)},
    {NC,    NC,    0}
};

// Ensure that the spi_data pins are defined in the order 0 to 7
const PinMap PinMap_QSPI_DATA[] = {          // does not use PERI clock, uses HFCLK2
    {P11_6, SMIF_0, CY_PIN_OUT_FUNCTION(P11_6_SMIF_SPI_DATA0, 0)}, // spi_data0
    {P11_5, SMIF_0, CY_PIN_OUT_FUNCTION(P11_5_SMIF_SPI_DATA1, 0)}, // spi_data1
    {P11_4, SMIF_0, CY_PIN_OUT_FUNCTION(P11_4_SMIF_SPI_DATA2, 0)}, // spi_data2
    {P11_3, SMIF_0, CY_PIN_OUT_FUNCTION(P11_3_SMIF_SPI_DATA3, 0)}, // spi_data3
    {P12_0, SMIF_0, CY_PIN_OUT_FUNCTION(P12_0_SMIF_SPI_DATA4, 0)}, // spi_data4
    {P12_1, SMIF_0, CY_PIN_OUT_FUNCTION(P12_1_SMIF_SPI_DATA5, 0)}, // spi_data5
    {P12_2, SMIF_0, CY_PIN_OUT_FUNCTION(P12_2_SMIF_SPI_DATA6, 0)}, // spi_data6
    {P12_3, SMIF_0, CY_PIN_OUT_FUNCTION(P12_3_SMIF_SPI_DATA7, 0)}, // spi_data7
    {NC,    NC,    0}
};    

// Ensure that the spi_select pins are defined in the order 0 to 3
const PinMap PinMap_QSPI_SSEL[] = {          // does not use PERI clock, uses HFCLK2
    {P11_2, SMIF_0, CY_PIN_OUT_FUNCTION(P11_2_SMIF_SPI_SELECT0, 0)}, // spi_select0
    {P11_1, SMIF_0, CY_PIN_OUT_FUNCTION(P11_1_SMIF_SPI_SELECT1, 0)}, // spi_select1
    {P11_0, SMIF_0, CY_PIN_OUT_FUNCTION(P11_0_SMIF_SPI_SELECT2, 0)}, // spi_select2
    {P12_4, SMIF_0, CY_PIN_OUT_FUNCTION(P12_4_SMIF_SPI_SELECT3, 0)}, // spi_select3
    {NC,    NC,    0}
};
#endif // DEVICE_QSPI

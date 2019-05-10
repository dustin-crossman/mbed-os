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
    {P0_2, UART_0, CY_PIN_IN_FUNCTION(P0_2_SCB0_UART_RX)},
    {P1_0, UART_7, CY_PIN_IN_FUNCTION(P1_0_SCB7_UART_RX)},
    {P5_0, UART_5, CY_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P6_0, UART_3, CY_PIN_IN_FUNCTION(P6_0_SCB3_UART_RX)},
    {P6_4, UART_6, CY_PIN_IN_FUNCTION(P6_4_SCB6_UART_RX)},
    {P7_0, UART_4, CY_PIN_IN_FUNCTION(P7_0_SCB4_UART_RX)},
    {P8_0, UART_4, CY_PIN_IN_FUNCTION(P8_0_SCB4_UART_RX)},
    {P9_0, UART_2, CY_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, UART_1, CY_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
    {P11_0, UART_5, CY_PIN_IN_FUNCTION(P11_0_SCB5_UART_RX)},
    {P12_0, UART_6, CY_PIN_IN_FUNCTION(P12_0_SCB6_UART_RX)},
    {P13_0, UART_6, CY_PIN_IN_FUNCTION(P13_0_SCB6_UART_RX)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_TX[] = {
    {P0_3, UART_0, CY_PIN_OUT_FUNCTION(P0_3_SCB0_UART_TX)},
    {P1_1, UART_7, CY_PIN_OUT_FUNCTION(P1_1_SCB7_UART_TX)},
    {P5_1, UART_5, CY_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P6_1, UART_3, CY_PIN_OUT_FUNCTION(P6_1_SCB3_UART_TX)},
    {P6_5, UART_6, CY_PIN_OUT_FUNCTION(P6_5_SCB6_UART_TX)},
    {P7_1, UART_4, CY_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P8_1, UART_4, CY_PIN_OUT_FUNCTION(P8_1_SCB4_UART_TX)},
    {P9_1, UART_2, CY_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, UART_1, CY_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
    {P11_1, UART_5, CY_PIN_OUT_FUNCTION(P11_1_SCB5_UART_TX)},
    {P12_1, UART_6, CY_PIN_OUT_FUNCTION(P12_1_SCB6_UART_TX)},
    {P13_1, UART_6, CY_PIN_OUT_FUNCTION(P13_1_SCB6_UART_TX)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_RTS[] = {
    {P0_4, UART_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_UART_RTS)},
    {P1_2, UART_7, CY_PIN_OUT_FUNCTION(P1_2_SCB7_UART_RTS)},
    {P5_2, UART_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_UART_RTS)},
    {P6_2, UART_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P6_6, UART_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_UART_RTS)},
    {P7_2, UART_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P8_2, UART_4, CY_PIN_OUT_FUNCTION(P8_2_SCB4_UART_RTS)},
    {P9_2, UART_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, UART_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
    {P11_2, UART_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_UART_RTS)},
    {P12_2, UART_6, CY_PIN_OUT_FUNCTION(P12_2_SCB6_UART_RTS)},
    {NC,    NC,     0}
};
const PinMap PinMap_UART_CTS[] = {
    {P0_5, UART_0, CY_PIN_IN_FUNCTION(P0_5_SCB0_UART_CTS)},
    {P1_3, UART_7, CY_PIN_IN_FUNCTION(P1_3_SCB7_UART_CTS)},
    {P5_3, UART_5, CY_PIN_IN_FUNCTION(P5_3_SCB5_UART_CTS)},
    {P6_3, UART_3, CY_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P6_7, UART_6, CY_PIN_IN_FUNCTION(P6_7_SCB6_UART_CTS)},
    {P7_3, UART_4, CY_PIN_IN_FUNCTION(P7_3_SCB4_UART_CTS)},
    {P8_3, UART_4, CY_PIN_IN_FUNCTION(P8_3_SCB4_UART_CTS)},
    {P9_3, UART_2, CY_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, UART_1, CY_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
    {P11_3, UART_5, CY_PIN_IN_FUNCTION(P11_3_SCB5_UART_CTS)},
    {P12_3, UART_6, CY_PIN_IN_FUNCTION(P12_3_SCB6_UART_CTS)},
    {NC,    NC,     0}
};
#endif // DEVICE_SERIAL


#if DEVICE_I2C
//*** I2C ***
const PinMap PinMap_I2C_SCL[] = {
    {P0_2, I2C_0, CY_PIN_OD_FUNCTION(P0_2_SCB0_I2C_SCL)},
    {P1_0, I2C_7, CY_PIN_OD_FUNCTION(P1_0_SCB7_I2C_SCL)},
    {P5_0, I2C_5, CY_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_0, I2C_3, CY_PIN_OD_FUNCTION(P6_0_SCB3_I2C_SCL)},
    {P6_4, I2C_6, CY_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P7_0, I2C_4, CY_PIN_OD_FUNCTION(P7_0_SCB4_I2C_SCL)},
    {P8_0, I2C_4, CY_PIN_OD_FUNCTION(P8_0_SCB4_I2C_SCL)},
    {P9_0, I2C_2, CY_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, I2C_1, CY_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
    {P11_0, I2C_5, CY_PIN_OD_FUNCTION(P11_0_SCB5_I2C_SCL)},
    {P12_0, I2C_6, CY_PIN_OD_FUNCTION(P12_0_SCB6_I2C_SCL)},
    {P13_0, I2C_6, CY_PIN_OD_FUNCTION(P13_0_SCB6_I2C_SCL)},
    {NC,    NC,    0}
};
const PinMap PinMap_I2C_SDA[] = {
    {P0_3, I2C_0, CY_PIN_OD_FUNCTION(P0_3_SCB0_I2C_SDA)},
    {P1_1, I2C_7, CY_PIN_OD_FUNCTION(P1_1_SCB7_I2C_SDA)},
    {P5_1, I2C_5, CY_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_1, I2C_3, CY_PIN_OD_FUNCTION(P6_1_SCB3_I2C_SDA)},
    {P6_5, I2C_6, CY_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P7_1, I2C_4, CY_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P8_1, I2C_4, CY_PIN_OD_FUNCTION(P8_1_SCB4_I2C_SDA)},
    {P9_1, I2C_2, CY_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, I2C_1, CY_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
    {P11_1, I2C_5, CY_PIN_OD_FUNCTION(P11_1_SCB5_I2C_SDA)},
    {P12_1, I2C_6, CY_PIN_OD_FUNCTION(P12_1_SCB6_I2C_SDA)},
    {P13_1, I2C_6, CY_PIN_OD_FUNCTION(P13_1_SCB6_I2C_SDA)},
    {NC,    NC,    0}
};
#endif // DEVICE_I2C

#if DEVICE_SPI
//*** SPI ***
const PinMap PinMap_SPI_MOSI[] = {
    {P0_2, SPI_0, CY_PIN_OUT_FUNCTION(P0_2_SCB0_SPI_MOSI)},
    {P1_0, SPI_7, CY_PIN_OUT_FUNCTION(P1_0_SCB7_SPI_MOSI)},
    {P5_0, SPI_5, CY_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_0, SPI_3, CY_PIN_OUT_FUNCTION(P6_0_SCB3_SPI_MOSI)},
    {P6_4, SPI_6, CY_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P7_0, SPI_4, CY_PIN_OUT_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P8_0, SPI_4, CY_PIN_OUT_FUNCTION(P8_0_SCB4_SPI_MOSI)},
    {P9_0, SPI_2, CY_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, SPI_1, CY_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
    {P11_0, SPI_5, CY_PIN_OUT_FUNCTION(P11_0_SCB5_SPI_MOSI)},
    {P12_0, SPI_6, CY_PIN_OUT_FUNCTION(P12_0_SCB6_SPI_MOSI)},
    {P13_0, SPI_6, CY_PIN_OUT_FUNCTION(P13_0_SCB6_SPI_MOSI)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_MISO[] = {
    {P0_3, SPI_0, CY_PIN_IN_FUNCTION(P0_3_SCB0_SPI_MISO)},
    {P1_1, SPI_7, CY_PIN_IN_FUNCTION(P1_1_SCB7_SPI_MISO)},
    {P5_1, SPI_5, CY_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_1, SPI_3, CY_PIN_IN_FUNCTION(P6_1_SCB3_SPI_MISO)},
    {P6_5, SPI_6, CY_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P7_1, SPI_4, CY_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P8_1, SPI_4, CY_PIN_IN_FUNCTION(P8_1_SCB4_SPI_MISO)},
    {P9_1, SPI_2, CY_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, SPI_1, CY_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
    {P11_1, SPI_5, CY_PIN_IN_FUNCTION(P11_1_SCB5_SPI_MISO)},
    {P12_1, SPI_6, CY_PIN_IN_FUNCTION(P12_1_SCB6_SPI_MISO)},
    {P13_1, SPI_6, CY_PIN_IN_FUNCTION(P13_1_SCB6_SPI_MISO)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_SCLK[] = {
    {P0_4, SPI_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P1_2, SPI_7, CY_PIN_OUT_FUNCTION(P1_2_SCB7_SPI_CLK)},
    {P5_2, SPI_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_SPI_CLK)},
    {P6_2, SPI_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_6, SPI_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P7_2, SPI_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},

    {P8_2, SPI_4, CY_PIN_OUT_FUNCTION(P8_2_SCB4_SPI_CLK)},
    {P9_2, SPI_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, SPI_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, SPI_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_SPI_CLK)},
    {P12_2, SPI_6, CY_PIN_OUT_FUNCTION(P12_2_SCB6_SPI_CLK)},
    {NC,    NC,    0}
};
const PinMap PinMap_SPI_SSEL[] = {
    {P0_5, SPI_0, CY_PIN_OUT_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P1_3, SPI_7, CY_PIN_OUT_FUNCTION(P1_3_SCB7_SPI_SELECT0)},
    {P5_3, SPI_5, CY_PIN_OUT_FUNCTION(P5_3_SCB5_SPI_SELECT0)},
    {P6_3, SPI_3, CY_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_7, SPI_6, CY_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P7_3, SPI_4, CY_PIN_OUT_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P8_3, SPI_4, CY_PIN_OUT_FUNCTION(P8_3_SCB4_SPI_SELECT0)},
    {P9_3, SPI_2, CY_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, SPI_1, CY_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, SPI_5, CY_PIN_OUT_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
    {P12_3, SPI_6, CY_PIN_OUT_FUNCTION(P12_3_SCB6_SPI_SELECT0)},
    {NC,    NC,    0}
};
#endif // DEVICE_SPI

#if DEVICE_PWMOUT
//*** PWM ***
const PinMap PinMap_PWM_OUT[] = {
    // 16-bit PWM outputs
    {P0_0, PWM_16b_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P0_2, PWM_16b_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM1_LINE1)},
    {P0_4, PWM_16b_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2)},
    {P1_0, PWM_16b_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM1_LINE3)},
    {P1_2, PWM_16b_12, CY_PIN_OUT_FUNCTION(P1_2_TCPWM1_LINE12)},
    {P1_4, PWM_16b_13, CY_PIN_OUT_FUNCTION(P1_4_TCPWM1_LINE13)},
    {P5_0, PWM_16b_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE4)},
    {P5_2, PWM_16b_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM1_LINE5)},
    {P5_4, PWM_16b_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM1_LINE6)},
    {P5_6, PWM_16b_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE7)},
    {P6_0, PWM_16b_8, CY_PIN_OUT_FUNCTION(P6_0_TCPWM1_LINE8)},
    {P6_2, PWM_16b_9, CY_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE9)},
    {P6_4, PWM_16b_10, CY_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE10)},
    {P6_6, PWM_16b_11, CY_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE11)},
    {P7_0, PWM_16b_12, CY_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE12)},
    {P7_2, PWM_16b_13, CY_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE13)},
    {P7_4, PWM_16b_14, CY_PIN_OUT_FUNCTION(P7_4_TCPWM1_LINE14)},
    {P7_6, PWM_16b_15, CY_PIN_OUT_FUNCTION(P7_6_TCPWM1_LINE15)},
    {P8_0, PWM_16b_16, CY_PIN_OUT_FUNCTION(P8_0_TCPWM1_LINE16)},
    {P8_2, PWM_16b_17, CY_PIN_OUT_FUNCTION(P8_2_TCPWM1_LINE17)},
    {P8_4, PWM_16b_18, CY_PIN_OUT_FUNCTION(P8_4_TCPWM1_LINE18)},
    {P8_6, PWM_16b_19, CY_PIN_OUT_FUNCTION(P8_6_TCPWM1_LINE19)},
    {P9_0, PWM_16b_20, CY_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE20)},
    {P9_2, PWM_16b_21, CY_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE21)},
    {P9_4, PWM_16b_0, CY_PIN_OUT_FUNCTION(P9_4_TCPWM1_LINE0)},
    {P9_6, PWM_16b_1, CY_PIN_OUT_FUNCTION(P9_6_TCPWM1_LINE1)},
    {P10_0, PWM_16b_22, CY_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE22)},
    {P10_2, PWM_16b_23, CY_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE23)},
    {P10_4, PWM_16b_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0)},
    {P10_6, PWM_16b_2, CY_PIN_OUT_FUNCTION(P10_6_TCPWM1_LINE2)},
    {P11_0, PWM_16b_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM1_LINE1)},
    {P11_2, PWM_16b_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE2)},
    {P11_4, PWM_16b_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE3)},
    {P12_0, PWM_16b_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM1_LINE4)},
    {P12_2, PWM_16b_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM1_LINE5)},
    {P12_4, PWM_16b_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM1_LINE6)},
    {P12_6, PWM_16b_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7)},
    {P13_0, PWM_16b_8, CY_PIN_OUT_FUNCTION(P13_0_TCPWM1_LINE8)},
    {P13_6, PWM_16b_11, CY_PIN_OUT_FUNCTION(P13_6_TCPWM1_LINE11)},
    // 16-bit PWM inverted outputs
    {P0_1, PWM_16b_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P0_3, PWM_16b_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM1_LINE_COMPL1)},
    {P0_5, PWM_16b_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2)},
    {P1_1, PWM_16b_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM1_LINE_COMPL3)},
    {P1_3, PWM_16b_12, CY_PIN_OUT_FUNCTION(P1_3_TCPWM1_LINE_COMPL12)},
    {P1_5, PWM_16b_14, CY_PIN_OUT_FUNCTION(P1_5_TCPWM1_LINE_COMPL14)},
    {P5_1, PWM_16b_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL4)},
    {P5_3, PWM_16b_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM1_LINE_COMPL5)},
    {P5_5, PWM_16b_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM1_LINE_COMPL6)},
    {P6_1, PWM_16b_8, CY_PIN_OUT_FUNCTION(P6_1_TCPWM1_LINE_COMPL8)},
    {P6_3, PWM_16b_9, CY_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL9)},
    {P6_5, PWM_16b_10, CY_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL10)},
    {P6_7, PWM_16b_11, CY_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL11)},
    {P7_1, PWM_16b_12, CY_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL12)},
    {P7_3, PWM_16b_13, CY_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL13)},
    {P7_5, PWM_16b_14, CY_PIN_OUT_FUNCTION(P7_5_TCPWM1_LINE_COMPL14)},
    {P7_7, PWM_16b_15, CY_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL15)},
    {P8_1, PWM_16b_16, CY_PIN_OUT_FUNCTION(P8_1_TCPWM1_LINE_COMPL16)},
    {P8_3, PWM_16b_17, CY_PIN_OUT_FUNCTION(P8_3_TCPWM1_LINE_COMPL17)},
    {P8_5, PWM_16b_18, CY_PIN_OUT_FUNCTION(P8_5_TCPWM1_LINE_COMPL18)},
    {P8_7, PWM_16b_19, CY_PIN_OUT_FUNCTION(P8_7_TCPWM1_LINE_COMPL19)},
    {P9_1, PWM_16b_20, CY_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL20)},
    {P9_3, PWM_16b_21, CY_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL21)},
    {P9_5, PWM_16b_0, CY_PIN_OUT_FUNCTION(P9_5_TCPWM1_LINE_COMPL0)},
    {P9_7, PWM_16b_1, CY_PIN_OUT_FUNCTION(P9_7_TCPWM1_LINE_COMPL1)},
    {P10_1, PWM_16b_22, CY_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL22)},
    {P10_3, PWM_16b_23, CY_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL23)},
    {P10_5, PWM_16b_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P11_1, PWM_16b_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM1_LINE_COMPL1)},
    {P11_3, PWM_16b_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL2)},
    {P11_5, PWM_16b_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL3)},
    {P12_1, PWM_16b_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM1_LINE_COMPL4)},
    {P12_3, PWM_16b_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM1_LINE_COMPL5)},
    {P12_5, PWM_16b_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM1_LINE_COMPL6)},
    {P12_7, PWM_16b_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7)},
    {P13_1, PWM_16b_8, CY_PIN_OUT_FUNCTION(P13_1_TCPWM1_LINE_COMPL8)},
    {P13_7, PWM_16b_11, CY_PIN_OUT_FUNCTION(P13_7_TCPWM1_LINE_COMPL11)},
    // 32-bit PWM outputs
    {P0_0, PWM_32b_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_2, PWM_32b_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM0_LINE1)},
    {P0_4, PWM_32b_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2)},
    {P1_0, PWM_32b_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM0_LINE3)},
    {P1_2, PWM_32b_4, CY_PIN_OUT_FUNCTION(P1_2_TCPWM0_LINE4)},
    {P1_4, PWM_32b_5, CY_PIN_OUT_FUNCTION(P1_4_TCPWM0_LINE5)},
    {P5_0, PWM_32b_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE4)},
    {P5_2, PWM_32b_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM0_LINE5)},
    {P5_4, PWM_32b_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM0_LINE6)},
    {P5_6, PWM_32b_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE7)},
    {P6_0, PWM_32b_0, CY_PIN_OUT_FUNCTION(P6_0_TCPWM0_LINE0)},
    {P6_2, PWM_32b_1, CY_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE1)},
    {P6_4, PWM_32b_2, CY_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE2)},
    {P6_6, PWM_32b_3, CY_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE3)},
    {P7_0, PWM_32b_4, CY_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE4)},
    {P7_2, PWM_32b_5, CY_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE5)},
    {P7_4, PWM_32b_6, CY_PIN_OUT_FUNCTION(P7_4_TCPWM0_LINE6)},
    {P7_6, PWM_32b_7, CY_PIN_OUT_FUNCTION(P7_6_TCPWM0_LINE7)},
    {P8_0, PWM_32b_0, CY_PIN_OUT_FUNCTION(P8_0_TCPWM0_LINE0)},
    {P8_2, PWM_32b_1, CY_PIN_OUT_FUNCTION(P8_2_TCPWM0_LINE1)},
    {P8_4, PWM_32b_2, CY_PIN_OUT_FUNCTION(P8_4_TCPWM0_LINE2)},
    {P8_6, PWM_32b_3, CY_PIN_OUT_FUNCTION(P8_6_TCPWM0_LINE3)},
    {P9_0, PWM_32b_4, CY_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE4)},
    {P9_2, PWM_32b_5, CY_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE5)},
    {P9_4, PWM_32b_7, CY_PIN_OUT_FUNCTION(P9_4_TCPWM0_LINE7)},
    {P9_6, PWM_32b_0, CY_PIN_OUT_FUNCTION(P9_6_TCPWM0_LINE0)},
    {P10_0, PWM_32b_6, CY_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE6)},
    {P10_2, PWM_32b_7, CY_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE7)},
    {P10_4, PWM_32b_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0)},
    {P10_6, PWM_32b_1, CY_PIN_OUT_FUNCTION(P10_6_TCPWM0_LINE1)},
    {P11_0, PWM_32b_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM0_LINE1)},
    {P11_2, PWM_32b_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE2)},
    {P11_4, PWM_32b_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE3)},
    {P12_0, PWM_32b_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM0_LINE4)},
    {P12_2, PWM_32b_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM0_LINE5)},
    {P12_4, PWM_32b_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM0_LINE6)},
    {P12_6, PWM_32b_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE7)},
    {P13_0, PWM_32b_0, CY_PIN_OUT_FUNCTION(P13_0_TCPWM0_LINE0)},
    {P13_6, PWM_32b_3, CY_PIN_OUT_FUNCTION(P13_6_TCPWM0_LINE3)},
    // 32-bit PWM inverted outputs
    {P0_1, PWM_32b_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_3, PWM_32b_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM0_LINE_COMPL1)},
    {P0_5, PWM_32b_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2)},
    {P1_1, PWM_32b_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM0_LINE_COMPL3)},
    {P1_3, PWM_32b_4, CY_PIN_OUT_FUNCTION(P1_3_TCPWM0_LINE_COMPL4)},
    {P1_5, PWM_32b_5, CY_PIN_OUT_FUNCTION(P1_5_TCPWM0_LINE_COMPL5)},
    {P5_1, PWM_32b_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL4)},
    {P5_3, PWM_32b_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM0_LINE_COMPL5)},
    {P5_5, PWM_32b_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM0_LINE_COMPL6)},
    {P6_1, PWM_32b_0, CY_PIN_OUT_FUNCTION(P6_1_TCPWM0_LINE_COMPL0)},
    {P6_3, PWM_32b_1, CY_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL1)},
    {P6_5, PWM_32b_2, CY_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL2)},
    {P6_7, PWM_32b_3, CY_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL3)},
    {P7_1, PWM_32b_4, CY_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL4)},
    {P7_3, PWM_32b_5, CY_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL5)},
    {P7_5, PWM_32b_6, CY_PIN_OUT_FUNCTION(P7_5_TCPWM0_LINE_COMPL6)},
    {P7_7, PWM_32b_7, CY_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL7)},
    {P8_1, PWM_32b_0, CY_PIN_OUT_FUNCTION(P8_1_TCPWM0_LINE_COMPL0)},
    {P8_3, PWM_32b_1, CY_PIN_OUT_FUNCTION(P8_3_TCPWM0_LINE_COMPL1)},
    {P8_5, PWM_32b_2, CY_PIN_OUT_FUNCTION(P8_5_TCPWM0_LINE_COMPL2)},
    {P8_7, PWM_32b_3, CY_PIN_OUT_FUNCTION(P8_7_TCPWM0_LINE_COMPL3)},
    {P9_1, PWM_32b_4, CY_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL4)},
    {P9_3, PWM_32b_5, CY_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL5)},
    {P9_5, PWM_32b_7, CY_PIN_OUT_FUNCTION(P9_5_TCPWM0_LINE_COMPL7)},
    {P9_7, PWM_32b_0, CY_PIN_OUT_FUNCTION(P9_7_TCPWM0_LINE_COMPL0)},
    {P10_1, PWM_32b_6, CY_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL6)},
    {P10_3, PWM_32b_7, CY_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL7)},
    {P10_5, PWM_32b_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P11_1, PWM_32b_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM0_LINE_COMPL1)},
    {P11_3, PWM_32b_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL2)},
    {P11_5, PWM_32b_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL3)},
    {P12_1, PWM_32b_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM0_LINE_COMPL4)},
    {P12_3, PWM_32b_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM0_LINE_COMPL5)},
    {P12_5, PWM_32b_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM0_LINE_COMPL6)},
    {P12_7, PWM_32b_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL7)},
    {P13_1, PWM_32b_0, CY_PIN_OUT_FUNCTION(P13_1_TCPWM0_LINE_COMPL0)},
    {P13_7, PWM_32b_3, CY_PIN_OUT_FUNCTION(P13_7_TCPWM0_LINE_COMPL3)},
    {NC,    NC,     0}
};
#endif // DEVICE_PWMOUT

#if DEVICE_ANALOGIN
const PinMap PinMap_ADC[] = {
    {P10_0, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_1, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_2, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_3, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_4, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_5, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_6, ADC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {NC,    NC,     0}
};
#endif // DEVICE_ANALOGIN

#if DEVICE_ANALOGOUT
const PinMap PinMap_DAC[] = {
    {P9_6,  DAC_0, CY_PIN_ANALOG_FUNCTION(HSIOM_SEL_GPIO)},
    {P10_5, DAC_0, CY_GPIO_CFG_CREATE(HSIOM_SEL_AMUXA, CY_GPIO_DM_ANALOG)},  // CTDAC connects to the P10_5 pin through the AMUXA bus
    {NC,    NC,     0}
};
#endif // DEVICE_ANALOGIN

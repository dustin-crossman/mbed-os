/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "PeripheralPins.h"

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have also to modify the corresponding xxx_api.c file
//       for pwmout, analogin, analogout, ...
// =====

//*** ADC ***

const PinMap PinMap_ADC[] = {
    {PA_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0)},  // ADC_IN0
    {PA_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)},  // ADC_IN1   // Pin not available on any connector
    {PA_2,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)},  // ADC_IN2   - used by STDIO TX
    {PA_3,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 3, 0)},  // ADC_IN3   - used by STDIO RX
    {PA_4,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 4, 0)},  // ADC_IN4
    {PA_5,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 5, 0)},  // ADC_IN5   - used also to drive the LED LD2
    {PA_6,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6, 0)},  // ADC_IN6   // Pin not available on any connector
    {PA_7,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7, 0)},  // ADC_IN7   // Pin not available on any connector
    {PB_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 8, 0)},  // ADC_IN8   // Pin not available on any connector
    {PB_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9, 0)},  // ADC_IN9   // Pin not available on any connector
    {PC_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC_IN10 // Pin not available on any connector
    {PC_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC_IN11 // Pin not available on any connector
    {PC_2,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC_IN12 // Pin not available on any connector
    {NC,   NC,    0}
};

//*** DAC ***

const PinMap PinMap_DAC[] = {
    {PA_4, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // DAC_OUT1
    {PA_5, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // DAC_OUT2 - used also to drive the LED LD2
    {NC,   NC,    0}
};

//*** I2C ***

const PinMap PinMap_I2C_SDA[] = {
    {PA_10, I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF6_I2C1)}, // also used by UART RX
    {PB_4,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF7_I2C3)}, // Pin not available on any connector
    {PB_7,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF1_I2C1)}, // also used to drive the LED LD4
    {PB_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_11, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF6_I2C2)}, // Pin not available on any connector
    {PB_14, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF5_I2C2)},
    {PC_1,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF7_I2C3)}, // Pin not available on any connector
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PA_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF7_I2C3)},
    {PA_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF6_I2C1)},
    {PB_6,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF1_I2C1)},
    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_10, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF6_I2C2)}, // Pin not available on any connector
    {PB_13, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF5_I2C2)},
    {PC_0,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF7_I2C3)}, // Pin not available on any connector
    {NC,    NC,    0}
};

//*** PWM ***

// TIM21 cannot be used because already used by the us_ticker
const PinMap PinMap_PWM[] = {
    {PA_0,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 1, 0)},   // TIM2_CH1
    {PA_1,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 2, 0)},   // TIM2_CH2  // Pin not available on any connector
    {PA_2,  PWM_21,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_TIM21, 1, 0)},  // TIM21_CH1 // Pin not available on any connector
    {PA_2,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 3, 0)},   // TIM2_CH3 - used by STDIO TX
    {PA_3,  PWM_21,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_TIM21, 2, 0)},  // TIM21_CH2 // Pin not available on any connector
    {PA_3,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 4, 0)},   // TIM2_CH4 - used by STDIO RX
    {PA_5,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_TIM2, 1, 0)},   // TIM2_CH1 - used also to drive the LED LD2
    {PA_6,  PWM_22,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_TIM22, 1, 0)},  // TIM22_CH1 // Pin not available on any connector
    {PA_6,  PWM_3,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)},   // TIM3_CH1  // Pin not available on any connector
    {PA_7,  PWM_22,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_TIM22, 2, 0)},  // TIM22_CH2 // Pin not available on any connector
    {PA_7,  PWM_3,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)},   // TIM3_CH2  // Pin not available on any connector
    {PA_15, PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_TIM2, 1, 0)},   // TIM2_CH1
    {PB_0,  PWM_3,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)},   // TIM3_CH3  // Pin not available on any connector
    {PB_1,  PWM_3,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)},   // TIM3_CH4  // Pin not available on any connector
    {PB_3,  PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 2, 0)},   // TIM2_CH2  // Pin not available on any connector
    {PB_4,  PWM_22,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM22, 1, 0)},  // TIM22_CH1 // Pin not available on any connector
    {PB_4,  PWM_3,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)},   // TIM3_CH1  // Pin not available on any connector
    {PB_5,  PWM_22,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM22, 2, 0)},  // TIM22_CH2 - used also to drive the LED LD1
    {PB_5_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM3, 2, 0)},   // TIM3_CH2   - used also to drive the LED LD1
    {PB_10, PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 3, 0)},   // TIM2_CH3  // Pin not available on any connector
    {PB_11, PWM_2,       STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM2, 4, 0)},   // TIM2_CH4  // Pin not available on any connector
    {PB_13, PWM_21,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_TIM21, 1, 0)},  // TIM21_CH1 // Pin not available on any connector
    {PB_14, PWM_21,      STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_TIM21, 2, 0)},  // TIM21_CH2 // Pin not available on any connector
    {NC,    NC,    0}
};

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {PA_0,  UART_4,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART4)},
    {PA_2_ALT0,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)}, // Pin not available on any connector
    {PA_2,  UART_2,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},
    {PA_9,  UART_1,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)},
    {PA_14, UART_2,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},  // Warning: this pin is used by SWCLK
    {PA_14_ALT0, LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)}, // Warning: this pin is used by SWCLK
    {PB_3,  UART_5,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART5)},  // Pin not available on any connector
    {PB_6,  UART_1,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_USART1)},
    {PB_10, LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)}, // Pin not available on any connector
    {PB_11, LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_LPUART1)}, // Already used by UART_RX
    {PC_1,  LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)}, // Pin not available on any connector
    {NC,    NC,    0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_1,  UART_4,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART4)},  // Pin not available on any connector
    {PA_3_ALT0,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)},
    {PA_3,  UART_2,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},
    {PA_10, UART_1,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)},
    {PA_13, LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)}, // Warning: this pin is used by SWDIO
    {PA_15, UART_2,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},
    {PB_4,  UART_5,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART5)}, 
    {PB_7,  UART_1,        STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_USART1)},  // Warning: this pin is used by LD4
    {PB_10, LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_LPUART1)}, // Already used by UART_TX
    {PB_11, LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)}, // Pin not available on any connector
    {PC_0,  LPUART_1,      STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_LPUART1)}, // Pin not available on any connector
    {NC,    NC,    0}
};

const PinMap PinMap_UART_RTS[] = {
    {PA_1,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},  // Pin not available on any connector
    {PA_12, UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)},
    {PA_15, UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART4)},
    {PB_1,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)}, // Pin not available on any connector
    {PB_3,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_USART1)},  // Pin not available on any connector
    {PB_5,  UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART5)},  // Warning: this pin is used by LD1
    {PB_12, LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_LPUART1)},
    {PB_14, LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)},
    {NC,    NC,    0}
};

const PinMap PinMap_UART_CTS[] = {
    {PA_0,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART2)},
    {PA_6,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)}, // Pin not available on any connector
    {PA_11, UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)},
    {PB_4,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_USART1)},  // Pin not available on any connector
    {PB_7,  UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_USART4)},  // Warning: this pin is used by LD4
    {PB_13, LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_LPUART1)},
    {NC,    NC,    0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    {PA_7,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},  // Pin not available on any connector
    {PA_12, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},
    {PB_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},  // Warning: this pin is used by LD1
    {PB_15, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI2)},
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA_6,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)}, // Pin not available on any connector
    {PA_11, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},
    {PB_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)}, // Pin not available on any connector
    {PB_14, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI2)},
    {PC_2,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_SPI2)}, // Pin not available on any connector
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PA_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)}, // - used also to drive the LED LD2
    {PB_3,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)}, // Pin not available on any connector
    {PB_10, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Pin not available on any connector
    {PB_13, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI2)},
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PA_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},
    {PA_15, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI1)},
    {PB_9,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PB_12, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF0_SPI2)},
    {NC,    NC,    0}
};

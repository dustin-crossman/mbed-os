/* mbed Microcontroller Library
 '*******************************************************************************
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
#include "mbed_toolchain.h"

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have also to modify the corresponding xxx_api.c file
//       for pwmout, analogin, analogout, ...
// =====

//*** ADC ***

MBED_WEAK const PinMap PinMap_ADC[] = {
    {PA_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0)}, // ADC1_IN0
//  {PA_0,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0)}, // ADC2_IN0
//  {PA_0,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0)}, // ADC3_IN0
//  {PA_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // ADC1_IN1 (pin used by ethernet)
//  {PA_1,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // ADC2_IN1 (pin used by ethernet)
//  {PA_1,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // ADC3_IN1 (pin used by ethernet)
//  {PA_2,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // ADC1_IN2 (pin used by ethernet)
//  {PA_2,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // ADC2_IN2 (pin used by ethernet)
//  {PA_2,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // ADC3_IN2 (pin used by ethernet)
    {PA_3,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 3, 0)}, // ADC1_IN3 - ARDUINO A0
//  {PA_3,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 3, 0)}, // ADC2_IN3 - ARDUINO A0
//  {PA_3,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 3, 0)}, // ADC3_IN3 - ARDUINO A0
    {PA_4,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 4, 0)}, // ADC1_IN4
//  {PA_4,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 4, 0)}, // ADC2_IN4
    {PA_5,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 5, 0)}, // ADC1_IN5 - ARDUINO D13
//  {PA_5,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 5, 0)}, // ADC2_IN5 - ARDUINO D13
    {PA_6,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6, 0)}, // ADC1_IN6 - ARDUINO D12
//  {PA_6,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6, 0)}, // ADC2_IN6 - ARDUINO D12
//  {PA_7,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7, 0)}, // ADC1_IN7 (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
    {PA_7,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7, 0)}, // ADC2_IN7 (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
//  {PB_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 8, 0)}, // ADC1_IN8 (pin used by LED1)
//  {PB_0,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 8, 0)}, // ADC2_IN8 (pin used by LED1)
    {PB_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9, 0)}, // ADC1_IN9
//  {PB_1,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9, 0)}, // ADC2_IN9
    {PC_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC1_IN10 - ARDUINO A1
//  {PC_0,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC2_IN10 - ARDUINO A1
//  {PC_0,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC3_IN10 - ARDUINO A1
//  {PC_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC1_IN11 (pin used by ethernet)
//  {PC_1,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC2_IN11 (pin used by ethernet)
//  {PC_1,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC3_IN11 (pin used by ethernet)
    {PC_2,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC1_IN12
//  {PC_2,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC2_IN12
//  {PC_2,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC3_IN12
    {PC_3,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 13, 0)}, // ADC1_IN13 - ARDUINO A2
//  {PC_3,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 13, 0)}, // ADC2_IN13 - ARDUINO A2
//  {PC_3,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 13, 0)}, // ADC3_IN13 - ARDUINO A2
    {PC_4,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 14, 0)}, // ADC1_IN14 (pin used by ethernet when JP6 ON)
//  {PC_4,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 14, 0)}, // ADC2_IN14 (pin used by ethernet when JP6 ON)
    {PC_5,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 15, 0)}, // ADC1_IN15 (pin used by ethernet when JP6 ON)
//  {PC_5,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 15, 0)}, // ADC2_IN15 (pin used by ethernet when JP6 ON)
    {PF_3,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9, 0)}, // ADC3_IN9 - ARDUINO A3
    {PF_4,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 14, 0)}, // ADC3_IN14
    {PF_5,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 15, 0)}, // ADC3_IN15 - ARDUINO A4
    {PF_6,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 4, 0)}, // ADC3_IN4
    {PF_7,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 5, 0)}, // ADC3_IN5
    {PF_8,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6, 0)}, // ADC3_IN6
    {PF_9,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7, 0)}, // ADC3_IN7
    {PF_10,  ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 8, 0)}, // ADC3_IN8 - ARDUINO A5
    {NC,   NC,    0}
};

MBED_WEAK const PinMap PinMap_ADC_Internal[] = {
    {ADC_TEMP, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 16, 0)}, // See in analogin_api.c the correct ADC channel used
    {ADC_VREF, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 17, 0)}, // See in analogin_api.c the correct ADC channel used
    {ADC_VBAT, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 18, 0)}, // See in analogin_api.c the correct ADC channel used
    {NC,   NC,    0}
};

//*** DAC ***

MBED_WEAK const PinMap PinMap_DAC[] = {
    {PA_4,  DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // DAC_OUT1
    {PA_5,  DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // DAC_OUT2 - ARDUINO D13
    {NC,   NC,    0}
};

//*** I2C ***

MBED_WEAK const PinMap PinMap_I2C_SDA[] = {
//  {PB_7,  I2C_1   , STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // (pin used by LED2)
    {PB_9,  I2C_1   , STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // ARDUINO D14
    {PB_11, I2C_2   , STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PC_9,  I2C_3   , STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
    {PF_0,  I2C_2   , STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_I2C_SCL[] = {
    {PA_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
    {PB_6,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // ARDUINO D15
    {PB_10, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PF_1,  I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {NC,    NC,    0}
};

//*** PWM ***

MBED_WEAK const PinMap PinMap_PWM[] = {
    {PA_0,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)},  // TIM2_CH1
//  {PA_0,  PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 1, 0)},  // TIM5_CH1 (TIM5 used by us_ticker)
//  {PA_1,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)},  // TIM2_CH2 (pin used by ethernet)
//  {PA_1,  PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 2, 0)},  // TIM5_CH2 (TIM5 used by us_ticker) (pin used by ethernet)
//  {PA_2,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)},  // TIM2_CH3 (pin used by ethernet)
//  {PA_2,  PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 3, 0)},  // TIM5_CH3 (TIM5 used by us_ticker)
//  {PA_2,  PWM_9,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9, 1, 0)},  // TIM9_CH1 (pin used by ethernet)
//  {PA_3,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)},  // TIM2_CH4 - ARDUINO A0
    {PA_3,  PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 4, 0)},  // TIM5_CH4 (TIM5 used by us_ticker) - ARDUINO A0
//  {PA_3,  PWM_9,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9, 2, 0)},  // TIM9_CH2 - ARDUINO A0
//  {PA_5,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)},  // TIM2_CH1 - ARDUINO D13
    {PA_5,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)},  // TIM8_CH1N - ARDUINO D13
//  {PA_6,  PWM_13, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM13, 1, 0)}, // TIM13_CH1 - ARDUINO D12
    {PA_6,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1 - ARDUINO D12
    {PA_7,  PWM_14, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM14, 1, 0)}, // TIM14_CH1 (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
//  {PA_7,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)},  // TIM1_CH1N (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
//  {PA_7,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2 (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
//  {PA_7,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)},  // TIM8_CH1N (pin used by ethernet when JP6 ON) - ARDUINO D11 (default configuration)
    {PA_8,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)},  // TIM1_CH1
//  {PA_9,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)},  // TIM1_CH2 (pin used by usb)
//  {PA_10, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)},  // TIM1_CH3 (pin used by usb)
//  {PA_11, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)},  // TIM1_CH4 (pin used by usb)
    {PA_15, PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)},  // TIM2_CH1
//  {PB_0,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N (pin used by LED1)
//  {PB_0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3 (pin used by LED1)
//  {PB_0,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)},  // TIM8_CH2N (pin used by LED1)
//  {PB_1,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
//  {PB_1,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
    {PB_1,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)},  // TIM8_CH3N
    {PB_3,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)},  // TIM2_CH2
    {PB_4,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
    {PB_5,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2 - ARDUINO D11 (need HW and SW updates)
                                                                                          // HW solder bridge update : SB121 off, SB122 on
                                                                                          // SW : config from json files
    {PB_6,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)},  // TIM4_CH1
//  {PB_7,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)},  // TIM4_CH2 (pin used by LED2)
//  {PB_8,  PWM_10, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM10, 1, 0)},  // TIM10_CH1 - ARDUINO D15
    {PB_8,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)},  // TIM4_CH3 - ARDUINO D15
    {PB_9,  PWM_11, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM11, 1, 0)},  // TIM11_CH1 - ARDUINO D14
//  {PB_9,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)},  // TIM4_CH4 - ARDUINO D14
    {PB_10, PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)},  // TIM2_CH3
    {PB_11, PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)},  // TIM2_CH4
    {PB_13, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)},  // TIM1_CH1N (pin used by ethernet when JP7 ON)
//  {PB_14, PWM_12, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM12, 1, 0)},  // TIM12_CH1 (pin used by LED3)
//  {PB_14, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N (pin used by LED3)
//  {PB_14, PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)},  // TIM8_CH2N (pin used by LED3)
    {PB_15, PWM_12, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM12, 2, 0)},  // TIM12_CH2
//  {PB_15, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
//  {PB_15, PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)},  // TIM8_CH3N
    {PC_6,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
//  {PC_6,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 0)},  // TIM8_CH1
    {PC_7,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2
//  {PC_7,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 0)},  // TIM8_CH2
    {PC_8,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
//  {PC_8,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 0)},  // TIM8_CH3
    {PC_9,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
//  {PC_9,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 4, 0)},  // TIM8_CH4
    {PD_12, PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)},  // TIM4_CH1
    {PD_13, PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)},  // TIM4_CH2
    {PD_14, PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)},  // TIM4_CH3 - ARDUINO D10
    {PD_15, PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)},  // TIM4_CH4 - ARDUINO D9
    {PE_5,  PWM_9,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9, 1, 0)},  // TIM9_CH1
    {PE_6,  PWM_9,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9, 2, 0)},  // TIM9_CH2
    {PE_8,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)},  // TIM1_CH1N
    {PE_9,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)},  // TIM1_CH1 - ARDUINO D6
    {PE_10, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N
    {PE_11, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)},  // TIM1_CH2 - ARDUINO D5
    {PE_12, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
    {PE_13, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)},  // TIM1_CH3 - ARDUINO D3
    {PE_14, PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)},  // TIM1_CH4
    {PF_6,  PWM_10, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM10, 1, 0)},  // TIM10_CH1
    {PF_7,  PWM_11, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM11, 1, 0)},  // TIM11_CH1
    {PF_8,  PWM_13, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM13, 1, 0)},  // TIM13_CH1
    {PF_9,  PWM_14, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM14, 1, 0)},  // TIM14_CH1
    {NC,    NC,    0}
};

//*** SERIAL ***

MBED_WEAK const PinMap PinMap_UART_TX[] = {
    {PA_0,  UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)},
    {PA_2,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // (pin used by ethernet)
//  {PA_9,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // (pin used by usb)
    {PB_6,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
    {PB_10, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PC_6,  UART_6,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
//  {PC_10, UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)},
    {PC_10, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PC_12, UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)},
    {PD_5,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {PD_8,  UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PE_1,  UART_8,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART8)},
    {PE_8,  UART_7,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART7)},
    {PF_7,  UART_7,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART7)},
    {PG_14, UART_6,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_UART_RX[] = {
//  {PA_1,  UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // (pin used by ethernet)
    {PA_3,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
//  {PA_10, UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // (pin used by usb)
//  {PB_7,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // (pin used by LED2)
    {PB_11, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PC_7,  UART_6,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
    {PC_11, UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)},
//  {PC_11, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PD_2,  UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)},
    {PD_6,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {PD_9,  UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PE_0,  UART_8,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART8)},
    {PE_7,  UART_7,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART7)},
    {PF_6,  UART_7,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART7)},
    {PG_9,  UART_6,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)}, // ARDUINO D0
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_UART_RTS[] = {
    {PA_1,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // (pin used by ethernet)
//  {PA_12, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // (pin used by usb)
//  {PB_14, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // (pin used by LED3)
    {PD_4,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {PD_12, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PG_8,  UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
    {PG_12, UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
    {NC,    NC,     0}
};

MBED_WEAK const PinMap PinMap_UART_CTS[] = {
    {PA_0,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
//  {PA_11, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // (pin used by usb)
    {PB_13, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // (pin used by ethernet when JP7 ON)
    {PD_3,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {PD_11, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
//  {PG_13, UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)}, // (pin used by ethernet)
    {PG_15, UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
    {NC,    NC,     0}
};

//*** SPI ***

MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
    {PA_7,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // pin used by ethernet when JP6 ON - ARDUINO D11 (default configuration)
    {PB_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D11 (need HW and SW updates)
                                                                              // HW solder bridge update : SB121 off, SB122 on
                                                                              // SW : config from json files
//  {PB_5,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PB_15, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PC_3,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // ARDUINO A2
    {PC_12, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PD_6,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {PE_6,  SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PE_14, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PF_9,  SPI_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI5)},
    {PF_11, SPI_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI5)},
    {PG_14, SPI_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI6)}, // ARDUINO D1
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
    {PA_6,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D12
    {PB_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
//  {PB_4,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//  {PB_14, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // (pin used by LED3)
    {PC_2,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PC_11, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PE_5,  SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PE_13, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)}, // ARDUINO D3
    {PF_8,  SPI_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI5)},
    {PG_12, SPI_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI6)},
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_SPI_SCLK[] = {
    {PA_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D13
//  {PB_3,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {PB_3,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PB_10, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PB_13, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // (pin used by ethernet when JP7 ON)
    {PC_10, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PD_3,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PE_2,  SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PE_12, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PF_7,  SPI_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI5)},
//  {PG_13, SPI_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI6)}, // (pin used by ethernet)
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_SPI_SSEL[] = {
    {PA_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
//  {PA_4,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//  {PA_15, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {PA_15, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
    {PB_9,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // ARDUINO D14
    {PB_12, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
    {PE_4,  SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)},
    {PE_11, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI4)}, // ARDUINO D5
    {PF_6,  SPI_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI5)},
    {PG_8,  SPI_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI6)},
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_CAN_RD[] = {
    {PB_8,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {PB_12, CAN_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)},
    {PD_0, CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {PB_5 , CAN_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)}, // ARDUINO D11 (need HW and SW updates)
                                                                              // HW solder bridge update : SB121 off, SB122 on
                                                                              // SW : config from json files
    {PA_11, CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {NC,    NC,    0}
};

MBED_WEAK const PinMap PinMap_CAN_TD[] = {
    {PB_9,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {PB_13, CAN_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)},
    {PD_1,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {PB_6 , CAN_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)},
    {PA_12, CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)},
    {NC,    NC,    0}
};

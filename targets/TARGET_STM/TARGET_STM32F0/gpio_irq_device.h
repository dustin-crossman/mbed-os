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
#ifndef MBED_GPIO_IRQ_DEVICE_H
#define MBED_GPIO_IRQ_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

// Number of EXTI irq vectors (EXTI0_1, EXTI2_3, EXTI4_15)
#define CHANNEL_NUM (3)

#define EXTI_IRQ0_NUM_LINES 2
#define EXTI_IRQ1_NUM_LINES 2
#define EXTI_IRQ2_NUM_LINES 12
// Max pins for one line (max with EXTI4_15)
#define MAX_PIN_LINE (EXTI_IRQ2_NUM_LINES)

/*  Structure to describe how the HW EXTI lines are defined in this HW */
typedef struct exti_lines {
    uint32_t gpio_idx;   // an index entry for each EXIT line
    uint32_t irq_index;  // the IRQ index
    IRQn_Type  irq_n;    // the corresponding EXTI IRQn
} exti_lines_t;

// Used to return the index for channels array.
static exti_lines_t pin_lines_desc[16] =
{
    // EXTI0_1
    {.gpio_idx = 0, .irq_index = 0, .irq_n = EXTI0_1_IRQn}, // pin 0
    {.gpio_idx = 1, .irq_index = 0, .irq_n = EXTI0_1_IRQn}, // pin 1
    // EXTI2_3
    {.gpio_idx = 0, .irq_index = 1, .irq_n = EXTI2_3_IRQn}, // pin 2
    {.gpio_idx = 1, .irq_index = 1, .irq_n = EXTI2_3_IRQn}, // pin 3
    // EXTI4_15
    {.gpio_idx = 0, .irq_index = 2, .irq_n = EXTI4_15_IRQn}, // pin 4
    {.gpio_idx = 1, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 5
    {.gpio_idx = 2, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 6
    {.gpio_idx = 3, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 7
    {.gpio_idx = 4, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 8
    {.gpio_idx = 5, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 9
    {.gpio_idx = 6, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 10
    {.gpio_idx = 7, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 11
    {.gpio_idx = 8, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 12
    {.gpio_idx = 9, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 13
    {.gpio_idx = 10, .irq_index = 2, .irq_n = EXTI4_15_IRQn},// pin 14
    {.gpio_idx = 11, .irq_index = 2, .irq_n = EXTI4_15_IRQn}// pin 15
};

#endif

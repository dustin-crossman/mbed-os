/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_WCO_IN_ENABLED 1U
#define CYBSP_WCO_IN_PORT GPIO_PRT0
#define CYBSP_WCO_IN_PIN 0U
#define CYBSP_WCO_IN_NUM 0U
#define CYBSP_WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define CYBSP_WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_WCO_OUT_ENABLED 1U
#define CYBSP_WCO_OUT_PORT GPIO_PRT0
#define CYBSP_WCO_OUT_PIN 1U
#define CYBSP_WCO_OUT_NUM 1U
#define CYBSP_WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define CYBSP_WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_LED9_ENABLED 1U
#define CYBSP_LED9_PORT GPIO_PRT11
#define CYBSP_LED9_PIN 1U
#define CYBSP_LED9_NUM 1U
#define CYBSP_LED9_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED9_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
	#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED9_HSIOM ioss_0_port_11_pin_1_HSIOM
#define CYBSP_LED9_IRQ ioss_interrupts_gpio_11_IRQn
#define ioss_0_port_12_pin_0_ENABLED 1U
#define ioss_0_port_12_pin_0_PORT GPIO_PRT12
#define ioss_0_port_12_pin_0_PIN 0U
#define ioss_0_port_12_pin_0_NUM 0U
#define ioss_0_port_12_pin_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_0_HSIOM
	#define ioss_0_port_12_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_0_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_1_ENABLED 1U
#define ioss_0_port_12_pin_1_PORT GPIO_PRT12
#define ioss_0_port_12_pin_1_PIN 1U
#define ioss_0_port_12_pin_1_NUM 1U
#define ioss_0_port_12_pin_1_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_1_HSIOM
	#define ioss_0_port_12_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_1_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_2_ENABLED 1U
#define ioss_0_port_12_pin_2_PORT GPIO_PRT12
#define ioss_0_port_12_pin_2_PIN 2U
#define ioss_0_port_12_pin_2_NUM 2U
#define ioss_0_port_12_pin_2_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_2_HSIOM
	#define ioss_0_port_12_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_2_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_3_ENABLED 1U
#define ioss_0_port_12_pin_3_PORT GPIO_PRT12
#define ioss_0_port_12_pin_3_PIN 3U
#define ioss_0_port_12_pin_3_NUM 3U
#define ioss_0_port_12_pin_3_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_3_HSIOM
	#define ioss_0_port_12_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_3_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_4_ENABLED 1U
#define ioss_0_port_12_pin_4_PORT GPIO_PRT12
#define ioss_0_port_12_pin_4_PIN 4U
#define ioss_0_port_12_pin_4_NUM 4U
#define ioss_0_port_12_pin_4_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_4_HSIOM
	#define ioss_0_port_12_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_4_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_5_ENABLED 1U
#define ioss_0_port_12_pin_5_PORT GPIO_PRT12
#define ioss_0_port_12_pin_5_PIN 5U
#define ioss_0_port_12_pin_5_NUM 5U
#define ioss_0_port_12_pin_5_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_5_HSIOM
	#define ioss_0_port_12_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_5_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_BTN0_ENABLED 1U
#define CYBSP_BTN0_PORT GPIO_PRT1
#define CYBSP_BTN0_PIN 4U
#define CYBSP_BTN0_NUM 4U
#define CYBSP_BTN0_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
	#define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BTN0_HSIOM ioss_0_port_1_pin_4_HSIOM
#define CYBSP_BTN0_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_LED8_ENABLED 1U
#define CYBSP_LED8_PORT GPIO_PRT1
#define CYBSP_LED8_PIN 5U
#define CYBSP_LED8_NUM 5U
#define CYBSP_LED8_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED8_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
	#define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED8_HSIOM ioss_0_port_1_pin_5_HSIOM
#define CYBSP_LED8_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_DEBUG_UART_RX_ENABLED 1U
#define CYBSP_DEBUG_UART_RX_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_RX_PIN 0U
#define CYBSP_DEBUG_UART_RX_NUM 0U
#define CYBSP_DEBUG_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_DEBUG_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
	#define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_RX_HSIOM ioss_0_port_5_pin_0_HSIOM
#define CYBSP_DEBUG_UART_RX_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_DEBUG_UART_TX_ENABLED 1U
#define CYBSP_DEBUG_UART_TX_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_TX_PIN 1U
#define CYBSP_DEBUG_UART_TX_NUM 1U
#define CYBSP_DEBUG_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_DEBUG_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_TX_HSIOM ioss_0_port_5_pin_1_HSIOM
#define CYBSP_DEBUG_UART_TX_IRQ ioss_interrupts_gpio_5_IRQn
#define ioss_0_port_6_pin_6_ENABLED 1U
#define ioss_0_port_6_pin_6_PORT GPIO_PRT6
#define ioss_0_port_6_pin_6_PIN 6U
#define ioss_0_port_6_pin_6_NUM 6U
#define ioss_0_port_6_pin_6_DRIVEMODE CY_GPIO_DM_PULLUP
#define ioss_0_port_6_pin_6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_6_pin_6_IRQ ioss_interrupts_gpio_6_IRQn
#define ioss_0_port_6_pin_7_ENABLED 1U
#define ioss_0_port_6_pin_7_PORT GPIO_PRT6
#define ioss_0_port_6_pin_7_PIN 7U
#define ioss_0_port_6_pin_7_NUM 7U
#define ioss_0_port_6_pin_7_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define ioss_0_port_6_pin_7_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_6_pin_7_IRQ ioss_interrupts_gpio_6_IRQn

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED9_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_1_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_2_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_3_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_4_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_5_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BTN0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED8_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_6_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_7_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */

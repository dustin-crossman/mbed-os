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

#define WCO_IN_ENABLED 1U
#define WCO_IN_PORT GPIO_PRT0
#define WCO_IN_PIN 0U
#define WCO_IN_NUM 0U
#define WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#define WCO_OUT_ENABLED 1U
#define WCO_OUT_PORT GPIO_PRT0
#define WCO_OUT_PIN 1U
#define WCO_OUT_NUM 1U
#define WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_SW2_ENABLED 1U
#define CYBSP_SW2_PORT GPIO_PRT0
#define CYBSP_SW2_PIN 4U
#define CYBSP_SW2_NUM 4U
#define CYBSP_SW2_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SW2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
	#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SW2_HSIOM ioss_0_port_0_pin_4_HSIOM
#define CYBSP_SW2_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_UART_RX_ENABLED 1U
#define CYBSP_UART_RX_PORT GPIO_PRT5
#define CYBSP_UART_RX_PIN 0U
#define CYBSP_UART_RX_NUM 0U
#define CYBSP_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
	#define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_UART_RX_HSIOM ioss_0_port_5_pin_0_HSIOM
#define CYBSP_UART_RX_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_UART_TX_ENABLED 1U
#define CYBSP_UART_TX_PORT GPIO_PRT5
#define CYBSP_UART_TX_PIN 1U
#define CYBSP_UART_TX_NUM 1U
#define CYBSP_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_UART_TX_HSIOM ioss_0_port_5_pin_1_HSIOM
#define CYBSP_UART_TX_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_LED3_ENABLED 1U
#define CYBSP_LED3_PORT GPIO_PRT6
#define CYBSP_LED3_PIN 3U
#define CYBSP_LED3_NUM 3U
#define CYBSP_LED3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_3_HSIOM
	#define ioss_0_port_6_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED3_HSIOM ioss_0_port_6_pin_3_HSIOM
#define CYBSP_LED3_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_I2C_SCL_ENABLED 1U
#define CYBSP_I2C_SCL_PORT GPIO_PRT6
#define CYBSP_I2C_SCL_PIN 4U
#define CYBSP_I2C_SCL_NUM 4U
#define CYBSP_I2C_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_I2C_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_I2C_SCL_HSIOM ioss_0_port_6_pin_4_HSIOM
#define CYBSP_I2C_SCL_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_I2C_SDA_ENABLED 1U
#define CYBSP_I2C_SDA_PORT GPIO_PRT6
#define CYBSP_I2C_SDA_PIN 5U
#define CYBSP_I2C_SDA_NUM 5U
#define CYBSP_I2C_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_I2C_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_5_HSIOM
	#define ioss_0_port_6_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_I2C_SDA_HSIOM ioss_0_port_6_pin_5_HSIOM
#define CYBSP_I2C_SDA_IRQ ioss_interrupts_gpio_6_IRQn
#define SWDIO_ENABLED 1U
#define SWDIO_PORT GPIO_PRT6
#define SWDIO_PIN 6U
#define SWDIO_NUM 6U
#define SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#define SWCLK_ENABLED 1U
#define SWCLK_PORT GPIO_PRT6
#define SWCLK_PIN 7U
#define SWCLK_NUM 7U
#define SWCLK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define SWCLK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define SWCLK_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_LED4_ENABLED 1U
#define CYBSP_LED4_PORT GPIO_PRT7
#define CYBSP_LED4_PIN 1U
#define CYBSP_LED4_NUM 1U
#define CYBSP_LED4_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED4_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_LED4_IRQ ioss_interrupts_gpio_7_IRQn

extern const cy_stc_gpio_pin_config_t WCO_IN_config;
extern const cy_stc_gpio_pin_config_t WCO_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SW2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_config;
extern const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_config;
extern const cy_stc_gpio_pin_config_t SWDIO_config;
extern const cy_stc_gpio_pin_config_t SWCLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED4_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */

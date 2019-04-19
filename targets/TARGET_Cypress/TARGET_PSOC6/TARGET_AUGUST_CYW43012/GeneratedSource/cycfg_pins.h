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

#define ioss_0_port_0_pin_0_PORT GPIO_PRT0
#define ioss_0_port_0_pin_0_PIN 0U
#define ioss_0_port_0_pin_0_NUM 0U
#define ioss_0_port_0_pin_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_0_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_0_IRQ ioss_interrupts_gpio_0_IRQn
#define ioss_0_port_0_pin_1_PORT GPIO_PRT0
#define ioss_0_port_0_pin_1_PIN 1U
#define ioss_0_port_0_pin_1_NUM 1U
#define ioss_0_port_0_pin_1_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_0_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_1_IRQ ioss_interrupts_gpio_0_IRQn
#define MOT_CUR_FILT_PORT GPIO_PRT10
#define MOT_CUR_FILT_PIN 1U
#define MOT_CUR_FILT_NUM 1U
#define MOT_CUR_FILT_DRIVEMODE CY_GPIO_DM_ANALOG
#define MOT_CUR_FILT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_1_HSIOM
	#define ioss_0_port_10_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_CUR_FILT_HSIOM ioss_0_port_10_pin_1_HSIOM
#define MOT_CUR_FILT_IRQ ioss_interrupts_gpio_10_IRQn
#define ioss_0_port_10_pin_2_PORT GPIO_PRT10
#define ioss_0_port_10_pin_2_PIN 2U
#define ioss_0_port_10_pin_2_NUM 2U
#define ioss_0_port_10_pin_2_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_10_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_2_HSIOM
	#define ioss_0_port_10_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_2_IRQ ioss_interrupts_gpio_10_IRQn
#define FLASH_CS_PORT GPIO_PRT11
#define FLASH_CS_PIN 2U
#define FLASH_CS_NUM 2U
#define FLASH_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define FLASH_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_CS_HSIOM ioss_0_port_11_pin_2_HSIOM
#define FLASH_CS_IRQ ioss_interrupts_gpio_11_IRQn
#define FLASH_IO3_PORT GPIO_PRT11
#define FLASH_IO3_PIN 3U
#define FLASH_IO3_NUM 3U
#define FLASH_IO3_DRIVEMODE CY_GPIO_DM_STRONG
#define FLASH_IO3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_IO3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define FLASH_IO3_IRQ ioss_interrupts_gpio_11_IRQn
#define FLASH_IO2_PORT GPIO_PRT11
#define FLASH_IO2_PIN 4U
#define FLASH_IO2_NUM 4U
#define FLASH_IO2_DRIVEMODE CY_GPIO_DM_STRONG
#define FLASH_IO2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_IO2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define FLASH_IO2_IRQ ioss_interrupts_gpio_11_IRQn
#define FLASH_IO1_PORT GPIO_PRT11
#define FLASH_IO1_PIN 5U
#define FLASH_IO1_NUM 5U
#define FLASH_IO1_DRIVEMODE CY_GPIO_DM_STRONG
#define FLASH_IO1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_IO1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define FLASH_IO1_IRQ ioss_interrupts_gpio_11_IRQn
#define FLASH_IO0_PORT GPIO_PRT11
#define FLASH_IO0_PIN 6U
#define FLASH_IO0_NUM 6U
#define FLASH_IO0_DRIVEMODE CY_GPIO_DM_STRONG
#define FLASH_IO0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_IO0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define FLASH_IO0_IRQ ioss_interrupts_gpio_11_IRQn
#define FLASH_SCK_PORT GPIO_PRT11
#define FLASH_SCK_PIN 7U
#define FLASH_SCK_NUM 7U
#define FLASH_SCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define FLASH_SCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define FLASH_SCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define FLASH_SCK_IRQ ioss_interrupts_gpio_11_IRQn
#define ioss_0_port_12_pin_6_PORT GPIO_PRT12
#define ioss_0_port_12_pin_6_PIN 6U
#define ioss_0_port_12_pin_6_NUM 6U
#define ioss_0_port_12_pin_6_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_6_HSIOM
	#define ioss_0_port_12_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_6_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_7_PORT GPIO_PRT12
#define ioss_0_port_12_pin_7_PIN 7U
#define ioss_0_port_12_pin_7_NUM 7U
#define ioss_0_port_12_pin_7_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_7_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_7_HSIOM
	#define ioss_0_port_12_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_7_IRQ ioss_interrupts_gpio_12_IRQn
#define UTX_PORT GPIO_PRT13
#define UTX_PIN 0U
#define UTX_NUM 0U
#define UTX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define UTX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_0_HSIOM
	#define ioss_0_port_13_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define UTX_HSIOM ioss_0_port_13_pin_0_HSIOM
#define UTX_IRQ ioss_interrupts_gpio_13_IRQn
#define URX_PORT GPIO_PRT13
#define URX_PIN 1U
#define URX_NUM 1U
#define URX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define URX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_1_HSIOM
	#define ioss_0_port_13_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define URX_HSIOM ioss_0_port_13_pin_1_HSIOM
#define URX_IRQ ioss_interrupts_gpio_13_IRQn
#define LED_RED_PORT GPIO_PRT13
#define LED_RED_PIN 3U
#define LED_RED_NUM 3U
#define LED_RED_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define LED_RED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_3_HSIOM
	#define ioss_0_port_13_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_RED_HSIOM ioss_0_port_13_pin_3_HSIOM
#define LED_RED_IRQ ioss_interrupts_gpio_13_IRQn
#define LED_BLUE_PORT GPIO_PRT13
#define LED_BLUE_PIN 5U
#define LED_BLUE_NUM 5U
#define LED_BLUE_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define LED_BLUE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_13_pin_5_HSIOM
	#define ioss_0_port_13_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_BLUE_HSIOM ioss_0_port_13_pin_5_HSIOM
#define LED_BLUE_IRQ ioss_interrupts_gpio_13_IRQn
#define LED_GREEN_PORT GPIO_PRT13
#define LED_GREEN_PIN 7U
#define LED_GREEN_NUM 7U
#define LED_GREEN_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define LED_GREEN_INIT_DRIVESTATE 0
#ifndef ioss_0_port_13_pin_7_HSIOM
	#define ioss_0_port_13_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_GREEN_HSIOM ioss_0_port_13_pin_7_HSIOM
#define LED_GREEN_IRQ ioss_interrupts_gpio_13_IRQn
#define SCL_PORT GPIO_PRT1
#define SCL_PIN 0U
#define SCL_NUM 0U
#define SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define SCL_HSIOM ioss_0_port_1_pin_0_HSIOM
#define SCL_IRQ ioss_interrupts_gpio_1_IRQn
#define SDA_PORT GPIO_PRT1
#define SDA_PIN 1U
#define SDA_NUM 1U
#define SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
	#define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define SDA_HSIOM ioss_0_port_1_pin_1_HSIOM
#define SDA_IRQ ioss_interrupts_gpio_1_IRQn
#define GAS_GAUGE_EN_PORT GPIO_PRT1
#define GAS_GAUGE_EN_PIN 3U
#define GAS_GAUGE_EN_NUM 3U
#define GAS_GAUGE_EN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define GAS_GAUGE_EN_INIT_DRIVESTATE 0
#ifndef ioss_0_port_1_pin_3_HSIOM
	#define ioss_0_port_1_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define GAS_GAUGE_EN_HSIOM ioss_0_port_1_pin_3_HSIOM
#define GAS_GAUGE_EN_IRQ ioss_interrupts_gpio_1_IRQn
#define MAG_INT_PORT GPIO_PRT1
#define MAG_INT_PIN 4U
#define MAG_INT_NUM 4U
#define MAG_INT_DRIVEMODE CY_GPIO_DM_HIGHZ
#define MAG_INT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
	#define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define MAG_INT_HSIOM ioss_0_port_1_pin_4_HSIOM
#define MAG_INT_IRQ ioss_interrupts_gpio_1_IRQn
#define SDIO_DATA_0_PORT GPIO_PRT2
#define SDIO_DATA_0_PIN 0U
#define SDIO_DATA_0_NUM 0U
#define SDIO_DATA_0_DRIVEMODE CY_GPIO_DM_STRONG
#define SDIO_DATA_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_0_HSIOM
	#define ioss_0_port_2_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_DATA_0_HSIOM ioss_0_port_2_pin_0_HSIOM
#define SDIO_DATA_0_IRQ ioss_interrupts_gpio_2_IRQn
#define SDIO_DATA_1_PORT GPIO_PRT2
#define SDIO_DATA_1_PIN 1U
#define SDIO_DATA_1_NUM 1U
#define SDIO_DATA_1_DRIVEMODE CY_GPIO_DM_STRONG
#define SDIO_DATA_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_1_HSIOM
	#define ioss_0_port_2_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_DATA_1_HSIOM ioss_0_port_2_pin_1_HSIOM
#define SDIO_DATA_1_IRQ ioss_interrupts_gpio_2_IRQn
#define SDIO_DATA_2_PORT GPIO_PRT2
#define SDIO_DATA_2_PIN 2U
#define SDIO_DATA_2_NUM 2U
#define SDIO_DATA_2_DRIVEMODE CY_GPIO_DM_STRONG
#define SDIO_DATA_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_2_HSIOM
	#define ioss_0_port_2_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_DATA_2_HSIOM ioss_0_port_2_pin_2_HSIOM
#define SDIO_DATA_2_IRQ ioss_interrupts_gpio_2_IRQn
#define SDIO_DATA_3_PORT GPIO_PRT2
#define SDIO_DATA_3_PIN 3U
#define SDIO_DATA_3_NUM 3U
#define SDIO_DATA_3_DRIVEMODE CY_GPIO_DM_STRONG
#define SDIO_DATA_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_3_HSIOM
	#define ioss_0_port_2_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_DATA_3_HSIOM ioss_0_port_2_pin_3_HSIOM
#define SDIO_DATA_3_IRQ ioss_interrupts_gpio_2_IRQn
#define SDIO_CMD_PORT GPIO_PRT2
#define SDIO_CMD_PIN 4U
#define SDIO_CMD_NUM 4U
#define SDIO_CMD_DRIVEMODE CY_GPIO_DM_STRONG
#define SDIO_CMD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
	#define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_CMD_HSIOM ioss_0_port_2_pin_4_HSIOM
#define SDIO_CMD_IRQ ioss_interrupts_gpio_2_IRQn
#define SDIO_CLK_PORT GPIO_PRT2
#define SDIO_CLK_PIN 5U
#define SDIO_CLK_NUM 5U
#define SDIO_CLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SDIO_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
	#define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define SDIO_CLK_HSIOM ioss_0_port_2_pin_5_HSIOM
#define SDIO_CLK_IRQ ioss_interrupts_gpio_2_IRQn
#define WL_REG_ON_PORT GPIO_PRT2
#define WL_REG_ON_PIN 6U
#define WL_REG_ON_NUM 6U
#define WL_REG_ON_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define WL_REG_ON_INIT_DRIVESTATE 0
#ifndef ioss_0_port_2_pin_6_HSIOM
	#define ioss_0_port_2_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define WL_REG_ON_HSIOM ioss_0_port_2_pin_6_HSIOM
#define WL_REG_ON_IRQ ioss_interrupts_gpio_2_IRQn
#define WL_HOST_WAKE_PORT GPIO_PRT2
#define WL_HOST_WAKE_PIN 7U
#define WL_HOST_WAKE_NUM 7U
#define WL_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define WL_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_2_pin_7_HSIOM
	#define ioss_0_port_2_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define WL_HOST_WAKE_HSIOM ioss_0_port_2_pin_7_HSIOM
#define WL_HOST_WAKE_IRQ ioss_interrupts_gpio_2_IRQn
#define BT_UART_TXD_PORT GPIO_PRT3
#define BT_UART_TXD_PIN 0U
#define BT_UART_TXD_NUM 0U
#define BT_UART_TXD_DRIVEMODE CY_GPIO_DM_HIGHZ
#define BT_UART_TXD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_0_HSIOM
	#define ioss_0_port_3_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_TXD_HSIOM ioss_0_port_3_pin_0_HSIOM
#define BT_UART_TXD_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_RXD_PORT GPIO_PRT3
#define BT_UART_RXD_PIN 1U
#define BT_UART_RXD_NUM 1U
#define BT_UART_RXD_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define BT_UART_RXD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_1_HSIOM
	#define ioss_0_port_3_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_RXD_HSIOM ioss_0_port_3_pin_1_HSIOM
#define BT_UART_RXD_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_CTS_PORT GPIO_PRT3
#define BT_UART_CTS_PIN 2U
#define BT_UART_CTS_NUM 2U
#define BT_UART_CTS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define BT_UART_CTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_2_HSIOM
	#define ioss_0_port_3_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_CTS_HSIOM ioss_0_port_3_pin_2_HSIOM
#define BT_UART_CTS_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_RTS_PORT GPIO_PRT3
#define BT_UART_RTS_PIN 3U
#define BT_UART_RTS_NUM 3U
#define BT_UART_RTS_DRIVEMODE CY_GPIO_DM_HIGHZ
#define BT_UART_RTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
	#define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_RTS_HSIOM ioss_0_port_3_pin_3_HSIOM
#define BT_UART_RTS_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_REG_ON_PORT GPIO_PRT3
#define BT_REG_ON_PIN 4U
#define BT_REG_ON_NUM 4U
#define BT_REG_ON_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define BT_REG_ON_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_REG_ON_HSIOM ioss_0_port_3_pin_4_HSIOM
#define BT_REG_ON_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_HOST_WAKE_PORT GPIO_PRT3
#define BT_HOST_WAKE_PIN 5U
#define BT_HOST_WAKE_NUM 5U
#define BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define BT_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_5_HSIOM
	#define ioss_0_port_3_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_HOST_WAKE_HSIOM ioss_0_port_3_pin_5_HSIOM
#define BT_HOST_WAKE_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_DEV_WAKE_PORT GPIO_PRT4
#define BT_DEV_WAKE_PIN 0U
#define BT_DEV_WAKE_NUM 0U
#define BT_DEV_WAKE_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define BT_DEV_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_0_HSIOM
	#define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_DEV_WAKE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define BT_DEV_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#define WL_DEV_WAKE_PORT GPIO_PRT4
#define WL_DEV_WAKE_PIN 1U
#define WL_DEV_WAKE_NUM 1U
#define WL_DEV_WAKE_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define WL_DEV_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_1_HSIOM
	#define ioss_0_port_4_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define WL_DEV_WAKE_HSIOM ioss_0_port_4_pin_1_HSIOM
#define WL_DEV_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#define PCLK_PORT GPIO_PRT5
#define PCLK_PIN 1U
#define PCLK_NUM 1U
#define PCLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define PCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define PCLK_HSIOM ioss_0_port_5_pin_1_HSIOM
#define PCLK_IRQ ioss_interrupts_gpio_5_IRQn
#define LRCLK_PORT GPIO_PRT5
#define LRCLK_PIN 2U
#define LRCLK_NUM 2U
#define LRCLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LRCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_2_HSIOM
	#define ioss_0_port_5_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define LRCLK_HSIOM ioss_0_port_5_pin_2_HSIOM
#define LRCLK_IRQ ioss_interrupts_gpio_5_IRQn
#define DATA_L_PORT GPIO_PRT5
#define DATA_L_PIN 3U
#define DATA_L_NUM 3U
#define DATA_L_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define DATA_L_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_3_HSIOM
	#define ioss_0_port_5_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define DATA_L_HSIOM ioss_0_port_5_pin_3_HSIOM
#define DATA_L_IRQ ioss_interrupts_gpio_5_IRQn
#define AUDIO_GAIN_SEL_PORT GPIO_PRT5
#define AUDIO_GAIN_SEL_PIN 5U
#define AUDIO_GAIN_SEL_NUM 5U
#define AUDIO_GAIN_SEL_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define AUDIO_GAIN_SEL_INIT_DRIVESTATE 0
#ifndef ioss_0_port_5_pin_5_HSIOM
	#define ioss_0_port_5_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define AUDIO_GAIN_SEL_HSIOM ioss_0_port_5_pin_5_HSIOM
#define AUDIO_GAIN_SEL_IRQ ioss_interrupts_gpio_5_IRQn
#define AUDIO_SD_MODE_PORT GPIO_PRT5
#define AUDIO_SD_MODE_PIN 6U
#define AUDIO_SD_MODE_NUM 6U
#define AUDIO_SD_MODE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define AUDIO_SD_MODE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_5_pin_6_HSIOM
	#define ioss_0_port_5_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define AUDIO_SD_MODE_HSIOM ioss_0_port_5_pin_6_HSIOM
#define AUDIO_SD_MODE_IRQ ioss_interrupts_gpio_5_IRQn
#define ENCODER_1_PORT GPIO_PRT6
#define ENCODER_1_PIN 3U
#define ENCODER_1_NUM 3U
#define ENCODER_1_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ENCODER_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_3_HSIOM
	#define ioss_0_port_6_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ENCODER_1_HSIOM ioss_0_port_6_pin_3_HSIOM
#define ENCODER_1_IRQ ioss_interrupts_gpio_6_IRQn
#define SWO_PORT GPIO_PRT6
#define SWO_PIN 4U
#define SWO_NUM 4U
#define SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define SWO_IRQ ioss_interrupts_gpio_6_IRQn
#define ENCODER_3_PORT GPIO_PRT6
#define ENCODER_3_PIN 5U
#define ENCODER_3_NUM 5U
#define ENCODER_3_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ENCODER_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_5_HSIOM
	#define ioss_0_port_6_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ENCODER_3_HSIOM ioss_0_port_6_pin_5_HSIOM
#define ENCODER_3_IRQ ioss_interrupts_gpio_6_IRQn
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
#define ENCODER_0_PORT GPIO_PRT7
#define ENCODER_0_PIN 0U
#define ENCODER_0_NUM 0U
#define ENCODER_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ENCODER_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_0_HSIOM
	#define ioss_0_port_7_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ENCODER_0_HSIOM ioss_0_port_7_pin_0_HSIOM
#define ENCODER_0_IRQ ioss_interrupts_gpio_7_IRQn
#define ENCODER_2_PORT GPIO_PRT7
#define ENCODER_2_PIN 5U
#define ENCODER_2_NUM 5U
#define ENCODER_2_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ENCODER_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_5_HSIOM
	#define ioss_0_port_7_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ENCODER_2_HSIOM ioss_0_port_7_pin_5_HSIOM
#define ENCODER_2_IRQ ioss_interrupts_gpio_7_IRQn
#define MOT_EN_1_PORT GPIO_PRT8
#define MOT_EN_1_PIN 0U
#define MOT_EN_1_NUM 0U
#define MOT_EN_1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define MOT_EN_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_0_HSIOM
	#define ioss_0_port_8_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_EN_1_HSIOM ioss_0_port_8_pin_0_HSIOM
#define MOT_EN_1_IRQ ioss_interrupts_gpio_8_IRQn
#define MOT_SLEEP_PORT GPIO_PRT8
#define MOT_SLEEP_PIN 1U
#define MOT_SLEEP_NUM 1U
#define MOT_SLEEP_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define MOT_SLEEP_INIT_DRIVESTATE 0
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_SLEEP_HSIOM ioss_0_port_8_pin_1_HSIOM
#define MOT_SLEEP_IRQ ioss_interrupts_gpio_8_IRQn
#define BUTTON_PORT GPIO_PRT8
#define BUTTON_PIN 3U
#define BUTTON_NUM 3U
#define BUTTON_DRIVEMODE CY_GPIO_DM_PULLUP_DOWN
#define BUTTON_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define BUTTON_HSIOM ioss_0_port_8_pin_3_HSIOM
#define BUTTON_IRQ ioss_interrupts_gpio_8_IRQn
#define MOT_EN_2_PORT GPIO_PRT8
#define MOT_EN_2_PIN 4U
#define MOT_EN_2_NUM 4U
#define MOT_EN_2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define MOT_EN_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_4_HSIOM
	#define ioss_0_port_8_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_EN_2_HSIOM ioss_0_port_8_pin_4_HSIOM
#define MOT_EN_2_IRQ ioss_interrupts_gpio_8_IRQn
#define BAT_VOLT_DIV_PORT GPIO_PRT9
#define BAT_VOLT_DIV_PIN 0U
#define BAT_VOLT_DIV_NUM 0U
#define BAT_VOLT_DIV_DRIVEMODE CY_GPIO_DM_ANALOG
#define BAT_VOLT_DIV_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define BAT_VOLT_DIV_HSIOM ioss_0_port_9_pin_0_HSIOM
#define BAT_VOLT_DIV_IRQ ioss_interrupts_gpio_9_IRQn
#define MOT_CUR_PGA_OUT_PORT GPIO_PRT9
#define MOT_CUR_PGA_OUT_PIN 3U
#define MOT_CUR_PGA_OUT_NUM 3U
#define MOT_CUR_PGA_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define MOT_CUR_PGA_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_3_HSIOM
	#define ioss_0_port_9_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_CUR_PGA_OUT_HSIOM ioss_0_port_9_pin_3_HSIOM
#define MOT_CUR_PGA_OUT_IRQ ioss_interrupts_gpio_9_IRQn
#define MOT_CUR_PGA_NEG_PORT GPIO_PRT9
#define MOT_CUR_PGA_NEG_PIN 4U
#define MOT_CUR_PGA_NEG_NUM 4U
#define MOT_CUR_PGA_NEG_DRIVEMODE CY_GPIO_DM_ANALOG
#define MOT_CUR_PGA_NEG_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_4_HSIOM
	#define ioss_0_port_9_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_CUR_PGA_NEG_HSIOM ioss_0_port_9_pin_4_HSIOM
#define MOT_CUR_PGA_NEG_IRQ ioss_interrupts_gpio_9_IRQn
#define MOT_CUR_PGA_POS_PORT GPIO_PRT9
#define MOT_CUR_PGA_POS_PIN 5U
#define MOT_CUR_PGA_POS_NUM 5U
#define MOT_CUR_PGA_POS_DRIVEMODE CY_GPIO_DM_ANALOG
#define MOT_CUR_PGA_POS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_5_HSIOM
	#define ioss_0_port_9_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define MOT_CUR_PGA_POS_HSIOM ioss_0_port_9_pin_5_HSIOM
#define MOT_CUR_PGA_POS_IRQ ioss_interrupts_gpio_9_IRQn

extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_1_config;
extern const cy_stc_gpio_pin_config_t MOT_CUR_FILT_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_2_config;
extern const cy_stc_gpio_pin_config_t FLASH_CS_config;
extern const cy_stc_gpio_pin_config_t FLASH_IO3_config;
extern const cy_stc_gpio_pin_config_t FLASH_IO2_config;
extern const cy_stc_gpio_pin_config_t FLASH_IO1_config;
extern const cy_stc_gpio_pin_config_t FLASH_IO0_config;
extern const cy_stc_gpio_pin_config_t FLASH_SCK_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_6_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_7_config;
extern const cy_stc_gpio_pin_config_t UTX_config;
extern const cy_stc_gpio_pin_config_t URX_config;
extern const cy_stc_gpio_pin_config_t LED_RED_config;
extern const cy_stc_gpio_pin_config_t LED_BLUE_config;
extern const cy_stc_gpio_pin_config_t LED_GREEN_config;
extern const cy_stc_gpio_pin_config_t SCL_config;
extern const cy_stc_gpio_pin_config_t SDA_config;
extern const cy_stc_gpio_pin_config_t GAS_GAUGE_EN_config;
extern const cy_stc_gpio_pin_config_t MAG_INT_config;
extern const cy_stc_gpio_pin_config_t SDIO_DATA_0_config;
extern const cy_stc_gpio_pin_config_t SDIO_DATA_1_config;
extern const cy_stc_gpio_pin_config_t SDIO_DATA_2_config;
extern const cy_stc_gpio_pin_config_t SDIO_DATA_3_config;
extern const cy_stc_gpio_pin_config_t SDIO_CMD_config;
extern const cy_stc_gpio_pin_config_t SDIO_CLK_config;
extern const cy_stc_gpio_pin_config_t WL_REG_ON_config;
extern const cy_stc_gpio_pin_config_t WL_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t BT_UART_TXD_config;
extern const cy_stc_gpio_pin_config_t BT_UART_RXD_config;
extern const cy_stc_gpio_pin_config_t BT_UART_CTS_config;
extern const cy_stc_gpio_pin_config_t BT_UART_RTS_config;
extern const cy_stc_gpio_pin_config_t BT_REG_ON_config;
extern const cy_stc_gpio_pin_config_t BT_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t BT_DEV_WAKE_config;
extern const cy_stc_gpio_pin_config_t WL_DEV_WAKE_config;
extern const cy_stc_gpio_pin_config_t PCLK_config;
extern const cy_stc_gpio_pin_config_t LRCLK_config;
extern const cy_stc_gpio_pin_config_t DATA_L_config;
extern const cy_stc_gpio_pin_config_t AUDIO_GAIN_SEL_config;
extern const cy_stc_gpio_pin_config_t AUDIO_SD_MODE_config;
extern const cy_stc_gpio_pin_config_t ENCODER_1_config;
extern const cy_stc_gpio_pin_config_t SWO_config;
extern const cy_stc_gpio_pin_config_t ENCODER_3_config;
extern const cy_stc_gpio_pin_config_t SWDIO_config;
extern const cy_stc_gpio_pin_config_t SWCLK_config;
extern const cy_stc_gpio_pin_config_t ENCODER_0_config;
extern const cy_stc_gpio_pin_config_t ENCODER_2_config;
extern const cy_stc_gpio_pin_config_t MOT_EN_1_config;
extern const cy_stc_gpio_pin_config_t MOT_SLEEP_config;
extern const cy_stc_gpio_pin_config_t BUTTON_config;
extern const cy_stc_gpio_pin_config_t MOT_EN_2_config;
extern const cy_stc_gpio_pin_config_t BAT_VOLT_DIV_config;
extern const cy_stc_gpio_pin_config_t MOT_CUR_PGA_OUT_config;
extern const cy_stc_gpio_pin_config_t MOT_CUR_PGA_NEG_config;
extern const cy_stc_gpio_pin_config_t MOT_CUR_PGA_POS_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */

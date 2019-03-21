/*******************************************************************************
* File Name: cycfg_pins.c
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t MOT_CUR_FILT_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = MOT_CUR_FILT_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = ioss_0_port_10_pin_2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_CS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = FLASH_CS_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_IO3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = FLASH_IO3_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_IO2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = FLASH_IO2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_IO1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = FLASH_IO1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_IO0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = FLASH_IO0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t FLASH_SCK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = FLASH_SCK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_6_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = ioss_0_port_12_pin_6_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_7_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = ioss_0_port_12_pin_7_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t UTX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = UTX_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t URX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = URX_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t LED_RED_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = LED_RED_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t LED_BLUE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = LED_BLUE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t LED_GREEN_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = LED_GREEN_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SCL_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = SCL_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDA_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = SDA_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t GAS_GAUGE_EN_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = GAS_GAUGE_EN_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MAG_INT_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = MAG_INT_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_DATA_0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_DATA_0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_DATA_1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_DATA_1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_DATA_2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_DATA_2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_DATA_3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_DATA_3_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_CMD_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_CMD_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SDIO_CLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = SDIO_CLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t WL_REG_ON_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF,
	.hsiom = WL_REG_ON_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t WL_HOST_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = WL_HOST_WAKE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_UART_TXD_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = BT_UART_TXD_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_UART_RXD_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = BT_UART_RXD_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_UART_CTS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = BT_UART_CTS_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_UART_RTS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = BT_UART_RTS_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_REG_ON_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF,
	.hsiom = BT_REG_ON_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_HOST_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = BT_HOST_WAKE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BT_DEV_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_PULLDOWN,
	.hsiom = BT_DEV_WAKE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t WL_DEV_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_PULLDOWN,
	.hsiom = WL_DEV_WAKE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t PCLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = PCLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t LRCLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = LRCLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t DATA_L_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = DATA_L_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t AUDIO_GAIN_SEL_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = AUDIO_GAIN_SEL_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t AUDIO_SD_MODE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = AUDIO_SD_MODE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ENCODER_1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = ENCODER_1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SWO_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = SWO_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ENCODER_3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = ENCODER_3_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SWDIO_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLUP,
	.hsiom = SWDIO_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t SWCLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLDOWN,
	.hsiom = SWCLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ENCODER_0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = ENCODER_0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t ENCODER_2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = ENCODER_2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_EN_1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = MOT_EN_1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_SLEEP_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = MOT_SLEEP_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BUTTON_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLUP_DOWN,
	.hsiom = BUTTON_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_EN_2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = MOT_EN_2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t BAT_VOLT_DIV_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = BAT_VOLT_DIV_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_CUR_PGA_OUT_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = MOT_CUR_PGA_OUT_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_CUR_PGA_NEG_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = MOT_CUR_PGA_NEG_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
const cy_stc_gpio_pin_config_t MOT_CUR_PGA_POS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = MOT_CUR_PGA_POS_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_FULL,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};


void init_cycfg_pins(void)
{
	Cy_GPIO_Pin_Init(MOT_CUR_FILT_PORT, MOT_CUR_FILT_PIN, &MOT_CUR_FILT_config);

	Cy_GPIO_Pin_Init(ioss_0_port_10_pin_2_PORT, ioss_0_port_10_pin_2_PIN, &ioss_0_port_10_pin_2_config);

	Cy_GPIO_Pin_Init(FLASH_CS_PORT, FLASH_CS_PIN, &FLASH_CS_config);

	Cy_GPIO_Pin_Init(FLASH_IO3_PORT, FLASH_IO3_PIN, &FLASH_IO3_config);

	Cy_GPIO_Pin_Init(FLASH_IO2_PORT, FLASH_IO2_PIN, &FLASH_IO2_config);

	Cy_GPIO_Pin_Init(FLASH_IO1_PORT, FLASH_IO1_PIN, &FLASH_IO1_config);

	Cy_GPIO_Pin_Init(FLASH_IO0_PORT, FLASH_IO0_PIN, &FLASH_IO0_config);

	Cy_GPIO_Pin_Init(FLASH_SCK_PORT, FLASH_SCK_PIN, &FLASH_SCK_config);

	Cy_GPIO_Pin_Init(ioss_0_port_12_pin_6_PORT, ioss_0_port_12_pin_6_PIN, &ioss_0_port_12_pin_6_config);

	Cy_GPIO_Pin_Init(ioss_0_port_12_pin_7_PORT, ioss_0_port_12_pin_7_PIN, &ioss_0_port_12_pin_7_config);

	Cy_GPIO_Pin_Init(UTX_PORT, UTX_PIN, &UTX_config);

	Cy_GPIO_Pin_Init(URX_PORT, URX_PIN, &URX_config);

	Cy_GPIO_Pin_Init(LED_RED_PORT, LED_RED_PIN, &LED_RED_config);

	Cy_GPIO_Pin_Init(LED_BLUE_PORT, LED_BLUE_PIN, &LED_BLUE_config);

	Cy_GPIO_Pin_Init(LED_GREEN_PORT, LED_GREEN_PIN, &LED_GREEN_config);

	Cy_GPIO_Pin_Init(SCL_PORT, SCL_PIN, &SCL_config);

	Cy_GPIO_Pin_Init(SDA_PORT, SDA_PIN, &SDA_config);

	Cy_GPIO_Pin_Init(GAS_GAUGE_EN_PORT, GAS_GAUGE_EN_PIN, &GAS_GAUGE_EN_config);

	Cy_GPIO_Pin_Init(MAG_INT_PORT, MAG_INT_PIN, &MAG_INT_config);

	Cy_GPIO_Pin_Init(SDIO_DATA_0_PORT, SDIO_DATA_0_PIN, &SDIO_DATA_0_config);

	Cy_GPIO_Pin_Init(SDIO_DATA_1_PORT, SDIO_DATA_1_PIN, &SDIO_DATA_1_config);

	Cy_GPIO_Pin_Init(SDIO_DATA_2_PORT, SDIO_DATA_2_PIN, &SDIO_DATA_2_config);

	Cy_GPIO_Pin_Init(SDIO_DATA_3_PORT, SDIO_DATA_3_PIN, &SDIO_DATA_3_config);

	Cy_GPIO_Pin_Init(SDIO_CMD_PORT, SDIO_CMD_PIN, &SDIO_CMD_config);

	Cy_GPIO_Pin_Init(SDIO_CLK_PORT, SDIO_CLK_PIN, &SDIO_CLK_config);

	Cy_GPIO_Pin_Init(WL_REG_ON_PORT, WL_REG_ON_PIN, &WL_REG_ON_config);

	Cy_GPIO_Pin_Init(WL_HOST_WAKE_PORT, WL_HOST_WAKE_PIN, &WL_HOST_WAKE_config);

	Cy_GPIO_Pin_Init(BT_UART_TXD_PORT, BT_UART_TXD_PIN, &BT_UART_TXD_config);

	Cy_GPIO_Pin_Init(BT_UART_RXD_PORT, BT_UART_RXD_PIN, &BT_UART_RXD_config);

	Cy_GPIO_Pin_Init(BT_UART_CTS_PORT, BT_UART_CTS_PIN, &BT_UART_CTS_config);

	Cy_GPIO_Pin_Init(BT_UART_RTS_PORT, BT_UART_RTS_PIN, &BT_UART_RTS_config);

	Cy_GPIO_Pin_Init(BT_REG_ON_PORT, BT_REG_ON_PIN, &BT_REG_ON_config);

	Cy_GPIO_Pin_Init(BT_HOST_WAKE_PORT, BT_HOST_WAKE_PIN, &BT_HOST_WAKE_config);

	Cy_GPIO_Pin_Init(BT_DEV_WAKE_PORT, BT_DEV_WAKE_PIN, &BT_DEV_WAKE_config);

	Cy_GPIO_Pin_Init(WL_DEV_WAKE_PORT, WL_DEV_WAKE_PIN, &WL_DEV_WAKE_config);

	Cy_GPIO_Pin_Init(PCLK_PORT, PCLK_PIN, &PCLK_config);

	Cy_GPIO_Pin_Init(LRCLK_PORT, LRCLK_PIN, &LRCLK_config);

	Cy_GPIO_Pin_Init(DATA_L_PORT, DATA_L_PIN, &DATA_L_config);

	Cy_GPIO_Pin_Init(AUDIO_GAIN_SEL_PORT, AUDIO_GAIN_SEL_PIN, &AUDIO_GAIN_SEL_config);

	Cy_GPIO_Pin_Init(AUDIO_SD_MODE_PORT, AUDIO_SD_MODE_PIN, &AUDIO_SD_MODE_config);

	Cy_GPIO_Pin_Init(ENCODER_1_PORT, ENCODER_1_PIN, &ENCODER_1_config);

	Cy_GPIO_Pin_Init(SWO_PORT, SWO_PIN, &SWO_config);

	Cy_GPIO_Pin_Init(ENCODER_3_PORT, ENCODER_3_PIN, &ENCODER_3_config);

	Cy_GPIO_Pin_Init(SWDIO_PORT, SWDIO_PIN, &SWDIO_config);

	Cy_GPIO_Pin_Init(SWCLK_PORT, SWCLK_PIN, &SWCLK_config);

	Cy_GPIO_Pin_Init(ENCODER_0_PORT, ENCODER_0_PIN, &ENCODER_0_config);

	Cy_GPIO_Pin_Init(ENCODER_2_PORT, ENCODER_2_PIN, &ENCODER_2_config);

	Cy_GPIO_Pin_Init(MOT_EN_1_PORT, MOT_EN_1_PIN, &MOT_EN_1_config);

	Cy_GPIO_Pin_Init(MOT_SLEEP_PORT, MOT_SLEEP_PIN, &MOT_SLEEP_config);

	Cy_GPIO_Pin_Init(BUTTON_PORT, BUTTON_PIN, &BUTTON_config);

	Cy_GPIO_Pin_Init(MOT_EN_2_PORT, MOT_EN_2_PIN, &MOT_EN_2_config);

	Cy_GPIO_Pin_Init(BAT_VOLT_DIV_PORT, BAT_VOLT_DIV_PIN, &BAT_VOLT_DIV_config);

	Cy_GPIO_Pin_Init(MOT_CUR_PGA_OUT_PORT, MOT_CUR_PGA_OUT_PIN, &MOT_CUR_PGA_OUT_config);

	Cy_GPIO_Pin_Init(MOT_CUR_PGA_NEG_PORT, MOT_CUR_PGA_NEG_PIN, &MOT_CUR_PGA_NEG_config);

	Cy_GPIO_Pin_Init(MOT_CUR_PGA_POS_PORT, MOT_CUR_PGA_POS_PIN, &MOT_CUR_PGA_POS_config);
}

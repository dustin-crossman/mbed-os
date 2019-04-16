/*******************************************************************************
* File Name: cy_hal.h
*
* Description:
* Top-level HAL header file that includes all available HAL header files. This
* will pull in all of the specific HAL files needed. Not all of these may be
* supported in the target device. The target device must provide a 
* cy_hal_implementation.h file that is in the include path for this to depend
* on. The cy_hal_implementation.h file must provide definitions for each of
* the resource types consumed by the HAL driver functions.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

/**
* \addtogroup group_hal
* \{
* TODO: high-level HAL description
* update hal/include/cy_hal.h to change this text
* \} group_hal
*/

#pragma once

#include "cy_hal_adc.h"
#include "cy_hal_comp.h"
#include "cy_hal_crc.h"
#include "cy_hal_dac.h"
#include "cy_hal_dma.h"
#include "cy_hal_flash.h"
#include "cy_hal_gpio.h"
#include "cy_hal_gpio_impl.h"
#include "cy_hal_hwmgr.h"
#include "cy_hal_i2c.h"
#include "cy_hal_i2s.h"
#include "cy_hal_interconnect.h"
#include "cy_hal_opamp.h"
#include "cy_hal_pdmpcm.h"
#include "cy_hal_pwm.h"
#include "cy_hal_qspi.h"
#include "cy_hal_rng.h"
#include "cy_hal_rtc.h"
#include "cy_hal_sdhc.h"
#include "cy_hal_sdio.h"
#include "cy_hal_spi.h"
#include "cy_hal_system.h"
#include "cy_hal_timer.h"
#include "cy_hal_uart.h"
#include "cy_hal_wdt.h"
#include "cy_hal_crc.h"
#include "cy_hal_crc_impl.h"

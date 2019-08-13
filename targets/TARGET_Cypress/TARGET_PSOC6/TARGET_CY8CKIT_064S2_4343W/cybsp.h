/***************************************************************************//**
* \file CY8CKIT-064S2-4343W/cybsp.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-064S2-4343W pioneer kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_bsp_cy8ckit_064s2_4343w CY8CKIT-064S2-4343W
* \ingroup group_bsp
* \{
* The PSoC 6 WiFi-BT Pioneer Kit is a low-cost hardware platform
* that enables design and debug of the PSoC 64 MCU (CYB0644ABZI-D44)
* and the Murata LBEE5KL1DX Module (CYW4343W WiFi + Bluetooth Combo Chip).
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>BLE v5.0</li>
* <li>Serial memory interface</li>
* <li>PDM-PCM digital microphone interface</li>
* <li>Industry-leading CapSense</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>CY8CKIT-064S2-4343W evaluation board</li>
* <li>TFT display shield with a 2.4" TFT display, light sensor, 6-axis motion sensor, and digital microphone</li>
* <li>USB cable</li>
* </ul>
*
* \defgroup group_bsp_cy8ckit_064s2_4343w_macros Macros
* \defgroup group_bsp_cy8ckit_064s2_4343w_functions Functions
* \defgroup group_bsp_cy8ckit_064s2_4343w_enums Enumerated Types
*/

#pragma once

#include "cybsp_types.h"
#include "cybsp_core.h"
#if defined(CYBSP_WIFI_CAPABLE)
#include "cybsp_wifi.h"
#endif
#ifndef __MBED__
#include "cybsp_retarget.h"
#include "cybsp_serial_flash.h"
#endif /* __MBED__ */

#if defined(__cplusplus)
extern "C" {
#endif

/** \cond INTERNAL */

// HAL HW configuration data
extern cyhal_qspi_t cybsp_qspi;
extern cyhal_uart_t cybsp_bt_uart;
extern cyhal_uart_t cybsp_debug_uart;
extern cyhal_i2c_t cybsp_i2c;
extern cyhal_rtc_t cybsp_rtc;

/** \endcond */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8ckit_064s2_4343w */

/***************************************************************************//**
* \file cybsp_cy8cproto_063_ble.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-063-BLE prototyping kit.
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
* \addtogroup group_bsp_cy8cproto_063_ble CY8CPROTO-063-BLE
* \ingroup group_bsp
* \{
* The PSoC 6 BLE Prototyping Kit (CY8CPROTO-063-BLE) is a low-cost hardware platform that enables design and debug of PSoC 6 MCUs. 
* This kit is designed with a snap-away form-factor, allowing users to separate the KitProg (on-board programmer and debugger) from the 
* target board and use independently.
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>BLE 5.0 certified Cypress CYBLE-416045-02 EZ-BLE module with onboard crystal oscillators, trace antenna, passive components and PSoC 63 MCU</li>
* <li>Up to 36 GPIOs in a 14x18.5x2 mm package</li>
* <li>Supports digital programmable logic, capacitive-sensing with CapSense, a PDM-PCM digital microphone interface, a Quad-SPI interface, high-performance analog-to-digital converter (ADC), low-power comparators, and standard communication and timing peripherals.</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>PSoC 6 BLE Prototyping Board</li>
* <li>USB Type-A to Micro-B cable</li>
* <li>Quick Start Guide</li>
* </ul>
*
* \defgroup group_bsp_cy8cproto_063_ble_macros Macros
* \defgroup group_bsp_cy8cproto_063_ble_functions Functions
* \defgroup group_bsp_cy8cproto_063_ble_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cybsp_api_core.h"
#include "cybsp_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_cy8cproto_063_ble_macros
* \{
*/


/** \} group_bsp_cy8cproto_063_ble_macros */

/** \cond INTERNAL */

/** \endcond */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8cproto_063_ble */

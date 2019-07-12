/***************************************************************************//**
* \file CYW9P62S1-43438EVB-01/cybsp.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CYW9P62S1-43438EVB-01 pioneer kit.
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
* \addtogroup group_bsp_cyw9p62s1_43438evb_01 CYW9P62S1-43438EVB-01
* \ingroup group_bsp
* \{
* The CYW9P62S1-43438-EVB-01 Kit is a low-cost hardware platform
* that enables design and debug of the Azurewave AW-CU427 Module. AW-CU427
* is a System in Package (SiP) module that contains the MCU part, PSoC 62 MCU (CY8C6247BZI-D54)
* and the radio part CYW43438 ( WiFi + Bluetooth Combo Chip).
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>BLE v4.2</li>
* <li>Serial memory interface</li>
* <li>Industry-leading CapSense</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>CYW9P62S1-43438 evaluation board</li>
* <li>Quick Start Guide</li>
* <li>USB Type-A to Micro-B cable</li>
* </ul>
*
* \defgroup group_bsp_cyw9p62s1_43438evb_01_macros Macros
* \defgroup group_bsp_cyw9p62s1_43438evb_01_functions Functions
* \defgroup group_bsp_cyw9p62s1_43438evb_01_enums Enumerated Types
*/

#pragma once

#include "cybsp_types.h"
#include "cybsp_core.h"
#ifdef __MBED__
#include "cybsp_wifi.h"
#else 
#include "cybsp_retarget.h"
#endif /* __MBED__ */

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cyw9p62s1_43438evb_01 */

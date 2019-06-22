/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2016 u-blox
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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
 */

#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#if defined(TARGET_CY8CKIT_062_4343W)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x000FD800)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CKIT_062_BLE)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x00045800)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CKIT_062_WIFI_BT)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x00045800)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CMOD_062_4343W)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x000FD800)  /* Ram origin + length */
#endif
#elif defined(TARGET_CYW943012P6EVB_01)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x00045800)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CPROTO_064_SB)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x0001E000)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CPROTO_064_SB_M0_PSA)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08010000 + 0x0001E000)  /* Ram origin + length */
#endif
#elif defined(TARGET_CY8CPROTO_064_SB_PSA)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08000000 + 0x0000FD00)  /* Ram origin + length */
#endif
#elif defined(TARGET_AUGUST_CYW43012)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08002000 + 0x0001E000)  /* Ram origin + length */ 
#endif
#elif defined(TARGET_AUGUST_CYW43012_M0_PSA)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08010000 + 0x0001E000)  /* Ram origin + length */
#endif
#elif defined(TARGET_AUGUST_CYW43012_PSA)
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08000000 + 0x0000FD00)  /* Ram origin + length */
#endif
#endif
#endif  /* MBED_MBED_RTX_H */

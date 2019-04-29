/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BLE_TESTS_H
#define BLE_TESTS_H

#include "utest.h"
#include "greentea-client/test_env.h"
#include "utest.h"
#include "rtos.h"
#include "unity.h"

using namespace utest::v1;

/*
 * Test cases
 */
void BLE_MultiADV_Discovery_Test();
void BLE_MultiADV_SetAdvType_01();
void BLE_MultiADV_SetAdvType_02();
void BLE_MultiADV_SetAdvType_03();
void BLE_MultiADV_SetAdvType_04();
void BLE_SingleADVInstance_Test();
void BLE_MultiADV_SetDiffADVType_01();
void BLE_MultiADV_SetDiffADVType_02();
void BLE_CustomADVInterval();
void BLE_MultiADVChannelMap_01();
void BLE_MultiADVChannelMap_02();
void BLE_DisableMultiADV_Test();

#endif //BLE_TESTS_H

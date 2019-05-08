/***************************************************************************//**
* \file cy_bsp_cy8ckit_062_wifi_bt.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-WIFI-BT pioneer kit.
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

#include "cy_bsp_cy8ckit_062_wifi_bt.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cybsp_init(void)
{
    init_cycfg_all();
    return CY_RSLT_SUCCESS;
}

uint32_t cybsp_led_count(void)
{
    return 0;
}

cy_rslt_t cybsp_led_enable(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_led_set_state(int which, bool on)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_led_disable(int which)
{
    return CY_RSLT_SUCCESS;
}

uint32_t cybsp_switch_count(void)
{
    return 0;
}

cy_rslt_t cybsp_switch_enable(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_switch_get_state(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_switch_set_interrupt(int which, int type, void * callback)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_switch_disable(int which)
{
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

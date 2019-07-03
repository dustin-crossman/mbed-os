/***************************************************************************//**
* \file cybsp_cy8cproto_064_sb.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-064-SB pioneer kit.
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

//#include "cybsp_retarget.h"
#include "cybsp_cy8cproto_064_sb.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cybsp_init(void)
{
    init_cycfg_system();
    cy_rslt_t result = cybsp_led_init(CYBSP_USER_LED1);
    result |= cybsp_led_init(CYBSP_USER_LED2);
    result |= cybsp_btn_init(CYBSP_USER_BTN1);
    
#if defined(CYBSP_RETARGET_ENABLED)
    /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_retarget_init();
    }
#endif
    
    return result;
}

#if defined(__cplusplus)
}
#endif

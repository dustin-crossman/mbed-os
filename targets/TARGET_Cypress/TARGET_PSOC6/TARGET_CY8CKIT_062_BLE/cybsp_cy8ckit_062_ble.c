/***************************************************************************//**
* \file cybsp_cy8ckit_062_ble.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-BLE pioneer kit.
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

#include <stdlib.h>
#include "cybsp_cy8ckit_062_ble.h"
#include "cyhal_implementation.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if 0
cyhal_qspi_t cybsp_qspi;
cyhal_i2c_t cybsp_i2c;
cyhal_rtc_t cybsp_rtc;

static cy_rslt_t init_peripherals(void)
{
    cy_rslt_t result = cyhal_qspi_init(&cybsp_qspi, CYBSP_QSPI_D0, CYBSP_QSPI_D1, CYBSP_QSPI_D2, CYBSP_QSPI_D3,
            NC, NC, NC, NC, CYBSP_QSPI_SCK, CYBSP_QSPI_SS, 50000000U, 0);

    result = cyhal_i2c_init(&cybsp_i2c, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    return cyhal_rtc_init(&cybsp_rtc);
}

static cy_rslt_t reserve_pin(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t pin_inst = cyhal_utils_get_gpio_resource(pin);
    return cyhal_hwmgr_reserve(&pin_inst);
}

static cy_rslt_t reserve_pins(void)
{
    // Pins not connected to peripherals must be reserve manually.
    cy_rslt_t result = reserve_pin(CYBSP_WCO_IN);
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    result = reserve_pin(CYBSP_WCO_OUT);
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    return reserve_pin(CYBSP_SWDIO);
}

static cy_rslt_t reserve_peri_diviers(void)
{
    // Clock dividers that are preconfigured must be manually reserved.
    // Divider 0 is used for This kit need 4 dividers allocated.
    cyhal_resource_inst_t divider;
    divider.type = CYHAL_RSC_CLOCK;
    // I2C
    divider.block_num = CYBSP_CSD_COMM_CLK_DIV_HW;
    divider.channel_num = CYBSP_CSD_COMM_CLK_DIV_NUM;
    cy_rslt_t result = cyhal_hwmgr_reserve(&divider);
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    // UART, BT_UART
    divider.block_num = CYBSP_DEBUG_UART_CLK_DIV_HW;
    divider.channel_num = CYBSP_DEBUG_UART_CLK_DIV_NUM;
    result = cyhal_hwmgr_reserve(&divider);
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    // CSD
    divider.block_num = CYBSP_CSD_CLK_DIV_NUM;
    divider.channel_num = CYBSP_CSD_CLK_DIV_HW;
    return cyhal_hwmgr_reserve(&divider);
}
#endif

cy_rslt_t cybsp_init(void)
{
	init_cycfg_system();

    cy_rslt_t result = CY_RSLT_SUCCESS;
    /* Initialize User LEDs */
    result |= cybsp_led_init(CYBSP_LED_RGB_RED);
    result |= cybsp_led_init(CYBSP_LED_RGB_BLUE);
    result |= cybsp_led_init(CYBSP_LED_RGB_GREEN);
    result |= cybsp_led_init(CYBSP_USER_LED0);
    result |= cybsp_led_init(CYBSP_USER_LED1);
    /* Initialize User Buttons */
    result |= cybsp_btn_init(CYBSP_USER_BTN0);

    CY_ASSERT(CY_RSLT_SUCCESS == result);

#if defined(CYBSP_RETARGET_ENABLED)
    /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_retarget_init();
    }
#endif

#if 0
    init_cycfg_all();
    result = reserve_pins();
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    result = reserve_peri_diviers();
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }
    result = init_peripherals();
#endif
    return result;
}

#if defined(__cplusplus)
}
#endif

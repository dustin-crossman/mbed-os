/***************************************************************************//**
* \file cy_bsp_cy8cproto_062_4343w.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-062-4343W prototyping kit.
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

#include "cy_bsp_cy8cproto_062_4343w.h"
#include "cycfg.h"
#include "cy_result.h"
#include "cyhal.h"

#ifdef MBED
#include "whd_wifi_api.h"
#endif /* MBED */

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
*       Internal
*******************************************************************************/
#ifdef MBED

#define THREAD_STACK_SIZE   	(1024 + 1400)
#define THREAD_PRIORITY   	0
//TODO: Need to use priority from whd_rtos.h when available
//#define THREAD_PRIORITY   	RTOS_HIGHEST_PRIORITY
#define COUNTRY              WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		0
#define WLAN_INTR_PRIORITY	1

extern whd_buffer_funcs_t buffer_ops;
extern whd_netif_funcs_t netif_ops;
extern whd_resource_source_t resource_ops;

whd_driver_t whd_drv;
static uint8_t thread_stack[THREAD_STACK_SIZE];

whd_driver_t* get_whd_driver(uint8_t idx)
{
    return &whd_drv;
}

void wlan_irq_handler(void *arg, cyhal_gpio_irq_event_t event)
{
    //TODO: Switch MCU to LP mode here.
}


cy_rslt_t init_wlan_wakeup(void)
{
    //TODO: confirm correct configuration. Currently configured as per wwd_SDIO_2m.c line 93 ModusToolbox 1.0
    //TODO: confirm PIN. not clear from module schematic or MT1.0 whether this is P2_7.
    cy_rslt_t result = cyhal_gpio_init(CY_WIFI_HOST_WAKE, CYHAL_GPIO_DIR_INPUT, CY_GPIO_DRIVE_ANALOG, 0);
    if(result == CY_RSLT_SUCCESS)
    {
        cyhal_gpio_register_irq(CY_WIFI_HOST_WAKE, WLAN_INTR_PRIORITY, wlan_irq_handler, NULL);
        cyhal_gpio_irq_enable(CY_WIFI_HOST_WAKE, CYHAL_GPIO_IRQ_RISE, true);
    }
    return result;
}

cy_rslt_t sdio_bus_initialize(whd_driver_t *whd_driver)
{
    whd_sdio_config_t whd_sdio_config;
    cyhal_sdio_t sdio_obj;

    cy_rslt_t result = cyhal_sdio_init(&sdio_obj, CY_WIFI_SDIO_CMD, CY_WIFI_SDIO_CLK, CY_WIFI_SDIO_DATA_0, CY_WIFI_SDIO_DATA_1, CY_WIFI_SDIO_DATA_2, CY_WIFI_SDIO_DATA_3);
    if(result == CY_RSLT_SUCCESS)
    {
        whd_sdio_config.sdio_1bit_mode = WHD_FALSE;
        whd_sdio_config.high_speed_sdio_clock = WHD_FALSE;
        //TODO:Check if this needs to return a error
        whd_bus_sdio_attach(*whd_driver, &whd_sdio_config, &sdio_obj);
    }

    return result;
}

cy_rslt_t init_cycfg_wlan_hw(void)
{
    whd_init_config_t whd_init_config;
    whd_init_config.thread_stack_size = ( uint32_t ) THREAD_STACK_SIZE;
    whd_init_config.thread_stack_start = &thread_stack;
    whd_init_config.thread_priority = (uint32_t) THREAD_PRIORITY;
    whd_init_config.oob_gpio_pin = DEFAULT_OOB_PIN;
    whd_init_config.country = COUNTRY;

    uint32_t ret = whd_init(&whd_drv, &whd_init_config, &resource_ops, &buffer_ops, &netif_ops);
    if(ret != WHD_SUCCESS)
    {
        return CY_RSLT_BSP_ERR_WIFI_INIT;
    }

    cy_rslt_t result = sdio_bus_initialize(&whd_drv);
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    result = init_wlan_wakeup();

    return result;
}
#endif /* MBED */


/*******************************************************************************
*       Functions
*******************************************************************************/

cy_rslt_t cybsp_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    init_cycfg_all();
#ifdef MBED
    result = init_cycfg_wlan_hw();
#endif /* MBED */
    return result;
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

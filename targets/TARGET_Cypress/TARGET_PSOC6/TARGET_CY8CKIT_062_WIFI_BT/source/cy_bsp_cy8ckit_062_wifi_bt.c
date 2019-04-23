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
#include "cy_hal.h"
#include "whd_wifi_api.h"
#include "cy_network_buffer.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define THREAD_STACK_SIZE   	(1024 + 1400)
#define THREAD_PRIORITY   	    0
//TODO: Need to use priority from whd_rtos.h when available
//#define THREAD_PRIORITY   	RTOS_HIGHEST_PRIORITY
#define COUNTRY                 WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		    0
#define WLAN_INTR_PRIORITY	    1
#define WLAN_POWER_UP_DELAY_MS  185 

static whd_buffer_funcs_t buffer_ops =
{
    .whd_host_buffer_get = cy_host_buffer_get,
    .whd_buffer_release = cy_buffer_release,
    .whd_buffer_get_current_piece_data_pointer = cy_buffer_get_current_piece_data_pointer,
    .whd_buffer_get_current_piece_size = cy_buffer_get_current_piece_size,
    .whd_buffer_set_size = cy_buffer_set_size,
    .whd_buffer_add_remove_at_front = cy_buffer_add_remove_at_front,
};

extern whd_netif_funcs_t netif_ops;
extern whd_resource_source_t resource_ops;

whd_driver_t whd_drv;
whd_interface_t ifp;

static uint8_t thread_stack[THREAD_STACK_SIZE];

whd_driver_t* get_whd_driver(void)
{
    return &whd_drv;
}

whd_interface_t* get_whd_interface(void)
{
    return &ifp;
}


void wlan_irq_handler(void *arg, cy_gpio_irq_event_t event)
{
    //TODO: Switch MCU to LP mode here.
}

static cy_rslt_t init_wlan_wakeup(void)
{
    cy_rslt_t result = cy_gpio_init(CY_WIFI_HOST_WAKE, CY_GPIO_DIR_INPUT, CY_GPIO_DRIVE_ANALOG, 0);
    if(result == CY_RSLT_SUCCESS)
    {
        cy_gpio_register_irq(CY_WIFI_HOST_WAKE, WLAN_INTR_PRIORITY, wlan_irq_handler, NULL);
        cy_gpio_irq_enable(CY_WIFI_HOST_WAKE, CY_GPIO_IRQ_RISE, true);
    }
    return result;
}

static cy_rslt_t sdio_bus_initialize(whd_driver_t *whd_driver)
{
    whd_sdio_config_t whd_sdio_config;
    cy_sdio_t sdio_obj;

    cy_rslt_t result = cy_sdio_init(&sdio_obj, CY_WIFI_SDIO_CMD, CY_WIFI_SDIO_CLK, CY_WIFI_SDIO_DATA_0, CY_WIFI_SDIO_DATA_1, CY_WIFI_SDIO_DATA_2, CY_WIFI_SDIO_DATA_3);
    if(result == CY_RSLT_SUCCESS)
    {
        whd_sdio_config.sdio_1bit_mode = WHD_FALSE;
        whd_sdio_config.high_speed_sdio_clock = WHD_FALSE;
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

    cy_rslt_t result = cy_gpio_init(CY_WIFI_WL_REG_ON, CY_GPIO_DIR_OUTPUT, CY_GPIO_DM_PULLUP, 1);
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    Cy_SysLib_Delay(WLAN_POWER_UP_DELAY_MS);

    uint32_t ret = whd_init(&whd_drv, &whd_init_config, &resource_ops, &buffer_ops, &netif_ops);
    if(ret != WHD_SUCCESS)
    {
        return CY_RSLT_BSP_ERR_WIFI_INIT;
    }

    result = sdio_bus_initialize(&whd_drv);
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    result = init_wlan_wakeup();
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    ret = whd_wifi_on(whd_drv, &ifp /* OUT */);
    if(ret != WHD_SUCCESS)
    {
        return CY_RSLT_BSP_ERR_WIFI_INIT;
    }
//TODO: Need to deinitialize wifi if error.
    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_board_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    init_cycfg_all();
    result = init_cycfg_wlan_hw();
    return result;
}

uint32_t cy_board_led_count(void)
{
    return 0;
}

cy_rslt_t cy_board_led_enable(uint32_t which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_board_led_set_state(uint32_t which, bool on)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_board_led_disable(uint32_t which)
{
    return CY_RSLT_SUCCESS;
}

uint32_t cy_board_switch_count(void)
{
    return 0;
}

cy_rslt_t cy_board_switch_enable(uint32_t which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_board_switch_get_state(uint32_t which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_board_switch_set_interrupt(uint32_t which, uint32_t type, void * callback)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_board_switch_disable(uint32_t which)
{
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif
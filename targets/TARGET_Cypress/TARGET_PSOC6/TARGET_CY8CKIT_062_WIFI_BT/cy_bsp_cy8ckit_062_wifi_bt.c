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

#include "cy_network_buffer.h"
#include "SDIO_HOST.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define THREAD_STACK_SIZE   	(5*1024)
#define THREAD_PRIORITY   	    osPriorityRealtime7
//TODO: Need to use priority from whd_rtos.h when available
//#define THREAD_PRIORITY   	RTOS_HIGHEST_PRIORITY
#define COUNTRY                 WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		    0
#define WLAN_INTR_PRIORITY	    1
#define WLAN_POWER_UP_DELAY_MS  250

#define SDIO_ENUMERATION_TRIES  500
#define SDIO_RETRY_DELAY_MS     1
#define SDIO_BUS_LEVEL_MAX_RETRIES 5

extern void host_network_process_ethernet_data(whd_buffer_t buffer, whd_interface_t role);

static whd_buffer_funcs_t buffer_ops =
{
    .whd_host_buffer_get = cy_host_buffer_get,
    .whd_buffer_release = cy_buffer_release,
    .whd_buffer_get_current_piece_data_pointer = cy_buffer_get_current_piece_data_pointer,
    .whd_buffer_get_current_piece_size = cy_buffer_get_current_piece_size,
    .whd_buffer_set_size = cy_buffer_set_size,
    .whd_buffer_add_remove_at_front = cy_buffer_add_remove_at_front,
};

whd_netif_funcs_t netif_ops =
{
    .whd_network_process_ethernet_data = host_network_process_ethernet_data,
};

extern whd_resource_source_t resource_ops;

whd_driver_t whd_drv;

whd_driver_t* get_whd_driver(void)
{
    return &whd_drv;
}

void wlan_irq_handler(void *arg, cy_gpio_irq_event_t event)
{
    //TODO: Switch MCU to LP mode here.
} 

static cy_rslt_t init_sdio_wlan(cy_sdio_t *sdio_obj)
{
    /* WiFi into reset */
    cy_rslt_t result = cy_sdio_init(sdio_obj, CY_WIFI_SDIO_CMD, CY_WIFI_SDIO_CLK, CY_WIFI_SDIO_DATA_0, CY_WIFI_SDIO_DATA_1, CY_WIFI_SDIO_DATA_2, CY_WIFI_SDIO_DATA_3);
    if(result == CY_RSLT_SUCCESS)
    {
        Cy_SysLib_Delay(10);
        /* Init SDIO Host */
        result = cy_gpio_init(CY_WIFI_WL_REG_ON, CY_GPIO_DIR_OUTPUT, CY_GPIO_DM_PULLUP, 1);
        if(result == CY_RSLT_SUCCESS)
        {
                //low
            cy_gpio_write(CY_WIFI_WL_REG_ON, false);
            Cy_SysLib_Delay(10);
            /* WiFi out of reset */
            cy_gpio_write(CY_WIFI_WL_REG_ON, true);
            Cy_SysLib_Delay(WLAN_POWER_UP_DELAY_MS);

            //SDIO_Reset();
        }  
    }
    return result;
}
static cy_rslt_t sdio_try_cmd(const cy_sdio_t *obj, cy_transfer_t direction, \
                          cy_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    uint8_t loop_count = 0;
    cy_rslt_t result = CY_RSLT_BSP_ERR_WIFI_SDIO_ENUM_TIMEOUT;
    do
    {
        result = cy_sdio_send_cmd(obj, direction, command, argument, response);
        loop_count++;
        if(result != CY_RSLT_SUCCESS)
        {
            Cy_SysLib_Delay(SDIO_RETRY_DELAY_MS);
        }
    }
    while(result != CY_RSLT_SUCCESS && loop_count <= SDIO_BUS_LEVEL_MAX_RETRIES);

    return result;
}

cy_rslt_t sdio_enumerate(const cy_sdio_t *sdio_obj)
{
    cy_rslt_t result = CY_RSLT_BSP_ERR_WIFI_SDIO_ENUM_TIMEOUT;
    uint32_t loop_count = 0;
    uint32_t rel_addr;
    uint32_t response_ignored = 0;
    uint32_t no_argument = 0;

    do
    {
        //TODO: The wiced code ignores result for the following 2 calls.
        //TODO: Need to check if bus level retries are needed for each call.

        /* Send CMD0 to set it to idle state */
        sdio_try_cmd(sdio_obj, CY_WRITE, SDIO_CMD_GO_IDLE_STATE, no_argument, &response_ignored /*ignored*/);

        /* CMD5. */
        sdio_try_cmd(sdio_obj, CY_READ, SDIO_CMD_IO_SEND_OP_COND, no_argument, &response_ignored /*ignored*/);

        /* Send CMD3 to get RCA. */
        result = sdio_try_cmd(sdio_obj, CY_READ, SDIO_CMD_SEND_RELATIVE_ADDR, no_argument, &rel_addr);
        if(result != CY_RSLT_SUCCESS)
        {
            Cy_SysLib_Delay(SDIO_RETRY_DELAY_MS);
        }
        loop_count++;
    } while (result != CY_RSLT_SUCCESS && loop_count <= 30);
    if(result == CY_RSLT_SUCCESS)
    {
        /* Send CMD7 with the returned RCA to select the card */
        result = sdio_try_cmd(sdio_obj, CY_WRITE, SDIO_CMD_SELECT_CARD, rel_addr, &response_ignored);
    }
    return result;
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

static cy_rslt_t sdio_bus_initialize(whd_driver_t *whd_driver, const cy_sdio_t *sdio_obj)
{
    whd_sdio_config_t whd_sdio_config;
    cy_rslt_t result = sdio_enumerate(sdio_obj);
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
    cy_sdio_t sdio_obj;
    whd_init_config_t whd_init_config;
    whd_init_config.thread_stack_size = ( uint32_t ) THREAD_STACK_SIZE;
    whd_init_config.thread_stack_start = (char*)malloc(THREAD_STACK_SIZE) ;
    whd_init_config.thread_priority = (uint32_t) THREAD_PRIORITY;
    whd_init_config.oob_gpio_pin = DEFAULT_OOB_PIN;
    whd_init_config.country = COUNTRY;

    cy_rslt_t result = init_sdio_wlan(&sdio_obj);
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    uint32_t ret = whd_init(&whd_drv, &whd_init_config, &resource_ops, &buffer_ops, &netif_ops);
    if(ret != WHD_SUCCESS)
    {
        return CY_RSLT_BSP_ERR_WIFI_INIT_FAILED;
    }

    result = sdio_bus_initialize(&whd_drv, &sdio_obj);
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    result = init_wlan_wakeup();
    if(result != CY_RSLT_SUCCESS)
    {
        return result;
    }
//TODO: Need to deinitialize wifi if error.
    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_board_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    //init_cycfg_all();
    result = init_cycfg_wlan_hw();
    printf("Wifi init result: %d \n", result);
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

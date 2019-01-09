/*******************************************************************************
* File Name: cy_bsp_cy8ckit_062_wifi_bt.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-WIFI-BT kit.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_bsp_cy8ckit_062_wifi_bt.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cybrd_init(void)
{
    init_cycfg_all();
    return CY_RSLT_SUCCESS;
}

uint32_t cybrd_led_count(void)
{
    return 0;
}

cy_rslt_t cybrd_led_enable(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybrd_led_set_state(int which, bool on)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybrd_led_disable(int which)
{
    return CY_RSLT_SUCCESS;
}

uint32_t cybrd_switch_count(void)
{
    return 0;
}

cy_rslt_t cybrd_switch_enable(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybrd_switch_get_state(int which)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybrd_switch_set_interrupt(int which, int type, void * callback)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybrd_switch_disable(int which)
{
    return CY_RSLT_SUCCESS;
}


#if defined(__cplusplus)
}
#endif

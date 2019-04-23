/***************************************************************************//**
* \file cy_hal_pwm.c
*
* \brief
* Provides a high level interface for interacting with the Cypress PWM. This is
* a wrapper around the lower level PDL API.
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

#include <string.h>
#include "cy_hal_pwm.h"
#include "cy_hal_gpio.h"
#include "cy_hal_interconnect.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    TCPWM_Type *base;
    en_clk_dst_t clock_dst;
    uint32_t max_period;
} cy_internal_pwm_data_t;

static const cy_internal_pwm_data_t cy_internal_pwm_data[] = {
#ifdef TCPWM0
    {TCPWM0, PCLK_TCPWM0_CLOCKS0, (uint32_t)((1ULL << TCPWM0_CNT_CNT_WIDTH) - 1ULL)},
#endif
#ifdef TCPWM1
    {TCPWM1, PCLK_TCPWM1_CLOCKS0, (uint32_t)((1ULL << TCPWM1_CNT_CNT_WIDTH) - 1ULL)},
#endif
};
#if CY_IP_MXTCPWM_INSTANCES > 2
#error PWM driver must be updated to support more TCPWM instances
#endif

#define CY_NC_PIN_VALUE_GPIO_VALUE ((cy_gpio_t)CY_NC_PIN_VALUE)

cy_rslt_t cy_pwm_init(cy_pwm_t *obj, cy_gpio_t pin, const cy_clock_divider_t *clk)
{
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    memset(obj, 0, sizeof(cy_pwm_t));
    obj->resource.block_num = CY_RSC_INVALID;
    obj->pin = CY_NC_PIN_VALUE_GPIO_VALUE;
    obj->clock.div_num = CY_RSC_INVALID;
    cy_rslt_t result;
    const cy_resource_pin_mapping_t *map = CY_UTILS_GET_RESOURCE(pin, cy_pin_map_tcpwm_line);
    if (NULL == map)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    
    cy_resource_inst_t pwm_inst = *map->inst;
    obj->resource = pwm_inst;
    obj->base = cy_internal_pwm_data[pwm_inst.block_num].base;
    en_clk_dst_t pclk = (en_clk_dst_t)(cy_internal_pwm_data[pwm_inst.block_num].clock_dst + pwm_inst.channel_num);
    if (CY_RSLT_SUCCESS == (result = cy_gpio_init(pin, CY_GPIO_DIR_OUTPUT, CY_GPIO_DRIVE_STRONG, true)))
    {
        obj->pin = pin;
        if (CY_RSLT_SUCCESS == (result = cy_connect_pin(map)))
        {
            if (NULL != clk)
            {
                obj->clock_hz = cy_PeriClkFreqHz / (1 + Cy_SysClk_PeriphGetDivider(obj->clock.div_type, obj->clock.div_num));
                if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, clk->div_type, clk->div_num))
                    result = CYCSP_PWM_RSLT_FAILED_CLOCK;
            }
            else if (CY_RSLT_SUCCESS == (result = cy_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, false)))
            {
                uint32_t div = cy_PeriClkFreqHz / 1000000u;
                if (0 == div ||
                    CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, div - 1) ||
                    CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num) ||
                    CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, obj->clock.div_type, obj->clock.div_num))
                    result = CYCSP_PWM_RSLT_FAILED_CLOCK;
                else
                    obj->clock_hz = cy_PeriClkFreqHz / div;
            }
            bool configured;
            if (CY_RSLT_SUCCESS == result &&
                CY_RSLT_SUCCESS == (result = cy_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num, &configured)) &&
                !configured)
            {
                static const cy_stc_tcpwm_pwm_config_t config =
                {
                /* .pwmMode           = */ CY_TCPWM_PWM_MODE_PWM,
                /* .clockPrescaler    = */ CY_TCPWM_PWM_PRESCALER_DIVBY_1,
                /* .pwmAlignment      = */ CY_TCPWM_PWM_LEFT_ALIGN,
                /* .deadTimeClocks    = */ 0UL,
                /* .runMode           = */ CY_TCPWM_PWM_CONTINUOUS,
                /* .period0           = */ 0UL,
                /* .period1           = */ 0UL,
                /* .enablePeriodSwap  = */ false,
                /* .compare0          = */ 0UL,
                /* .compare1          = */ 0UL,
                /* .enableCompareSwap = */ false,
                /* .interruptSources  = */ CY_TCPWM_INT_NONE,
                /* .invertPWMOut      = */ CY_TCPWM_PWM_INVERT_DISABLE,
                /* .invertPWMOutN     = */ CY_TCPWM_PWM_INVERT_ENABLE,
                /* .killMode          = */ CY_TCPWM_PWM_STOP_ON_KILL,
                /* .swapInputMode     = */ CY_TCPWM_INPUT_RISINGEDGE,
                /* .swapInput         = */ CY_TCPWM_INPUT_0,
                /* .reloadInputMode   = */ CY_TCPWM_INPUT_RISINGEDGE,
                /* .reloadInput       = */ CY_TCPWM_INPUT_0,
                /* .startInputMode    = */ CY_TCPWM_INPUT_RISINGEDGE,
                /* .startInput        = */ CY_TCPWM_INPUT_0,
                /* .killInputMode     = */ CY_TCPWM_INPUT_RISINGEDGE,
                /* .killInput         = */ CY_TCPWM_INPUT_0,
                /* .countInputMode    = */ CY_TCPWM_INPUT_LEVEL,
                /* .countInput        = */ CY_TCPWM_INPUT_1
                };
                if (CY_TCPWM_SUCCESS == Cy_TCPWM_PWM_Init(obj->base, obj->resource.channel_num, &config))
                    result = cy_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
                else
                    result = CYCSP_PWM_RSLT_FAILED_INIT;
            }
        }
    }
    if (result == CY_RSLT_SUCCESS)
        Cy_TCPWM_PWM_Enable(obj->base, obj->resource.channel_num);
    else
        cy_pwm_free(obj);
    return result;
}

cy_rslt_t cy_pwm_free(cy_pwm_t *obj)
{
    cy_rslt_t result, ret = CY_RSLT_SUCCESS;
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    if (obj->pin != CY_NC_PIN_VALUE)
    {
        result = cy_gpio_free(obj->pin);
        if (ret == CY_RSLT_SUCCESS)
            ret = result;
        obj->pin = CY_NC_PIN_VALUE_GPIO_VALUE;
    }
    if (NULL != obj->base)
    {
        Cy_TCPWM_PWM_Disable(obj->base, obj->resource.channel_num);
        result = cy_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        if (ret == CY_RSLT_SUCCESS)
            ret = result;
        result = cy_hwmgr_free(&(obj->resource));
        if (ret == CY_RSLT_SUCCESS)
            ret = result;
        obj->base = NULL;
        obj->resource.block_num = CY_RSC_INVALID;
    }
    if (CY_RSC_INVALID != obj->clock.div_num)
    {
        if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num) &&
            ret == CY_RSLT_SUCCESS)
            ret = CYCSP_PWM_RSLT_FAILED_CLOCK;
        result = cy_hwmgr_free_clock(&(obj->clock));
        if (ret == CY_RSLT_SUCCESS)
            ret = result;
        obj->clock.div_num = CY_RSC_INVALID;
    }
    return ret;
}

static cy_rslt_t cy_pwm_set_period_and_compare(cy_pwm_t *obj, uint32_t period, uint32_t compare)
{
    if (period < 1 || period > cy_internal_pwm_data[obj->resource.block_num].max_period)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    if (compare > period)
        compare = period;
    Cy_TCPWM_PWM_SetCompare0(obj->base, obj->resource.channel_num, 0u);
    Cy_TCPWM_PWM_SetPeriod0(obj->base, obj->resource.channel_num, period - 1u);
    Cy_TCPWM_PWM_SetCompare0(obj->base, obj->resource.channel_num, compare);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_pwm_period(cy_pwm_t *obj, uint32_t period_us, uint32_t pulse_width_us)
{
    static const uint32_t US_PER_SEC = 1000000u;
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    uint32_t period = (uint32_t)((uint64_t)period_us * obj->clock_hz / US_PER_SEC);
    uint32_t width = (uint32_t)((uint64_t)pulse_width_us * obj->clock_hz / US_PER_SEC);
    return cy_pwm_set_period_and_compare(obj, period, width);
}

cy_rslt_t cy_pwm_duty_cycle(cy_pwm_t *obj, float duty_cycle, uint32_t frequency_hz)
{
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    if (duty_cycle < 0.0f || duty_cycle > 100.0f || frequency_hz < 1)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    uint32_t period = obj->clock_hz / frequency_hz;
    uint32_t width = (uint32_t)(duty_cycle * 0.01f * period);
    return cy_pwm_set_period_and_compare(obj, period, width);
}

cy_rslt_t cy_pwm_start(cy_pwm_t *obj)
{
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    Cy_TCPWM_TriggerReloadOrIndex(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_pwm_stop(cy_pwm_t *obj)
{
    if (NULL == obj)
        return CYCSP_PWM_RSLT_BAD_ARGUMENT;
    Cy_TCPWM_TriggerStopOrKill(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

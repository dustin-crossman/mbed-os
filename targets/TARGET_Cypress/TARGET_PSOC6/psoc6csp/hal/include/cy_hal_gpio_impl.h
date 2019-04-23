/***************************************************************************//**
* \file cy_hal_gpio_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
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

#pragma once

#include "cy_hal_gpio.h"
#include "cy_hal_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Defines
*******************************************************************************/
#define CY_GET_PORTADDR(pin)    (Cy_GPIO_PortToAddr(CY_GET_PORT(pin)))  /**< Macro to get the port address from pin */

/*******************************************************************************
*       Functions
*******************************************************************************/

__STATIC_INLINE cy_rslt_t cy_gpio_write(cy_gpio_t pin, bool value)
{
    Cy_GPIO_Write(CY_GET_PORTADDR(pin), CY_GET_PIN(pin), value);

    return CY_RSLT_SUCCESS;
}

__STATIC_INLINE cy_rslt_t cy_gpio_read(cy_gpio_t pin, bool *value)
{
    *value = Cy_GPIO_Read(CY_GET_PORTADDR(pin), CY_GET_PIN(pin));

    return CY_RSLT_SUCCESS;
}

__STATIC_INLINE cy_rslt_t cy_gpio_toggle(cy_gpio_t pin)
{
    Cy_GPIO_Inv(CY_GET_PORTADDR(pin), CY_GET_PIN(pin));

    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

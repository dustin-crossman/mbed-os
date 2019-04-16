/***************************************************************************//**
* \file cy_hal_gpio_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

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

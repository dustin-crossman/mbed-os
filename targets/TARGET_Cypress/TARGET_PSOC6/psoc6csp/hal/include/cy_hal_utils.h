/***************************************************************************//**
* \file cy_hal_utils.h
*
* \brief
* Provides utility functions for working with the PSoC 6 HAL implementation.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_hal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*       Defines
*******************************************************************************/
#define CY_GET_PIN(pin)         ((uint32_t)(pin & 0xFFFFUL))  /**< Macro to extract the pin number */
#define CY_GET_PORT(pin)        ((uint32_t)((uint32_t)(pin >> 16) & 0xFFUL)) /**< Macro to extract the port number */

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 * This is a convinience utility for cy_utils_get_resource() if the mappings is an array of known size.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
#define CY_UTILS_GET_RESOURCE(pin, mappings) cy_utils_get_resource(pin, mappings, sizeof(mappings)/sizeof(cy_resource_pin_mapping_t))

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Converts the provided gpio pin to a resource instance object
 *
 * @param[in] pin  The pin to get a resource object for
 * @return The equivilant resource instance object for the provided pin.
 */
static inline cy_resource_inst_t cy_utils_get_gpio_resource(cy_gpio_t pin)
{
    cy_resource_inst_t rsc = { CY_RSC_GPIO, CY_GET_PORT(pin), CY_GET_PIN(pin) };
    return rsc;
}

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @param[in] count      The number of items in the mappings table
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
const cy_resource_pin_mapping_t *cy_utils_get_resource(cy_gpio_t pin, const cy_resource_pin_mapping_t* mappings, size_t count);

#if defined(__cplusplus)
}
#endif

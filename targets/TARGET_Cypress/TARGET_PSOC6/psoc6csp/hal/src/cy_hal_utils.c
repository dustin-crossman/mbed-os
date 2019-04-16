/***************************************************************************//**
* \file cy_hal_utils.c
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

#include "cy_hal_utils.h"

const cy_resource_pin_mapping_t *cy_utils_get_resource(cy_gpio_t pin, const cy_resource_pin_mapping_t* mappings, size_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (pin == mappings->pin)
        {
            return &mappings[i];
        }
    }
    return NULL;
}

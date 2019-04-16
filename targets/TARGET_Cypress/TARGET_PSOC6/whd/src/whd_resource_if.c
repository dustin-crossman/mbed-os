/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *  Provides generic APSTA functionality that chip specific files use
 */

#include "whd_debug.h"
#include "whd_int.h"
#include "whd_resource_if.h"

/******************************************************
 *  * @cond               Constants
 *   ******************************************************/

/******************************************************
 *  *                   Enumerations
 *   ******************************************************/

/******************************************************
 *  *               Function Declarations
 *   ******************************************************/

/******************************************************
 *        Variables Definitions
 *****************************************************/

/******************************************************
*               Function Definitions
******************************************************/
uint32_t whd_resource_size(whd_driver_t whd_driver, whd_resource_type_t resource, uint32_t *size_out)
{
    if (whd_driver->resource_if->whd_resource_size)
    {
        return whd_driver->resource_if->whd_resource_size(whd_driver, resource, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_ERROR;
}

uint32_t whd_get_resource_block_size(whd_driver_t whd_driver, whd_resource_type_t type, uint32_t *size_out)
{

    if (whd_driver->resource_if->whd_get_resource_block_size)
    {
        return whd_driver->resource_if->whd_get_resource_block_size(whd_driver, type, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_ERROR;
}

uint32_t whd_get_resource_no_of_blocks(whd_driver_t whd_driver, whd_resource_type_t type, uint32_t *block_count)
{
    if (whd_driver->resource_if->whd_get_resource_no_of_blocks)
    {
        return whd_driver->resource_if->whd_get_resource_no_of_blocks(whd_driver, type, block_count);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_ERROR;
}

uint32_t whd_get_resource_block(whd_driver_t whd_driver, whd_resource_type_t type, const uint8_t **data,
                                uint32_t *size_out)
{

    if (whd_driver->resource_if->whd_get_resource_block)
    {
        return whd_driver->resource_if->whd_get_resource_block(whd_driver, type, data, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_ERROR;
}


/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *  Provides prototypes / declarations for common APSTA functionality
 */
#ifndef _WHD_INTERNAL_RESOURCE_API_H_
#define _WHD_INTERNAL_RESOURCE_API_H_

#include "whd.h"
#include "whd_int.h"
#include "whd_resource_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Function prototypes
******************************************************/
uint32_t whd_resource_size(whd_driver_t whd_driver, whd_resource_type_t resource, uint32_t *size_out);
uint32_t whd_get_resource_block_size(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out);
uint32_t whd_get_resource_no_of_blocks(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count);
uint32_t whd_get_resource_block(whd_driver_t whd_driver, whd_resource_type_t type, const uint8_t **data,
                                uint32_t *size_out);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _WHD_INTERNAL_RESOURCE_API_H_ */


/*******************************************************************************
* File Name: cy_abstraction_resource.h
*
* Description:
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
*
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t cy_handle_t;

/**
 * \brief open a resource with the given name.  The name can consists of any characters
 * \param name the name of the resource
 * \param handle the return value for the handle
 * \returns CY_RSLT_SUCCESS if the resources if found, otherwise an error
 */
cy_rslt_t cy_resource_open(const char *name, cy_handle_t *handle);

/**
 * \brief return the block size for the resource
 * \param handle handle to a resource
 * \param size pointer to a uint32_t to receive the size
 * \returns CY_RSLT_SUCCESS if the resources if found, otherwise an error
 */
cy_rslt_t cy_resource_get_block_size(cy_handle_t handle, uint32_t *size);

/**
 * \brief return the total size for the resource
 * \param handle handle to a resource
 * \param size pointer to a uint32_t to receive the size
 * \returns CY_RSLT_SUCCESS if the resources if found, otherwise an error
 */
cy_rslt_t cy_resource_get_total_size(cy_handle_t handle, uint32_t *size);

/**
 * \brief read data from a resource
 * \param handle the handle to the open resource
 * \param buffer pointer to receive buffer address from resource
 * \param blockno the block number of the data from the resource
 * \param size location to receive the size of the block read
 * \returns CY_RSLT_SUCCESS if data read, otherwise an error
 */
cy_rslt_t cy_resource_read(cy_handle_t handle, uint32_t blockno, uint8_t **buffer, size_t *size);

/**
 * \brief close an open resource
 * \param handle handle to an open resource
 * \return CY_RSLT_SUCCESS if data read, otherwise an error
 */
cy_rslt_t cy_resource_close(cy_handle_t handle);

#if defined(__cplusplus)
}
#endif

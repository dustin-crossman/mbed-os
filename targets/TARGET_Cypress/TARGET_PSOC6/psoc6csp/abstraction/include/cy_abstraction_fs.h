/*******************************************************************************
* File Name: cy_abstraction_fs.h
*
* Description:
* Basic file system abstraction layer. This API provides convenience methods 
* for reading and writing values.
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

#define CY_O_RDONLY             (1 << 0)
#define CY_O_WRONLY             (1 << 1)
#define CY_O_RDWR               (1 << 2)
#define CY_O_APPEND             (1 << 3)
#define CY_O_CREAT              (1 << 4)

typedef uint32_t cy_handle_t;

/**
  * \brief open or create a file and return a handle
  * \param path the path name of the file to open
  * \param oflag the mode to use when opening the file
  * \param handle pointer to location re receive handle
  * \returns CY_RSLT_SUCCESS if successful, otherwise error code
  */
cy_rslt_t cy_fs_open(const char *path, uint32_t oflag, cy_handle_t *handle) ;

/**
 * \brief close an open file
 * \param handle a file handle to an open file
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
 cy_rslt_t cy_fs_close(cy_handle_t handle) ;

/**
 * \brief read data from a file
 * \param handle a file handle open for reading or read/write
 * \param buf the buffer for the read data
 * \param nbyte the size of the buffer in bytes, the number of bytes read on return
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
cy_rslt_t cy_fs_read(cy_handle_t handle, void *buf, size_t *nbyte) ;

/**
 * \brief write data to a file
 * \param handle a file handle open for writing or read/write
 * \param buf the buffer for the data to write
 * \param nbyte the size of the buffer in bytes, the number of bytes written on return
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
cy_rslt_t cy_fs_write(cy_handle_t handle, const void *buf, size_t *nbyte) ;

#if defined(__cplusplus)
}
#endif

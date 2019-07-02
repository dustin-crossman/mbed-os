/*
 * Copyright (c) 2019, Cypress Semiconductor Corporation, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 * Defines WHD resource functions for BCM943340WCD1 platform
 */
#include "resources.h"
#include "wifi_nvram_image.h"
#include "whd_resource_api.h"
#include "whd_debug.h"
#include "whd.h"

/******************************************************
*                      Macros
******************************************************/
#define BLOCK_BUFFER_SIZE                    (1024)

/******************************************************
*                    Constants
******************************************************/

#if defined(WHD_DYNAMIC_NVRAM)
#define NVRAM_SIZE             dynamic_nvram_size
#define NVRAM_IMAGE_VARIABLE   dynamic_nvram_image
#else
#define NVRAM_SIZE             sizeof(wifi_nvram_image)
#define NVRAM_IMAGE_VARIABLE   wifi_nvram_image
#endif

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*                    Structures
******************************************************/

/******************************************************
*               Static Function Declarations
******************************************************/
uint32_t host_platform_resource_size(whd_driver_t whd_drv, whd_resource_type_t resource, uint32_t *size_out);
uint32_t host_get_resource_block(whd_driver_t whd_drv, whd_resource_type_t type, const uint8_t **data,
                                 uint32_t *size_out);
uint32_t host_get_resource_no_of_blocks(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count);
uint32_t host_get_resource_block_size(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out);
resource_result_t resource_read(const resource_hnd_t *resource, uint32_t offset, uint32_t maxsize, uint32_t *size,
                                void *buffer);

/******************************************************
*               Variable Definitions
******************************************************/

extern const resource_hnd_t wifi_firmware_image;
extern const resource_hnd_t wifi_firmware_clm_blob;
unsigned char r_buffer[BLOCK_BUFFER_SIZE];

#if defined(WHD_DYNAMIC_NVRAM)
uint32_t dynamic_nvram_size = sizeof(wifi_nvram_image);
void *dynamic_nvram_image = &wifi_nvram_image;
#endif

/******************************************************
*               Function Definitions
******************************************************/

resource_result_t resource_read(const resource_hnd_t *resource, uint32_t offset, uint32_t maxsize, uint32_t *size,
                                void *buffer)
{
    if (offset > resource->size)
    {
        return RESOURCE_OFFSET_TOO_BIG;
    }

    *size = MIN(maxsize, resource->size - offset);

    if (resource->location == RESOURCE_IN_MEMORY)
    {
        memcpy(buffer, &resource->val.mem.data[offset], *size);
    }
#ifdef USES_RESOURCES_IN_EXTERNAL_STORAGE
    else if (resource->location == RESOURCE_IN_EXTERNAL_STORAGE)
    {
        return platform_read_external_resource(resource, offset, maxsize, size, buffer);
    }
#endif
#ifdef USES_RESOURCE_GENERIC_FILESYSTEM
    else
    {
        wiced_file_t file_handle;
        uint64_t size64;
        uint64_t maxsize64 =  maxsize;
        if (WICED_SUCCESS !=
            wiced_filesystem_file_open (&resource_fs_handle, &file_handle, resource->val.fs.filename,
                                        WICED_FILESYSTEM_OPEN_FOR_READ) )
        {
            return RESOURCE_FILE_OPEN_FAIL;
        }
        if (WICED_SUCCESS != wiced_filesystem_file_seek (&file_handle, (offset + resource->val.fs.offset), SEEK_SET) )
        {
            return RESOURCE_FILE_SEEK_FAIL;
        }
        if (WICED_SUCCESS != wiced_filesystem_file_read (&file_handle, buffer, maxsize64, &size64) )
        {
            wiced_filesystem_file_close (&file_handle);
            return RESOURCE_FILE_READ_FAIL;
        }
        *size = (uint32_t)size64;
        wiced_filesystem_file_close (&file_handle);
    }
#else
#ifdef USES_RESOURCE_FILESYSTEM
    else
    {
        wicedfs_file_t file_hnd;

        if (0 != wicedfs_fopen(&resource_fs_handle, &file_hnd, resource->val.fs.filename) )
        {
            return RESOURCE_FILE_OPEN_FAIL;
        }

        if (0 != wicedfs_fseek(&file_hnd, (long)(offset + resource->val.fs.offset), SEEK_SET) )
        {
            wicedfs_fclose(&file_hnd);
            return RESOURCE_FILE_SEEK_FAIL;
        }

        if (*size != wicedfs_fread(buffer, 1, *size, &file_hnd) )
        {
            wicedfs_fclose(&file_hnd);
            return RESOURCE_FILE_READ_FAIL;
        }

        wicedfs_fclose(&file_hnd);
    }
#endif /* ifdef USES_RESOURCE_FILESYSTEM */
#endif /* USES_RESOURCE_GENERIC_FILESYSTEM */
    return RESOURCE_SUCCESS;
}

uint32_t host_platform_resource_size(whd_driver_t whd_drv, whd_resource_type_t resource, uint32_t *size_out)
{
    if (resource == WHD_RESOURCE_WLAN_FIRMWARE)
    {
#ifdef NO_WIFI_FIRMWARE
        whd_assert("Request firmware in a no wifi firmware application", 0 == 1);
        *size_out = 0;
#else
#ifdef WIFI_FIRMWARE_IN_MULTI_APP
        wiced_app_t wifi_app;

        *size_out = 0;
        if (wiced_waf_app_open(DCT_WIFI_FIRMWARE_INDEX, &wifi_app) != WICED_SUCCESS)
        {
            return ( whd_result_t )RESOURCE_UNSUPPORTED;
        }
        wiced_waf_app_get_size(&wifi_app, size_out);
#else
        *size_out = (uint32_t)resource_get_size(&wifi_firmware_image);
#endif
#endif

    }
    else if (resource == WHD_RESOURCE_WLAN_NVRAM)
    {
        *size_out = NVRAM_SIZE;
    }
    else
    {
        *size_out = (uint32_t)resource_get_size(&wifi_firmware_clm_blob);
    }
    return WHD_SUCCESS;
}

uint32_t host_get_resource_block(whd_driver_t whd_drv, whd_resource_type_t type, const uint8_t **data,
                                 uint32_t *size_out)
{
    uint32_t resource_size;
    uint32_t block_size;
    uint32_t block_count;
    static uint32_t transfer_progress;
    static uint32_t i;
    uint32_t result;

    host_platform_resource_size(whd_drv, type, &resource_size);
    host_get_resource_block_size(whd_drv, type, &block_size);
    host_get_resource_no_of_blocks(whd_drv, type, &block_count);
    memset(r_buffer, 0, block_size);
    if (i < block_count)
    {
        transfer_progress = i * block_size;
        i++;
    }
    else
    {
        i = 0;
        transfer_progress = i * block_size;
        i++;
    }

    if (type == WHD_RESOURCE_WLAN_FIRMWARE)
    {
        result = resource_read( (const resource_hnd_t *)&wifi_firmware_image, transfer_progress, block_size, size_out,
                                r_buffer );
        if (result != WHD_SUCCESS)
        {
            return result;
        }
        *data = (uint8_t *)&r_buffer;
        /*
         * In case of local buffer read use the following code
         *
         *  *size_out = MIN(BLOCK_BUFFER_SIZE, resource_size - transfer_progress);
         *  *data = (uint8_t *)wifi_firmware_image_data;
         *
         * For sending the entire buffer in single block set size out as following
         *  *size_out = (uint32_t)resource_get_size(&wifi_firmware_image);
         */
    }
    else if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        *size_out = NVRAM_SIZE;
        *data = (uint8_t *)NVRAM_IMAGE_VARIABLE;
        i = 0;
    }
    else
    {
        result = resource_read( (const resource_hnd_t *)&wifi_firmware_clm_blob, transfer_progress, block_size,
                                size_out,
                                r_buffer );
        if (result != WHD_SUCCESS)
        {
            return result;
        }
        *data = (uint8_t *)&r_buffer;
        /*
         * In case of local buffer read use the following code
         *
         *  *size_out = MIN(BLOCK_BUFFER_SIZE, resource_size - transfer_progress);
         *  *data = (uint8_t *)wifi_firmware_clm_blob_image_data;
         *
         * For sending the entire buffer in single block set size out as following
         *  *size_out = sizeof(wifi_firmware_clm_blob_image_data);
         */

    }
    transfer_progress = 0;

    return WHD_SUCCESS;
}

uint32_t host_get_resource_block_size(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out)
{
    *size_out = BLOCK_BUFFER_SIZE;
    return WHD_SUCCESS;
}

uint32_t host_get_resource_no_of_blocks(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count)
{
    if (type == WHD_RESOURCE_WLAN_NVRAM)
        *block_count = 1;
    else
    {
        uint32_t resource_size;
        uint32_t block_size;

        host_platform_resource_size(whd_drv, type, &resource_size);
        host_get_resource_block_size(whd_drv, type, &block_size);
        *block_count = resource_size / block_size;
        if (resource_size % block_size)
            *block_count += 1;
    }
    return WHD_SUCCESS;
}

whd_resource_source_t resource_ops =
{
    .whd_resource_size = host_platform_resource_size,
    .whd_get_resource_block_size = host_get_resource_block_size,
    .whd_get_resource_no_of_blocks = host_get_resource_no_of_blocks,
    .whd_get_resource_block = host_get_resource_block
};


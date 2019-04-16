/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *  Provides generic clm blob file download functionality
 */

#include <stdlib.h>
#include "whd_clm.h"
#include "whd_wlioctl.h"
#include "whd_sdpcm.h"
#include "whd_debug.h"
#include "whd_int.h"
#include "whd_buffer_api.h"
#include "whd_resource_if.h"
#include "whd_types_int.h"

/******************************************************
* @cond       Constants
******************************************************/

/*
   Generic interface for downloading required data onto the dongle
 */
static int whd_download_wifi_clm_image(whd_interface_t ifp, const char *iovar, uint16_t flag, uint16_t dload_type,
                                       unsigned char *dload_buf, uint32_t len)
{
    wl_dload_data_t *dload_ptr = (wl_dload_data_t *)dload_buf;
    unsigned int dload_data_offset;
    whd_buffer_t buffer;
    uint8_t *iov_data;
    whd_driver_t whd_driver = ifp->whd_driver;

    dload_data_offset = offsetof(wl_dload_data_t, data);
    dload_ptr->flag = htod16( (DLOAD_HANDLER_VER << DLOAD_FLAG_VER_SHIFT) | flag );
    dload_ptr->dload_type = htod16(dload_type);
    dload_ptr->len = htod32(len - dload_data_offset);

    dload_ptr->crc = 0;

    whd_assert("dload buffer too large", len < 0xffffffff - 8);
    len = len + 8 - (len % 8);

    iov_data = (uint8_t *)whd_sdpcm_get_iovar_buffer(whd_driver, &buffer, (uint16_t)len, iovar);
    CHECK_IOCTL_BUFFER(iov_data);
    memcpy(iov_data, (uint8_t *)dload_ptr, len);
    CHECK_RETURN(whd_sdpcm_send_iovar(ifp, SDPCM_SET, buffer, NULL) );
    return WHD_SUCCESS;
}

whd_result_t whd_process_clm_data(whd_interface_t ifp)
{
    whd_result_t ret = WHD_SUCCESS;
    uint32_t clm_blob_size;
    uint32_t datalen;
    unsigned int size2alloc, data_offset;
    unsigned char *chunk_buf;
    uint16_t dl_flag = DL_BEGIN;
    unsigned int cumulative_len = 0;
    unsigned int chunk_len;
    uint32_t size_read;
    uint8_t *image;
    uint32_t block_size;
    uint32_t blocks_count = 0;
    uint32_t i;
    whd_driver_t whd_driver = ifp->whd_driver;

    /* clm file size is the initial datalen value which is decremented */
    ret = whd_resource_size(whd_driver, WHD_RESOURCE_WLAN_CLM, &clm_blob_size);

    if (ret != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Fatal error: download_resource doesn't exist\n") );
        return ret;
    }
    datalen = clm_blob_size;

    ret = whd_get_resource_block_size(whd_driver, WHD_RESOURCE_WLAN_CLM, &block_size);
    if (ret != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Fatal error: download_resource block size not know\n") );
        return ret;
    }

    ret = whd_get_resource_no_of_blocks(whd_driver, WHD_RESOURCE_WLAN_CLM, &blocks_count);
    if (ret != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Fatal error: download_resource blocks count not know\n") );
        return ret;
    }
    data_offset = offsetof(wl_dload_data_t, data);
    size2alloc = data_offset + block_size;

    if ( (chunk_buf = (unsigned char *)malloc(size2alloc) ) != NULL )
    {
        memset(chunk_buf, 0, size2alloc);

        for (i = 0; i < blocks_count; i++)
        {
            whd_get_resource_block(whd_driver, WHD_RESOURCE_WLAN_CLM, (const uint8_t **)&image, &size_read);
            if (datalen >= block_size)
                chunk_len = block_size;
            else
                chunk_len = datalen;

            memcpy(chunk_buf + data_offset, &image[0], size_read);
            cumulative_len += chunk_len;

            if (datalen - chunk_len == 0)
                dl_flag |= DL_END;

            ret = whd_download_wifi_clm_image(ifp, IOVAR_STR_CLMLOAD, dl_flag, DL_TYPE_CLM, chunk_buf,
                                              data_offset + chunk_len);
            dl_flag &= (uint16_t) ~DL_BEGIN;

            datalen = datalen - chunk_len;
        }

        free(chunk_buf);
        if (ret != WHD_SUCCESS)
        {
            whd_result_t ret_clmload_status;
            whd_buffer_t buffer;
            whd_buffer_t response;
            void *data;

            WPRINT_WHD_DEBUG( ("clmload (%ld byte file) failed with return %d; ", clm_blob_size, ret) );
            data = (int *)whd_sdpcm_get_iovar_buffer(whd_driver, &buffer, 4, IOVAR_STR_CLMLOAD_STATUS);
            CHECK_IOCTL_BUFFER(data);
            ret_clmload_status = whd_sdpcm_send_iovar(ifp, SDPCM_GET, buffer, &response);
            if (ret_clmload_status != WHD_SUCCESS)
            {
                WPRINT_WHD_DEBUG( ("clmload_status failed with return %d\n", ret_clmload_status) );
            }
            else
            {
                uint8_t *clmload_status = (uint8_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, response);
                if (clmload_status != NULL)
                {
                    WPRINT_WHD_DEBUG( ("clmload_status is %lu\n", *clmload_status) );
                    CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
                }
            }
        }
    }
    else
    {
        WPRINT_WHD_ERROR( ("Memory allocation failure, %s failed at %d \n", __func__, __LINE__) );
        ret = WHD_MALLOC_FAILURE;
    }

    return ret;
}


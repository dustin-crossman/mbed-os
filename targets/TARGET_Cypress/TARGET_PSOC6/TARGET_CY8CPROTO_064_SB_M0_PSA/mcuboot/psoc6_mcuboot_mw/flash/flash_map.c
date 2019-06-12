/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*******************************************************************************
* \copyright
*
* (c) 2018, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
*
* This software is a port of the open source MCUBoot project.
*
* This file was modified to fit PSoC6-based MCUBoot applications.
*
* Portions of this software, including source code, documentation and related
* materials ("Software"), are owned by Cypress Semiconductor
* Corporation or one of its subsidiaries ("Cypress") and is protected by
* and subject to worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-
* exclusive, non-transferable license to copy, modify, and compile the
* Software source code solely for use in connection with Cypress's
* integrated circuit products. Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. Cypress reserves the right to make
* changes to the Software without notice. Cypress does not assume any
* liability arising out of the application or use of the Software or any
* product or circuit described in the Software. Cypress does not
* authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*
********************************************************************************/
#include <stdbool.h>

#include <flash_map/flash_map.h>
#include <hal/hal_flash.h>
#include <sysflash/sysflash.h>

#include <stddef.h>	/* offsetof() */
#include <mcuboot_errno.h>	/* EINVAL */

#include "bootutil/bootutil_log.h"

#include "flash.h"
#include "flash_psoc6.h"
#include "flash_smif.h"
#include "cy_flash.h"
#include "cy_pdl.h"
/* The following 4 macros are taken from zephyr project, util.h file. */

/*
 * Copyright (c) 2011-2014, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Evaluates to 0 if cond is true-ish; compile error otherwise */
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

#define CONTAINER_OF(ptr, type, field) \
	((type *)(((char *)(ptr)) - offsetof(type, field)))

#if 0 
/* Linker error in IAR no definition for "__builtin_types_compatible_p" */
/* Evaluates to 0 if array is an array; compile error if not array (e.g.
 * pointer)
 */
 #define IS_ARRAY(array) \
	ZERO_OR_COMPILE_ERROR( \
		!__builtin_types_compatible_p(__typeof__(array), \
					      __typeof__(&(array)[0])))

/* Evaluates to number of elements in an array; compile error if not
 * an array (e.g. pointer)
 */
#define ARRAY_SIZE(array) \
	((unsigned long) (IS_ARRAY(array) + \
		(sizeof(array) / sizeof((array)[0]))))
#else
	
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))		

#endif /* 0 */


extern struct device *boot_flash_device;

/*
 * For now, we only support one flash device.
 *
 * Pick a random device ID for it that's unlikely to collide with
 * anything "real".
 */
#define FLASH_DEVICE_ID 	111
#define FLASH_MAP_ENTRY_MAGIC 0xd00dbeef

#define SMIF_ZERO_BUFF_SIZE (256)
/*
 * The flash area describes essentially the partition table of the
 * flash.  In this case, it starts with FLASH_AREA_IMAGE_0.
 */
struct flash_map_entry part_map[] = {
    {
        .magic = FLASH_MAP_ENTRY_MAGIC,
        .area = {
            .fa_id = FLASH_AREA_IMAGE_0,
            .fa_device_id = FLASH_DEVICE_ID,
            .fa_off = FLASH_AREA_IMAGE_0_OFFSET,
            .fa_size = FLASH_AREA_IMAGE_0_SIZE,
        },
    },
    {
        .magic = FLASH_MAP_ENTRY_MAGIC,
        .area = {
            .fa_id = FLASH_AREA_IMAGE_1,
            .fa_device_id = FLASH_DEVICE_ID,
            .fa_off = FLASH_AREA_IMAGE_1_OFFSET,
            .fa_size = FLASH_AREA_IMAGE_1_SIZE,
        },
    },
    {
        .magic = FLASH_MAP_ENTRY_MAGIC,
        .area = {
            .fa_id = FLASH_AREA_IMAGE_SCRATCH,
            .fa_device_id = FLASH_DEVICE_ID,
            .fa_off = FLASH_AREA_IMAGE_SCRATCH_OFFSET,
            .fa_size = FLASH_AREA_IMAGE_SCRATCH_SIZE,
        },
    }
};

int flash_device_base(uint8_t fd_id, uintptr_t *ret)
{
    if (fd_id != FLASH_DEVICE_ID) {
        BOOT_LOG_ERR("invalid flash ID %d; expected %d",
                     fd_id, FLASH_DEVICE_ID);
        return -EINVAL;
    }
    *ret = FLASH_DEVICE_BASE;
    return 0;
}

/*
 * `open` a flash area.  The `area` in this case is not the individual
 * sectors, but describes the particular flash area in question.
 */
int flash_area_open(uint8_t id, const struct flash_area **area)
{
    int i;

    BOOT_LOG_DBG("area %d", id);

    for (i = 0; i < ARRAY_SIZE(part_map); i++) {
        if (id == part_map[i].area.fa_id) {
            break;
        }
    }
    if (i == ARRAY_SIZE(part_map)) {
        return -1;
    }

    *area = &part_map[i].area;
    part_map[i].ref_count++;

    return 0;
}

/*
 * Nothing to do on close.
 */
void flash_area_close(const struct flash_area *area)
{
    struct flash_map_entry *entry;

    if (!area) {
        return;
    }

    entry = CONTAINER_OF(area, struct flash_map_entry, area);
    if (entry->magic != FLASH_MAP_ENTRY_MAGIC) {
        BOOT_LOG_ERR("invalid area %p (id %u)", area, area->fa_id);
        return;
    }
    if (entry->ref_count == 0) {
        BOOT_LOG_ERR("area %u use count underflow", area->fa_id);
        return;
    }
    entry->ref_count--;
}

void zephyr_flash_area_warn_on_open(void)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(part_map); i++) {
        struct flash_map_entry *entry = &part_map[i];
        if (entry->ref_count) {
            BOOT_LOG_WRN("area %u has %u users",
                         entry->area.fa_id, entry->ref_count);
        }
    }
}

int flash_area_read(const struct flash_area *area, uint32_t off, void *dst,
            uint32_t len)
{
    int rc = 0;
    uint32_t addr = area->fa_off + off;
    BOOT_LOG_DBG("area=%d, off=%x, len=%x", area->fa_id, off, len);
    /* Expecting command mode READ will only be used in non-XIP mode per user settings */
#if (defined(MCUBOOT_USE_SMIF_STAGE) && !defined(MCUBOOT_USE_SMIF_XIP))
    uint8_t addrBuf[4];
    if(IS_FLASH_SMIF(addr))
    {
        addr = addr - smifMemConfigs[0]->baseAddress + FLASH_DEVICE_BASE;
        Flash_SMIF_GetAddrBuff(addr, addrBuf)
        rc = Flash_SMIF_ReadMemory(SMIF0             /* SMIF_Type *baseaddr*/,
                                    &QSPIContext   /* cy_stc_smif_context_t *smifContext*/,
                                    dst             /* uint8_t rxBuffer[]*/,
                                    len             /* uint32_t rxSize*/,
                                    addrBuf           /* uint8_t *address */);
    }
    else
#endif
    {
#if (defined(MCUBOOT_USE_SMIF_STAGE) && defined(MCUBOOT_USE_SMIF_XIP))
        /* Preferable READ mode is Memory/XIP. */
        Cy_SMIF_SetMode(SMIF0, CY_SMIF_MEMORY);
#endif
        rc = psoc6_flash_read(addr, dst, len);
    }
    return rc;
}

int flash_area_write(const struct flash_area *area, uint32_t off, const void *src,
             uint32_t len)
{
    int rc = 0;
    uint32_t addr = area->fa_off + off;
    BOOT_LOG_DBG("area=%d, off=%x, len=%x", area->fa_id, off, len);
#ifdef MCUBOOT_USE_SMIF_STAGE
    uint8_t addrBuf[4];
    if(IS_FLASH_SMIF(addr))
    {
#ifdef MCUBOOT_USE_SMIF_XIP
        /* Memory/XIP Read is Done. Switching back to Normal/CMD */
        Cy_SMIF_SetMode(SMIF0, CY_SMIF_NORMAL);
#endif
        addr = addr - smifMemConfigs[0]->baseAddress+FLASH_DEVICE_BASE;
        Flash_SMIF_GetAddrBuff(addr, addrBuf);
        rc = Flash_SMIF_WriteMemory(SMIF0    /* SMIF_Type *baseaddr */,
                                    &QSPIContext       /* cy_stc_smif_context_t *smifContext */,
                                    src     /* uint8_t txBuffer[] */,
                                    len     /* uint32_t txSize */,
                                    addrBuf   /* uint8_t *address */);
    }
    else
#endif
    {
        rc = psoc6_flash_write(area->fa_off + off, src, len);
    }
    return rc;
}

int flash_area_erase(const struct flash_area *area, uint32_t off, uint32_t len)
{
    int rc = 0;
    uint32_t addr = area->fa_off + off;
    BOOT_LOG_DBG("area=%d, off=%x, len=%x", area->fa_id, off, len);
#ifdef MCUBOOT_USE_SMIF_STAGE
    uint8_t addrBuf[4];
    if(IS_FLASH_SMIF(addr))
    {
#ifdef MCUBOOT_USE_SMIF_XIP
        /* Memory/XIP Read is Done. Switching back to Normal/CMD */
        Cy_SMIF_SetMode(SMIF0, CY_SMIF_NORMAL);
#endif
        uint8_t zero_buff[SMIF_ZERO_BUFF_SIZE];
        uint32_t buff_num, rem_num, cur_addr;
        /* Zeroise data on external Flash instead of Erasing */
        memset(zero_buff, 0x00, sizeof(zero_buff));

        /* We do not know how big piece of data needs
         * to be Erased, so splitting it into 256-bytes
         * chunks to save zero-buffer RAM */
        buff_num = len/SMIF_ZERO_BUFF_SIZE;
        rem_num  = len%SMIF_ZERO_BUFF_SIZE;
        cur_addr = addr-smifMemConfigs[0]->baseAddress+FLASH_DEVICE_BASE;

        for(;((buff_num>0)&&(0 == rc)); buff_num--)
        {
            Flash_SMIF_GetAddrBuff(cur_addr, addrBuf);

            rc = Flash_SMIF_WriteMemory(SMIF0       /* SMIF_Type *baseaddr */,
                                        &QSPIContext/* cy_stc_smif_context_t *smifContext */,
                                        zero_buff   /* uint8_t txBuffer[] */,
                                        SMIF_ZERO_BUFF_SIZE     /* uint32_t txSize */,
                                        addrBuf   /* uint8_t *address */);

            cur_addr += SMIF_ZERO_BUFF_SIZE;
        }

        if((0 != rem_num)&&(0 == rc))
        {
            Flash_SMIF_GetAddrBuff(cur_addr, addrBuf);
            rc = Flash_SMIF_WriteMemory(SMIF0       /* SMIF_Type *baseaddr */,
                                        &QSPIContext/* cy_stc_smif_context_t *smifContext */,
                                        zero_buff   /* uint8_t txBuffer[] */,
                                        rem_num     /* uint32_t txSize */,
                                        addrBuf   /* uint8_t *address */);
        }
    }
    else
#endif
    {
        rc = psoc6_flash_erase(addr, len);
    }
    return rc;
}

uint8_t flash_area_align(const struct flash_area *area)
{
    return hal_flash_align(area->fa_id);
}

/*
 * This depends on the mappings defined in sysflash.h, and assumes
 * that slot 0, slot 1, and the scratch area area contiguous.
 */
int flash_area_id_from_image_slot(int slot)
{
    return slot + FLASH_AREA_IMAGE_0;
}

/*
 * This is used by the legacy file as well; don't mark it static until
 * that file is removed.
 */
int flash_area_get_bounds(int idx, uint32_t *off, uint32_t *len)
{
    /*
     * This simple layout has uniform slots, so just fill in the
     * right one.
     */
    if (idx < FLASH_AREA_IMAGE_0 || idx > FLASH_AREA_IMAGE_SCRATCH) {
        return -1;
    }

    switch (idx) {
    case FLASH_AREA_IMAGE_0:
        *off = part_map[0].area.fa_off;
        *len = part_map[0].area.fa_size;
        break;
    case FLASH_AREA_IMAGE_1:
        *off = part_map[1].area.fa_off;
        *len = part_map[1].area.fa_size;
        break;
    case FLASH_AREA_IMAGE_SCRATCH:
        *off = FLASH_AREA_IMAGE_SCRATCH_OFFSET;
        *len = FLASH_AREA_IMAGE_SCRATCH_SIZE;
        break;
    default:
        BOOT_LOG_ERR("unknown flash area %d", idx);
        return -1;
    }

    BOOT_LOG_DBG("area %d: offset=0x%x, length=0x%x", idx, *off, *len);
    return 0;
}

/*
 * The legacy fallbacks are used instead if the flash driver doesn't
 * provide page layout support.
 */
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
struct layout_data {
    uint32_t area_idx;
    uint32_t area_off;
    uint32_t area_len;
    void *ret;        /* struct flash_area* or struct flash_sector* */
    uint32_t ret_idx;
    uint32_t ret_len;
    int status;
};

/*
 * Generic page layout discovery routine. This is kept separate to
 * support both the deprecated flash_area_to_sectors() and the current
 * flash_area_get_sectors(). A lot of this can be inlined once
 * flash_area_to_sectors() is removed.
 */
static int flash_area_layout(int idx, int *cnt, void *ret,
                             flash_page_cb cb, struct layout_data *cb_data)
{
    cb_data->area_idx = idx;
    if (flash_area_get_bounds(idx, &cb_data->area_off, &cb_data->area_len)) {
        return -1;
    }
    cb_data->ret = ret;
    cb_data->ret_idx = 0;
    cb_data->ret_len = *cnt;
    cb_data->status = 0;

    flash_page_foreach(boot_flash_device, cb, cb_data);

    if (cb_data->status == 0) {
        *cnt = cb_data->ret_idx;
    }

    return cb_data->status;
}

/*
 * Check if a flash_page_foreach() callback should exit early, due to
 * one of the following conditions:
 *
 * - The flash page described by "info" is before the area of interest
 *   described in "data"
 * - The flash page is after the end of the area
 * - There are too many flash pages on the device to fit in the array
 *   held in data->ret. In this case, data->status is set to -ENOMEM.
 *
 * The value to return to flash_page_foreach() is stored in
 * "bail_value" if the callback should exit early.
 */
static bool should_bail(const struct flash_pages_info *info,
                        struct layout_data *data,
                        bool *bail_value)
{
    if (info->start_offset < data->area_off) {
        *bail_value = true;
        return true;
    } else if (info->start_offset >= data->area_off + data->area_len) {
        *bail_value = false;
        return true;
    } else if (data->ret_idx >= data->ret_len) {
        data->status = -ENOMEM;
        *bail_value = false;
        return true;
    }

    return false;
}

static bool to_sectors_cb(const struct flash_pages_info *info, void *datav)
{
    struct layout_data *data = datav;
    struct flash_area *ret = data->ret;
    bool bail;

    if (should_bail(info, data, &bail)) {
        return bail;
    }

    ret[data->ret_idx].fa_id = data->area_idx;
    ret[data->ret_idx].fa_device_id = 0;
    ret[data->ret_idx].pad16 = 0;
    ret[data->ret_idx].fa_off = info->start_offset;
    ret[data->ret_idx].fa_size = info->size;
    data->ret_idx++;

    return true;
}

int flash_area_to_sectors(int idx, int *cnt, struct flash_area *ret)
{
    struct layout_data data;

    return flash_area_layout(idx, cnt, ret, to_sectors_cb, &data);
}

static bool get_sectors_cb(const struct flash_pages_info *info, void *datav)
{
    struct layout_data *data = datav;
    struct flash_sector *ret = data->ret;
    bool bail;

    if (should_bail(info, data, &bail)) {
        return bail;
    }

    ret[data->ret_idx].fs_off = info->start_offset - data->area_off;
    ret[data->ret_idx].fs_size = info->size;
    data->ret_idx++;

    return true;
}

int flash_area_get_sectors(int idx, int *cnt, struct flash_sector *ret)
{
    struct layout_data data;

    return flash_area_layout(idx, cnt, ret, get_sectors_cb, &data);
}
#endif /* defined(CONFIG_FLASH_PAGE_LAYOUT) */

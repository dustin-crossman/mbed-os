/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Public API for FLASH drivers
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include "stdbool.h"

#include "fb_types.h"
#include <stddef.h>
#if defined(__ARMCC_VERSION) /* ARM-MDK */
typedef long int off_t;
#elif defined (__ICCARM__) /* IAR */
typedef long int off_t;
#else /* (__GNUC__)  GCC */
#include <sys/types.h>
#endif
#include <fb_device.h>

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
struct flash_pages_layout {
    size_t pages_count; /* count of pages sequence of the same size */
    size_t pages_size;
};
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

typedef int (*flash_api_read)(off_t offset, void *data,
                  size_t len);
typedef int (*flash_api_write)(off_t offset,
                   const void *data, size_t len);
typedef int (*flash_api_erase)(off_t offset, size_t size);
typedef int (*flash_api_write_protection)(bool enable);

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
/**
 * @brief Retrieve a flash device's layout.
 *
 * A flash device layout is a run-length encoded description of the
 * pages on the device. (Here, "page" means the smallest erasable
 * area on the flash device.)
 *
 * For flash memories which have uniform page sizes, this routine
 * returns an array of length 1, which specifies the page size and
 * number of pages in the memory.
 *
 * Layouts for flash memories with nonuniform page sizes will be
 * returned as an array with multiple elements, each of which
 * describes a group of pages that all have the same size. In this
 * case, the sequence of array elements specifies the order in which
 * these groups occur on the device.
 *
 * @param dev         Flash device whose layout to retrieve.
 * @param layout      The flash layout will be returned in this argument.
 * @param layout_size The number of elements in the returned layout.
 */
typedef void (*flash_api_pages_layout)(const struct flash_pages_layout **layout,
                                       size_t *layout_size);
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

struct flash_driver_api {
    flash_api_read read;
    flash_api_write write;
    flash_api_erase erase;
    flash_api_write_protection write_protection;
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
    flash_api_pages_layout page_layout;
#endif /* CONFIG_FLASH_PAGE_LAYOUT */
    const size_t write_block_size;
};

struct flash_pages_info {
    off_t start_offset; /* offset from the base of flash address */
    size_t size;
    u32_t index;
};
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
/**
 * @brief Callback type for iterating over flash pages present on a device.
 *
 * The callback should return true to continue iterating, and false to halt.
 *
 * @param info Information for current page
 * @param data Private data for callback
 * @return True to continue iteration, false to halt iteration.
 * @see flash_page_foreach()
 */
typedef bool (*flash_page_cb)(const struct flash_pages_info *info, void *data);

/**
 * @brief Iterate over all flash pages on a device
 *
 * This routine iterates over all flash pages on the given device,
 * ordered by increasing start offset. For each page, it invokes the
 * given callback, passing it the page's information and a private
 * data object.
 *
 * @param dev Device whose pages to iterate over
 * @param cb Callback to invoke for each flash page
 * @param data Private data for callback function
 */
void flash_page_foreach(struct device *dev, flash_page_cb cb, void *data);
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

#endif /* _FLASH_H_ */

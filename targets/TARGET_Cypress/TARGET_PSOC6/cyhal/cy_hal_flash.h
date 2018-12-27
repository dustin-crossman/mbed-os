/*******************************************************************************
* File Name: cy_hal_flash.h
*
* Description:
* Provides a high level interface for interacting with the Cypress Flash. This 
* is wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

struct flash_info_t
{
    uint32_t start_address;
    uint32_t size;
    uint32_t sector_size;
    uint32_t page_size;
    uint8_t erase_value;
}

/** Initialize the flash peripheral and the flash_t object
 *
 * @param[out] obj The flash object
 * @return The status of the init request
 */
cy_rslt_t flash_init(flash_t *obj);

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param[in,out] obj The flash object
 * @return The status of the free request
 */
cy_rslt_t flash_free(flash_t *obj);

/** Gets key flash charactoristics including the start address
 *  size, and erase values
 *
 * @param[in] obj The flash object
 * @param[out] info The flash configuration info
 * @return The status of the get_info request
 */
cy_rslt_t flash_get_info(const flash_t *obj, flash_info_t *info)

/** Read data starting at defined address
 *
 * This function has a WEAK implementation using memcpy for backwards compatibility.
 * @param[in] obj The flash object
 * @param[in] address Address to begin reading from
 * @param[out] data The buffer to read data into
 * @param[in] size The number of bytes to read
 * @return The status of the read request
 */
cy_rslt_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size);

/** Erase one page starting at defined address. The address should be at page boundary.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @return The status of the erase_page request
 */
cy_rslt_t flash_erase_page(flash_t *obj, uint32_t address);

/** Program one page starting at defined address. The address should be at page boundary.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @param[in] data The data to write to the flash
 * @return The status of the write_page request
 */
cy_rslt_t flash_write_page(flash_t *obj, uint32_t address, const uint8_t *data);

/** Program pages starting at defined address
 *
 * The pages should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * @param[in] obj The flash object
 * @param[in] address The sector starting address
 * @param[in] data The data buffer to be programmed
 * @param[in] size The number of bytes to program
 * @return The status of the program_page request
 */
cy_rslt_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size);

/** Starts erasing a single row of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start erasing from
 * @return The status of the start_erase request
 */
cy_rslt_t flash_start_erase(flash_t *obj, uint32_t address);

/** Starts writing a single row of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start writing to
 * @param[in] data The data to write to flash
 * @return The status of the start_write request
 */
cy_rslt_t flash_start_write(flash_t *obj, uint32_t address, const uint32_t* data);

/** Starts programming a single row of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start programming
 * @param[in] data The data to write to flash
 * @return The status of the start_program request
 */
cy_rslt_t flash_start_program(flash_t *obj, uint32_t address, const uint32_t* data);

/** Reports a successful operation result, reason of failure or busy status
 *
 * @param[in] obj The GPIO object to initialize
 * @param[out] complete Whether the flash operation is complete
 * @return The status of the set_handler request
 */
cy_rslt_t flash_is_operation_complete(flash_t *obj, bool *complete);

#if defined(__cplusplus)
}
#endif
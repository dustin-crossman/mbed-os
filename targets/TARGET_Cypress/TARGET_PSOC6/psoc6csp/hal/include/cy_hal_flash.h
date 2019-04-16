/***************************************************************************//**
* \file cy_hal_flash.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Flash memory. 
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Information about a flash memory */
typedef struct 
{
	uint32_t start_address; //!< Start address of the memory
	uint32_t size; //!< Size of the flash memory
	uint32_t sector_size; //!< Sector size of the memory
	uint32_t page_size; //!< Sector size of the memory
	uint8_t erase_value; //!< The flash erase value
} cy_flash_info_t;

/** Flash interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_FLASH_TBD,
} cy_flash_irq_event_t;

/** Handler for flash interrupts */
typedef void (*cy_flash_irq_handler)(void *handler_arg, cy_flash_irq_event_t event);

/** Initialize the flash peripheral and the flash_t object
 *
 * @param[out] obj The flash object
 * @return The status of the init request
 */
cy_rslt_t cy_flash_init(cy_flash_t *obj);

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param[in,out] obj The flash object
 * @return The status of the free request
 */
cy_rslt_t cy_flash_free(cy_flash_t *obj);

/** Gets key flash charactoristics including the start address
 *  size, and erase values
 *
 * @param[in] obj The flash object
 * @param[out] info The flash configuration info
 * @return The status of the get_info request
 */
cy_rslt_t cy_flash_get_info(const cy_flash_t *obj, cy_flash_info_t *info);

/** Read data starting at defined address.
 *
 * @param[in] obj The flash object
 * @param[in] address Address to begin reading from
 * @param[out] data The buffer to read data into
 * @param[in] size The number of bytes to read
 * @return The status of the read request
 */
cy_rslt_t cy_flash_read(cy_flash_t *obj, uint32_t address, uint8_t *data, size_t size);

/** Erase one page starting at defined address. The address must be at page boundary. This
 *  will block until the erase operation is complete.
 *  
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total erase size.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @return The status of the erase request
 */
cy_rslt_t cy_flash_erase(cy_flash_t *obj, uint32_t address);

/** Write one page starting at defined address. The address must be at page boundary. This
 *  will block until the write operation is complete.
 *
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total write size. The provided data buffer must be at least as large as the flash 
 *  page_size.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @param[in] data The data to write to the flash
 * @return The status of the write request
 */
cy_rslt_t cy_flash_write(cy_flash_t *obj, uint32_t address, const uint8_t *data);

/** Program one page starting at defined address. The address must be at page boundary. This
 *  will block until the write operation is complete.
 *
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total program size. The provided data buffer must be at least as large as the flash 
 *  page_size.
 *
 * @param[in] obj The flash object
 * @param[in] address The sector starting address
 * @param[in] data The data buffer to be programmed
 * @return The status of the program request
 */
cy_rslt_t cy_flash_program(cy_flash_t *obj, uint32_t address, const uint8_t *data);

/** Starts an asynchronous erase of a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total erase size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start erasing from
 * @return The status of the start_erase request
 */
cy_rslt_t cy_flash_start_erase(cy_flash_t *obj, uint32_t address);

/** Starts an asynchronous write to a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total write size. The provided data buffer must be at least as large as the flash 
 *  page_size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start writing to
 * @param[in] data The data to write to flash
 * @return The status of the start_write request
 */
cy_rslt_t cy_flash_start_write(cy_flash_t *obj, uint32_t address, const uint32_t* data);

/** Starts asynchronous programming of a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cy_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total program size. The provided data buffer must be at least as large as the flash 
 *  page_size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start programming
 * @param[in] data The data to write to flash
 * @return The status of the start_program request
 */
cy_rslt_t cy_flash_start_program(cy_flash_t *obj, uint32_t address, const uint32_t* data);

/** Reports a successful operation result, reason of failure or busy status
 *
 * @param[in] obj The Flash object being operated on
 * @param[out] complete Whether the flash operation is complete
 * @return The status of the set_handler request
 */
cy_rslt_t cy_flash_is_operation_complete(cy_flash_t *obj, bool *complete);

/** The Flash interrupt handler registration
 *
 * @param[in] obj         The Flash object being operated on
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_flash_register_irq(cy_flash_t *obj, cy_flash_irq_handler handler, void *handler_arg);

/** Configure Flash interrupt enablement.
 *
 * @param[in] obj      The Flash object being operated on
 * @param[in] event    The Flash IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_flash_irq_enable(cy_flash_t *obj, cy_flash_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

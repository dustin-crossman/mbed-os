/*******************************************************************************
* File Name: cy_hal_i2c.h
*
* Description:
* Provides a high level interface for interacting with the Cypress I2C. This is
* a wrapper around the lower level PDL API.
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

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The I2C object
 * @param[in]  sda The sda pin
 * @param[in]  scl The scl pin
 * @return The status of the init request
 */
cy_rslt_t i2c_init(i2c_t *obj, PinName sda, PinName scl);

/** Deinitialize the i2c object
 *
 * @param[in,out] obj The i2c object
 * @return The status of the free request
 */
cy_rslt_t i2c_free(i2c_t *obj);

/** Configure the I2C frequency
 *
 * @param[in] obj The I2C object
 * @param[in] hz  Frequency in Hz
 * @return The status of the frequency request
 */
cy_rslt_t i2c_frequency(i2c_t *obj, int hz);

/** Configure I2C as slave or master.
 * @param[in] obj The I2C object
 * @param[in] is_slave Enable hardware as a slave (true) or master (false)
 * @param[in] address what address the I2C responds to
 * @return The status of the mode request
 */
cy_rslt_t i2c_mode(i2c_t *obj, int is_slave, uint32_t address);

/** Send START command
 *
 * @param[in] obj The I2C object
 * @return The status of the start request
 */
cy_rslt_t i2c_start(i2c_t *obj);

/** Send STOP command
 *
 * @param[in] obj The I2C object
 * @return The status of the stop request
 */
cy_rslt_t i2c_stop(i2c_t *obj);

/** Send RESET command.
 *
 * @param[in] obj The I2C object
 * @return The status of the reset request
 */
cy_rslt_t i2c_reset(i2c_t *obj);

/** Blocking reading data
 *
 * @param[in]     obj     The I2C object
 * @param[in]     address 7/12-bit address (last bit 1)
 * @param[out]    data    The buffer for receiving
 * @param[in,out] length  in - Number of bytes to read, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t i2c_read(i2c_t *obj, int address, char *data, int *length);

/** Blocking sending data
 *
 * @param[in] obj     The I2C object
 * @param[in] address 7/12-bit address (last bit is 0)
 * @param[in] data    The buffer for sending
 * @param[in] length  Number of bytes to write
 * @param[in] stop    Stop to be generated after the transfer is done
 * @return The status of the write request
 */
cy_rslt_t i2c_write(i2c_t *obj, int address, const char *data, int length, int stop);

/** Read one byte
 *
 * @param[in] obj The I2C object
 * @param[out] data The read byte
 * @return The status of the read_byte request
 */
cy_rslt_t i2c_read_byte(i2c_t *obj, uint8_t *data);

/** Write one byte
 *
 * @param[in] obj The I2C object
 * @param[in] data Byte to be written
 * @return The status of the write_byte request
 */
cy_rslt_t i2c_write_byte(i2c_t *obj, int data);

/** Start I2C asynchronous transfer
 *
 * @param[in]     obj       The I2C object
 * @param[in]     tx        The transmit buffer
 * @param[in,out] tx_length in - The number of bytes to transmit, out - The number of bytes actually transmitted
 * @param[out]    rx        The receive buffer
 * @param[in,out] rx_length in - The number of bytes to receive, out - The number of bytes actually received
 * @param[in]     address   The address to be set - 7bit or 9bit
 * @param[in]     stop      If true, stop will be generated after the transfer is done
 * @param[in]     handler   The I2C IRQ handler to be set
 * @param[in]     event     Event mask for the transfer. See \ref hal_I2CEvents
 * @param[in]     hint      DMA hint usage
 * @return The status of the transfer_asynch request
 */
cy_rslt_t i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t *tx_length, void *rx, size_t *rx_length, uint32_t address, bool stop, uint32_t handler, uint32_t event, DMAUsage hint);

/** Attempts to determine if the I2C peripheral is already in use
 *
 * @param[in]  obj The I2C object
 * @param[out] active Is the I2C still transfering data or not
 * @return The status of the active request
 */
cy_rslt_t i2c_active(i2c_t *obj, bool *active);

/** Abort asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2C object
 * @return The status of the abort_asynch request
 */
cy_rslt_t i2c_abort_asynch(i2c_t *obj);

#if defined(__cplusplus)
}
#endif
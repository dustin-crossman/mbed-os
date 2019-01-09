/*******************************************************************************
* File Name: cy_hal_spi.h
*
* Description:
* Provides a high level interface for interacting with the Cypress SPI. This is
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

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 * @return The status of the init request
 */
cy_rslt_t spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel);

/** Release a SPI object
 *
 * Return the peripheral, pins and clock owned by the SPI object to their reset state
 * @param[in,out] obj The SPI object to deinitialize
 * @return The status of the free request
 */
cy_rslt_t spi_free(spi_t *obj);

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 * @return The status of the format request
 */
cy_rslt_t spi_format(spi_t *obj, uint8_t bits, uint8_t mode, uint8_t slave);

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 * @return The status of the frequency request
 */
cy_rslt_t spi_frequency(spi_t *obj, uint32_t hz);

/** Get a received value out of the SPI receive buffer
 *
 * Blocks until a value is available
 * @param[in] obj   The SPI peripheral to read
 * @param[in] value The value received
 * @return The status of the read request
 */
cy_rslt_t spi_read(spi_t *obj, uint8_t value);

/** Write a byte out 
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return The status of the write request
 */
cy_rslt_t spi_write(spi_t *obj, uint32_t value);

/** Write a block out and receive a value
 *
 *  The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param[in] obj           The SPI peripheral to use for sending
 * @param[in] tx            Pointer to the byte-array of data to write to the device
 * @param[in,out] tx_length Number of bytes to write, updated with the number actually written
 * @param[out] rx           Pointer to the byte-array of data to read from the device
 * @param[in,out] rx_length Number of bytes to read, udpated with the number actually read
 * @param[in] write_fill    Default data transmitted while performing a read
 * @return The status of the transfer request
 */
cy_rslt_t spi_transfer(spi_t *obj, const uint8_t *tx, size_t *tx_length, uint8_t *rx, size_t *rx_length, uint8_t write_fill);

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj           The SPI object that holds the transfer information
 * @param[in] tx            The transmit buffer
 * @param[in,out] tx_length The number of bytes to transmit
 * @param[out] rx           The receive buffer
 * @param[in,out] rx_length The number of bytes to receive
 * @param[in] event         The logical OR of events to be registered
 * @param[in] handler       SPI interrupt handler
 * @param[in] hint          A suggestion for how to use DMA with this transfer
 * @return The status of the transfer_asynch request
 */
cy_rslt_t spi_transfer_asynch(spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length, uint32_t handler, uint32_t event, DMAUsage hint);

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj  The SPI peripheral to check
 * @param[in] busy Indication of whether the SPI is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t spi_is_busy(spi_t *obj, bool *busy);

/** Abort an SPI transfer
 *
 * @param[in] obj The SPI peripheral to stop
 * @return The status of the abort_asynch request
 */
cy_rslt_t spi_abort_asynch(spi_t *obj);

#if defined(__cplusplus)
}
#endif
/***************************************************************************//**
* \file cy_hal_spi.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SPI. 
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

/** SPI interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_SPI_TBD,
} cy_spi_irq_event_t;

/** Handler for SPI interrupts */
typedef void (*cy_spi_irq_handler)(void *handler_arg, cy_spi_irq_event_t event);

/** SPI operating modes */
typedef enum
{
    /** Standard motorola SPI operation */
    CY_SPI_MOTOROLA,
    /** TI SPI operation, where the slave select is a pulse. In this mode,
      * the pulse coincides with the first bit 
      */
    CY_SPI_TI_COINCIDES,
    /** National SPI operation. This is a half-duplex mode of operation. */
    CY_SPI_NATIONAL,
    /** TI SPI operation, where the slave select is a pulse. In this mode,
      * the pulse precedes with the first bit 
      */
    CY_SPI_TI_PRECEDES,
} cy_spi_mode_t;

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cy_spi_init(cy_spi_t *obj, cy_gpio_t mosi, cy_gpio_t miso, cy_gpio_t sclk, cy_gpio_t ssel, const cy_clock_divider_t *clk);

/** Release a SPI object
 *
 * Return the peripheral, pins and clock owned by the SPI object to their reset state
 * @param[in,out] obj The SPI object to deinitialize
 * @return The status of the free request
 */
cy_rslt_t cy_spi_free(cy_spi_t *obj);

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * @param[in,out] obj       The SPI object to configure
 * @param[in]     bits      The number of bits per frame
 * @param[in]     mode      The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     is_slave  false for master mode or true for slave mode operation
 * @return The status of the format request
 */
cy_rslt_t cy_spi_format(cy_spi_t *obj, uint8_t bits, cy_spi_mode_t mode, bool is_slave);

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 * @return The status of the frequency request
 */
cy_rslt_t cy_spi_frequency(cy_spi_t *obj, uint32_t hz);

/** Get a received value out of the SPI receive buffer
 *
 * Blocks until a value is available
 * @param[in] obj   The SPI peripheral to read
 * @param[in] value The value received
 * @return The status of the read request
 */
cy_rslt_t cy_spi_read(cy_spi_t *obj, uint8_t* value);

/** Write a byte out 
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return The status of the write request
 */
cy_rslt_t cy_spi_write(cy_spi_t *obj, uint32_t value);

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
cy_rslt_t cy_spi_transfer(cy_spi_t *obj, const uint8_t *tx, size_t *tx_length, uint8_t *rx, size_t *rx_length, uint8_t write_fill);

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj           The SPI object that holds the transfer information
 * @param[in] tx            The transmit buffer
 * @param[in,out] tx_length The number of bytes to transmit
 * @param[out] rx           The receive buffer
 * @param[in,out] rx_length The number of bytes to receive
 * @return The status of the transfer_async request
 */
cy_rslt_t cy_spi_transfer_async(cy_spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length);

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj  The SPI peripheral to check
 * @param[in] busy Indication of whether the SPI is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t cy_spi_is_busy(cy_spi_t *obj, bool *busy);

/** Abort an SPI transfer
 *
 * @param[in] obj The SPI peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cy_spi_abort_async(cy_spi_t *obj);

/** The SPI interrupt handler registration
 *
 * @param[in] obj         The SPI object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_spi_register_irq(cy_spi_t *obj, cy_spi_irq_handler handler, void *handler_arg);

/** Configure SPI interrupt. This function is used for word-approach
 *
 * @param[in] obj      The SPI object
 * @param[in] event    The SPI IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_spi_irq_enable(cy_spi_t *obj, cy_spi_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

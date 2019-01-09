/*******************************************************************************
* File Name: cy_hal_uart.h
*
* Description:
* Provides a high level interface for interacting with the Cypress UART. This is
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

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The serial object
 * @param[in]  tx  The TX pin name
 * @param[in]  rx  The RX pin name
 * @return The status of the init request
 */
cy_rslt_t serial_init(serial_t *obj, PinName tx, PinName rx);

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The serial object
 * @return The status of the free request
 */
cy_rslt_t serial_free(serial_t *obj);

/** Configure the baud rate
 *
 * @param[in,out] obj      The serial object
 * @param[in]     baudrate The baud rate to be configured
 * @return The status of the baud request
 */
cy_rslt_t serial_baud(serial_t *obj, uint32_t baudrate);

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param[in,out] obj       The serial object
 * @param[in]     data_bits The number of data bits
 * @param[in]     parity    The parity
 * @param[in]     stop_bits The number of stop bits
 * @return The status of the format request
 */
cy_rslt_t serial_format(serial_t *obj, uint32_t data_bits, SerialParity parity, uint32_t stop_bits);

/** Get character. This is a blocking call, waiting for a character
 *
 * @param[in] obj    The serial object
 * @param[out] value The value read from the serial port
 * @return The status of the getc request
 */
cy_rslt_t serial_getc(serial_t *obj, utin8_t value);

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param[in] obj The serial object
 * @param[in] value The character to be sent
 * @return The status of the putc request
 */
cy_rslt_t serial_putc(serial_t *obj, uint32_t value);

/** Check if the serial peripheral is readable
 *
 * @param[in]  obj      The serial object
 * @param[out] readable Non-zero value if a character can be read, 0 if nothing to read
 * @return The status of the is_readable request
 */
cy_rslt_t  serial_is_readable(serial_t *obj, bool *readable);

/** Check if the serial peripheral is writable
 *
 * @param[in]  obj      The serial object
 * @param[out] writable Non-zero value if a character can be written, 0 otherwise.
 * @return The status of the is_writable request
 */
cy_rslt_t serial_is_writable(serial_t *obj, bool *writable);

/** Clear the serial peripheral
 *
 * @param[in] obj The serial object
 * @return The status of the clear request
 */
cy_rslt_t serial_clear(serial_t *obj);

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param[in,out] obj    The serial object
 * @param[in]     type   The type of the flow control. Look at the available FlowControl types.
 * @param[in]     rxflow The TX pin name
 * @param[in]     txflow The RX pin name
 * @return The status of the set_flow_control request
 */
cy_rslt_t serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow);

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * @param[in] obj    The serial object
 * @param[in] tx     The transmit buffer
 * @param[in] length The number of bytes to transmit
 * @param[in] hint   A suggestion for how to use DMA with this transfer
 * @return The status of the tx request
 */
cy_rslt_t serial_tx(serial_t *obj, const void *tx, size_t *tx_length, DMAUsage hint);

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * @param[in]     obj    The serial object
 * @param[in]     rx     The receive buffer
 * @param[in,out] length The number of bytes to receive
 * @param[in]     hint   A suggestion for how to use DMA with this transfer
 * @return The status of the rx request
 */
cy_rslt_t serial_rx(serial_t *obj, void *rx, size_t *rx_length, DMAUsage hint);

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * @param[in] obj     The serial object
 * @param[in] tx      The transmit buffer
 * @param[in] length  The number of bytes to transmit
 * @param[in] handler The serial handler
 * @param[in] event   The logical OR of events to be registered
 * @param[in] hint    A suggestion for how to use DMA with this transfer
 * @return The status of the tx_asynch request
 */
cy_rslt_t serial_tx_asynch(serial_t *obj, const void *tx, size_t length, uint32_t handler, uint32_t event, DMAUsage hint);

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * @param[in]     obj     The serial object
 * @param[out]    rx      The receive buffer
 * @param[in,out] length  The number of bytes to receive
 * @param[in]     handler The serial handler
 * @param[in]     event   The logical OR of events to be registered
 * @param[in]     hint    A suggestion for how to use DMA with this transfer
 * @return The status of the rx_asynch request
 */
cy_rslt_t serial_rx_asynch(serial_t *obj, void *rx, size_t *length, uint32_t handler, uint32_t event, DMAUsage hint);

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param[in]  obj    The serial object
 * @param[out] active Is the TX channel active
 * @return The status of the is_tx_active request
 */
cy_rslt_t serial_is_tx_active(serial_t *obj, bool *active);

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param[in]  obj    The serial object
 * @param[out] active Is the RX channel active
 * @return The status of the is_rx_active request
 */
cy_rslt_t serial_is_rx_active(serial_t *obj, bool *active);

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * @param[in] obj The serial object
 * @return The status of the tx_abort request
 */
cy_rslt_t serial_tx_abort(serial_t *obj);

/** Abort the ongoing RX transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * @param[in] obj The serial object
 * @return The status of the rx_abort request
 */
cy_rslt_t serial_rx_abort(serial_t *obj);

#if defined(__cplusplus)
}
#endif
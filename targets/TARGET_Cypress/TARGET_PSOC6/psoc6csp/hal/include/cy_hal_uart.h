/***************************************************************************//**
* \file cy_hal_uart.h
*
* \brief
* Provides a high level interface for interacting with the Cypress UART. 
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

/** Failed to initial clock for uart. */
#define CY_UART_CLOCK_ERROR 1

/** Enum to enable/disable/report interrupt cause flags. */
typedef enum
{
    CY_UART_IRQ_NONE                = 0,      //!< Disable all interrupt call backs
    CY_UART_IRQ_TX_TRANSMIT_IN_FIFO = 1 << 1, //!< All tx data from transmit has been moved to uart FIFO
    CY_UART_IRQ_TX_DONE             = 1 << 2, //!< All tx data has been transmitted
    CY_UART_IRQ_TX_ERROR            = 1 << 3, //!< An error occurred in tx
    CY_UART_IRQ_RX_FULL             = 1 << 4, //!< The rx software buffer is full, additional data are store into fifo buffer.
    CY_UART_IRQ_RX_DONE             = 1 << 5, //!< All rx data has been received
    CY_UART_IRQ_RX_ERROR            = 1 << 6, //!< An error occurred in rx
} cy_uart_irq_event_t;

/** UART callback function type */
typedef void (*cy_uart_irq_handler_t)(void *handler_arg, cy_uart_irq_event_t event);

/** UART Parity */
typedef enum
{
    CY_UART_PARITY_NONE,   /**< UART has no parity check   */
    CY_UART_PARITY_EVEN,   /**< UART has even parity check */
    CY_UART_PARITY_ODD,    /**< UART has odd parity check  */
} cy_uart_parity_t;

/** Initial UART configuration */
typedef struct
{
    uint32_t data_bits; //!< The number of start bits
    uint32_t stop_bits; //!< The number of stop bits
    cy_uart_parity_t parity; //!< The parity
    uint8_t *rx_buffer; //!< The rx software buffer pointer, if NULL, no rx software buffer will be used
    uint32_t rx_buffer_size; //!< The number of bytes in the rx software buffer
} cy_hal_uart_cfg_t;

/** The requested resource type is invalid */
#define CY_RSLT_ERR_CSP_UART_INVALID_PIN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_UART, 0))

/** Initialize the uart peripheral. It sets the default parameters for uart
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The uart object
 * @param[in]  tx  The TX pin name
 * @param[in]  rx  The RX pin name
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @param[in]  cfg The uart configuration data for data bits, stop bits and parity, 
 *                  if not provided, default values of (8, 1, none) will be used
 * @return The status of the init request
 */
cy_rslt_t cy_uart_init(cy_uart_t *obj, cy_gpio_t tx, cy_gpio_t rx, const cy_clock_divider_t *clk, const cy_hal_uart_cfg_t *cfg);

/** Release the uart peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The uart object
 * @return The status of the free request
 */
cy_rslt_t cy_uart_free(cy_uart_t *obj);

/** Configure the baud rate
 *
 * @param[in,out] obj      The uart object
 * @param[in]     baudrate The baud rate to be configured
 * @return The status of the baud request
 */
cy_rslt_t cy_uart_baud(cy_uart_t *obj, uint32_t baudrate);

/** Get character. This is a blocking call, waiting for a character
 *
 * @param[in] obj    The uart object
 * @param[out] value The value read from the serial port
 * @return The status of the getc request
 */
cy_rslt_t cy_uart_getc(cy_uart_t *obj, uint8_t *value);

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param[in] obj The uart object
 * @param[in] value The character to be sent
 * @return The status of the putc request
 */
cy_rslt_t cy_uart_putc(cy_uart_t *obj, uint32_t value);

/** Check if the uart peripheral is readable
 *
 * @param[in]  obj      The uart object
 * @param[out] readable Non-zero value if a character can be read, 0 if nothing to read
 * @return The status of the is_readable request
 */
cy_rslt_t cy_uart_is_readable(cy_uart_t *obj, bool *readable);

/** Check if the uart peripheral is writable
 *
 * @param[in]  obj      The uart object
 * @param[out] writable Non-zero value if a character can be written, 0 otherwise.
 * @return The status of the is_writable request
 */
cy_rslt_t cy_uart_is_writable(cy_uart_t *obj, bool *writable);

/** Clear the uart peripheral
 *
 * @param[in] obj The uart object
 * @return The status of the clear request
 */
cy_rslt_t cy_uart_clear(cy_uart_t *obj);

/** Configure the uart for the flow control. It sets flow control in the hardware
 *  if a uart peripheral supports it, otherwise software emulation is used.
 *
 * @param[in,out] obj    The uart object
 * @param[in]     cts    The TX pin name
 * @param[in]     rts    The RX pin name
 * @return The status of the init_flow_control request
 */
cy_rslt_t cy_uart_set_flow_control(cy_uart_t *obj, cy_gpio_t cts, cy_gpio_t rts);

/** Begin synchronous TX transfer. The used buffer is specified in the uart object,
 *  tx_buff
 *
 * @param[in]     obj        The uart object
 * @param[in]     tx         The transmit buffer
 * @param[in,out] tx_length  [in] The number of bytes to transmit, [out] number actually transmitted
 * @return The status of the tx request
 */
cy_rslt_t cy_uart_tx(cy_uart_t *obj, void *tx, size_t *tx_length);

/** Begin synchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the uart object - rx_buff
 *
 * @param[in]     obj       The uart object
 * @param[in]     rx        The receive buffer
 * @param[in,out] rx_length [in] The number of bytes to receive, [out] number actually received
 * @return The status of the rx request
 */
cy_rslt_t cy_uart_rx(cy_uart_t *obj, void *rx, size_t *rx_length);

/** Begin asynchronous TX transfer. The used buffer is specified in the uart object,
 *  tx_buff
 *
 * @param[in] obj     The uart object
 * @param[in] tx      The transmit buffer
 * @param[in] length  The number of bytes to transmit
 * @return The status of the tx_asynch request
 */
cy_rslt_t cy_uart_tx_asynch(cy_uart_t *obj, void *tx, size_t length);

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the uart object - rx_buff
 *
 * @param[in]  obj     The uart object
 * @param[out] rx      The receive buffer
 * @param[in]  length  The number of bytes to receive
 * @return The status of the rx_asynch request
 */
cy_rslt_t cy_uart_rx_asynch(cy_uart_t *obj, void *rx, size_t length);
/** Attempts to determine if the uart peripheral is already in use for TX
 *
 * @param[in]  obj    The uart object
 * @param[out] active Is the TX channel active
 * @return The status of the is_tx_active request
 */
cy_rslt_t cy_uart_is_tx_active(cy_uart_t *obj, bool *active);

/** Attempts to determine if the uart peripheral is already in use for RX
 *
 * @param[in]  obj    The uart object
 * @param[out] active Is the RX channel active
 * @return The status of the is_rx_active request
 */
cy_rslt_t cy_uart_is_rx_active(cy_uart_t *obj, bool *active);

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * @param[in] obj The uart object
 * @return The status of the tx_abort request
 */
cy_rslt_t cy_uart_tx_abort(cy_uart_t *obj);

/** Abort the ongoing RX transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * @param[in] obj The uart object
 * @return The status of the rx_abort request
 */
cy_rslt_t cy_uart_rx_abort(cy_uart_t *obj);

/** The uart interrupt handler registration
 *
 * @param[in] obj         The uart object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_uart_register_irq(cy_uart_t *obj, cy_uart_irq_handler_t handler, void *handler_arg);

/** Configure uart interrupt. This function is used for word-approach
 *
 * @param[in] obj      The uart object
 * @param[in] event    The uart IRQ type, this argument supports the bitwise-or of multiple enum flag values
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_uart_irq_enable(cy_uart_t *obj, cy_uart_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif
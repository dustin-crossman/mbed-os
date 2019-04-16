/***************************************************************************//**
* \file cy_hal_qspi.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Quad-SPI. 
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

/** QSPI command settings */
typedef struct {
    uint32_t instruction; //!< qspi instruction
    uint32_t address;     //!< qspi cmd address
    uint32_t size;        //!< qspi cmd size
} cy_qspi_command_t;

/** QSPI interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_QSPI_TBD,
} cy_qspi_irq_event_t;

/** Handler for QSPI interrupts */
typedef void (*cy_qspi_irq_handler)(void *handler_arg, cy_qspi_irq_event_t event);

/** Initialize QSPI peripheral.
 *
 * It should initialize QSPI pins (io0-io3, sclk and ssel), set frequency, clock polarity and phase mode. The clock for the peripheral should be enabled
 *
 * @param[out] obj  QSPI object
 * @param[in]  io0  Data pin 0
 * @param[in]  io1  Data pin 1
 * @param[in]  io2  Data pin 2
 * @param[in]  io3  Data pin 3
 * @param[in]  sclk The clock pin
 * @param[in]  ssel The chip select pin
 * @param[in]  hz   The bus frequency
 * @param[in]  mode Clock polarity and phase mode (0 - 3)
 * @return The status of the init request
 */
cy_rslt_t cy_qspi_init(cy_qspi_t *obj, cy_gpio_t io0, cy_gpio_t io1, cy_gpio_t io2, cy_gpio_t io3, cy_gpio_t sclk, cy_gpio_t ssel, uint32_t hz, uint8_t mode);

/** Deinitilize QSPI peripheral
 *
 * It should release pins that are associated with the QSPI object, and disable clocks for QSPI peripheral module that was associated with the object
 *
 * @param[in,out] obj QSPI object
 * @return The status of the free request
 */
cy_rslt_t cy_qspi_free(cy_qspi_t *obj);

/** Set the QSPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and the bus clock
 * Configures the QSPI peripheral's baud rate
 * @param[in] obj The SPI object to configure
 * @param[in] hz  The baud rate in Hz
 * @return The status of the frequency request
 */
cy_rslt_t cy_qspi_frequency(cy_qspi_t *obj, uint32_t hz);

/** Receive a command and block of data
 *
 * @param[in]     obj QSPI object
 * @param[in]     command QSPI command
 * @param[out]    data RX buffer
 * @param[in,out] length in - RX buffer length in bytes, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t cy_qspi_read(cy_qspi_t *obj, const cy_qspi_command_t *command, void *data, size_t *length);

/** Send a command and block of data
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     data    TX buffer
 * @param[in,out] length  in - TX buffer length in bytes, out - number of bytes written
 * @return The status of the write request
 */
cy_rslt_t cy_qspi_write(cy_qspi_t *obj, const cy_qspi_command_t *command, const void *data, size_t *length);

/** Send a command (and optionally data) and get the response. Can be used to send/receive device specific commands
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     tx_data TX buffer
 * @param[in,out] tx_size in - TX buffer length in bytes, out - bytes actually written
 * @param[out]    rx_data RX buffer
 * @param[in,out] rx_size in - RX buffer length in bytes, out - bytes actually read
 * @return The status of the transfer request
 */
cy_rslt_t cy_qspi_transfer(cy_qspi_t *obj, const cy_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size);

//TODO: asynch

/** The QSPI interrupt handler registration
 *
 * @param[in] obj         The QSPI object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_qspi_register_irq(cy_qspi_t *obj, cy_qspi_irq_handler handler, void *handler_arg);

/** Configure QSPI interrupt enablement.
 *
 * @param[in] obj      The QSPI object
 * @param[in] event    The QSPI IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_qspi_irq_enable(cy_qspi_t *obj, cy_qspi_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

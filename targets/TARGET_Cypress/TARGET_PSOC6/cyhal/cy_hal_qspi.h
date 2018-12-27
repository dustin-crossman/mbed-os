/*******************************************************************************
* File Name: cy_hal_qspi.h
*
* Description:
* Provides a high level interface for interacting with the Cypress QSPI. This 
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
cy_rslt_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode);

/** Deinitilize QSPI peripheral
 *
 * It should release pins that are associated with the QSPI object, and disable clocks for QSPI peripheral module that was associated with the object
 *
 * @param[in,out] obj QSPI object
 * @return The status of the free request
 */
cy_rslt_t qspi_free(qspi_t *obj);

/** Set the QSPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and the bus clock
 * Configures the QSPI peripheral's baud rate
 * @param[in] obj The SPI object to configure
 * @param[in] hz  The baud rate in Hz
 * @return The status of the frequency request
 */
cy_rslt_t qspi_frequency(qspi_t *obj, uint32_t hz);

/** Receive a command and block of data
 *
 * @param[in]     obj QSPI object
 * @param[in]     command QSPI command
 * @param[out]    data RX buffer
 * @param[in,out] length in - RX buffer length in bytes, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length);

/** Send a command and block of data
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     data    TX buffer
 * @param[in,out] length  in - TX buffer length in bytes, out - number of bytes written
 * @return The status of the write request
 */
cy_rslt_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length);

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
cy_rslt_t qspi_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size);

//TODO: asynch

#if defined(__cplusplus)
}
#endif
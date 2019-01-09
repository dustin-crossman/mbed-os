/*******************************************************************************
* File Name: cy_hal_i2s.h
*
* Description:
* Provides a high level interface for interacting with the Cypress I2S. This is
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

/** Initialize the I2S peripheral. It sets the default parameters for I2S
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The I2S object
 * @param[in]  tx_sck The transmit clock pin
 * @param[in]  tx_ws  The transmit word select pin
 * @param[in]  tx_sdo The data out pin
 * @param[in]  rx_sck The receive clock pin
 * @param[in]  rx_ws The receive word select pin
 * @param[in]  rx_sdi The data in pin
 * @return The status of the init request
 */
cy_rslt_t i2s_init(i2s_t *obj, PinName tx_sck, PinName tx_ws, PinName tx_sdo, PinName rx_sck, PinName rx_ws, PinName rx_sdi);

/** Deinitialize the i2s object
 *
 * @param[in,out] obj The i2s object
 * @return The status of the free request
 */
cy_rslt_t i2s_free(i2s_t *obj);

/** Configure the I2S frequency
 *
 * @param[in] obj The I2S object
 * @param[in] hz  Frequency in Hz
 * @return The status of the frequency request
 */
cy_rslt_t i2s_frequency(i2s_t *obj, int hz);

/** Configure I2S as slave or master.
 * @param[in] obj The I2S object
 * @param[in] is_slave Enable hardware as a slave (true) or master (false)
 * @return The status of the mode request
 */
cy_rslt_t i2s_mode(i2s_t *obj, int is_slave);

/** Blocking reading data
 *
 * @param[in]     obj    The I2S object
 * @param[out]    data   The buffer for receiving
 * @param[in,out] length in - Number of bytes to read, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t i2s_read(i2s_t *obj, char *data, int *length);

/** Blocking sending data
 *
 * @param[in] obj     The I2S object
 * @param[in] data    The buffer for sending
 * @param[in] length  Number of bytes to write
 * @param[in] stop    Stop to be generated after the transfer is done
 * @return The status of the write request
 */
cy_rslt_t i2s_write(i2s_t *obj, const char *data, int length, int stop);

/** Start I2S asynchronous transfer
 *
 * @param[in]     obj       The I2S object
 * @param[in]     tx        The transmit buffer
 * @param[in,out] tx_length in - The number of bytes to transmit, out - The number of bytes actually transmitted
 * @param[out]    rx        The receive buffer
 * @param[in,out] rx_length in - The number of bytes to receive, out - The number of bytes actually received
 * @param[in]     handler   The I2S IRQ handler to be set
 * @param[in]     event     Event mask for the transfer. See \ref hal_I2SEvents
 * @param[in]     hint      DMA hint usage
 * @return The status of the transfer_asynch request
 */
cy_rslt_t i2s_transfer_asynch(i2s_t *obj, const void *tx, size_t *tx_length, void *rx, size_t *rx_length, uint32_t handler, uint32_t event, DMAUsage hint);

/** Attempts to determine if the I2S peripheral is already in use
 *
 * @param[in]  obj The I2S object
 * @param[out] active Is the I2S still transfering data or not
 * @return The status of the active request
 */
cy_rslt_t i2s_active(i2s_t *obj, bool *active);

/** Abort asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2S object
 * @return The status of the abort_asynch request
 */
cy_rslt_t i2s_abort_asynch(i2s_t *obj);

#if defined(__cplusplus)
}
#endif
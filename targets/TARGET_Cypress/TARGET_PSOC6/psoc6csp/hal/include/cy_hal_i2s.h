/***************************************************************************//**
* \file cy_hal_i2s.h
*
* \brief
* Provides a high level interface for interacting with the Cypress I2S. 
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

/** I2S interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_I2S_TBD,
} cy_i2s_irq_t;

/** Handler for SPI interrupts */
typedef void (*cy_i2s_irq_handler)(void *handler_arg, cy_i2s_irq_t event);

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
cy_rslt_t cy_i2s_init(cy_i2s_t *obj, cy_gpio_t tx_sck, cy_gpio_t tx_ws, cy_gpio_t tx_sdo, cy_gpio_t rx_sck, cy_gpio_t rx_ws, cy_gpio_t rx_sdi);

/** Deinitialize the i2s object
 *
 * @param[in,out] obj The i2s object
 * @return The status of the free request
 */
cy_rslt_t cy_i2s_free(cy_i2s_t *obj);

/** Configure the I2S frequency
 *
 * @param[in] obj The I2S object
 * @param[in] hz  Frequency in Hz
 * @return The status of the frequency request
 */
cy_rslt_t cy_i2s_frequency(cy_i2s_t *obj, int hz);

/** Configure I2S as slave or master.
 * @param[in] obj The I2S object
 * @param[in] is_slave Enable hardware as a slave (true) or master (false)
 * @return The status of the mode request
 */
cy_rslt_t cy_i2s_mode(cy_i2s_t *obj, int is_slave);

/** Blocking reading data
 *
 * @param[in]     obj    The I2S object
 * @param[out]    data   The buffer for receiving
 * @param[in,out] length in - Number of bytes to read, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t cy_i2s_read(cy_i2s_t *obj, char *data, int *length);

/** Blocking sending data
 *
 * @param[in] obj     The I2S object
 * @param[in] data    The buffer for sending
 * @param[in] length  Number of bytes to write
 * @param[in] stop    Stop to be generated after the transfer is done
 * @return The status of the write request
 */
cy_rslt_t cy_i2s_write(cy_i2s_t *obj, const char *data, int length, int stop);

/** Start I2S asynchronous transfer
 *
 * @param[in]     obj       The I2S object
 * @param[in]     tx        The transmit buffer
 * @param[in,out] tx_length in - The number of bytes to transmit, out - The number of bytes actually transmitted
 * @param[out]    rx        The receive buffer
 * @param[in,out] rx_length in - The number of bytes to receive, out - The number of bytes actually received
 * @return The status of the transfer_async request
 */
cy_rslt_t cy_i2s_transfer_async(cy_i2s_t *obj, const void *tx, size_t *tx_length, void *rx, size_t *rx_length);

/** Abort I2S asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2S object
 * @return The status of the abort_async request
 */
cy_rslt_t cy_i2s_abort_async(cy_i2s_t *obj);

/** The I2S interrupt handler registration
 *
 * @param[in] obj         The I2S object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_i2s_register_irq(cy_i2s_t *obj, cy_i2s_irq_handler handler, void *handler_arg);

/** Configure I2S interrupt. This function is used for word-approach
 *
 * @param[in] obj      The I2S object
 * @param[in] event    The I2S IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_i2s_irq_enable(cy_i2s_t *obj, cy_i2s_irq_t event, bool enable);

#if defined(__cplusplus)
}
#endif

/***************************************************************************//**
* \file cyhal_i2s.h
*
* \brief
* Provides a high level interface for interacting with the Cypress I2S.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_i2s I2S (Inter-IC Sound)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress I2S.
*
* \defgroup group_hal_i2s_macros Macros
* \defgroup group_hal_i2s_functions Functions
* \defgroup group_hal_i2s_data_structures Data Structures
* \defgroup group_hal_i2s_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_i2s_enums
* \{
*/

/** I2S interrupt triggers */
typedef enum {
    /** TODO: Fill in */
    CYHAL_I2S_TBD,
} cyhal_i2s_event_t;

/** \} group_hal_i2s_enums */


/**
* \addtogroup group_hal_i2s_data_structures
* \{
*/

/** Handler for SPI event callbacks */
typedef void (*cyhal_i2s_event_callback_t)(void *callback_arg, cyhal_i2s_event_t event);

/** \} group_hal_i2s_data_structures */


/**
* \addtogroup group_hal_i2s_functions
* \{
*/

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
cy_rslt_t cyhal_i2s_init(cyhal_i2s_t *obj, cyhal_gpio_t tx_sck, cyhal_gpio_t tx_ws, cyhal_gpio_t tx_sdo, cyhal_gpio_t rx_sck, cyhal_gpio_t rx_ws, cyhal_gpio_t rx_sdi);

/** Deinitialize the i2s object
 *
 * @param[in,out] obj The i2s object
 */
void cyhal_i2s_free(cyhal_i2s_t *obj);

/** Configure the I2S frequency
 *
 * @param[in] obj The I2S object
 * @param[in] hz  Frequency in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_i2s_set_frequency(cyhal_i2s_t *obj, int hz);

/** Configure I2S as slave or master.
 * @param[in] obj The I2S object
 * @param[in] is_slave Enable hardware as a slave (true) or master (false)
 * @return The status of the set_mode request
 */
cy_rslt_t cyhal_i2s_set_mode(cyhal_i2s_t *obj, int is_slave);

/** Blocking reading data
 *
 * @param[in]     obj    The I2S object
 * @param[out]    data   The buffer for receiving
 * @param[in,out] length in - Number of bytes to read, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t cyhal_i2s_read(cyhal_i2s_t *obj, uint8_t *data, size_t *length);

/** Blocking sending data
 *
 * @param[in] obj     The I2S object
 * @param[in] data    The buffer for sending
 * @param[in] length  Number of bytes to write
 * @param[in] stop    Stop to be generated after the transfer is done
 * @return The status of the write request
 */
cy_rslt_t cyhal_i2s_write(cyhal_i2s_t *obj, const uint8_t *data, size_t length, bool stop);

/** Start I2S asynchronous transfer
 *
 * @param[in]     obj       The I2S object
 * @param[in]     tx        The transmit buffer
 * @param[in,out] tx_length in - The number of bytes to transmit, out - The number of bytes actually transmitted
 * @param[out]    rx        The receive buffer
 * @param[in,out] rx_length in - The number of bytes to receive, out - The number of bytes actually received
 * @return The status of the transfer_async request
 */
cy_rslt_t cyhal_i2s_transfer_async(cyhal_i2s_t *obj, const void *tx, size_t *tx_length, void *rx, size_t *rx_length);

/** Abort I2S asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2S object
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_i2s_abort_async(cyhal_i2s_t *obj);

/** The I2S callback handler registration
 *
 * @param[in] obj          The I2S object
 * @param[in] callback     The callback handler which will be invoked when the interrupt fires
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_i2s_register_callback(cyhal_i2s_t *obj, cyhal_i2s_event_callback_t callback, void *callback_arg);

/** Configure I2S interrupt. This function is used for word-approach
 *
 * @param[in] obj           The I2S object
 * @param[in] event         The I2S event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on specified events, False to turn off
 */
void cyhal_i2s_enable_event(cyhal_i2s_t *obj, cyhal_i2s_event_t event, uint8_t intrPriority, bool enable);

/** \} group_hal_i2s_functions */

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_I2S_IMPL_HEADER
#include CYHAL_I2S_IMPL_HEADER
#endif /* CYHAL_I2S_IMPL_HEADER */

/** \} group_hal_i2s */

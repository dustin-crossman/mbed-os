/***************************************************************************//**
* \file cyhal_usb_dev.h
*
* \brief
* Provides a high level interface for interacting with the Cypress USB Device.
* This interface abstracts out the chip specific details. 
* If any chip specific functionality is necessary, or performance is critical 
* the low level functions can be used directly.
* 
********************************************************************************
* \copyright
* Copyright 2019 Cypress Semiconductor Corporation
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
* \addtogroup group_hal_usb_dev USB Device
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress USB Device.
*
* \defgroup group_hal_usb_dev_macros Macros
* \defgroup group_hal_usb_dev_functions Functions
* \defgroup group_hal_usb_dev_data_structures Data Structures
* \defgroup group_hal_usb_dev_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_implementation.h"
#include "cyhal_hwmgr.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_usb_dev_macros
* \{
*/

/** The usb error */
#define CYHAL_USB_DEV_RSLT_ERR (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_USB, 0))

/** The driver configuration is not supported by the HAL */
#define CYHAL_USB_DEV_RSLT_ERR_BAD_DRV_CFG (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_USB, 1))
 
/** The configuration of USB clock failed */
#define CYHAL_USB_DEV_RSLT_ERR_CLK_CFG   (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_USB, 2))

/** Returns true if endpoint direction is IN */
#define CYHAL_USB_DEV_IS_IN_EP(endpoint)        (0U != (0x80U & (uint32_t) (endpoint)))

/** Returns endpoint number (type uint32_t) */
#define CYHAL_USB_DEV_GET_EP_NUM(endpoint)      ((uint32_t) (endpoint) & 0x0FU)

/** Returns endpoint index (type uint32_t) */
#define CYHAL_USB_DEV_GET_EP_IDX(endpoint)      (CYHAL_USB_DEV_GET_EP_NUM(endpoint) - 1U)

/** \} group_hal_usb_dev_macros */


/**
* \addtogroup group_hal_usb_dev_enums
* \{
*/

/** USB Device Endpoints types */
typedef enum 
{
    CYHAL_USB_DEV_EP_TYPE_CTRL = 0,
    CYHAL_USB_DEV_EP_TYPE_ISO  = 1,
    CYHAL_USB_DEV_EP_TYPE_BULK = 2,
    CYHAL_USB_DEV_EP_TYPE_INT  = 3
} cyhal_usb_dev_ep_type_t;

/** Service Callback Events */
typedef enum
{
    CYHAL_USB_DEV_EVENT_BUS_RESET,  /**< Callback hooked to bus reset interrupt */
    CYHAL_USB_DEV_EVENT_EP0_SETUP,  /**< Callback hooked to endpoint 0 SETUP packet interrupt */
    CYHAL_USB_DEV_EVENT_EP0_IN,     /**< Callback hooked to endpoint 0 IN packet interrupt */
    CYHAL_USB_DEV_EVENT_EP0_OUT,    /**< Callback hooked to endpoint 0 OUT packet interrupt */
} cyhal_usb_dev_event_t;

/** \} group_hal_usb_dev_enums */


/**
* \addtogroup group_hal_usb_dev_data_structures
* \{
*/

/** USB endpoint address (it consists from endpoint number and direction) */
typedef uint8_t cyhal_usb_dev_ep_t;

/** Handler for USB Device interrupt  */
typedef void (*cyhal_usb_dev_irq_handler_t)(void);

/** Handler for the transfer completion event for data endpoints (not applicable for endpoint 0)*/
typedef void (* cyhal_usb_dev_endpoint_handler_t)(cyhal_usb_dev_ep_t endpoint);

/** Handler for the events for USB Device */
typedef void (*cyhal_usb_dev_event_handler_t)(void);

/** Handler for the events for USB Device */
typedef void (*cyhal_usb_dev_sof_handler_t)(uint32_t frame_number);

/** \} group_hal_usb_dev_data_structures */


/**
* \addtogroup group_hal_usb_dev_functions
* \{
*/

/**
 * Initialize this USBPhy instance.
 *
 * @param[in,out] obj The usb device object
 * @param[in] dp      The D+ pin to initialize
 * @param[in] dm      The D- pin to initialize
 * @param[in] clk     The clock to use can be shared, if not provided a new clock will be allocated
 *
 * @return The status of the initialization request
 */
 cy_rslt_t cyhal_usb_dev_init(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, cyhal_gpio_t dm, const cyhal_clock_divider_t *clk);


/**
 * Power down this USBPhy instance
 *
 * Disable interrupts and stop sending events.
 *
 * @param[in,out] obj The usb device object
 */
 void cyhal_usb_dev_free(cyhal_usb_dev_t *obj);

/**
 * Make the USB phy visible to the USB host
 *
 * Enable either the D+ or D- pull-up so the host can detect
 * the presence of this device.
 *
 * @param[in,out] obj The usb device object
 */
 void cyhal_usb_dev_connect(cyhal_usb_dev_t *obj);

/**
 * Detach the USB phy
 *
 * Disable the D+ and D- pull-up and stop responding to
 * USB traffic.
 *
 * @param[in,out] obj The usb device object
 */
 void cyhal_usb_dev_disconnect(cyhal_usb_dev_t *obj);

/**
 * Set this device to the configured state
 *
 * Enable added endpoints if they are not enabled
 * already.
 *
 * @param[in,out] obj The usb device object
 */
 void cyhal_usb_dev_configure(cyhal_usb_dev_t *obj);

/**
 * Leave the configured state
 *
 * This is a notification to the USBPhy indicating that the device
 * is leaving the configured state. The USBPhy can disable all
 * endpoints other than endpoint 0.
 *
 * @param[in,out] obj The usb device object
 */
 void cyhal_usb_dev_unconfigure(cyhal_usb_dev_t *obj);

/**
 * Configure start of frame interrupt enablement.
 *
 * @param[in,out] obj The usb device object
 * @param[in] enable  True to turn on interrupt and start calling sof callback on every frame, 
 *                    False to turn off interrupt and stop calling sof callback.
 */
 void cyhal_usb_dev_sof_enable(cyhal_usb_dev_t *obj, bool enable);

/**
 * Set the USBPhy's address
 *
 * @param[in,out] obj The usb device object
 * @param[in] address This device's USB address
 */
 void cyhal_usb_dev_set_address(cyhal_usb_dev_t *obj, uint8_t address);

/**
 * Get wMaxPacketSize of endpoint 0. 
 * The endpoint 0 has dedicated buffer.
 *
 * @param[in,out] obj       The usb device object
 *
 * @return The size allocated for endpoint 0
 */
uint32_t cyhal_usb_dev_ep0_get_max_packet(cyhal_usb_dev_t *obj);

/**
 * Read the contents of the SETUP packet
 *
 * @param[in,out] obj The usb device object
 * @param[in] buffer  Buffer to fill with data
 * @param[in] size    Size of buffer passed in
 */
 void cyhal_usb_dev_ep0_setup_read_result(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size);

/**
 * Start receiving a packet of up to wMaxPacketSize on endpoint 0
 *
 * @param[in,out] obj The usb device object
 * @param[in] buffer  Buffer to fill with the data read
 * @param[in] size    Size of buffer
 *
 * @return The status of the read endpoint 0 request
 */
 void cyhal_usb_dev_ep0_read(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size);

/**
 * Read the contents of a received packet
 *
 * @param[in,out] obj  The usb device object
 *
 * @return Actual number of bytes that was read
 */
 uint32_t cyhal_usb_dev_ep0_read_result(cyhal_usb_dev_t *obj);

/**
 * Write a packet on endpoint 0
 *
 * @param[in,out] obj The usb device object
 * @param[in] buffer  Buffer fill with data to send
 * @param[in] size    Size of data to send
 *
 * @return The number of bytes that were written.
 */
 uint32_t cyhal_usb_dev_ep0_write(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size);

/**
 * Protocol stall on endpoint 0.
 * Stall all IN and OUT packets on endpoint 0 until a SETUP packet is received.
 *
 * @param[in,out] obj The usb device object
 *
 * @note The stall is cleared automatically when a setup packet is received
 */
 void cyhal_usb_dev_ep0_stall(cyhal_usb_dev_t *obj);

/**
 * Configure an endpoint and optionally enables it.
 *
 * @param[in,out] obj   The usb device object
 * @param[in] alloc     True to allocates buffer for the endpoint, false to skip allocation
 * @param[in] enable    True to enable endpoint operation, false to skip enablement
 * @param[in] endpoint  Endpoint to configure and enable
 * @param[in] maxPacket The maximum packet size that can be sent or received
 * @param[in] type      The type of endpoint (does not care when enable parameter is false)
 *
 * @return The status of the endpoint add request
 * 
 * @note 
 * - This function cannot be used to configure endpoint 0. That must be done
 *   with cyhal_usb_dev_ep0_get_max_packet.
 * - After endpoint was enabled it must be removed with cyhal_usb_dev_endpoint_remove 
 *   and then enabled again.
 */
cy_rslt_t cyhal_usb_dev_endpoint_add(cyhal_usb_dev_t *obj, bool alloc, bool enable ,cyhal_usb_dev_ep_t endpoint, uint32_t maxPacket, cyhal_usb_dev_ep_type_t type);

/**
 * Disable an endpoint
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to disable
 * 
 * @return The status of the endpoint remove request
 */
 cy_rslt_t cyhal_usb_dev_endpoint_remove(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint);

/**
 * Perform a functional stall on the given endpoint
 *
 * Set the HALT feature for this endpoint so that all further
 * communication is aborted.
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to stall
 * 
 * @return The status of the endpoint stall request
 */
 cy_rslt_t cyhal_usb_dev_endpoint_stall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint);

/**
 * Unstall the endpoint
 *
 * Clear the HALT feature on this endpoint so communication can
 * resume.
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to stall
 * 
 * @return The status of the endpoint unstall request
 */
 cy_rslt_t cyhal_usb_dev_endpoint_unstall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint);

/**
 * Return the endpoint stall state
 *
 * @param[in,out] obj    The usb device object
 * @param[in] endpoint   Endpoint to check stall state
 * 
 * @return True if endpoint stalled, false otherwise.
 */
 bool cyhal_usb_dev_endpoint_is_stalled(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint);

/**
 * Start a read on the given endpoint
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to start the read on
 * @param[in] data     Buffer to fill with data
 * @param[in] size     Size of the read buffer. This must be at least
 *                     the max packet size for this endpoint.
 * 
 * @return The status of start a read operation
 */
 cy_rslt_t cyhal_usb_dev_endpoint_read(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t *data, uint32_t size);

/**
 * Finish a read on the given endpoint
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to check
 * @param[out] actSize Actual number of bytes that was read
 *
 * @return The status of a finish read
 */
 cy_rslt_t cyhal_usb_dev_endpoint_read_result(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint32_t *actSize);

/**
 * Start a write on the given endpoint
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to write to
 * @param[in] data     Buffer to write
 * @param[in] size     Size of data to write
 *
 * @return The status of a write request
 */
 cy_rslt_t cyhal_usb_dev_endpoint_write(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t const *data, uint32_t size);

/**
 * Abort the current transfer if it has not yet been sent
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to abort the transfer on. It is implementation defined
 * if this function has an effect on receive endpoints.
 *
 * @return The status of an abort request
 */
 cy_rslt_t cyhal_usb_dev_endpoint_abort(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint);

 /** The USB Device interrupt handler registration
 *
 * @param[in,out] obj The usb device object
 * @param[in] handler The interrupt handler function which will be invoked when the interrupt fires
 *
 * @return The status of the register_irq request
 */
cy_rslt_t cyhal_usb_dev_register_irq(cyhal_usb_dev_t *obj, cyhal_usb_dev_irq_handler_t handler);

/** 
 * Configure USB Device interrupt enablement.
 *
 * @param[in,out] obj The usb device object
 * @param[in] enable  True to turn on interrupts, False to turn off
 */
void cyhal_usb_dev_irq_enable(cyhal_usb_dev_t *obj, bool enable);


/** 
 *  Default USB Device interrupt handler.
 *
 * @param[in,out] obj The usb device object
 */
void cyhal_usb_dev_process_irq(cyhal_usb_dev_t *obj);

/** 
 * The USB Device endpoint complete callback handler registration
 *
 * @param[in,out] obj  The usb device object
 * @param[in] endpoint Endpoint to registers handler 
 * @param[in] handler  The callback handler which will be invoked when the endpoint comp
 */
void cyhal_usb_dev_register_endpoint_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, cyhal_usb_dev_endpoint_handler_t handler);

/** 
 * The USB Device event complete callback handler registration. The events are defined by x type.
 *
 * @param[in,out] obj The usb device object
 * @param[in] event   The event that triggers the callback, see /ref cyhal_usb_dev_event_t
 * @param[in] handler The callback handler which will be invoked when the interrupt fires
 */
void cyhal_usb_dev_register_event_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_event_t event, cyhal_usb_dev_event_handler_t handler);


/** 
 * The USB Device start of frame (SOF) complete callback handler registration.
 *
 * @param[in,out] obj The usb device object
 * @param[in] handler The callback handler which will be invoked when the interrupt fires
 */
void cyhal_usb_dev_register_sof_callback( cyhal_usb_dev_t *obj, cyhal_usb_dev_sof_handler_t handler);

/** \} group_hal_usb_dev_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_usb_dev */

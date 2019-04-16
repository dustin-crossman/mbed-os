/***************************************************************************//**
* \file cy_hal_interconnect.h
*
* \brief
* Provides a high level interface for interacting with the internal digital 
* routing on the chip. This interface abstracts out the chip specific details. 
* If any chip specific functionality is necessary, or performance is critical 
* the low level functions can be used directly.
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

/** No connection is available */
#define CYCSP_CONNECT_RSLT_NO_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_INTERCONNECT, 0))
/** The connections source and destination are already connected */
#define CYCSP_CONNECT_RSLT_ALREADY_CONNECTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_INTERCONNECT, 1))
/** Invalid 1-to-1 trigger connection */
#define CYCSP_CONNECT_RSLT_INVALID_1TO1_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_INTERCONNECT, 2))

/** Connects two digital terminals. The source will be routed to the destination via the trigger routing
 * framework in the device. This supports both the trigger mux routing and the 1:1 triggers. As expected,
 * a single source can drive multiple destinations, but a destination can only be driven by a single source.
 * If the output is already connected, or the connection can not be established an error will be returned. 
 * @param[in] source The source of the signal to connect
 * @param[in] dest   The destination of the signal to connect
 * @return The status of the connect request
 */
cy_rslt_t cy_connect_trigger(cy_source_t source, cy_dest_t dest);

/** Connect a pin to a peripheral terminal. This will route a direct connect from the pin to the peripheral.
 * Any previous direct connection from the pin will be overriden.
 * @param[in] pin_connection  Details about the pin and its target connection
 * @return The status of the connect request
 */
cy_rslt_t cy_connect_pin(const cy_resource_pin_mapping_t *pin_connection);

/** Disconnect a peripheral from a pin. This will also reset the pin's drive mode to High-Z.
 * @param[in] pin The pin to disconnect
 * @return The status of the disconnect request
 */
cy_rslt_t cy_disconnect_pin(cy_gpio_t pin);

#if defined(__cplusplus)
}
#endif

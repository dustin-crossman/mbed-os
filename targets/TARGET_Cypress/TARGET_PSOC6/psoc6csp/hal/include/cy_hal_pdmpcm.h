/***************************************************************************//**
* \file cy_hal_pdmpcm.h
*
* \brief
* Provides a high level interface for interacting with the Cypress PDM/PCM. 
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

/** Describes the current configuration of a PDM/PCM */
typedef struct 
{
	//TODO: define
	void * cfg;
} cy_pdm_pcm_cfg_t;

/** PDM/PCM interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_PDM_PCM_TBD,
} cy_pdm_pcm_irq_event_t;

/** Handler for PDM/PCM interrupts */
typedef void (*cy_pdm_pcm_irq_handler)(void *handler_arg, cy_pdm_pcm_irq_event_t event);

/** Initialize the PDM/PCM peripheral
 *
 * Configures the pins used by PDM/PCM converter, sets a default format and frequency, and enables the peripheral
 * @param[out] obj The PDM/PCM object to initialize
 * @param[in]  in  The pin to use for PDM input
 * @param[in]  clk The pin to use for PDM clock output
 * @return The status of the init request
 */
cy_rslt_t cy_pdm_pcm_init(cy_pdm_pcm_t *obj, cy_gpio_t in, cy_gpio_t clk);

/** Release a PDM/PCM object
 *
 * Return the peripheral, pins and clock owned by the PDM/PCM object to their reset state
 * @param[in,out] obj The PDM/PCM object to deinitialize
 * @return The status of the free request
 */
cy_rslt_t cy_pdm_pcm_free(cy_pdm_pcm_t *obj);

/** Updates the configuration of the PDM/PCM object
 *
 * @param[inout] obj   The PDM/PCM object to configure
 * @param[in] cfg  The configuration of the PDM/PCM
 * @return The status of the format request
 */
cy_rslt_t cy_pdm_pcm_config(cy_pdm_pcm_t *obj, const cy_pdm_pcm_cfg_t *cfg);

/** Clears the FIFO
 *
 * @param[in] obj The PDM/PCM peripheral to use for sending
 * @return The status of the clear request
 */
cy_rslt_t cy_pdm_pcm_clear(cy_pdm_pcm_t *obj);

/** Reads a block out of the FIFO
 *
 * @param[in]     obj    The PDM/PCM peripheral to use for sending
 * @param[out]    data   Pointer to the byte-array of data to read from the device
 * @param[in,out] length Number of bytes to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t cy_pdm_pcm_read(cy_pdm_pcm_t *obj, uint8_t *data, uint32_t *length);

/** Begin the PDM/PCM transfer
 *
 * @param[in]     obj     The PDM/PCM object that holds the transfer information
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read_async request
 */
cy_rslt_t cy_pdm_pcm_read_async(cy_pdm_pcm_t *obj, void *data, size_t length);

/** Checks if the specified PDM/PCM peripheral is in use
 *
 * @param[in] obj  The PDM/PCM peripheral to check
 * @param[in] busy Indication of whether the PDM/PCM is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t cy_pdm_pcm_is_busy(cy_pdm_pcm_t *obj, bool *busy);

/** Abort an PDM/PCM transfer
 *
 * @param[in] obj The PDM/PCM peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cy_pdm_pcm_abort_async(cy_pdm_pcm_t *obj);

/** The PDM/PCM interrupt handler registration
 *
 * @param[in] obj         The PDM/PCM object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_pdm_pcm_register_irq(cy_pdm_pcm_t *obj, cy_i2c_irq_handler handler, void *handler_arg);

/** Configure PDM/PCM interrupt enablement.
 *
 * @param[in] obj      The PDM/PCM object
 * @param[in] event    The PDM/PCM IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_pdm_pcm_irq_enable(cy_pdm_pcm_t *obj, cy_pdm_pcm_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
* File Name: cy_hal_pdmpcm.h
*
* Description:
* Provides a high level interface for interacting with the Cypress PDM/PCM. This 
* is a wrapper around the lower level PDL API.
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

/** Initialize the PDM/PCM peripheral
 *
 * Configures the pins used by PDM/PCM converter, sets a default format and frequency, and enables the peripheral
 * @param[out] obj The PDM/PCM object to initialize
 * @param[in]  in  The pin to use for PDM input
 * @param[in]  clk The pin to use for PDM clock output
 * @return The status of the init request
 */
cy_rslt_t pdm_pcm_init(pdm_pcm_t *obj, PinName in, PinName clk);

/** Release a PDM/PCM object
 *
 * Return the peripheral, pins and clock owned by the PDM/PCM object to their reset state
 * @param[in,out] obj The PDM/PCM object to deinitialize
 * @return The status of the free request
 */
cy_rslt_t pdm_pcm_free(pdm_pcm_t *obj);

/** Configure the PDM/PCM format
 *
 * Set the ???????.

 * @param[in,out] obj   The PDM/PCM object to configure
 * @return The status of the format request
 */
cy_rslt_t pdm_pcm_format(pdm_pcm_t *obj, TBD ??);

/** Clears the FIFO
 *
 * @param[in] obj The PDM/PCM peripheral to use for sending
 * @return The status of the clear request
 */
cy_rslt_t pdm_pcm_clear(pdm_pcm_t *obj);

/** Reads a block out of the FIFO
 *
 * @param[in]     obj    The PDM/PCM peripheral to use for sending
 * @param[out]    data   Pointer to the byte-array of data to read from the device
 * @param[in,out] length Number of bytes to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t pdm_pcm_read(pdm_pcm_t *obj, uint8_t *data, uint32_t *length);

/** Begin the PDM/PCM transfer
 *
 * @param[in]     obj     The PDM/PCM object that holds the transfer information
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @param[in]     event   The logical OR of events to be registered
 * @param[in]     handler PDM/PCM interrupt handler
 * @param[in]     hint    A suggestion for how to use DMA with this transfer
 * @return The status of the read_asynch request
 */
cy_rslt_t pdm_pcm_read_asynch(pdm_pcm_t *obj, void *data, size_t length, uint32_t handler, uint32_t event, DMAUsage hint);

/** Checks if the specified PDM/PCM peripheral is in use
 *
 * @param[in] obj  The PDM/PCM peripheral to check
 * @param[in] busy Indication of whether the PDM/PCM is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t pdm_pcm_is_busy(pdm_pcm_t *obj, bool *busy);

/** Abort an PDM/PCM transfer
 *
 * @param[in] obj The PDM/PCM peripheral to stop
 * @return The status of the abort_asynch request
 */
cy_rslt_t pdm_pcm_abort_asynch(pdm_pcm_t *obj);

#if defined(__cplusplus)
}
#endif
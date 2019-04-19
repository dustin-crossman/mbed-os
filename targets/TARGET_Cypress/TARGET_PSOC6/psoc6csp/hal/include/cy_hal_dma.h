/***************************************************************************//**
* \file cy_hal_dma.h
*
* \brief
* Provides a high level interface for interacting with the Cypress DMA.
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
* \addtogroup group_hal_dma DMA (Direct Memory Access)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress DMA.
*
* \defgroup group_hal_dma_macros Macros
* \defgroup group_hal_dma_functions Functions
* \defgroup group_hal_dma_data_structures Data Structures
* \defgroup group_hal_dma_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_dma_enums
* \{
*/

/** Direction for DMA transfers */
typedef enum
{
    CY_DMA_DIRECTION_MEM2MEM, //!< Memory to memory
    CY_DMA_DIRECTION_MEM2PERIPH, //!< Memory to peripheral
    CY_DMA_DIRECTION_PERIPH2MEM, //!< Peripheral to memory
} cy_dma_direction_t;

/** DMA interrupt triggers */
typedef enum {
    /** TODO: Fill in */
    CY_DMA_TBD,
} cy_dma_irq_event_t;

/** \} group_hal_dma_enums */


/**
* \addtogroup group_hal_dma_data_structures
* \{
*/

/** Initial configuration of a DMA channel */
typedef struct
{
    uint32_t src_addr; //!< source address
    int8_t src_increment; //!< source address auto increment amount
    uint32_t dst_addr; //!< destination address
    int8_t dst_increment; //!< destination address auto increment amount
    uint32_t length; //!< length of data to be transferred
    uint8_t beat_size; //!< beat size to be set (8, 16, 32)
    //cy_source_t trigger_source; //!< Source signal for initiating the DMA transfer
} cy_dma_cfg_t;

/** Handler for DMA interrupts */
typedef void (*cy_dma_irq_handler)(void *handler_arg, cy_dma_irq_event_t event);

/** \} group_hal_dma_data_structures */


/**
* \addtogroup group_hal_dma_functions
* \{
*/

/** Initialize the DMA peripheral
 *
 * @param[out] obj          The DMA object to initialize
 * @param[in]  priority     The priority of this DMA operation relative to others
 * @param[in]  direction    The direction memory is copied
 * @return The status of the init request
 */
cy_rslt_t cy_dma_init(cy_dma_t *obj, uint8_t priority, cy_dma_direction_t direction);

/** Release the DMA object
 *
 * @param[in,out] obj The DMA object
 * @return The status of the free request
 */
cy_rslt_t cy_dma_free(cy_dma_t *obj);

/** Setup a DMA descriptor for specified resource
 *
 * @param[in] obj    The DMA object
 * @param[in] cfg    Configuration prameters for the transfer
 * @return The status of the setup_transfer request
 */
cy_rslt_t cy_dma_setup_transfer(cy_dma_t *obj, const cy_dma_cfg_t *cfg);

/** Start DMA transfer
 *
 * Kick starts transfer in DMA channel with specified channel id
 * @param[in] obj    The DMA object
 * @return The status of the start_transfer request
 */
cy_rslt_t cy_dma_start_transfer(cy_dma_t *obj);

/** DMA channel busy check
 *
 * To check whether DMA channel is busy with a job or not
 * @param[in] obj    The DMA object
 * @param[out] busy  Is the DMA object being used
 * @return The status of the busy request
 */
cy_rslt_t cy_dma_busy(cy_dma_t *obj, bool *busy);

/** The DMA interrupt handler registration
 *
 * @param[in] obj         The DMA object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_dma_register_irq(cy_dma_t *obj, cy_dma_irq_handler handler, void *handler_arg);

/** Configure DMA interrupt enablement.
 *
 * @param[in] obj      The DMA object
 * @param[in] event    The DMA IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_dma_irq_enable(cy_dma_t *obj, cy_dma_irq_event_t event, bool enable);

/** \} group_hal_dma_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_dma */

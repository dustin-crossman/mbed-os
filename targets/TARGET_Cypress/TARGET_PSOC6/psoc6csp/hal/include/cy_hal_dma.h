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

/** Direction for DMA transfers */
typedef enum
{
	CY_DMA_DIRECTION_MEM2MEM, //!< Memory to memory
	CY_DMA_DIRECTION_MEM2PERIPH, //!< Memory to peripheral
	CY_DMA_DIRECTION_PERIPH2MEM, //!< Peripheral to memory
} cy_dma_direction_t;

/** DMA interrupt triggers */
typedef enum {
	/* TODO: Fill in */
	CY_DMA_TBD,
} cy_dma_irq_event_t;

/** Handler for DMA interrupts */
typedef void (*cy_dma_irq_handler)(void *handler_arg, cy_dma_irq_event_t event);

/** Initialize the DMA peripheral
 *
 * @param[out] obj			The DMA object to initialize
 * @param[in]  priority     The priority of this DMA operation relative to others
 * @param[in]  direction	The direction memory is copied
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
 * @param[in] obj	The DMA object
 * @param[in] cfg	Configuration prameters for the transfer
 * @return The status of the setup_transfer request
 */
cy_rslt_t cy_dma_setup_transfer(cy_dma_t *obj, const cy_dma_cfg_t *cfg);

/** Start DMA transfer
 *
 * Kick starts transfer in DMA channel with specified channel id
 * @param[in] obj The DMA object
 * @return The status of the start_transfer request
 */
cy_rslt_t cy_dma_start_transfer(cy_dma_t *obj);

/** DMA channel busy check
 *
 * To check whether DMA channel is busy with a job or not
 * @param[in] obj The DMA object
 * @param[out] busy Is the DMA object being used
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

#if defined(__cplusplus)
}
#endif

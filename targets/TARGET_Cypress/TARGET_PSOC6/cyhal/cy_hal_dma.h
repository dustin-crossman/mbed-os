/*******************************************************************************
* File Name: cy_hal_dma.h
*
* Description:
* Provides a high level interface for interacting with the Cypress DMA. This is
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
#include "dma_api.h"
#include "cy_device_headers.h"
#include "cy_trigmux.h"
#include "cy_dma.h"

#ifdef CY_IP_M4CPUSS_DMAC
  #include "cy_dmac.h"    
  
  typedef cy_stc_dmac_descriptor_t cy_hal_dma_descriptor_t;
#else
  typedef cy_stc_dma_descriptor_t cy_hal_dma_descriptor_t;
#endif /* CY_IP_M4CPUSS_DMAC */
  
//typedef cy_stc_dmac_descriptor_config_t cy_hal_dma_descriptor_config_t;

typedef struct
{
    uint8_t  retrigger;
    uint8_t  interruptType;
    uint8_t  triggerOutType;
    uint8_t  channelState;
    uint8_t  triggerInType;
    bool     dataPrefetch;    
    uint8_t  dataSize;
    uint8_t  srcTransferSize;
    uint8_t  dstTransferSize;
    uint8_t  descriptorType;
    void *   srcAddress;
    void *   dstAddress;
    int32_t  srcXincrement;
    int32_t  dstXincrement;
    uint32_t xCount;
    int32_t  srcYincrement;
    int32_t  dstYincrement;
    uint32_t yCount;
    void *   nextDescriptor;
} cy_hal_dma_descriptor_config_t;

/* capabilities */
#define CY_DMA_CRC                  (1UL)
#define CY_DMA_SCATTER              (2UL)
#define CY_DMA_MEMCPY               (4UL)
  

/* channelid */
#define CY_DMA_DW0_IDX              (0U)
#define CY_DMA_DW1_IDX              (1U)
#define CY_DMA_DMAC_IDX             (2U)

#ifdef CY_IP_M4CPUSS_DMAC  
#define CY_DMA_CHAN_ID_BLK_MSK      (0x3U) /* up to 3 blocks including 2 DWs and 1 DMAC */
#else
#define CY_DMA_CHAN_ID_BLK_MSK      (0x1U) /* up to 2 DW blocks */
#endif /* CY_IP_M4CPUSS_DMAC */
  
#define CY_DMA_CHAN_ID_CHAN_MSK     (0x1FU) /* up to 32 channels */
  
#define CY_DMA_CHAN_ID(blockIdx)    (((blockIdx) & CY_DMA_CHAN_ID_BLK_MSK) << 8U)
  
#define CY_DMA_CHANNEL(channelid)   ((uint32_t)(CY_DMA_CHAN_ID_CHAN_MSK & (channelid)))
#define CY_DMA_CHAN_MSK(channelid)  (0x1UL << CY_DMA_CHANNEL(channelid))
#define CY_DMA_BLOCK_IDX(channelid) (CY_DMA_CHAN_ID_BLK_MSK & ((channelid) >> 8U))
  
#define CY_DW_BLOCK(channelid)     ((CY_DMA_DW0_IDX == CY_DMA_BLOCK_IDX(channelid)) ? DW0 : DW1)
  
#if (CY_IP_MXPERI_VERSION == 1)
  #define CY_DMA_TRIG_IN(channelid) (((CY_DMA_DW0_IDX == CY_DMA_BLOCK_IDX(channelid)) ? TRIG0_OUT_CPUSS_DW0_TR_IN0 : TRIG1_OUT_CPUSS_DW1_TR_IN0) | CY_DMA_CHANNEL(channelid))
#else
  #define CY_DMA_TRIG_IN(channelid) ((CY_DMA_DMAC_IDX == CY_DMA_BLOCK_IDX(channelid)) ? (TRIG_OUT_MUX_6_MDMA_TR_IN0 | CY_DMA_CHANNEL(channelid)) : (dma_inputs[CY_DMA_BLOCK_IDX(channelid)][CY_DMA_CHANNEL(channelid)]))
#endif /* CY_IP_MXPERI_VERSION */

/** Initialize the DMA peripheral
 *
 * @param[in] obj The DMA object to initialize
 * @return The status of the init request
 */
//cy_rslt_t dma_init(dma_t *obj);

/** Release the DMA object
 *
 * @param[in,out] obj The DMA object
 * @return The status of the free request
 */
//cy_rslt_t dma_free(dma_t *obj);

/** Setup a DMA descriptor for specified resource
 *
 * @param[in] obj               The DMA object
 * @param[in] src               source address
 * @param[in] src_inc_enable    source address auto increment enable flag
 * @param[in] desc              destination address
 * @param[in] desc_inc_enable   destination address auto increment enable flag
 * @param[in] length            length of data to be transferred
 * @param[in] beat_size         beat size to be set
 * @return The status of the setup_transfer request
 */
//cy_rslt_t dma_setup_transfer(dma_t *obj, uint32_t src, bool src_inc_enable, uint32_t desc, bool desc_inc_enable, uint32_t length, uint8_t beat_size);


/** Start DMA transfer
 *
 * Kick starts transfer in DMA channel with specified channel id
 * @param[in] obj The DMA object
 * @return The status of the start_transfer request
 */
  
cy_rslt_t dma_descriptor_setup(int channelid, cy_hal_dma_descriptor_t * descriptor, const cy_hal_dma_descriptor_config_t * descriptor_cfg);

cy_rslt_t dma_channel_setup(int channelid, cy_hal_dma_descriptor_t * descriptor);
  
cy_rslt_t dma_channel_sw_trigger(int channelid);

/** DMA channel busy check
 *
 * To check whether DMA channel is busy with a job or not
 * @param[in] obj The DMA object
 * @param[out] busy Is the DMA object being used
 * @return The status of the busy request
 */
//cy_rslt_t dma_busy(dma_t *obj, bool *busy);

/** Registers callback function for DMA
 *
 * Registers callback function for DMA for specified events
 * @param[in] obj          The DMA object
 * @param[in] handler      Callback function pointer
 * @param[in] event        Events mask
 * @return The status of the set_handler request
 */
//cy_rslt_t dma_set_handler(dma_t *obj, uint32_t handler, uint32_t event);

#if defined(__cplusplus)
}
#endif
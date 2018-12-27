/*******************************************************************************
* File Name: cy_hal_dma.c
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

#include <string.h>
#include "cy_hal_dma.h"

static bool dma_inited = false;
static uint32_t dma_channels[CY_IP_M4CPUSS_DMA_INSTANCES];

#ifdef CY_IP_M4CPUSS_DMAC
  static uint32_t dmac_channels;
#endif /* CY_IP_M4CPUSS_DMAC */

#if (CY_IP_MXPERI_VERSION == 2)
const uint32_t dma_inputs[CY_IP_M4CPUSS_DMA_INSTANCES][CPUSS_DW0_CH_NR] =
{
    {
        TRIG_OUT_MUX_0_PDMA0_TR_IN0,
        TRIG_OUT_MUX_0_PDMA0_TR_IN1,
        TRIG_OUT_MUX_0_PDMA0_TR_IN2,
        TRIG_OUT_MUX_0_PDMA0_TR_IN3,
        TRIG_OUT_MUX_0_PDMA0_TR_IN4,
        TRIG_OUT_MUX_0_PDMA0_TR_IN5,
        TRIG_OUT_MUX_0_PDMA0_TR_IN6,
        TRIG_OUT_MUX_0_PDMA0_TR_IN7,
        TRIG_OUT_1TO1_5_USB_DMA0_TO_PDMA0_TR_IN8,
        TRIG_OUT_1TO1_5_USB_DMA1_TO_PDMA0_TR_IN9,
        TRIG_OUT_1TO1_5_USB_DMA2_TO_PDMA0_TR_IN10,
        TRIG_OUT_1TO1_5_USB_DMA3_TO_PDMA0_TR_IN11,
        TRIG_OUT_1TO1_5_USB_DMA4_TO_PDMA0_TR_IN12,
        TRIG_OUT_1TO1_5_USB_DMA5_TO_PDMA0_TR_IN13,
        TRIG_OUT_1TO1_5_USB_DMA6_TO_PDMA0_TR_IN14,
        TRIG_OUT_1TO1_5_USB_DMA7_TO_PDMA0_TR_IN15,
        TRIG_OUT_1TO1_0_SCB0_TX_TO_PDMA0_TR_IN16,
        TRIG_OUT_1TO1_0_SCB0_RX_TO_PDMA0_TR_IN17,
        TRIG_OUT_1TO1_0_SCB1_TX_TO_PDMA0_TR_IN18,
        TRIG_OUT_1TO1_0_SCB1_RX_TO_PDMA0_TR_IN19,
        TRIG_OUT_1TO1_0_SCB2_TX_TO_PDMA0_TR_IN20,
        TRIG_OUT_1TO1_0_SCB2_RX_TO_PDMA0_TR_IN21,
        TRIG_OUT_1TO1_0_SCB3_TX_TO_PDMA0_TR_IN22,
        TRIG_OUT_1TO1_0_SCB3_RX_TO_PDMA0_TR_IN23,
        TRIG_OUT_1TO1_0_SCB4_TX_TO_PDMA0_TR_IN24,
        TRIG_OUT_1TO1_0_SCB4_RX_TO_PDMA0_TR_IN25,
        TRIG_OUT_1TO1_0_SCB5_TX_TO_PDMA0_TR_IN26,
        TRIG_OUT_1TO1_0_SCB5_RX_TO_PDMA0_TR_IN27,
        TRIG_OUT_1TO1_2_PASS_SAR_DONE_TO_PDMA0_TR_IN28
    },
    {
        TRIG_OUT_MUX_1_PDMA1_TR_IN0,
        TRIG_OUT_MUX_1_PDMA1_TR_IN1,
        TRIG_OUT_MUX_1_PDMA1_TR_IN2,
        TRIG_OUT_MUX_1_PDMA1_TR_IN3,
        TRIG_OUT_MUX_1_PDMA1_TR_IN4,
        TRIG_OUT_MUX_1_PDMA1_TR_IN5,
        TRIG_OUT_MUX_1_PDMA1_TR_IN6,
        TRIG_OUT_MUX_1_PDMA1_TR_IN7,
        TRIG_OUT_1TO1_1_SCB6_TX_TO_PDMA1_TR_IN8,
        TRIG_OUT_1TO1_1_SCB6_RX_TO_PDMA1_TR_IN9,
        TRIG_OUT_1TO1_1_SCB7_TX_TO_PDMA1_TR_IN10,
        TRIG_OUT_1TO1_1_SCB7_RX_TO_PDMA1_TR_IN11,
        TRIG_OUT_1TO1_1_SCB8_TX_TO_PDMA1_TR_IN12,
        TRIG_OUT_1TO1_1_SCB8_RX_TO_PDMA1_TR_IN13,
        TRIG_OUT_1TO1_1_SCB9_TX_TO_PDMA1_TR_IN14,
        TRIG_OUT_1TO1_1_SCB9_RX_TO_PDMA1_TR_IN15,
        TRIG_OUT_1TO1_1_SCB10_TX_TO_PDMA1_TR_IN16,
        TRIG_OUT_1TO1_1_SCB10_RX_TO_PDMA1_TR_IN17,
        TRIG_OUT_1TO1_1_SCB11_TX_TO_PDMA1_TR_IN18,
        TRIG_OUT_1TO1_1_SCB11_RX_TO_PDMA1_TR_IN19,
        TRIG_OUT_1TO1_1_SCB12_TX_TO_PDMA1_TR_IN20,
        TRIG_OUT_1TO1_1_SCB12_RX_TO_PDMA1_TR_IN21,
        TRIG_OUT_1TO1_3_SMIF_TX_TO_PDMA1_TR_IN22,
        TRIG_OUT_1TO1_3_SMIF_RX_TO_PDMA1_TR_IN23,
        TRIG_OUT_1TO1_4_I2S0_TX_TO_PDMA1_TR_IN24,
        TRIG_OUT_1TO1_4_I2S0_RX_TO_PDMA1_TR_IN25,
        TRIG_OUT_1TO1_4_PDM0_RX_TO_PDMA1_TR_IN26,
        TRIG_OUT_1TO1_4_I2S1_TX_TO_PDMA1_TR_IN27,
        TRIG_OUT_1TO1_4_I2S1_RX_TO_PDMA1_TR_IN28
    }
};
#endif /* CY_IP_MXPERI_VERSION */


void dma_init(void)
{
    if(!dma_inited)
    {
        uint32_t i;  
        dma_inited = true;

        for(i = 0UL; i < CY_IP_M4CPUSS_DMA_INSTANCES; i++)
        {
            dma_channels[i] = 0UL;
        }
      
      #ifdef CY_IP_M4CPUSS_DMAC
        dmac_channels = 0UL;
      #endif /* CY_IP_M4CPUSS_DMAC */
    }
}


int dma_channel_allocate(uint32_t capabilities)
{
    uint32_t dwBlock;  
    int channelid = DMA_ERROR_OUT_OF_CHANNELS;
    dma_init();

    for (dwBlock = 0UL; dwBlock < CY_IP_M4CPUSS_DMA_INSTANCES; dwBlock++)
    {
        for(channelid = 0; channelid < CPUSS_DW0_CH_NR; channelid++)
        {
            if (0UL == (dma_channels[dwBlock] & CY_DMA_CHAN_MSK(channelid)))
            {
                dma_channels[dwBlock] |= CY_DMA_CHAN_MSK(channelid);
                channelid |= CY_DMA_CHAN_ID(dwBlock);
                Cy_DMA_Enable(CY_DW_BLOCK(channelid));
                break;
            }
        }
        
        if (CPUSS_DW0_CH_NR != channelid)
        {
            break;
        }
    }

    if (CPUSS_DW0_CH_NR == channelid)
    {
      #ifdef CY_IP_M4CPUSS_DMAC     
        for(channelid = 0; channelid < CPUSS_DMAC_CH_NR; channelid++)
        {
            if(0UL == (dmac_channels & CY_DMA_CHAN_MSK(channelid)))
            {
                dmac_channels |= CY_DMA_CHAN_MSK(channelid);
                channelid |= CY_DMA_CHAN_ID(CY_DMA_DMAC_IDX);
                Cy_DMAC_Enable(DMAC);
                break;
            }
        }

        if (CPUSS_DMAC_CH_NR == channelid)
        {
            channelid = DMA_ERROR_OUT_OF_CHANNELS;
        }
      #else
        channelid = DMA_ERROR_OUT_OF_CHANNELS;
      #endif /* CY_IP_M4CPUSS_DMAC */
    }

    return (channelid);
}


int dma_channel_free(int channelid)
{
    if (DMA_ERROR_OUT_OF_CHANNELS != channelid)
    {
        //MBED_ASSERT(check channelid);

        if (CY_IP_M4CPUSS_DMA_INSTANCES > CY_DMA_BLOCK_IDX(channelid))
        {
            dma_channels[CY_DMA_BLOCK_IDX(channelid)] &= ~CY_DMA_CHAN_MSK(channelid);
            Cy_DMA_Channel_DeInit(CY_DW_BLOCK(channelid), CY_DMA_CHANNEL(channelid));
        }
      #ifdef CY_IP_M4CPUSS_DMAC
        else if (CY_DMA_DMAC_IDX == CY_DMA_BLOCK_IDX(channelid))
        {
            dmac_channels &= ~CY_DMA_CHAN_MSK(channelid);
            Cy_DMAC_Channel_DeInit(DMAC, CY_DMA_CHANNEL(channelid));
        }
      #endif /* CY_IP_M4CPUSS_DMAC */
    }

    return (0);
}


cy_rslt_t dma_channel_sw_trigger(int channelid)
{
    cy_rslt_t retVal = 1; /* bad channelid */
    
    if (DMA_ERROR_OUT_OF_CHANNELS != channelid)
    {
        //MBED_ASSERT(check channelid);
         retVal = Cy_TrigMux_SwTrigger(CY_DMA_TRIG_IN(channelid), CY_TRIGGER_TWO_CYCLES);
    }
    
    return (retVal);
}


cy_rslt_t dma_channel_setup(int channelid, cy_hal_dma_descriptor_t * descriptor)
{
    cy_rslt_t retVal = CY_RSLT_SUCCESS;
  
    if (CY_IP_M4CPUSS_DMA_INSTANCES > CY_DMA_BLOCK_IDX(channelid))
    {
        cy_stc_dma_channel_config_t channel_cfg =
        {
            .descriptor = (cy_stc_dma_descriptor_t*)descriptor,
            .preemptable = false,
            .priority = 3,
            .enable = true,
            .bufferable = false
        };
        
        retVal = Cy_DMA_Channel_Init(CY_DW_BLOCK(channelid), CY_DMA_CHANNEL(channelid), &channel_cfg);
    }
  #ifdef CY_IP_M4CPUSS_DMAC
    else if (CY_DMA_DMAC_IDX == CY_DMA_BLOCK_IDX(channelid))
    {
        cy_stc_dmac_channel_config_t channel_cfg =
        {
            .descriptor = (cy_stc_dmac_descriptor_t*)descriptor,
            .priority = 3,
            .enable = true,
            .bufferable = false,
        };
        
        retVal = Cy_DMAC_Channel_Init(DMAC, CY_DMA_CHANNEL(channelid), &channel_cfg);
    }
  #endif /* CY_IP_M4CPUSS_DMAC */
    else
    {
        retVal = 1; /* bad channelid */
    }
    
    return (retVal);
}



cy_rslt_t dma_descriptor_setup(int channelid, cy_hal_dma_descriptor_t * descriptor, const cy_hal_dma_descriptor_config_t * descriptor_cfg)
{
    cy_rslt_t retVal = CY_RSLT_SUCCESS;
  
    if (CY_IP_M4CPUSS_DMA_INSTANCES > CY_DMA_BLOCK_IDX(channelid))
    {
        cy_stc_dma_descriptor_config_t dscr_cfg =
        {
            .retrigger       = (cy_en_dma_retrigger_t)descriptor_cfg->retrigger,
            .interruptType   = (cy_en_dma_trigger_type_t)descriptor_cfg->interruptType,
            .triggerOutType  = (cy_en_dma_trigger_type_t)descriptor_cfg->triggerOutType,
            .channelState    = (cy_en_dma_channel_state_t)descriptor_cfg->channelState,
            .triggerInType   = (cy_en_dma_trigger_type_t)descriptor_cfg->triggerInType,
            .dataSize        = (cy_en_dma_data_size_t)descriptor_cfg->dataSize,
            .srcTransferSize = (cy_en_dma_transfer_size_t)descriptor_cfg->srcTransferSize,
            .dstTransferSize = (cy_en_dma_transfer_size_t)descriptor_cfg->dstTransferSize,
            .descriptorType  = (cy_en_dma_descriptor_type_t)descriptor_cfg->descriptorType,
            .srcAddress      = descriptor_cfg->srcAddress,
            .dstAddress      = descriptor_cfg->dstAddress,
            .srcXincrement   = descriptor_cfg->srcXincrement,
            .dstXincrement   = descriptor_cfg->dstXincrement,
            .xCount          = descriptor_cfg->xCount,
            .srcYincrement   = descriptor_cfg->srcYincrement,
            .dstYincrement   = descriptor_cfg->dstYincrement,
            .yCount          = descriptor_cfg->yCount,
            .nextDescriptor  = (cy_stc_dma_descriptor_t*)descriptor_cfg->nextDescriptor
        };
        
        retVal = Cy_DMA_Descriptor_Init((cy_stc_dma_descriptor_t*)descriptor, &dscr_cfg);
    }
  #ifdef CY_IP_M4CPUSS_DMAC
    else if (CY_DMA_DMAC_IDX == CY_DMA_BLOCK_IDX(channelid))
    {
        cy_stc_dmac_descriptor_config_t dscr_cfg =
        {
            .retrigger       = (cy_en_dmac_retrigger_t)descriptor_cfg->retrigger,
            .interruptType   = (cy_en_dmac_trigger_type_t)descriptor_cfg->interruptType,
            .triggerOutType  = (cy_en_dmac_trigger_type_t)descriptor_cfg->triggerOutType,
            .channelState    = (cy_en_dmac_channel_state_t)descriptor_cfg->channelState,
            .triggerInType   = (cy_en_dmac_trigger_type_t)descriptor_cfg->triggerInType,
            .dataPrefetch    = descriptor_cfg->dataPrefetch,
            .dataSize        = (cy_en_dmac_data_size_t)descriptor_cfg->dataSize,
            .srcTransferSize = (cy_en_dmac_transfer_size_t)descriptor_cfg->srcTransferSize,
            .dstTransferSize = (cy_en_dmac_transfer_size_t)descriptor_cfg->dstTransferSize,
            .descriptorType  = (cy_en_dmac_descriptor_type_t)descriptor_cfg->descriptorType,
            .srcAddress      = descriptor_cfg->srcAddress,
            .dstAddress      = descriptor_cfg->dstAddress,
            .srcXincrement   = descriptor_cfg->srcXincrement,
            .dstXincrement   = descriptor_cfg->dstXincrement,
            .xCount          = descriptor_cfg->xCount,
            .srcYincrement   = descriptor_cfg->srcYincrement,
            .dstYincrement   = descriptor_cfg->dstYincrement,
            .yCount          = descriptor_cfg->yCount,
            .nextDescriptor  = (cy_stc_dmac_descriptor_t*)descriptor_cfg->nextDescriptor
        };
      
        retVal = Cy_DMAC_Descriptor_Init(descriptor, &dscr_cfg);
    }
  #endif /* CY_IP_M4CPUSS_DMAC */
    else
    {
        retVal = 1; /* bad channelid */
    }
    
    return (retVal);
}


/*******************************************************************************
* File Name: cy_hal_interconnect.c
*
* Description:
* Provides a high level interface for interacting with the internal digital 
* routing on the chip. This is a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cy_hal_interconnect.h"

#if (CY_IP_MXPERI_VERSION == 1U)

#define TR01NUM (50U)
#define TR28NUM (42U)
#define TRG_Pos (4U)
#define TRG_Msk (0xF0U)
#define TRN_Pos (0U)
#define TRN_Msk (0x0FU)

const uint8_t tr01[TR01NUM] =
{
    0xA0U, /* tr_group[10].output[0] */
    0xA1U, /* tr_group[10].output[1] */
    0xA2U, /* tr_group[10].output[2] */
    0xA3U, /* tr_group[10].output[3] */
    0xA4U, /* tr_group[10].output[4] */
    0xA5U, /* tr_group[10].output[5] */
    0xA6U, /* tr_group[10].output[6] */
    0xA7U, /* tr_group[10].output[7] */
    0xB0U, /* tr_group[11].output[0] */
    0xB1U, /* tr_group[11].output[1] */
    0xB2U, /* tr_group[11].output[2] */
    0xB3U, /* tr_group[11].output[3] */
    0xB4U, /* tr_group[11].output[4] */
    0xB5U, /* tr_group[11].output[5] */
    0xB6U, /* tr_group[11].output[6] */
    0xB7U, /* tr_group[11].output[7] */
    0xB8U, /* tr_group[11].output[8] */
    0xB9U, /* tr_group[11].output[9] */
    0xBAU, /* tr_group[11].output[10] */
    0xBBU, /* tr_group[11].output[11] */
    0xBCU, /* tr_group[11].output[12] */
    0xBDU, /* tr_group[11].output[13] */
    0xBEU, /* tr_group[11].output[14] */
    0xBFU, /* tr_group[11].output[15] */
    0xC8U, /* tr_group[12].output[8] */
    0xC9U, /* tr_group[12].output[9] */
    0xD0U, /* tr_group[13].output[0] */
    0xD1U, /* tr_group[13].output[1] */
    0xD2U, /* tr_group[13].output[2] */
    0xD3U, /* tr_group[13].output[3] */
    0xD4U, /* tr_group[13].output[4] */
    0xD5U, /* tr_group[13].output[5] */
    0xD6U, /* tr_group[13].output[6] */
    0xD7U, /* tr_group[13].output[7] */
    0xD8U, /* tr_group[13].output[8] */
    0xD9U, /* tr_group[13].output[9] */
    0xDAU, /* tr_group[13].output[10] */
    0xDBU, /* tr_group[13].output[11] */
    0xDCU, /* tr_group[13].output[12] */
    0xDDU, /* tr_group[13].output[13] */
    0xDEU, /* tr_group[13].output[14] */
    0xDFU, /* tr_group[13].output[15] */
    0xE0U, /* tr_group[14].output[0] */
    0xE1U, /* tr_group[14].output[1] */
    0xE2U, /* tr_group[14].output[2] */
    0xE3U, /* tr_group[14].output[3] */
    0xE4U, /* tr_group[14].output[4] */
    0xE5U, /* tr_group[14].output[5] */
    0xE6U, /* tr_group[14].output[6] */
    0xE7U  /* tr_group[14].output[7] */
};

const uint8_t tr28[TR28NUM] = { 0 };

#endif /* CY_IP_MXPERI_VERSION */


/** Connects two digital terminals. The source will be routed to the destination via the trigger routing
 * framework in the device. This supports both the trigger mux routing and the 1:1 triggers. As expected,
 * a single source can drive multiple destinations, but a destination can only be driven by a single source.
 * If the output is already connected, or the connection can not be established an error will be returned. 
 * @param[in] src The source of the signal to connect
 * @param[in] dst   The destination of the signal to connect
 * @return The status of the connect request
 */

//cy_rslt_t connect_trigger(cy_source_t src, cy_destination_t dst)
cy_rslt_t connect_trigger(uint32_t src, uint32_t dst)
{
    cy_rslt_t retVal = CYCSP_CONNECT_RSLT_NO_CONNECTION;
    
    //if (0UL == _FLD2VAL(PERI_TR_GR_TR_OUT_CTL_TR_SEL, CY_TRIGMUX_TR_CTL(dst)))
    if (0UL == (PERI_TR_GR_TR_OUT_CTL_TR_SEL_Msk & PERI_TR_GR_TR_CTL(_FLD2VAL(CY_PERI_TR_CMD_GROUP_SEL, dst), _FLD2VAL(CY_PERI_TR_CTL_SEL, dst))))
    {
      #if (CY_IP_MXPERI_VERSION == 1U)
        uint32_t i;
        uint32_t grp = _FLD2VAL(PERI_TR_CMD_GROUP_SEL, dst);
        uint32_t num = (grp < 2) ? TR01NUM : TR28NUM;
        uint8_t const * ptr = (grp < 2) ? tr01 : tr28;
        
        grp = _FLD2VAL(PERI_TR_CMD_GROUP_SEL, src);
        
        for (i = 0; (i < num) && (CYCSP_CONNECT_RSLT_NO_CONNECTION == retVal); i++)
        {
            if(grp == _FLD2VAL(TRG, ptr[i]))
            {
                uint32_t mux = _FLD2VAL(TRN, ptr[i]);

                if (0UL == (PERI_TR_GR_TR_OUT_CTL_TR_SEL_Msk & PERI_TR_GR_TR_CTL(grp, mux)))
                {
                    uint32_t trg = (CY_PERI_TR_CMD_GROUP_SEL_Msk & dst) | (i + 1UL);
                    
                    if (CY_TRIGMUX_SUCCESS == Cy_TrigMux_Connect(trg, dst, false, TRIGGER_TYPE_LEVEL))
                    {
                        trg = PERI_TR_CMD_OUT_SEL_Msk | (CY_PERI_TR_CMD_GROUP_SEL_Msk & src) | mux;
                        
                        if (CY_TRIGMUX_SUCCESS == Cy_TrigMux_Connect(src, trg, false, TRIGGER_TYPE_LEVEL))
                        {
                            retVal = CY_RSLT_SUCCESS;
                        }
                    }
                }
            }
        }
        
      #else /* CY_IP_MXPERI_VERSION != 1 */
        if (CY_TRIGMUX_SUCCESS == Cy_TrigMux_Connect(src, dst, false, TRIGGER_TYPE_LEVEL))
        {
            retVal = CY_RSLT_SUCCESS;
        }
      #endif /* CY_IP_MXPERI_VERSION */
    }
    else
    {
        retVal = CYCSP_CONNECT_RSLT_ALREADY_CONNECTED;
    }
    
    return (retVal);
}


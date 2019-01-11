/*******************************************************************************
* File Name: cy_hal_interconnect.h
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

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_trigmux.h"

#define CYCSP_CONNECT_RSLT_NO_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_BASE, 0))
#define CYCSP_CONNECT_RSLT_ALREADY_CONNECTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_BASE, 1))
#define CYCSP_CONNECT_RSLT_INVALID_1TO1_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_BASE, 2))
  
/*
typedef enum 
{
    LEVEL,
    EDGE,
    EITHER
} cy_trigger_type_t;*/

#if (CY_IP_MXPERI_VERSION == 1U)
typedef enum
{
    DW0_TR_OUT0 = TRIG10_IN_CPUSS_DW0_TR_OUT0,
    DW0_TR_OUT1 = TRIG10_IN_CPUSS_DW0_TR_OUT1,
    DW0_TR_OUT2 = TRIG10_IN_CPUSS_DW0_TR_OUT2,
    DW0_TR_OUT3 = TRIG10_IN_CPUSS_DW0_TR_OUT3,
    DW0_TR_OUT4 = TRIG10_IN_CPUSS_DW0_TR_OUT4,
    DW0_TR_OUT5 = TRIG10_IN_CPUSS_DW0_TR_OUT5,
    DW0_TR_OUT6 = TRIG10_IN_CPUSS_DW0_TR_OUT6,
    DW0_TR_OUT7 = TRIG10_IN_CPUSS_DW0_TR_OUT7,
    DW0_TR_OUT8 = TRIG10_IN_CPUSS_DW0_TR_OUT8,
    DW0_TR_OUT9 = TRIG10_IN_CPUSS_DW0_TR_OUT9,
    DW0_TR_OUT10 = TRIG10_IN_CPUSS_DW0_TR_OUT10,
    DW0_TR_OUT11 = TRIG10_IN_CPUSS_DW0_TR_OUT11,
    DW0_TR_OUT12 = TRIG10_IN_CPUSS_DW0_TR_OUT12,
    DW0_TR_OUT13 = TRIG10_IN_CPUSS_DW0_TR_OUT13,
    DW0_TR_OUT14 = TRIG10_IN_CPUSS_DW0_TR_OUT14,
    DW0_TR_OUT15 = TRIG10_IN_CPUSS_DW0_TR_OUT15,
    DW1_TR_OUT0 = TRIG10_IN_CPUSS_DW1_TR_OUT0,
    DW1_TR_OUT1 = TRIG10_IN_CPUSS_DW1_TR_OUT1,
    DW1_TR_OUT2 = TRIG10_IN_CPUSS_DW1_TR_OUT2,
    DW1_TR_OUT3 = TRIG10_IN_CPUSS_DW1_TR_OUT3,
    DW1_TR_OUT4 = TRIG10_IN_CPUSS_DW1_TR_OUT4,
    DW1_TR_OUT5 = TRIG10_IN_CPUSS_DW1_TR_OUT5,
    DW1_TR_OUT6 = TRIG10_IN_CPUSS_DW1_TR_OUT6,
    DW1_TR_OUT7 = TRIG10_IN_CPUSS_DW1_TR_OUT7,
    DW1_TR_OUT8 = TRIG10_IN_CPUSS_DW1_TR_OUT8,
    DW1_TR_OUT9 = TRIG10_IN_CPUSS_DW1_TR_OUT9,
    DW1_TR_OUT10 = TRIG10_IN_CPUSS_DW1_TR_OUT10,
    DW1_TR_OUT11 = TRIG10_IN_CPUSS_DW1_TR_OUT11,
    DW1_TR_OUT12 = TRIG10_IN_CPUSS_DW1_TR_OUT12,
    DW1_TR_OUT13 = TRIG10_IN_CPUSS_DW1_TR_OUT13,
    DW1_TR_OUT14 = TRIG10_IN_CPUSS_DW1_TR_OUT14,
    DW1_TR_OUT15 = TRIG10_IN_CPUSS_DW1_TR_OUT15
    /* ~400 items, auto generated per chip, contains all of the inputs into the TriggerMuxes or 1:1 Triggers */
} cy_source_t;

typedef enum
{
    DW0_TR_IN0 = TRIG0_OUT_CPUSS_DW0_TR_IN0,
    DW0_TR_IN1 = TRIG0_OUT_CPUSS_DW0_TR_IN1,
    DW0_TR_IN2 = TRIG0_OUT_CPUSS_DW0_TR_IN2,
    DW0_TR_IN3 = TRIG0_OUT_CPUSS_DW0_TR_IN3,
    DW0_TR_IN4 = TRIG0_OUT_CPUSS_DW0_TR_IN4,
    DW0_TR_IN5 = TRIG0_OUT_CPUSS_DW0_TR_IN5,
    DW0_TR_IN6 = TRIG0_OUT_CPUSS_DW0_TR_IN6,
    DW0_TR_IN7 = TRIG0_OUT_CPUSS_DW0_TR_IN7,
    DW0_TR_IN8 = TRIG0_OUT_CPUSS_DW0_TR_IN8,
    DW0_TR_IN9 = TRIG0_OUT_CPUSS_DW0_TR_IN9,
    DW0_TR_IN10 = TRIG0_OUT_CPUSS_DW0_TR_IN10,
    DW0_TR_IN11 = TRIG0_OUT_CPUSS_DW0_TR_IN11,
    DW0_TR_IN12 = TRIG0_OUT_CPUSS_DW0_TR_IN12,
    DW0_TR_IN13 = TRIG0_OUT_CPUSS_DW0_TR_IN13,
    DW0_TR_IN14 = TRIG0_OUT_CPUSS_DW0_TR_IN14,
    DW0_TR_IN15 = TRIG0_OUT_CPUSS_DW0_TR_IN15,
    DW1_TR_IN0 = TRIG1_OUT_CPUSS_DW1_TR_IN0,
    DW1_TR_IN1 = TRIG1_OUT_CPUSS_DW1_TR_IN1,
    DW1_TR_IN2 = TRIG1_OUT_CPUSS_DW1_TR_IN2,
    DW1_TR_IN3 = TRIG1_OUT_CPUSS_DW1_TR_IN3,
    DW1_TR_IN4 = TRIG1_OUT_CPUSS_DW1_TR_IN4,
    DW1_TR_IN5 = TRIG1_OUT_CPUSS_DW1_TR_IN5,
    DW1_TR_IN6 = TRIG1_OUT_CPUSS_DW1_TR_IN6,
    DW1_TR_IN7 = TRIG1_OUT_CPUSS_DW1_TR_IN7,
    DW1_TR_IN8 = TRIG1_OUT_CPUSS_DW1_TR_IN8,
    DW1_TR_IN9 = TRIG1_OUT_CPUSS_DW1_TR_IN9,
    DW1_TR_IN10 = TRIG1_OUT_CPUSS_DW1_TR_IN10,
    DW1_TR_IN11 = TRIG1_OUT_CPUSS_DW1_TR_IN11,
    DW1_TR_IN12 = TRIG1_OUT_CPUSS_DW1_TR_IN12,
    DW1_TR_IN13 = TRIG1_OUT_CPUSS_DW1_TR_IN13,
    DW1_TR_IN14 = TRIG1_OUT_CPUSS_DW1_TR_IN14,
    DW1_TR_IN15 = TRIG1_OUT_CPUSS_DW1_TR_IN15
    /* ~100 items, auto generated per chip, contains all of the inputs into the TriggerMuxes or 1:1 Triggers */
} cy_destination_t;
#endif /* CY_IP_MXPERI_VERSION */

/** Connects two digital terminals. The source will be routed to the destination via the trigger routing
 * framework in the device. This supports both the trigger mux routing and the 1:1 triggers. As expected,
 * a single source can drive multiple destinations, but a destination can only be driven by a single source.
 * If the output is already connected, or the connection can not be established an error will be returned. 
 * @param[in] source The source of the signal to connect
 * @param[in] dest   The destination of the signal to connect
 * @return The status of the connect request
 */
//cy_rslt_t connect_trigger(cy_source_t src, cy_destination_t dst);
cy_rslt_t connect_trigger(uint32_t src, uint32_t dst);


/*************************************** IMPLEMENTATION FRAMEWORK FOR C FILE *****************************************/



/*#define MUX_COUNT
#define NOT_CONTINUATION 0xFF
#define ONE_TO_ONE_IDENT 0x80
#define IS_1TO1(t muxId) (ONE_TO_ONE_IDENT == (muxId & ONE_TO_ONE_IDENT))*/

/* Maps each cy_destination_t to a mux index                    ~100b */
//uint8_t[] m_dest_to_mux;
/* Maps each cy_destination_t to a specific output in its mux   ~100b */
//uint8_t[] m_mux_dest_index;
/* Number of sources for each mux                               ~30b */
//uint8_t[] m_source_count_per_mux;
/* Mux index to a table of cy_source_t                          ~2400b (2b * 15muxes * 80sources_per_mux) (could be 1/2 the size if there are less than 255 sources)*/
//cy_source_t[][] m_mux_to_sources;
/* Mapping from cy_source_t to cy_destination_t for intra mux connections     ~80b*/
//cy_dest_t[] m_intra_trigger_source;
/* Trigger type for each input                                  ~400b */
//cy_trigger_type_t m_trigger_type_source;
/* Trigger type for each output                                 ~100b */
//cy_trigger_type_t m_trigger_type_dest;

/*bool has_connection(uint8_t mux, uint8_t outputIdx)
{
}

cy_result_t connect_trigger(cy_source_t source, cy_destination_t dest)
{
    uint8_t muxId = m_dest_to_mux[dest];
    uint8_t destId = dest - m_mux_dest_index[dest];
    uint8_t sourceCount = m_source_count_per_mux[muxId];
    
    if (has_connection(muxIdx, destId))
    {
        return CYCSP_CONNECT_RSLT_ALREADY_CONNECTED;
    }
    
    for (uint8_t sourceId = 0; i < sourceCount; i++)
    {
        cy_source_t foundSource = m_mux_to_sources[muxId], sourceId];
        if (foundSource == source)
        {
            if (IS_1TO1(muxId) && sourceId != destId)
            {
                return CYCSP_CONNECT_RSLT_INVALID_1TO1_CONNECTION;
            }
            
            WRITE_REGISTER (muxIdx, sourceId, destId);
            return SUCCESS;
        }
        else
        {
            cy_dest_t intraDest = m_intra_trigger_source[foundSource];
            if (NOT_CONTINUATION != intraDest)
            {
                cy_result_t result = connect_trigger(source, intraDest);
                if (result == SUCCESS)
                {
                    WRITE_REGISTER (muxIdx, sourceId, destId);
                    return SUCCESS;
                }
            }
        }
    }
    return CYCSP_CONNECT_RSLT_NO_CONNECTION;
}*/

#if defined(__cplusplus)
}
#endif
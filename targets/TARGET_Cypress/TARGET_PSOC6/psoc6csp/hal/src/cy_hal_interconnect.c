/***************************************************************************//**
* \file cy_hal_interconnect.c
*
* \brief
* Provides a high level interface for interacting with the internal digital 
* routing on the chip. This is a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include "cy_hal_interconnect.h"
#include "cy_hal_hwmgr.h"
#include "cy_hal_gpio_impl.h"

/** Trigger type */
typedef enum 
{
    CY_TRIGGER_LEVEL, //!< Level triggered
    CY_TRIGGER_EDGE,  //!< Edge triggered
    CY_TRIGGER_EITHER,//!< Level or edge triggered
} cy_trigger_type_t;

/** Number of muxes */
#define MUX_COUNT
/** Indicates that a mux output does not continue to another mux */
#define NOT_CONTINUATION 0xFF
/** Mux identiifer the one-to-one triggers */
#define ONE_TO_ONE_IDENT 0x80
/** Determines whether a mux is one-to-one */
#define IS_1TO1(muxId) (ONE_TO_ONE_IDENT == (muxId & ONE_TO_ONE_IDENT))
#define WRITE_REGISTER(muxIdx, sourceId, destId) /* TODO */

/* Maps each cy_destination_t to a mux index                    ~100b */
extern uint8_t* cy_dest_to_mux;
/* Maps each cy_destination_t to a specific output in its mux    ~100b */
extern uint8_t* cy_mux_dest_index;
/* Number of sources for each mux                                ~30b */
extern uint8_t* cy_source_count_per_mux;
/* Mux index to a table of cy_source_t                             ~2400b (2b * 15muxes * 80sources_per_mux) (could be 1/2 the size if there are less than 255 sources) */
extern cy_source_t** cy_mux_to_sources;
/* Mapping from cy_source_t to cy_destination_t for intra mux connections     ~80b*/
extern cy_dest_t* cy_intra_trigger_source;
/* Trigger type for each input                                    ~400b */
cy_trigger_type_t cy_trigger_type_source;
/* Trigger type for each output                                    ~100b */
cy_trigger_type_t cy_trigger_type_dest;

bool cy_has_connection(uint8_t mux, uint8_t outputIdx)
{
    // TODO
    return false;
}

cy_rslt_t cy_connect_trigger(cy_source_t source, cy_dest_t dest)
{
    uint8_t muxIdx = cy_dest_to_mux[dest];
    uint8_t destId = dest - cy_mux_dest_index[dest];
    uint8_t sourceCount = cy_source_count_per_mux[muxIdx];
    
    if (cy_has_connection(muxIdx, destId))
    {
        return CYCSP_CONNECT_RSLT_ALREADY_CONNECTED;
    }
    
    for (uint8_t sourceId = 0; sourceId < sourceCount; sourceId++)
    {
        cy_source_t foundSource = cy_mux_to_sources[muxIdx][sourceId];
        if (foundSource == source)
        {
            if (IS_1TO1(muxIdx) && sourceId != destId)
            {
                return CYCSP_CONNECT_RSLT_INVALID_1TO1_CONNECTION;
            }
            
            WRITE_REGISTER (muxIdx, sourceId, destId);
            return CY_RSLT_SUCCESS;
        }
        else
        {
            cy_dest_t intraDest = cy_intra_trigger_source[foundSource];
            if (NOT_CONTINUATION != intraDest)
            {
                cy_rslt_t result = cy_connect_trigger(source, intraDest);
                if (result == CY_RSLT_SUCCESS)
                {
                    WRITE_REGISTER (muxIdx, sourceId, destId);
                    return result;
                }
            }
        }
    }
    return CYCSP_CONNECT_RSLT_NO_CONNECTION;
}

cy_rslt_t cy_connect_pin(const cy_resource_pin_mapping_t *pin_connection)
{
    cy_gpio_t pin = pin_connection->pin;
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CY_GET_PORT(pin));
    en_hsiom_sel_t hsiom = CY_GPIO_CFG_GET_HSIOM(pin_connection->cfg);
    uint8_t mode = CY_GPIO_CFG_GET_MODE(pin_connection->cfg);

    Cy_GPIO_Pin_FastInit(port, CY_GET_PIN(pin), mode, 1, hsiom);
    // Force output to enable pulls.
    switch (mode) {
        case CY_GPIO_DM_PULLUP:
            Cy_GPIO_Write(port, CY_GET_PIN(pin), 1);
            break;
        case CY_GPIO_DM_PULLDOWN:
            Cy_GPIO_Write(port, CY_GET_PIN(pin), 0);
            break;
        default:
            /* do nothing */
            break;
    }    
    
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_disconnect_pin(cy_gpio_t pin)
{    
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CY_GET_PORT(pin));
    Cy_GPIO_Pin_FastInit(port, CY_GET_PIN(pin), CY_GPIO_DM_HIGHZ, 1, HSIOM_SEL_GPIO);
    return CY_RSLT_SUCCESS;
}
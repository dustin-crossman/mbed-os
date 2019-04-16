/***************************************************************************//**
* \file cy_scb_types.c
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cy_device.h"
#include "cy_pdl.h"

/** The start address of the SCB blocks */
extern CySCB_Type* CY_SCB_BASE_ADDRESSES[CY_IP_MXSCB_INSTANCES];
/** The hardware buffer size of the SCB blocks */
extern uint16_t CY_SCB_BUFFER_SIZE[CY_IP_MXSCB_INSTANCES];
/** The interrupt number of the SCB blocks. */
extern IRQn_Type CY_SCB_IRQ_N[CY_IP_MXSCB_INSTANCES];
/***************************************************************************//**
* \file cy_capsense_tuner.h
* \version 1.0
*
* \brief
* This file provides Tuner FW module function prototypes.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_TUNER_H)
#define CY_CAPSENSE_TUNER_H

#include "cy_syslib.h"
#include "cy_capsense_structure.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************/
/** \addtogroup group_capsense_high_level *//** \{ */
/******************************************************************************/

uint32_t Cy_CapSense_RunTuner(cy_stc_capsense_context_t * context);

/** \} */

/******************************************************************************/
/** \cond SECTION_CAPSENSE_INTERNAL */
/** \addtogroup group_capsense_internal *//** \{ */
/******************************************************************************/

void Cy_CapSense_TuInitialize(cy_stc_capsense_context_t * context);

/** \} \endcond */

#if defined(__cplusplus)
}
#endif

#endif /* CY_CAPSENSE_TUNER_H */


/* [] END OF FILE */

/***************************************************************************//**
* \file cy_capsense_control.h
* \version 1.1
*
* \brief
* This file provides the function prototypes of the Control module.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#if !defined(CY_CAPSENSE_CONTROL_H)
#define CY_CAPSENSE_CONTROL_H

#include "cy_syspm.h"
#include "cy_capsense_structure.h"
#include "cy_capsense_common.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Local definition
*******************************************************************************/
#define CY_CAPSENSE_INIT_DONE                           (1u)
#define CY_CAPSENSE_INIT_NEEDED                         (0u)

#define CY_CAPSENSE_ANALOG_SETTLING_TIME_US             (25u)


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************/
/** \addtogroup group_capsense_high_level *//** \{ */
/******************************************************************************/

cy_status Cy_CapSense_Init(cy_stc_capsense_context_t * context);
cy_status Cy_CapSense_DeInit(cy_stc_capsense_context_t * context);

cy_status Cy_CapSense_Enable(cy_stc_capsense_context_t * context);

cy_status Cy_CapSense_Restore(cy_stc_capsense_context_t * context);
cy_status Cy_CapSense_Save(cy_stc_capsense_context_t * context);

cy_status Cy_CapSense_ProcessAllWidgets(cy_stc_capsense_context_t * context);
cy_status Cy_CapSense_ProcessWidget(
                uint32_t widgetId, 
                cy_stc_capsense_context_t * context);

void Cy_CapSense_Wakeup(const cy_stc_capsense_context_t * context);

void Cy_CapSense_IncrementGestureTimestamp(cy_stc_capsense_context_t * context);
void Cy_CapSense_SetGestureTimestamp(
                uint32_t value, 
                cy_stc_capsense_context_t * context);

cy_en_syspm_status_t Cy_CapSense_DeepSleepCallback(
                cy_stc_syspm_callback_params_t * callbackParams, 
                cy_en_syspm_callback_mode_t mode);

cy_status Cy_CapSense_RegisterCallback(
                cy_en_capsense_callback_event_t callbackType,
                cy_capsense_callback_t callbackFunction,
                cy_stc_capsense_context_t * context);
cy_status Cy_CapSense_UnRegisterCallback(
                cy_en_capsense_callback_event_t callbackType,
                cy_stc_capsense_context_t * context);


/** \} */

/******************************************************************************/
/** \addtogroup group_capsense_low_level *//** \{ */
/******************************************************************************/

cy_status Cy_CapSense_ProcessWidgetExt(
                uint32_t widgetId, 
                uint32_t mode, 
                cy_stc_capsense_context_t * context);
cy_status Cy_CapSense_ProcessSensorExt(
                uint32_t widgetId, 
                uint32_t sensorId, 
                uint32_t mode, 
                const cy_stc_capsense_context_t * context);
/** \} */

/******************************************************************************/
/** \cond SECTION_CAPSENSE_INTERNAL */
/** \addtogroup group_capsense_internal *//** \{ */
/******************************************************************************/

cy_status Cy_CapSense_Initialize(cy_stc_capsense_context_t * context);

/** \} \endcond */

#if defined(__cplusplus)
}
#endif

#endif /* CY_CAPSENSE_CONTROL_H */


/* [] END OF FILE */

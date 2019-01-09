/***************************************************************************//**
* \file cy_capsense_tuner.c
* \version 1.0
*
* \brief
* This file provides the source code for the Tuner module functions.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_syslib.h"
#include "cy_capsense_structure.h"
#include "cy_capsense_tuner.h"
#include "cy_capsense_control.h"
#include "cy_capsense_common.h"


/*******************************************************************************
* Function Name: Cy_CapSense_TuInitialize
****************************************************************************//**
*
* Initializes the communication interface with the Tuner tool.
*
*******************************************************************************/
void Cy_CapSense_TuInitialize(cy_stc_capsense_context_t * context)
{
    volatile cy_stc_capsense_common_context_t * ptrCommonCxt = context->ptrCommonContext;
    ptrCommonCxt->tunerCmd = (uint16_t)CY_CAPSENSE_TU_CMD_NONE_E;
    ptrCommonCxt->tunerSt = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
}

/*******************************************************************************
* Function Name: Cy_CapSense_RunTuner
****************************************************************************//**
*
* Establishes synchronized communication with the CapSense Tuner tool.
*
* This function is used to establish synchronized communication between the
* CapSense FW and Tuner tool (or other Host controllers).
* This function is called periodically in the application program loop
* to serve the CapSense Tuner tool (or Host controller) requests and commands.
* In most cases, the best place to call this function is after processing and
* before next scanning.
*
* If this function is absent from the application program, then communication is
* asynchronous and the following disadvantages are applicable:
* - The raw counts displayed in the tuner may be filtered and/or unfiltered.
* As a result, noise and SNR measurements will not be accurate.
* - The Tuner tool may read the sensor data such as raw counts from a scan
* multiple times. As a result, noise and SNR measurement will not be
* accurate.
* - The Tuner tool and Host controller should not change the parameters
* via the tuner interface. Changing the parameters via the tuner
* interface in the async mode will result in abnormal behavior.
* - Displaying detected gestures may be missed.
*
* Note that calling this function is not mandatory for the application, but
* required only to synchronize the communication with the Host controller or
* the CapSense Tuner tool.
*
* \param context
* The pointer to the CapSense context structure \ref cy_stc_capsense_context_t.
*
* \return
* In some cases, the application program may need to know whether the CapSense was
* re-initialized. The return indicates whether a restart command was executed:
* - CY_CAPSENSE_STATUS_RESTART_DONE - Based on a received command, the
* CapSense was restarted.
* - CY_CAPSENSE_STATUS_RESTART_NONE - No restart was executed by this
* function.
*
*******************************************************************************/
uint32_t Cy_CapSense_RunTuner(cy_stc_capsense_context_t * context)
{
    uint32_t interruptState;
    uint32_t updateFlag = 0uL;
    uint32_t tunerStatus = CY_CAPSENSE_STATUS_RESTART_NONE;
    uint16_t tunerCommand;
    volatile cy_stc_capsense_common_context_t * ptrCommonCxt = context->ptrCommonContext;
    uint8_t tunerState = ptrCommonCxt->tunerSt;

    do
    {
        /* 
        * ONE_SCAN command could be interpreted as two commands:
        * RESUME till next call of this function and then
        * SUSPEND till next command receiving.
        * So, after one scan cycle tuner state is changed to suspend.
        */
        if ((uint8_t)CY_CAPSENSE_TU_FSM_ONE_SCAN == tunerState)
        {
            interruptState = Cy_SysLib_EnterCriticalSection();
            context->ptrCommonContext->tunerCmd = (uint16_t)CY_CAPSENSE_TU_CMD_SUSPEND_E;
            Cy_SysLib_ExitCriticalSection(interruptState);
        }

        /* Read command */
        tunerCommand = context->ptrCommonContext->tunerCmd;
        
        /* Check command register */
        switch (tunerCommand)
        {
        case (uint16_t)CY_CAPSENSE_TU_CMD_SUSPEND_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_SUSPENDED;
            updateFlag = 1uL;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_RESUME_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1uL;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_RESTART_E:
            (void)Cy_CapSense_Enable(context);
            tunerStatus = CY_CAPSENSE_STATUS_RESTART_DONE;
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1uL;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_PING_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1uL;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_ONE_SCAN_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_ONE_SCAN;
            updateFlag = 0uL;
            break;

        default:
            break;
        }
        
        ptrCommonCxt->tunerSt = tunerState;
        
        /* Set Complete flag in command register if needed */
        if (0uL != updateFlag)
        {
            interruptState = Cy_SysLib_EnterCriticalSection();
            /* Check that command wasn't overwritten with new command */
            if (tunerCommand == ptrCommonCxt->tunerCmd)
            {
                ptrCommonCxt->tunerCmd |= CY_CAPSENSE_TU_CMD_COMPLETE_BIT;
            }
            Cy_SysLib_ExitCriticalSection(interruptState);
            updateFlag = 0uL;
        }
    } while ((uint8_t)CY_CAPSENSE_TU_FSM_SUSPENDED == tunerState);

    return tunerStatus;
}


/* [] END OF FILE */

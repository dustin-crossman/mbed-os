/***************************************************************************//**
* \file cy_capsense_ballistic_multiplier_lib.c
* \version 1.0
*
* \brief
*  This file provides the API specific for Ballistic Multiplier implementation.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_capsense_lib.h"

/* Integer math multiplier */
#define CY_CAPSENSE_INTEGER_MATH_MULT                   (8u)

/*******************************************************************************
* Function Name: Cy_CapSense_BallisticMultiplier_Lib
********************************************************************************
*
* Summary:
*  This API is to run the ballistic multiplier.
*
* Parameters:
*  BALLISTIC_MULT_P4 *ballisticMultiplier: pointer to the structure
*   which contains API parameters;
*   uint16_t *pos - pointer to dX and dY coordinates
*
* Return:
*  None
*
* Global Variables:
*  None.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Cy_CapSense_BallisticMultiplier_Lib(
                const cy_stc_capsense_ballistic_config_t * config, 
                cy_stc_capsense_touch_t * touch,
                cy_stc_capsense_ballistic_delta_t * displacement,
                uint32_t timestamp,
                cy_stc_capsense_ballistic_context_t * context)
{
    uint32_t delta = 0u;
    uint32_t dT;
    uint32_t signFlag;
    uint32_t speedThreshold;
    uint32_t deltaSum;
    
    /* Process timestamp */
    dT = timestamp - context->oldTimestamp;
    context->oldTimestamp = timestamp;
    
    displacement->deltaX = 0;
    displacement->deltaY = 0;
    
    if (touch->numPosition == 1u)
    {
        /* There is a single touch that need to be filtered */
        if (context->oldTouchNumber == 1u)
        {
            /* X Axis */
            if (touch->ptrPosition->x < context->x)
            {
                signFlag = 1u;
                delta = context->x - touch->ptrPosition->x;
            }
            else
            {
                signFlag = 0u;
                delta = touch->ptrPosition->x - context->x;
            }
            /* Apply acceleration multiplier */
            speedThreshold = config->speedThresholdX * dT;
            if (delta >= speedThreshold)
            {
                delta = (delta - speedThreshold) * config->accelCoeff + speedThreshold;
            }
            /* Process speed configuration */
            deltaSum = config->speedCoeff * (delta << CY_CAPSENSE_INTEGER_MATH_MULT) + context->deltaXfrac;
            delta = deltaSum / config->divisorValue;
            context->deltaXfrac = deltaSum - (delta * config->divisorValue);
            displacement->deltaX = (int16_t)(delta >> CY_CAPSENSE_INTEGER_MATH_MULT);
            /* Put the sign back and prepare return value */
            if(signFlag == 1u)
            {
                displacement->deltaX *= -1;
            }

            /* Y Axis */
            if (touch->ptrPosition->y < context->y)
            {
                signFlag = 1u;
                delta = context->y - touch->ptrPosition->y;
            }
            else
            {
                signFlag = 0u;
                delta = touch->ptrPosition->y - context->y;
            }
            /* Apply acceleration multiplier */
            speedThreshold = config->speedThresholdY * dT;
            if (delta >= speedThreshold)
            {
                delta = (delta - speedThreshold) * config->accelCoeff + speedThreshold;
            }
            /* Process speed configuration */
            deltaSum = config->speedCoeff * (delta << CY_CAPSENSE_INTEGER_MATH_MULT) + context->deltaYfrac;
            delta = deltaSum / config->divisorValue;
            context->deltaYfrac = deltaSum - (delta * config->divisorValue);
            displacement->deltaY = (int16_t)(delta >> CY_CAPSENSE_INTEGER_MATH_MULT);
            /* Put the sign back and prepare return value */
            if(signFlag == 1u)
            {
                displacement->deltaY *= -1;
            }
        }
        else
        {
            /* Reset internal history */
            context->deltaXfrac = 0;
            context->deltaYfrac = 0;
        }
        context->x = touch->ptrPosition->x;
        context->y = touch->ptrPosition->y;
    }
    context->oldTouchNumber = touch->numPosition;
}


/* [] END OF FILE */

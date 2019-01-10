/***************************************************************************//**
* \file cy_capsense_adaptive_filter_lib.c
* \version 1.1
*
* \brief
*  This file provides source code of the Adaptive Filter APIs.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_capsense_lib.h"

/*******************************************************************************
* Local definition
*******************************************************************************/
#define CY_CAPSENSE_ADAPTIVE_FILTER_COEF_INC_STEP               (2u)
#define CY_CAPSENSE_ADAPTIVE_FILTER_COEF_DEC_STEP               (1u)

/*******************************************************************************
* Function prototype
*******************************************************************************/
static uint16_t Cy_CapSense_AdaptiveFilterRunIIR_Lib(const uint32_t curPos, const uint32_t prevPos, const uint32_t k, const uint32_t divisor);
static uint16_t Cy_CapSense_AdaptiveFilterAbs_Lib(int16_t arg);

/*******************************************************************************
* Function Name: Cy_CapSense_AdaptiveFilterAbs_Lib
****************************************************************************//**
*
* \brief
*  Returns absolute value of an argument.
*
* \details
*  Returns absolute value of an argument.
*
* \param arg
*  Specify a signed value that need to be converted to an absolute value.
*
* \param context
*  Returns absolute value of an argument.
*
*******************************************************************************/
static uint16_t Cy_CapSense_AdaptiveFilterAbs_Lib(int16_t arg)
{
    return (uint16_t)((arg < 0) ? -arg : arg);
}

/*******************************************************************************
* Function Name: Cy_CapSense_AdaptiveFilterInitialize_Lib
****************************************************************************//**
*
* \brief
*  Initializes the Adaptive Filter context structure.
*
* \details
*  Initializes the Adaptive Filter context structure by writing default
*  adaptive coefficient for the AIIR filter.
*
* \param config
*  The pointer to the configuration structure of the Adaptive Filter.
*
* \param context
*  The pointer to the context structure of the Adaptive Filter.
*
*******************************************************************************/
void Cy_CapSense_AdaptiveFilterInitialize_Lib(const cy_stc_capsense_adaptive_filter_config_t * config, cy_stc_capsense_position_t * context)
{
    context->z = config->minK;
}


/*******************************************************************************
* Function Name: Cy_CapSense_AdaptiveFilterRun_Lib
****************************************************************************//**
*
* \brief
*  This function runs the Adaptive Filter algorithm for the centroid position.
*
* \details
*  This function runs the Adaptive Filter algorithm for the centroid position.
*  Equation: result = ((k * curPos) + ((divisor-k) * prevPos)) / divisor
*  where k is adaptive coefficient.
*
*  The function supposes that the filter history is updated at first touch
*  outside of the library. I.e. at the first touchdown the filter history has
*  be initialized by touch positions before calling this function.
*
* \param config
*  The pointer to the configuration structure of the Adaptive Filter.
*
* \param context
*  The pointer to the context structure of the Adaptive Filter.
*
*******************************************************************************/
void Cy_CapSense_AdaptiveFilterRun_Lib(const cy_stc_capsense_adaptive_filter_config_t *config,
                        cy_stc_capsense_position_t *context, uint32_t *currentX, uint32_t *currentY)
{
    int16_t movementX;
    int16_t movementY;
    uint16_t movementXY;
    uint16_t currentCoeff;

    currentCoeff = context->z;
    movementX = (int16_t)context->x - (int16_t)(*currentX);
    movementY = (int16_t)context->y - (int16_t)(*currentY);

    /* Calculation of XY displacement */
    movementXY = Cy_CapSense_AdaptiveFilterAbs_Lib(movementX) + Cy_CapSense_AdaptiveFilterAbs_Lib(movementY);

    /* Adjustment of current coefficient */
    if(movementXY < config->noMovTh)
    {
        currentCoeff = config->minK;
    }
    else if(movementXY < config->littleMovTh)
    {
        currentCoeff -= CY_CAPSENSE_ADAPTIVE_FILTER_COEF_DEC_STEP;
    }
    else if(movementXY > config->largeMovTh)
    {
        currentCoeff += CY_CAPSENSE_ADAPTIVE_FILTER_COEF_INC_STEP;
    }
    else
    {
        /* Does nothing. */
    }

    /* Limit coefficient */
    if (currentCoeff < config->minK)
    {
        currentCoeff = config->minK;
    }
    if (currentCoeff > config->maxK)
    {
        currentCoeff = config->maxK;
    }

    /* Store coefficient to the context structure for the next cycle */
    context->z = currentCoeff;

    /* Runs filtering with the adjusted coefficient */
    context->x = Cy_CapSense_AdaptiveFilterRunIIR_Lib((uint32_t)(*currentX),
                                              (uint32_t)context->x,
                                              (uint32_t)currentCoeff,
                                              (uint32_t)config->divVal);
    context->y = Cy_CapSense_AdaptiveFilterRunIIR_Lib((uint32_t)(*currentY),
                                              (uint32_t)context->y,
                                              (uint32_t)currentCoeff,
                                              (uint32_t)config->divVal);
    (*currentX) = context->x;
    (*currentY) = context->y;
}


/*******************************************************************************
* Function Name: Cy_CapSense_AdaptiveFilterRunIIR_Lib
****************************************************************************//**
*
* \brief
*  This function runs the IIR Filter algorithm for a single position.
*
* \details
*  This function runs the IIR Filter algorithm for a single position.
*  Equation: result = round(((k * curPos) + ((divisor - k) * prevPos)) / divisor)
*  Divisor must be higher than k.
*
* \param curPos
*  Current position.
*
* \param prevPos
*  Previous position.
*
* \param k
*  IIR coefficient.
*
* \param divisor
*  The denominator of the IIR filter.
*
* \return
*  Returns the filtered position.
*
*******************************************************************************/
static uint16_t Cy_CapSense_AdaptiveFilterRunIIR_Lib(const uint32_t curPos, const uint32_t prevPos, const uint32_t k, const uint32_t divisor)
{
    uint32_t filtredPos;

    filtredPos = (((k * curPos) + ((divisor - k) * prevPos)) << 1Lu) + divisor;
    filtredPos /= (divisor << 1Lu);

    return((uint16_t)filtredPos);
}


/* [] END OF FILE */

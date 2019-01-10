/***************************************************************************//**
* \file cy_capsense_alp_filter_lib.c
* \version 1.1
*
* \brief
*  This file provides implementation for the ALP Filter.
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
/* IIR output scale factor. Prevents roundoff error */
#define CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR             (7u)
#define CY_CAPSENSE_ALP_IIR_DENOMINATOR                     (256u)
#define CY_CAPSENSE_ALP_IIR_SHIFT                           (8u)

/* ALP Filter States */
#define CY_CAPSENSE_ALP_STATE_IDLE                          (1u)
#define CY_CAPSENSE_ALP_STATE_POS_EDGE_TRACK                (2u)
#define CY_CAPSENSE_ALP_STATE_SIGNAL_DETECTED               (3u)
#define CY_CAPSENSE_ALP_STATE_NEG_EDGE_TRACK                (4u)

/* Re-definition of structure records for better readability */
#define ALP_iirHistory                          dataParam0
#define ALP_medHistory0                         dataParam1
#define ALP_medHistory1                         dataParam2
#define ALP_avgHistory0                         dataParam3
#define ALP_avgHistory1                         dataParam4
#define ALP_avgHistory2                         dataParam5
#define ALP_average                             dataParam6
#define ALP_state                               dataParam7

#define ALP_onThreshold                         configParam0
#define ALP_offThreshold                        configParam1
#define ALP_fingerTh                            configParam2
#define ALP_noiseTh                             configParam3
#define ALP_hysteresis                          configParam4
#define ALP_coeff                               configParam5


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
static uint32_t Cy_CapSense_AlpFtMedian_Lib(uint32_t x1, uint32_t x2, uint32_t x3);
static uint32_t Cy_CapSense_AlpFtIIR1stOrder_Lib(uint32_t input, uint32_t prevOutput, uint32_t n);


/*******************************************************************************
* Function Name: Cy_CapSense_AlpFtMedian_Lib
****************************************************************************//**
*
* \brief
*  Returns the median value from the three passed arguments.
*
* \details
*  Returns the median value from the three passed arguments.
*
* \param x1
*  The first value to be compared.
*
* \param x2
*  The second value to be compared.
*
* \param x3
*  The third value to be compared.
*
* \return
*  Returns the median value of input arguments.
*
*******************************************************************************/
static uint32_t Cy_CapSense_AlpFtMedian_Lib(uint32_t x1, uint32_t x2, uint32_t x3)
{
    uint32_t tmp;

    if (x1 > x2)
    {
        tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if (x2 > x3)
    {
        x2 = x3;
    }
    return ((x1 > x2) ? x1 : x2);
}

/*******************************************************************************
* Function Name: Cy_CapSense_AlpFtIIR1stOrder_Lib
****************************************************************************//**
*
* \brief
*  Implements the 1st order IIR filter.
*
* \details
*  Implements the 1st order IIR filter. Returns the IIR filtered value scaled
*  by factor (1 << CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR).
*
* \param input
*  The input value.
*
* \param prevOutput
*  The previous output of the filtering.
*
* \param n
*  The filter power.
*
* \return
*  Returns the IIR filtered value scaled by factor (1 << CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR).
*
*******************************************************************************/
static uint32_t Cy_CapSense_AlpFtIIR1stOrder_Lib(uint32_t input, uint32_t prevOutput, uint32_t n)
{
    uint32_t output;

    output = ((n * (input << CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR)) + ((CY_CAPSENSE_ALP_IIR_DENOMINATOR - n) * prevOutput)) >> CY_CAPSENSE_ALP_IIR_SHIFT;

    return output;
}

/*******************************************************************************
* Function Name: Cy_CapSense_AlpRun_Lib
****************************************************************************//**
*
* \brief
*  Applies advanced low pass filter algorithm on raw counts.
*
* \details
*  Applies advanced low pass filter algorithm on raw counts.
*
* \param ptrFilterObj
*  The pointer to sensor filter data structure
*
* \param ptrFilterConfig
*  The pointer to sensor filter configuration structure
*
* \param rawCount
*  The pointer to sensor raw count
*
* \param baseline
*  The pointer to sensor baseline
*
*******************************************************************************/
void Cy_CapSense_AlpRun_Lib(cy_stc_capsense_alp_fltr_channel_t *ptrFilterObj, const cy_stc_capsense_alp_fltr_config_t *ptrFilterConfig, uint16_t *rawCount, const uint16_t *baseline)
{
    uint32_t median;
    uint32_t average;
    uint32_t difference = 0u;

    /* Apply median filter */
    median = Cy_CapSense_AlpFtMedian_Lib((uint32_t)*rawCount, (uint32_t)ptrFilterObj->ALP_medHistory0, (uint32_t)ptrFilterObj->ALP_medHistory1);
    ptrFilterObj->ALP_medHistory1 = ptrFilterObj->ALP_medHistory0;
    ptrFilterObj->ALP_medHistory0 = *rawCount;
    *rawCount = (uint16_t)median;

    /* Apply average filter */
    average = *rawCount;
    average += ptrFilterObj->ALP_avgHistory0;
    average += ptrFilterObj->ALP_avgHistory1;
    average += ptrFilterObj->ALP_avgHistory2;
    average >>= 2u;
    ptrFilterObj->ALP_avgHistory2 = ptrFilterObj->ALP_avgHistory1;
    ptrFilterObj->ALP_avgHistory1 = ptrFilterObj->ALP_avgHistory0;
    ptrFilterObj->ALP_avgHistory0 = *rawCount;
    ptrFilterObj->ALP_average = (uint16_t)average;

    if (average > (uint32_t)(*baseline))
    {
        difference = average - (uint32_t)(*baseline);
    }

    /* Aplly adaptive IIR filter */
    switch(ptrFilterObj->ALP_state)
    {
    case CY_CAPSENSE_ALP_STATE_IDLE:
        ptrFilterObj->ALP_iirHistory = Cy_CapSense_AlpFtIIR1stOrder_Lib((uint32_t)*rawCount, ptrFilterObj->ALP_iirHistory, (uint32_t)ptrFilterConfig->ALP_coeff);
        /* Find next state */
        if(difference > ptrFilterConfig->ALP_onThreshold)
        {
            ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_POS_EDGE_TRACK;
        }
        break;

    case CY_CAPSENSE_ALP_STATE_POS_EDGE_TRACK:
        /* If difference is still greater than prox positive threshold, use tracking state IIR filter */
        if(difference > ptrFilterConfig->ALP_onThreshold)
        {
            ptrFilterObj->ALP_iirHistory = Cy_CapSense_AlpFtIIR1stOrder_Lib((uint32_t)*rawCount, ptrFilterObj->ALP_iirHistory, 128u);

            /* Compute difference to check if it has crossed FT+HY */
            difference = (uint32_t)(ptrFilterObj->ALP_iirHistory >> CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR) - (uint32_t)(*baseline);

            /* If difference has exceeded FT+HY, switch to signal detected state */
            if(difference >= ((uint32_t)ptrFilterConfig->ALP_fingerTh + (uint32_t)ptrFilterConfig->ALP_hysteresis))
            {
                ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_SIGNAL_DETECTED;
            }
        }
        else
        {
            ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_IDLE;
        }
        break;

    case CY_CAPSENSE_ALP_STATE_SIGNAL_DETECTED:
        ptrFilterObj->ALP_iirHistory = Cy_CapSense_AlpFtIIR1stOrder_Lib((uint32_t)*rawCount, ptrFilterObj->ALP_iirHistory, (uint32_t)ptrFilterConfig->ALP_coeff);

        /* Find next state */
        if(difference < ptrFilterConfig->ALP_offThreshold)
        {
            ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_NEG_EDGE_TRACK;
        }
        break;

    case CY_CAPSENSE_ALP_STATE_NEG_EDGE_TRACK:
        /* Find next state */
        if(difference < ptrFilterConfig->ALP_offThreshold)
        {
            if((ptrFilterObj->ALP_iirHistory >> CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR) > *rawCount)
            {
                ptrFilterObj->ALP_iirHistory = Cy_CapSense_AlpFtIIR1stOrder_Lib((uint32_t)*rawCount, ptrFilterObj->ALP_iirHistory, 128u);
            }
            else
            {
                ptrFilterObj->ALP_iirHistory = Cy_CapSense_AlpFtIIR1stOrder_Lib((uint32_t)*rawCount, ptrFilterObj->ALP_iirHistory, (uint32_t)ptrFilterConfig->ALP_coeff);
            }

            difference = (ptrFilterObj->ALP_iirHistory >> CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR) - *baseline;

            if(difference <= ptrFilterConfig->ALP_noiseTh)
            {
                ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_IDLE;
            }
        }
        else
        {
            ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_SIGNAL_DETECTED;
        }
        break;

    default:
        ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_IDLE;
        break;

    }

    /* Update output in rawCounts array */
    *rawCount = (uint16_t)(ptrFilterObj->ALP_iirHistory >> CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR);
}

/*******************************************************************************
* Function Name: Cy_CapSense_AlpInitialize_Lib
****************************************************************************//**
*
* \brief
*  Initializes filter data structure.
*
* \details
*  Initializes filter data structure.
*
* \param ptrFilterObj
*  Pointer to filter data structure
*
* \param rawCount
*  Pointer to sensor raw count
*
*******************************************************************************/
void Cy_CapSense_AlpInitialize_Lib(cy_stc_capsense_alp_fltr_channel_t *ptrFilterObj, const uint16_t *rawCount)
{
    uint16_t temp = *rawCount;

    ptrFilterObj->ALP_iirHistory = ((uint32_t)temp) << CY_CAPSENSE_ALP_IIR_FILTER_SCALE_FACTOR;
    ptrFilterObj->ALP_medHistory0 = temp;
    ptrFilterObj->ALP_medHistory1 = temp;
    ptrFilterObj->ALP_avgHistory0 = temp;
    ptrFilterObj->ALP_avgHistory1 = temp;
    ptrFilterObj->ALP_avgHistory2 = temp;
    ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_IDLE;
}

/*******************************************************************************
* Function Name: Cy_CapSense_AlpResetState_Lib
****************************************************************************//**
*
* \brief
*  Resets state machine of the filter.
*
* \details
*  Resets state machine of the filter.
*
* \param ptrFilterObj
*  Pointer to the filter data structure
*
*******************************************************************************/
void Cy_CapSense_AlpResetState_Lib(cy_stc_capsense_alp_fltr_channel_t *ptrFilterObj)
{
    ptrFilterObj->ALP_state = CY_CAPSENSE_ALP_STATE_IDLE;
}

/*******************************************************************************
* Function Name: Cy_CapSense_AlpGetAverage_Lib
****************************************************************************//**
*
* \brief
*  Returns the output of internal average filter
*
* \details
*  Returns the output of internal average filter
*
* \Return
*  Returns the output of internal average filter
*
*******************************************************************************/
uint32_t Cy_CapSense_AlpGetAverage_Lib(const cy_stc_capsense_alp_fltr_channel_t *ptrFilterObj)
{
    return (uint32_t)ptrFilterObj->ALP_average;
}


/* [] END OF FILE */

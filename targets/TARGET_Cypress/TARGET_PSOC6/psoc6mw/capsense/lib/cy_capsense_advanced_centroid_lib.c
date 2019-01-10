/***************************************************************************//**
* \file cy_capsense_advanced_centroid_lib.c
* \version 1.1
*
* \brief
*  This file provides source code of the Advanced Centroid APIs.
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

/* General macros */
#define CY_CAPSENSE_ADV_X_AXIS                                      (0u)
#define CY_CAPSENSE_ADV_Y_AXIS                                      (2u)
#define CY_CAPSENSE_ADV_AXES_NUMBER                                 (2u)
#define CY_CAPSENSE_ADV_CENTROID_SIZE                               (5u)
#define CY_CAPSENSE_ADV_VIRTUAL_SENSOR_MULTIPLIER                   (2u)
/* Local maximum macros */
#define CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER                            (2u)
#define CY_CAPSENSE_ADV_FIRST_MAX                                   (0u)
#define CY_CAPSENSE_ADV_SECOND_MAX                                  (1u)
#define CY_CAPSENSE_ADV_TOTAL_MAX                                   (4u)
/* Local maximum location in struct */
#define CY_CAPSENSE_ADV_POS_X0                                      (CY_CAPSENSE_ADV_X_AXIS + CY_CAPSENSE_ADV_FIRST_MAX)
#define CY_CAPSENSE_ADV_POS_X1                                      (CY_CAPSENSE_ADV_X_AXIS + CY_CAPSENSE_ADV_SECOND_MAX)
#define CY_CAPSENSE_ADV_POS_Y0                                      (CY_CAPSENSE_ADV_Y_AXIS + CY_CAPSENSE_ADV_FIRST_MAX)
#define CY_CAPSENSE_ADV_POS_Y1                                      (CY_CAPSENSE_ADV_Y_AXIS + CY_CAPSENSE_ADV_SECOND_MAX)
/* Advanced centroid indexes */
#define CY_CAPSENSE_ADV_LEFT_2                                      (0u)
#define CY_CAPSENSE_ADV_LEFT_1                                      (1u)
#define CY_CAPSENSE_ADV_CENTER                                      (2u)
#define CY_CAPSENSE_ADV_RIGHT_1                                     (3u)
#define CY_CAPSENSE_ADV_RIGHT_2                                     (4u)
/* Math constants */
#define CY_CAPSENSE_ADV_MULTIPLIER                                  (256)
#define CY_CAPSENSE_ADV_MULTIPLIER_ROUNDING                         (127)
#define CY_CAPSENSE_ADV_MULTIPLIER_HALF                             (128)
#define CY_CAPSENSE_ADV_UINT16_T_MAXIMUM                            (65535u)

/*******************************************************************************
* Structures
*******************************************************************************/
typedef struct
{
    uint32_t snsCount;                            /* Total number of sensors in axis */
    uint32_t resolution;                          /* Axis maximum position */
    uint32_t localMaxIndex;                       /* Sensor index of found maximum */
    uint32_t localMaxSignal[CY_CAPSENSE_ADV_CENTROID_SIZE];   /* Sensor signals around maximum */
} cy_stc_capsense_adv_local_max_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
static uint32_t Cy_CapSense_AdvFindDoubleLocalMax_Lib(
                const cy_stc_capsense_advanced_centroid_config_t * config,
                cy_stc_capsense_adv_local_max_t localMax[],
                const uint16 * ptrSns);
static void Cy_CapSense_AdvEdgeCorrection_Lib(
                const cy_stc_capsense_advanced_centroid_config_t * config, 
                cy_stc_capsense_adv_local_max_t *localMax);
static void Cy_CapSense_AdvExecuteAlgorithm_Lib(
                const cy_stc_capsense_advanced_centroid_config_t * config,
                cy_stc_capsense_adv_local_max_t * localMax, 
                uint16_t * pos, 
                uint16_t * posZ);

/*******************************************************************************
* Function Name: Cy_CapSense_AdvancedCentroidGetTouchCoordinates_Lib
****************************************************************************//**
*
* \brief
*  This API calculates the centroids for two fingers.
*
* \details
*  This API calculates the centroids for up to two fingers.
*
* \param config
*  The pointer to the configuration structure of the Advanced Centroid.
*
* \param context
*  The pointer to the context structure of the Advanced Centroid.
*
* \return
*  Returns the number of fingers on the touchpad.
*
*******************************************************************************/
void Cy_CapSense_AdvancedCentroidGetTouchCoordinates_Lib(
                const cy_stc_capsense_advanced_centroid_config_t * config,
                const uint16_t * ptrSns,
                cy_stc_capsense_touch_t * touch)
{
    uint32_t status;
    uint32_t fingerCountX;
    uint32_t fingerCountY;
    uint32_t maxFingers;
    cy_stc_capsense_adv_local_max_t localMax[CY_CAPSENSE_ADV_TOTAL_MAX];

    maxFingers = config->twoFingersEn + 1u;

    /* Initialization */
    localMax[CY_CAPSENSE_ADV_POS_X0].snsCount = config->snsCountX;
    localMax[CY_CAPSENSE_ADV_POS_X1].snsCount = config->snsCountX;
    localMax[CY_CAPSENSE_ADV_POS_Y0].snsCount = config->snsCountY;
    localMax[CY_CAPSENSE_ADV_POS_Y1].snsCount = config->snsCountY;
    localMax[CY_CAPSENSE_ADV_POS_X0].resolution = config->resolutionX;
    localMax[CY_CAPSENSE_ADV_POS_X1].resolution = config->resolutionX;
    localMax[CY_CAPSENSE_ADV_POS_Y0].resolution = config->resolutionY;
    localMax[CY_CAPSENSE_ADV_POS_Y1].resolution = config->resolutionY;

    /* Finds maximums */
    fingerCountX = Cy_CapSense_AdvFindDoubleLocalMax_Lib(config, &localMax[CY_CAPSENSE_ADV_X_AXIS], ptrSns);
    fingerCountY = Cy_CapSense_AdvFindDoubleLocalMax_Lib(config, &localMax[CY_CAPSENSE_ADV_Y_AXIS], &(ptrSns[config->snsCountX]));

    /* Check for found local maximum numbers */
    if((fingerCountX == 0u) || (fingerCountY == 0u))
    {
        status = CY_CAPSENSE_ADVANCED_CENTROID_NO_TOUCHES;
    }
    else
    {
        if((fingerCountX > maxFingers) || (fingerCountY > maxFingers))
        {
            status = CY_CAPSENSE_ADVANCED_CENTROID_POSITION_ERROR;
        }
        else
        {
            /* Find centroids */
            Cy_CapSense_AdvExecuteAlgorithm_Lib(config, &localMax[CY_CAPSENSE_ADV_POS_X0],
                                &(touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].x),
                                &(touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].z));
            Cy_CapSense_AdvExecuteAlgorithm_Lib(config, &localMax[CY_CAPSENSE_ADV_POS_Y0],
                                &(touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].y),
                                &(touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].id));
            if (maxFingers == CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
            {
                if (fingerCountX == CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
                {
                    Cy_CapSense_AdvExecuteAlgorithm_Lib(config, &localMax[CY_CAPSENSE_ADV_POS_X1],
                                        &(touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].x),
                                        &(touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].z));
                }
                if (fingerCountY == CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
                {
                    Cy_CapSense_AdvExecuteAlgorithm_Lib(config, &localMax[CY_CAPSENSE_ADV_POS_Y1],
                                        &(touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].y),
                                        &(touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].id));
                }
            }

            status = fingerCountX;

            if (maxFingers == CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
            {
                if (fingerCountX < fingerCountY)
                {
                    touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].x = touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].x;
                    touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].z = touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].z;
                    status = fingerCountY;
                }
                else
                {
                    if (fingerCountX > fingerCountY)
                    {
                        touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].y = touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].y;
                        touch->ptrPosition[CY_CAPSENSE_ADV_SECOND_MAX].id = touch->ptrPosition[CY_CAPSENSE_ADV_FIRST_MAX].id;
                    }
                }
            }
        }
    }
    touch->numPosition = (uint8_t)status;
}


/*******************************************************************************
* Function Name: Cy_CapSense_AdvExecuteAlgorithm_Lib
****************************************************************************//**
*
* \brief
*  This function calculates Centroid value using Advanced Centroid algorithm.
*
* \details
*  This function calculates Centroid value using Advanced Centroid algorithm.
*
* \param config
*  The pointer to the configuration structure of the Advanced Centroid.
*
* \param localMaximum
*  The pointer to the local maximum structure
*
* \param pos
*  The pointer where found position is stored
*
* \param posZ
*  The pointer where Z-value of the found position is stored
*
*******************************************************************************/
static void Cy_CapSense_AdvExecuteAlgorithm_Lib(const cy_stc_capsense_advanced_centroid_config_t *config,
                                cy_stc_capsense_adv_local_max_t *localMax, uint16_t *pos, uint16_t *posZ)
{
    uint32_t loopIndex;
    int32_t denominator = 0;
    int32_t centroid = 0;
    int32_t numerator = 0;
    int32_t snsSignal;

    /* Perform edge correction if enabled */
    if (0u != config->edgeCorrectionEn)
    {
        Cy_CapSense_AdvEdgeCorrection_Lib(config, localMax);
    }

    for (loopIndex = 0u; loopIndex < CY_CAPSENSE_ADV_CENTROID_SIZE; loopIndex++)
    {
        snsSignal = (int32_t)localMax->localMaxSignal[loopIndex] - (int32_t)config->crossCouplingTh;
        if(snsSignal < 0)
        {
            snsSignal = 0;
        }
        /* Calculates denominator: denominator += S(i); */
        denominator += snsSignal;
        /* Calculates numerator: numerator += i*S(i); */
        numerator += (snsSignal * (int32_t)loopIndex);
    }
    /* Numerator alignment with maximum position: ADVANCED_CENTROID_CENTER */
    numerator -= (denominator * (int32_t)CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER);

    /* Centroid calculation with x256 */
    if (denominator != 0)
    {
        centroid = (numerator * CY_CAPSENSE_ADV_MULTIPLIER) / denominator;
    }
    /* Centroid position scaling to whole touchpad */
    centroid += (int32_t)localMax->localMaxIndex * CY_CAPSENSE_ADV_MULTIPLIER;
    /* Correction for half of a sensor */
    centroid += CY_CAPSENSE_ADV_MULTIPLIER_HALF;
    /* Centroid position scaling to resolution */
    centroid = (centroid * (int32_t)localMax->resolution) / (int32_t)localMax->snsCount;
    /* Check for position overflow */
    if (centroid < 0)
    {
        centroid = 0;
    }
    /* Removing x256 multiplier with rounding */
    centroid = (centroid + CY_CAPSENSE_ADV_MULTIPLIER_ROUNDING) / CY_CAPSENSE_ADV_MULTIPLIER;
    /* Check for position overflow */
    if (centroid > (int32_t)localMax->resolution)
    {
        centroid = (int32_t)localMax->resolution;
    }
    /* Check for overflow */
    if((uint32_t)denominator > CY_CAPSENSE_ADV_UINT16_T_MAXIMUM)
    {
       denominator = (int32_t)CY_CAPSENSE_ADV_UINT16_T_MAXIMUM;
    }
    (*pos) = (uint16_t)centroid;
    (*posZ) = (uint16_t)denominator;
}

/*******************************************************************************
* Function Name: AdvancedCentroid_EdgeCorrection
****************************************************************************//**
*
* \brief
*  This function updates localMaxSignal[] array with values for virtual sensors
*  which are created to compensate positions on the edges of a touchpad.
*
* \details
*  This function updates localMaxSignal[] array with values for virtual sensors
*  which are created to compensate positions on the edges of a touchpad.
*
* \param config
*  The pointer to the configuration structure of the Advanced Centroid.
*
* \param localMaximum
*  The pointer to the local maximums structure.
*
*******************************************************************************/
static void Cy_CapSense_AdvEdgeCorrection_Lib(const cy_stc_capsense_advanced_centroid_config_t *config, cy_stc_capsense_adv_local_max_t *localMax)
{
    uint32_t centerSignal;

    centerSignal = localMax->localMaxSignal[CY_CAPSENSE_ADV_CENTER];

    /* Checks if there is the left edge */
    if (localMax->localMaxIndex == 0u)
    {
        if((localMax->localMaxSignal[CY_CAPSENSE_ADV_RIGHT_1] < config->penultimateTh) && (centerSignal < config->virtualSnsTh))
        {
            /* Perform edge correction */
            localMax->localMaxSignal[CY_CAPSENSE_ADV_LEFT_1] = (config->virtualSnsTh - centerSignal) * CY_CAPSENSE_ADV_VIRTUAL_SENSOR_MULTIPLIER;
        }
    }
    else
    {
        /* Checks if there is the right edge */
        if (localMax->localMaxIndex == (localMax->snsCount - 1u))
        {
            if((localMax->localMaxSignal[CY_CAPSENSE_ADV_LEFT_1] < config->penultimateTh) && (centerSignal < config->virtualSnsTh))
            {
                /* Perform edge correction */
                localMax->localMaxSignal[CY_CAPSENSE_ADV_RIGHT_1] = (config->virtualSnsTh - centerSignal) * CY_CAPSENSE_ADV_VIRTUAL_SENSOR_MULTIPLIER;
            }
        }
    }
}

/*******************************************************************************
* Function Name: AdvancedCentroid_FindDoubleLocalMax
****************************************************************************//**
*
* \brief
*  Finds maximums in the linear array of sensor signals (differences).
*
* \details
*  Finds maximums in the linear array of sensor signals (differences).
*  The function stores up to two local maximums and reports up to three
*  local maximums.
*
* \param config
*  The pointer to the configuration structure of the Advanced Centroid.
*
* \param localMax
*  The array to the local maximums structure.
*
* \param ptrSns
*  The pointer to the array of sensor signals.
*
* \return
*  Returns the number of found maximums.
*
*******************************************************************************/
static uint32_t Cy_CapSense_AdvFindDoubleLocalMax_Lib(
                const cy_stc_capsense_advanced_centroid_config_t * config,
                cy_stc_capsense_adv_local_max_t localMax[],
                const uint16 * ptrSns)
{
    uint32_t loopIndex;
    uint32_t snsIndex;
    uint32_t foundLocalMaxNumber = 0u;
    uint32_t snsCnt = localMax[0u].snsCount;

    /* Process the first sensor */
    if ((ptrSns[0u] > config->fingerTh) && (ptrSns[0u] >= ptrSns[1u]))
    {
        localMax[foundLocalMaxNumber].localMaxIndex = 0u;
        foundLocalMaxNumber = 1u;
    }

    /* Process the rest sensors */
    for (loopIndex = 1u; loopIndex < (snsCnt - 1u); loopIndex++)
    {
        /* S(i-1) < S(i) >= S(i+1) and S(i) > Finger Threshold */
        if ((ptrSns[loopIndex] > config->fingerTh) &&
            (ptrSns[loopIndex] >= ptrSns[loopIndex + 1u]) &&
            (ptrSns[loopIndex] > ptrSns[loopIndex - 1u]))
        {
            /* Check the number of found maxima */
            if (foundLocalMaxNumber >= CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
            {
                foundLocalMaxNumber++;
                break;
            }
            else
            {
                localMax[foundLocalMaxNumber].localMaxIndex = loopIndex;
                foundLocalMaxNumber++;
            }
        }
    }

    /* Process the last sensor */
    if ((ptrSns[snsCnt - 1u] > config->fingerTh) && (ptrSns[snsCnt - 1u] > ptrSns[snsCnt - 2u]))
    {
        /* Check the number of found maxima */
        if (foundLocalMaxNumber < CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
        {
            localMax[foundLocalMaxNumber].localMaxIndex = (snsCnt - 1u);
        }
        foundLocalMaxNumber++;
    }

    /* Check for three local maximums that is unsupported */
    if (foundLocalMaxNumber <= CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
    {
        /* Fill arrays with sensor signals around maximums */
        for (loopIndex = 0u; loopIndex < foundLocalMaxNumber; loopIndex++)
        {
            /* Clear the difference array */
            for (snsIndex = 0u; snsIndex < CY_CAPSENSE_ADV_CENTROID_SIZE; snsIndex++)
            {
                localMax[loopIndex].localMaxSignal[snsIndex] = 0u;
            }

            /* Get sensor index of local maximum */
            snsIndex = localMax[loopIndex].localMaxIndex;

            /* Store difference S(i) */
            localMax[loopIndex].localMaxSignal[CY_CAPSENSE_ADV_CENTER] = ptrSns[snsIndex];

            /* Store difference S(i - 1) */
            if (snsIndex > 0u)
            {
                localMax[loopIndex].localMaxSignal[CY_CAPSENSE_ADV_LEFT_1] = ptrSns[snsIndex - 1u];
            }
            /* Store difference S(i + 1) */
            if (snsIndex < (snsCnt - 1u))
            {
                localMax[loopIndex].localMaxSignal[CY_CAPSENSE_ADV_RIGHT_1] = ptrSns[snsIndex + 1u];
            }

            /* Use 5x5 centroid for one finger only */
            if (foundLocalMaxNumber < CY_CAPSENSE_ADV_LOCAL_MAX_NUMBER)
            {
                /* Store difference S(i - 2) */
                if (snsIndex > 1u)
                {
                    localMax[loopIndex].localMaxSignal[CY_CAPSENSE_ADV_LEFT_2] = ptrSns[snsIndex - 2u];
                }
                /* Store difference S(i + 2) */
                if (snsIndex < (snsCnt - 2u))
                {
                    localMax[loopIndex].localMaxSignal[CY_CAPSENSE_ADV_RIGHT_2] = ptrSns[snsIndex + 2u];
                }
            }
        }
    }

    return foundLocalMaxNumber;
}


/* [] END OF FILE */

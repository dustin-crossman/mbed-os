/***************************************************************************//**
* \file cy_capsense_smartsense_fw_lib.c
* \version 1.0
*
* \brief
*  This file provides implementation for the functions of the SmartSense
*  part of the Sensing module.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_capsense_lib.h"

/***************************************
* Initial Parameter Constants
***************************************/
#define CY_CAPSENSE_NOISE_ENVELOPE_SHIFT                        (0x02u)
#define CY_CAPSENSE_NOISE_ENVELOPE_RUN_DIFF_SHIFT               (0x03u)
#define CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG                     (0x0Fu)
#define CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG_SHIFT               (0x01u)
#define CY_CAPSENSE_NOISE_ENVELOPE_RESET_COUNTER                (0x0Au)
#define CY_CAPSENSE_NOISE_ENVELOPE_4_TIMES                      (0x02u)

#define CY_CAPSENSE_MAX_NOISE                                   (0xFFu)
#define CY_CAPSENSE_MAX_HYSTERESIS                              (0xFFu)

/* 
* Noise Threshold derived from sensitivity 
* candidate has to be twice lower than sensitivity 
*/
#define CY_CAPSENSE_NOISE_THRESHOLD_SHIFT                       (1u)

/* 
* Sensitivity parameter is defined in Counts/pF units, 
* therefore it has to be divided to correspond to 0.1pF touch 
*/
#define CY_CAPSENSE_SENSITIVITY_DIVIDER                         (10u)

/* Hysteresis has to be 12.5% of finger threshold */
#define CY_CAPSENSE_HYSTERESIS_SHIFT                            (3u)

/* Noise envelope result */
#define noiseEnvelope               param0

/* 
* Running Difference is used to store filtered differences 
* between consecutive scans 
*/
#define runningDifference           param1

/* 
* Previous Sample is used to store RawCount of previous scanning 
*/
#define previousSample              param2

/* 
* Sample Min is used to store minimum value of RawCount during 
* NOISE_ENVELOPE_RESET_COUNTER period 
*/
#define sampleMin                   param3

/* 
* Sample Max is used to store maximum value of RawCount 
* during NOISE_ENVELOPE_RESET_COUNTER period 
*/
#define sampleMax                   param4

/* 
* Sign Register is used to track sign of the difference 
* between last four consecutive scans 
*/
#define signRegister                param5

/* 
* Noise Reset Counter is used to set a period of time.
* After that period is elapsed Noise Envelope value can be set by algorithm.
* Noise Reset Counter is initialized if Raw Counts are monotonous during four
* consecutive scans or difference is four times higher than Running Difference.
*/
#define noiseRstCntr                param6



/*******************************************************************************
* Function Name: Cy_CapSense_UpdateThresholds_Lib
****************************************************************************//**
*
* \brief
*   Updates noise and finger thresholds for a specified widget.
*
* \param[in] ptrThresholds    The pointer to the thresholds object.
* \param[in] ptrNoiseEnvelope The pointer to the noise-envelope object in RAM.
* \param[in] sigPFC           Signal per finger capacitance.
*
* \details
*   This function comprises an algorithm of thresholds auto-tune. The thresholds
*   object contains updated thresholds after this API is called.
*
*******************************************************************************/
void Cy_CapSense_UpdateThresholds_Lib(const cy_stc_capsense_smartsense_csd_noise_envelope_t *ptrNoiseEnvelope,
                                 cy_stc_capsense_smartsense_update_thresholds_t *ptrThresholds,
                                 uint16_t sigPFC,
                                 uint32_t startFlag)
{
    uint16_t highestFingerThreshold;
    uint16_t currFingerTh;

    uint16_t sensCandidate = sigPFC;

    /* Initialize the highest Finger Threshold if the first sensor is under consideration */
    if (startFlag == 0u)
    {
        highestFingerThreshold = 0u;
    }
    else
    {
        highestFingerThreshold = ptrThresholds->fingerTh;
    }

    /* There are two finger threshold candidates: sensitivity (counts/0.1pF) and fourfold noise envelope (x4) */
    if (sensCandidate > (uint16_t)(ptrNoiseEnvelope->noiseEnvelope << CY_CAPSENSE_NOISE_ENVELOPE_SHIFT))
    {
        currFingerTh = sensCandidate;
    }
    else
    {
        currFingerTh = (ptrNoiseEnvelope->noiseEnvelope << CY_CAPSENSE_NOISE_ENVELOPE_SHIFT);
    }

    /* Check whether current finger threshold candidate is higher than previous threshold candidates in the same widget.
     * The worst case scenario is used for setting finger threshold */
    if (currFingerTh > highestFingerThreshold)
    {
        highestFingerThreshold = currFingerTh;
    }

    ptrThresholds->fingerTh = highestFingerThreshold;

    /*  Noise threshold has to be twice lower than current finger threshold */
    if (CY_CAPSENSE_MAX_NOISE < (highestFingerThreshold >> CY_CAPSENSE_NOISE_THRESHOLD_SHIFT))
    {
        ptrThresholds->noiseTh = (uint8_t)CY_CAPSENSE_MAX_NOISE;
        ptrThresholds->nNoiseTh = (uint8_t)CY_CAPSENSE_MAX_NOISE;
    }
    else
    {
        ptrThresholds->noiseTh = (uint8_t)(highestFingerThreshold >> CY_CAPSENSE_NOISE_THRESHOLD_SHIFT);
        ptrThresholds->nNoiseTh = (uint8_t)(highestFingerThreshold >> CY_CAPSENSE_NOISE_THRESHOLD_SHIFT);
    }

    /*  Hysteresis has to be eight times lower than current finger threshold */
    if (CY_CAPSENSE_MAX_HYSTERESIS < (highestFingerThreshold >> CY_CAPSENSE_HYSTERESIS_SHIFT))
    {
        ptrThresholds->hysteresis = (uint8_t)CY_CAPSENSE_MAX_HYSTERESIS;
    }
    else
    {
        ptrThresholds->hysteresis = (uint8_t)(highestFingerThreshold >> CY_CAPSENSE_HYSTERESIS_SHIFT);
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_InitializeNoiseEnvelope_Lib
****************************************************************************//**
*
* \brief    Initializes the noise-envelope filter
*
* \param    rawCount         The RawCount value for a given sensor.
* \param    sigPFC           Signal per finger capacitance.
* \param    ptrNoiseEnvelope The pointer to the noise-envelope RAM object of the sensor.
*
*******************************************************************************/
void Cy_CapSense_InitializeNoiseEnvelope_Lib(uint16_t rawCount,
                                        uint16_t sigPFC,
                                        cy_stc_capsense_smartsense_csd_noise_envelope_t *ptrNoiseEnvelope)
{
    ptrNoiseEnvelope->noiseEnvelope = sigPFC >> CY_CAPSENSE_NOISE_ENVELOPE_SHIFT;
    ptrNoiseEnvelope->runningDifference = sigPFC >> CY_CAPSENSE_NOISE_ENVELOPE_RUN_DIFF_SHIFT;
    ptrNoiseEnvelope->previousSample = rawCount;
    ptrNoiseEnvelope->sampleMin = rawCount;
    ptrNoiseEnvelope->sampleMax = rawCount;
    ptrNoiseEnvelope->signRegister = CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG;
    ptrNoiseEnvelope->noiseRstCntr = 0u;
}


/*******************************************************************************
* Function Name: Cy_CapSense_RunNoiseEnvelope_Lib
****************************************************************************//**
*
* \brief    Runs the noise-envelope filter.
*
* \param    rawCount         The RawCount value for a given sensor.
* \param    sigPFC           Signal per finger capacitance.
* \param    ptrNoiseEnvelope The pointer to the noise-envelope RAM object of the sensor.
*
*******************************************************************************/
void Cy_CapSense_RunNoiseEnvelope_Lib(uint16_t rawCount,
                                 uint16_t sigPFC,
                                 cy_stc_capsense_smartsense_csd_noise_envelope_t *ptrNoiseEnvelope)
{
    int32_t currentDifference;
    uint16_t minmax;

    /*
    * Difference between current and previous sample is calculated - currentDifference
    * CY_CAPSENSE_SIGN_REGISTER is used to track sign of difference for four consecutive scans.
    */

    currentDifference = ((int32_t)rawCount - (int32_t)ptrNoiseEnvelope->previousSample);
    ptrNoiseEnvelope->previousSample = rawCount;
    ptrNoiseEnvelope->signRegister <<= CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG_SHIFT;

    if (currentDifference < 0)
    {
        ptrNoiseEnvelope->signRegister++;
        currentDifference = -currentDifference;
    }

    ptrNoiseEnvelope->signRegister &= CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG;

    /*
    * CY_CAPSENSE_RUNNING_DIFFERENCE is inertial part of current difference
    * and proportional to amplitude of high frequency noise.
    */
    if ((uint32_t)currentDifference < ptrNoiseEnvelope->runningDifference)
    {
        if (ptrNoiseEnvelope->runningDifference > 1u)
        {
            ptrNoiseEnvelope->runningDifference--;
        }
    }
    else
    {
        ptrNoiseEnvelope->runningDifference++;
    }

    /*
    * If there are four consecutive scans with monotonic RawCounts, or
    * current difference is four times higher than running difference,
    * initialize minimum and maximum values andnoiseRStCnt.
    */

    if ((ptrNoiseEnvelope->signRegister == CY_CAPSENSE_NOISE_ENVELOPE_SIGN_REG) ||
        (ptrNoiseEnvelope->signRegister == 0x00u) ||
        ((uint32_t)currentDifference > ((uint32_t)ptrNoiseEnvelope->runningDifference << CY_CAPSENSE_NOISE_ENVELOPE_4_TIMES)))
    {
        ptrNoiseEnvelope->sampleMin = rawCount;
        ptrNoiseEnvelope->sampleMax = rawCount;
        ptrNoiseEnvelope->noiseRstCntr = 0u;
    }
     else
    {
        if (rawCount < ptrNoiseEnvelope->sampleMin)
        {
            ptrNoiseEnvelope->sampleMin = rawCount;
        }
        else
        {
            if (rawCount > ptrNoiseEnvelope->sampleMax)
            {
                ptrNoiseEnvelope->sampleMax = rawCount;
            }
        }

        /*
        * If there are CY_CAPSENSE_NOISE_ENVELOPE_RESET_COUNTER consecutive scans with non-monotonic RawCount
        * and current difference is not higher than four times running difference,
        * then noise-envelope value will be updated.
        * Noise-envelope value can be increased/decreased depending on difference between
        * minimum and maximum values
        */
        if (ptrNoiseEnvelope->noiseRstCntr < CY_CAPSENSE_NOISE_ENVELOPE_RESET_COUNTER)
        {
            ptrNoiseEnvelope->noiseRstCntr++;
        }
        else
        {
            minmax = ptrNoiseEnvelope->sampleMax - ptrNoiseEnvelope->sampleMin;

            if (ptrNoiseEnvelope->runningDifference > minmax)
            {
                if (ptrNoiseEnvelope->runningDifference > (sigPFC >> CY_CAPSENSE_NOISE_ENVELOPE_4_TIMES))
                {
                    ptrNoiseEnvelope->runningDifference--;
                }
            }
            else
            {
                ptrNoiseEnvelope->runningDifference++;
            }
            ptrNoiseEnvelope->noiseEnvelope = ptrNoiseEnvelope->runningDifference;
        }
    }
}


/* [] END OF FILE */

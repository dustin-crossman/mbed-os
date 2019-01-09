/***************************************************************************//**
* \file cy_capsense_smartsense_hw_lib.c
* \version 1.0
*
* \brief
*  This file provides implementation for the functions of the HW SmartSense
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
* API Constants
***************************************/

#define CY_CAPSENSE_MAX_RESOLUTION                      (16u)
#define CY_CAPSENSE_MIN_RESOLUTION                      (6u)

#define CY_CAPSENSE_RESOLUTION_16_BIT                   (0x0000FFFFLu)
#define CY_CAPSENSE_PERCENTAGE_100                      (100u)
#define CY_CAPSENSE_PERCENTAGE_75                       (75u)
#define CY_CAPSENSE_MAX_SIG_PER_FINGER_CAPACITANCE      (100u)
#define CY_CAPSENSE_CALIBR_SNS_CLK_KHZ                  (1500u)
#define CY_CAPSENSE_FEMTO_UNITS_FACTOR                  (1000u)
#define CY_CAPSENSE_QUANTIZATION_FACTOR                 (10Lu)
#define CY_CAPSENSE_FINGER_CAPACITANCE_UNITS_FACTOR     (1000000Lu / CY_CAPSENSE_QUANTIZATION_FACTOR)
#define CY_CAPSENSE_INTEGER_MATH_MULT_4                 (2u)
#define CY_CAPSENSE_OVERSAMPLING_CAPACITANCE            (25000Lu)
#define CY_CAPSENSE_OVERSAMPLING_DISABLE                (1Lu)
#define CY_CAPSENSE_OVERSAMPLING_ENABLE                 (2Lu)
#define CY_CAPSENSE_CP_SCALING                          (100u)
#define CY_CAPSENSE_SNS_CLK_SCALING                     (100u)
#define CY_CAPSENSE_PERIOD_CLK_SCALING                  (10u)
#define CY_CAPSENSE_SNS_CLK_UNITS_DIV                   (1000000u)

static uint32_t Cy_CapSense_SsGetSensorCp_Lib(const cy_stc_capsense_auto_tune_config_t * config);

/*******************************************************************************
* Function Name: Cy_CapSense_SsGetSensorCp_Lib
****************************************************************************//**
*
* \brief
*  This function calculates the sensor capacitance in fF (10^-15).
*
* \details
*  Calculates Cp in fF (10^-15) using following equation:
*  Cp = (calTarget * iDacMod + iDacComp) * iDacGain / vRef / (snsClkInputClock / ptrSenseClk)
*  where:
*  calTarget - [1000 * achieved rawcount / (2 ^ resolution - 1)]
*  iDacMod - Modulator IDAC code, bits;
*  iDacComp - Compensation IDAC code, bits;
*  iDacGain - IDAC gain in pA/bit;
*  vRef - reference voltage, mV;
*  ptrSenseClk - Sense Clock Divider.
*  snsClkInputClock - Modulation Clock, kHz.
*
* \param config
*  The configuration structure.
*
* \return 
*  Cp in fF (10^-15) 
*
*******************************************************************************/
static uint32_t Cy_CapSense_SsGetSensorCp_Lib(const cy_stc_capsense_auto_tune_config_t * config)
{
    uint32_t cp;
    uint32_t snsClk;
    uint32_t iDacCode;

    /* 
     * Calculation order changed to avoid overflow. Supposed the following input ranges:
     *  calTarget        [100..1000]
     *  iDacMod          [1..255]
     *  iDacComp         [0..127]
     *  iDacGain         [37500..4800000]
     *  vRef             [800..4400]
     *  snsClkInputClock [3000..50000]
     *  ptrSenseClk      (snsClkInputClock / 1500)
     *  snsClkInputClock / ptrSenseClk => [1500..2000]
     */
    snsClk = (uint32_t)config->snsClkInputClock / config->ptrSenseClk[0];
    iDacCode = (uint32_t)config->iDacMod * config->calTarget + (uint32_t)config->iDacComp * CY_CAPSENSE_FEMTO_UNITS_FACTOR;
    cp = (iDacCode * ((uint32_t)config->iDacGain / snsClk)) / config->vRef;
    
    return cp;
}


/*******************************************************************************
* Function Name: Cy_CapSense_TunePrescalers_Lib
****************************************************************************//**
*
* \brief
*  This internal function tunes the Sense Clock divider.
*
* \details
*  Found IDAC code in Single IDAC mode is used to calculate the optimal SnsClk.
*  The SnsClk divider is set to meet the requirement that the widget 
*  clock period should be greater than or equal to:
*  Period > 2*5*R*Cp,
*  where:
*  Cp is the maximum sensor parasitic capacitance within the widget.
*  R is the user input value in the expression view of the customizer for a 
*  series resistor.
*
* \param config
*  The configuration structure.
*
* \return 
*  Cp in fF (10^-15)
*
*******************************************************************************/
uint32_t Cy_CapSense_TunePrescalers_Lib(cy_stc_capsense_auto_tune_config_t * config)
{
    uint32_t cp;
    uint32_t divider;

    /* Get max Cp of widget in fF (10^-15) */
    cp = Cy_CapSense_SsGetSensorCp_Lib(config);

    /* Calculate divider */
    divider = ((uint32_t)config->snsClkConstantR * (cp / CY_CAPSENSE_CP_SCALING)) / CY_CAPSENSE_PERIOD_CLK_SCALING;
    divider = (divider * ((uint32_t)config->snsClkInputClock / CY_CAPSENSE_SNS_CLK_SCALING)) / CY_CAPSENSE_SNS_CLK_UNITS_DIV;

    if (0u == divider)
    {
        divider = 1u;
    }
    /* Make divider even */
    divider++;
    divider &= ~0x01u;

    config->ptrSenseClk[0u] = divider;

    return(cp);
}


/*******************************************************************************
* Function Name: Cy_CapSense_TuneSensitivity_Lib
****************************************************************************//**
*
* \brief
*  Configure scanning resolution to achieve the sufficient sensitivity.
*
* \details
*  The function searches the lowest possible resolution that produces signal 
*  greater than 50 counts (Difference Counts) for user defined finger capacitance.
*  In addition, function calculates 75%-value of the achieved signal, that becomes 
*  candidate to finger threshold.
*  
*  Used equation to calculate signal at resolution 16-bit:
*  sigPFCmax = (2^16-1) * vRef * snsClk * fingerCap / idacCurrent
*
*  sigPFCmax contains absolute number of difference counts that user receives as 
*  result of sensor scanning at corresponding resolution.
*
*  This function requires non-zero Modulator IDAC code (if IDAC is equal to zero it 
*  is considered as non-valid use case).
*
* \param config
*  The configuration structure.
*
* \return 
*  Scan resolution
*
*******************************************************************************/
uint8_t Cy_CapSense_TuneSensitivity_Lib(cy_stc_capsense_auto_tune_config_t *config)
{
    uint32_t  scanResolution;
    uint32_t idacCurrent;
    uint32_t snsClk;
    uint32_t sigPFCmax;               /* Signal per user defined finger capacitance */
    uint32_t oversamplingFactor = CY_CAPSENSE_OVERSAMPLING_DISABLE;

    /* Initialize Resolution to 16 bits */
    scanResolution = CY_CAPSENSE_MAX_RESOLUTION;

    /*  Calculate IDAC Current in NanoAmpers */
    idacCurrent = ((uint32_t)config->iDacMod * config->iDacGain) / CY_CAPSENSE_FEMTO_UNITS_FACTOR;

    /* Calculate Sense Clock Frequency in KHz */
    snsClk = (uint32_t)config->snsClkInputClock / config->ptrSenseClk[0u];

    /*  Calculate signal at Resolution = 16 for user defined finger capacitance */
    sigPFCmax = (((CY_CAPSENSE_RESOLUTION_16_BIT * config->vRef) / idacCurrent) * snsClk);
    sigPFCmax = ((sigPFCmax / CY_CAPSENSE_FINGER_CAPACITANCE_UNITS_FACTOR) * config->fingerCap) / CY_CAPSENSE_QUANTIZATION_FACTOR;

    /*  Check for oversampling */
    if (CY_CAPSENSE_OVERSAMPLING_CAPACITANCE <= config->sensorCap)
    {
        oversamplingFactor = CY_CAPSENSE_OVERSAMPLING_ENABLE;
    }

    /* Reduce Resolution to provide the lowest acceptable sensitivity, i.e. bigger than 50 counts */
    while (sigPFCmax > (CY_CAPSENSE_MAX_SIG_PER_FINGER_CAPACITANCE * oversamplingFactor))
    {
        /* Decrement Resolution */
        scanResolution -= 1u;

        /* Scale sigPFCmax to account for Resolution */
        sigPFCmax >>= 1u;

        /* The minimum resolution is 6 bit */
        if (CY_CAPSENSE_MIN_RESOLUTION >= scanResolution)
        {
            break;
        }
    }

    /* Return 75% of signal */
    config->sigPFC[0u] = (uint16_t)((sigPFCmax * CY_CAPSENSE_PERCENTAGE_75) / CY_CAPSENSE_PERCENTAGE_100);

    return ((uint8_t)scanResolution);
}


/* [] END OF FILE */

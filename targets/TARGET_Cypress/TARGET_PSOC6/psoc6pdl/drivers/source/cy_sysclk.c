/***************************************************************************//**
* \file cy_sysclk.c
* \version 1.30
*
* Provides an API implementation of the sysclk driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "cy_sysclk.h"
#include "cy_syslib.h"
#include <stdlib.h>


/* # of elements in an array */
#define  CY_SYSCLK_N_ELMTS(a)  (sizeof(a) / sizeof((a)[0]))

/* ========================================================================== */
/* ===========================    ECO SECTION    ============================ */
/* ========================================================================== */
/** \cond INTERNAL */
#define CY_SYSCLK_TRIM_ECO_Pos  (SRSS_CLK_TRIM_ECO_CTL_WDTRIM_Pos)
#define CY_SYSCLK_TRIM_ECO_Msk  (SRSS_CLK_TRIM_ECO_CTL_WDTRIM_Msk | \
                                 SRSS_CLK_TRIM_ECO_CTL_ATRIM_Msk  | \
                                 SRSS_CLK_TRIM_ECO_CTL_FTRIM_Msk  | \
                                 SRSS_CLK_TRIM_ECO_CTL_RTRIM_Msk  | \
                                 SRSS_CLK_TRIM_ECO_CTL_GTRIM_Msk)

                                 
/*******************************************************************************
* Function Name: cy_sqrt
* Calculates square root.
*******************************************************************************/
static uint32_t cy_sqrt(uint64_t x);
static uint32_t cy_sqrt(uint64_t x)
{
    uint32_t res = 0UL;
    uint32_t add = 0x80000000UL;
    uint32_t i;

    for(i = 0UL; i < 32UL; i++)
    {
        uint32_t tmp = res | add;

        if (x >= ((uint64_t)tmp * tmp))
        {
            res = tmp;
        }

        add >>= 1U;
    }

    return (res);
}

/** \endcond */


/**
* \addtogroup group_sysclk_eco_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_EcoConfigure
****************************************************************************//**
*
* Configures the external crystal oscillator (ECO) trim bits based on crystal 
* characteristics. This function should be called only when the ECO is disabled.
*
* \param freq Operating frequency of the crystal in Hz.
*
* \param cLoad Crystal load capacitance in pF.
*
* \param esr Effective series resistance of the crystal in ohms.
*
* \param driveLevel Crystal drive level in uW.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - ECO configuration completed successfully<br>
* CY_SYSCLK_BAD_PARAM - One or more invalid parameters<br>
* CY_SYSCLK_INVALID_STATE - ECO already enabled
*
* \note
* The following calculations are implemented, generally in floating point:
*
* \verbatim
*   freqMHz = freq / 1000000
*   max amplitude Vpp = 1000 * sqrt(drivelevel / 2 / esr) / 3.14 / freqMHz / cLoad
*   gm_min mA/V = 5 * 4 * 3.14 * 3.14 * freqMhz^2 * cLoad^2 * 4 * esr / 1000000000
*   Number of amplifier sections = INT(gm_min / 4.5)
*
*   As a result of the above calculations, max amplitude must be >= 0.5, and the
*   number of amplifier sections must be <= 3, otherwise this function returns with
*   a parameter error.
*
*   atrim = if (max amplitude < 0.5) then error
*           else 2 * the following:
*                    max amplitude < 0.6: 0
*                    max amplitude < 0.7: 1
*                    max amplitude < 0.8: 2
*                    max amplitude < 0.9: 3
*                    max amplitude < 1.15: 5
*                    max amplitude < 1.275: 6
*                    max amplitude >= 1.275: 7
*   wdtrim = if (max amplitude < 0.5) then error
*            else 2 * the following:
*                     max amplitude < 1.2: INT(5 * max amplitude) - 2
*                     max amplitude >= 1.2: 3
*   gtrim = if (number of amplifier sections > 3) then error
*           else the following:
*                number of amplifier sections > 1: number of amplifier sections
*                number of amplifier sections = 1: 0
*                number of amplifier sections < 1: 1
*   rtrim = if (gtrim = error) then error
*           else the following:
*                freqMHz > 26.8: 0
*                freqMHz > 23.33: 1
*                freqMHz > 16.5: 2
*                freqMHz <= 16.5: 3
*   ftrim = if (atrim = error) then error
*           else INT(atrim / 2)
* \endverbatim
*
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_EcoConfigure
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_EcoConfigure(uint32_t freq, uint32_t cLoad, uint32_t esr, uint32_t driveLevel)
{
    /* error if ECO is not disabled - any of the 3 enable bits are set */
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;
    if (0UL == (SRSS_CLK_ECO_CONFIG_ECO_EN_Msk & SRSS_CLK_ECO_CONFIG))
    {
        /* calculate intermediate values */
        uint32_t maxAmplitude = CY_SYSCLK_DIV_ROUND((1000000000000ULL *
                        cy_sqrt(CY_SYSCLK_DIV_ROUND(500000UL * driveLevel, esr))), (3141ULL * freq * cLoad)); /* Scaled by 10^3 */

        uint32_t nAmpSections = CY_SYSCLK_DIV_ROUND((uint64_t)freq *
                                                    (uint64_t)freq *
                                                    (uint64_t)cLoad *
                                                    (uint64_t)cLoad, 5704868154158ULL); /* Scaled by 10^6 */

        if ((maxAmplitude < 500UL) && (nAmpSections > 3000000UL))
        {
            /* Error if input parameters cause erroneous intermediate values. */
            retVal = CY_SYSCLK_BAD_PARAM;
        }
        else
        {
            uint32_t atrim, wdtrim, gtrim, rtrim, ftrim, reg;

            atrim = (maxAmplitude < 600UL) ? 0UL :
                     ((maxAmplitude < 700UL) ? 2UL :
                      ((maxAmplitude < 800UL) ? 4UL :
                       ((maxAmplitude < 900UL) ? 6UL :
                        ((maxAmplitude < 1150UL) ? 10UL :
                         ((maxAmplitude < 1275UL) ? 12UL : 14UL)))));

            wdtrim = (maxAmplitude < 1200UL) ? (maxAmplitude / 100UL) - 4UL : 6UL;

            gtrim = ((nAmpSections > 1000000UL) ? CY_SYSCLK_DIV_ROUND(nAmpSections, 1000000UL) :
                    ((nAmpSections == 1000000UL) ? 0UL : 1UL));

            rtrim = ((freq > 26800000UL) ? 0UL :
                     ((freq > 23330000UL) ? 1UL :
                      ((freq > 16500000UL) ? 2UL : 3UL)));

            ftrim = atrim / 2UL;

            /* Update all fields of trim control register with one write, without changing the ITRIM field */
            reg = _VAL2FLD(SRSS_CLK_TRIM_ECO_CTL_WDTRIM, wdtrim) |
                  _VAL2FLD(SRSS_CLK_TRIM_ECO_CTL_ATRIM, atrim)   |
                  _VAL2FLD(SRSS_CLK_TRIM_ECO_CTL_FTRIM, ftrim)   |
                  _VAL2FLD(SRSS_CLK_TRIM_ECO_CTL_RTRIM, rtrim)   |
                  _VAL2FLD(SRSS_CLK_TRIM_ECO_CTL_GTRIM, gtrim);
                  
            CY_REG32_CLR_SET(SRSS_CLK_TRIM_ECO_CTL, CY_SYSCLK_TRIM_ECO, reg);

            retVal = CY_SYSCLK_SUCCESS;
        } /* if valid parameters */
    } /* if ECO not enabled */

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_EcoEnable
****************************************************************************//**
*
* Enables the external crystal oscillator (ECO). This function should be called
* after \ref Cy_SysClk_EcoConfigure.
*
* \param timeoutus Amount of time in microseconds to wait for the ECO to stabilize.
* To avoid waiting for stabilization, set this parameter to 0.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - ECO locked<br>
* CY_SYSCLK_TIMEOUT - ECO timed out and did not lock
* CY_SYSCLK_INVALID_STATE - ECO already enabled
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_EcoEnable
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_EcoEnable(uint32_t timeoutus)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;
    bool zeroTimeout = (0UL == timeoutus);

    /* Invalid state error if ECO is already enabled */
    if (0UL == (SRSS_CLK_ECO_CONFIG_ECO_EN_Msk & SRSS_CLK_ECO_CONFIG))
    {
        /* Set ECO enable */
        SRSS_CLK_ECO_CONFIG |= SRSS_CLK_ECO_CONFIG_ECO_EN_Msk;

        /* Wait for CY_SYSCLK_ECOSTAT_STABLE */
        for (; (CY_SYSCLK_ECOSTAT_STABLE != Cy_SysClk_EcoGetStatus()) && (0UL != timeoutus); timeoutus--)
        {
            Cy_SysLib_DelayUs(1U);
        }

        retVal = (zeroTimeout || (0UL != timeoutus)) ? CY_SYSCLK_SUCCESS : CY_SYSCLK_TIMEOUT;
    }

    return (retVal);
}
/** \} group_sysclk_eco_funcs */


/* ========================================================================== */
/* ====================    INPUT MULTIPLEXER SECTION    ===================== */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_path_src_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_ClkPathSetSource
****************************************************************************//**
*
* Configures the source for the specified clock path.
*
* \param clkPath Selects which clock path to configure; 0 is the first clock
* path, which is the FLL.
*
* \param source \ref cy_en_clkpath_in_sources_t
*
* \return \ref cy_en_sysclk_status_t
*
* \note
* If calling this function changes an FLL or PLL input frequency, disable the FLL
* or PLL before calling this function. After calling this function, call the FLL
* or PLL configure function, for example \ref Cy_SysClk_FllConfigure().
*
* \note
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* it affects the CLK_HF0 frequency and the frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* it affects the CLK_HF0 frequency and the frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_ClkPathSetSource
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_ClkPathSetSource(uint32_t clkPath, cy_en_clkpath_in_sources_t source)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if ((clkPath < CY_SRSS_NUM_CLKPATH) &&
        ((source <= CY_SYSCLK_CLKPATH_IN_DSIMUX) ||
         ((CY_SYSCLK_CLKPATH_IN_DSI <= source) && (source <= CY_SYSCLK_CLKPATH_IN_PILO))))
    {
        if (source >= CY_SYSCLK_CLKPATH_IN_DSI)
        {
            SRSS_CLK_DSI_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_DSI_SELECT_DSI_MUX, (uint32_t)source);
            SRSS_CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)CY_SYSCLK_CLKPATH_IN_DSIMUX);
        }
        else
        {
            SRSS_CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)source);
        }
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPathGetSource
****************************************************************************//**
*
* Reports which source is selected for the path mux.
*
* \param clkPath Selects which clock path to report; 0 is the first clock path,
* which is the FLL.
*
* \return \ref cy_en_clkpath_in_sources_t
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_ClkPathGetSource
*
*******************************************************************************/
cy_en_clkpath_in_sources_t Cy_SysClk_ClkPathGetSource(uint32_t clkPath)
{
    CY_ASSERT_L1(clkPath < CY_SRSS_NUM_CLKPATH);
    cy_en_clkpath_in_sources_t retVal =
        (cy_en_clkpath_in_sources_t )_FLD2VAL(SRSS_CLK_PATH_SELECT_PATH_MUX, SRSS_CLK_PATH_SELECT[clkPath]);
    if (retVal == CY_SYSCLK_CLKPATH_IN_DSIMUX)
    {
        retVal = (cy_en_clkpath_in_sources_t)(CY_SYSCLK_CLKPATH_IN_DSI |
                    (_FLD2VAL(SRSS_CLK_DSI_SELECT_DSI_MUX, SRSS_CLK_DSI_SELECT[clkPath])));
    }
    return retVal;
}
/** \} group_sysclk_path_src_funcs */


/* ========================================================================== */
/* ===========================    FLL SECTION    ============================ */
/* ========================================================================== */
/* min and max FLL output frequencies, in Hz */
#define  CY_SYSCLK_FLL_MIN_CCO_OUTPUT_FREQ (48000000UL)
#define  CY_SYSCLK_FLL_MIN_OUTPUT_FREQ     (CY_SYSCLK_FLL_MIN_CCO_OUTPUT_FREQ / 2U)
#define  CY_SYSCLK_FLL_MAX_OUTPUT_FREQ     (100000000UL)

#define  CY_SYSCLK_FLL_IS_CCO_RANGE_VALID(range) (((range) == CY_SYSCLK_FLL_CCO_RANGE0) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE1) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE2) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE3) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE4))
/** \cond INTERNAL */
#define  CY_SYSCLK_FLL_INT_COEF (327680000UL)
#define  CY_SYSCLK_FLL_GAIN_IDX (11U)
#define  CY_SYSCLK_FLL_GAIN_VAL (8UL * CY_SYSCLK_FLL_INT_COEF)

#define TRIM_STEPS_SCALE 100000000ULL /* 10 ^ 8 */
#define MARGIN_SCALE 100000ULL /* 10 ^ 5 */
/** \endcond */


/**
* \addtogroup group_sysclk_fll_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_FllConfigure
****************************************************************************//**
*
* Configures the FLL, for best accuracy optimization.
*
* \param inputFreq frequency of input source, in Hz
*
* \param outputFreq Desired FLL output frequency, in Hz. Allowable range is
* 24 MHz to 100 MHz. In all cases, FLL_OUTPUT_DIV must be set; the output divide
* by 2 option is required.
*
* \param outputMode \ref cy_en_fll_pll_output_mode_t
* If output mode is bypass, then the output frequency equals the input source
* frequency regardless of the frequency parameter values.
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - FLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - desired output frequency is out of valid range
*
* \note
* Call this function after changing the FLL input frequency, for example if
* \ref Cy_SysClk_ClkPathSetSource() is called.
*
* \note
* Do not call this function when the FLL is enabled. If it is called, then this function
* returns with an CY_SYSCLK_INVALID_STATE return value and no register updates.
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note 
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the FLL is the source of CLK_HF0 and the FLL frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the FLL is the source of CLK_HF0 and the FLL frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_FllConfigure
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_FllConfigure(uint32_t inputFreq, uint32_t outputFreq, cy_en_fll_pll_output_mode_t outputMode)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_SUCCESS;

    /* check for errors */
    if ((outputFreq < CY_SYSCLK_FLL_MIN_OUTPUT_FREQ) || (CY_SYSCLK_FLL_MAX_OUTPUT_FREQ < outputFreq) || /* invalid output frequency */
       ((outputFreq * 5UL / inputFreq) < 11UL)) /* check output/input frequency ratio */
    {
        retVal = CY_SYSCLK_BAD_PARAM;
    }
    else /* no errors */
    {
        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            cy_stc_fll_manual_config_t config;
            uint32_t ccoFreq;
            bool wcoSource = (CY_SYSCLK_CLKPATH_IN_WCO == Cy_SysClk_ClkPathGetSource(0UL/*FLL*/)) ? true : false;

            config.outputMode = outputMode;
            /* 1. Output division by 2 is always required. */
            config.enableOutputDiv = true;
            /* 2. Compute the target CCO frequency from the target output frequency and output division. */
            ccoFreq = outputFreq * 2UL;
            /* 3. Compute the CCO range value from the CCO frequency */
            config.ccoRange = ((ccoFreq >= 150339200UL) ? CY_SYSCLK_FLL_CCO_RANGE4 :
                               ((ccoFreq >= 113009380UL) ? CY_SYSCLK_FLL_CCO_RANGE3 :
                                ((ccoFreq >=  84948700UL) ? CY_SYSCLK_FLL_CCO_RANGE2 :
                                 ((ccoFreq >=  63855600UL) ? CY_SYSCLK_FLL_CCO_RANGE1 : CY_SYSCLK_FLL_CCO_RANGE0))));
                                 
        /* 4. Compute the FLL reference divider value.
              refDiv is a constant if the WCO is the FLL source, otherwise the formula is
              refDiv = ROUNDUP((inputFreq / outputFreq) * 250) */
            config.refDiv = wcoSource ? 19U : (uint16_t)CY_SYSCLK_DIV_ROUNDUP(inputFreq * 50UL, (outputFreq / 5UL));

        /* 5. Compute the FLL multiplier value.
              Formula is fllMult = ccoFreq / (inputFreq / refDiv) */
            config.fllMult = CY_SYSCLK_DIV_ROUNDUP((uint64_t)ccoFreq * (uint64_t)config.refDiv, (uint64_t)inputFreq);
        /* 6. Compute the lock tolerance.
              Formula is lock tolerance = 1.5 * fllMult * (((1 + CCO accuracy) / (1 - source clock accuracy)) - 1)
              We assume CCO accuracy is 0.25%.
              We assume the source clock accuracy = 1%. This is the accuracy of the IMO.
              Therefore the formula is lock tolerance = 1.5 * fllMult * 0.012626 = 0.018939 * fllMult */
            config.lockTolerance = (uint16_t)CY_SYSCLK_DIV_ROUNDUP(config.fllMult * 18939UL, 1000000UL);
                
            {
                /* constants indexed by ccoRange */
                const uint32_t trimSteps[] = {110340UL, 110200UL, 110000UL, 110000UL, 117062UL}; /* Scaled by 10^8 */
                const uint32_t margin[] = {436UL, 581UL, 772UL, 1030UL, 1320UL}; /* Scaled by 10^5 */
        /* 7. Compute the CCO igain and pgain. */
                {
                    /* intermediate parameters */
                    uint32_t kcco = (trimSteps[config.ccoRange] * margin[config.ccoRange]);
                    uint32_t ki_p = (uint32_t)CY_SYSCLK_DIV_ROUND(850ULL * CY_SYSCLK_FLL_INT_COEF * inputFreq, (uint64_t)kcco * (uint64_t)config.refDiv);

                    /* find the largest IGAIN value that is less than or equal to ki_p */
                    uint32_t igain = CY_SYSCLK_FLL_GAIN_VAL;
                    uint32_t pgain = CY_SYSCLK_FLL_GAIN_VAL;

                    /* find the largest IGAIN value that is less than or equal to ki_p */
                    for(config.igain = CY_SYSCLK_FLL_GAIN_IDX; (igain > ki_p) && (config.igain != 0UL); config.igain--)
                    {
                        igain >>= 1U;
                    }
                    /* decrement igain if the WCO is the FLL source */
                    if (wcoSource && (config.igain > 0U))
                    {
                        config.igain--;
                        igain >>= 1U;
                    }
                    
                    /* then find the largest PGAIN value that is less than or equal to ki_p - igain */
                    for(config.pgain = CY_SYSCLK_FLL_GAIN_IDX; (pgain > (ki_p - igain)) && (config.pgain != 0UL); config.pgain--)
                    {
                        pgain >>= 1U;
                    }
                    /* decrement pgain if the WCO is the FLL source */
                    if (wcoSource && (config.pgain > 0U))
                    {
                        config.pgain--;
                    }
                }
                
        /* 8. Compute the CCO_FREQ bits in CLK_FLL_CONFIG4 register. */
                {
                    uint64_t cmp = (TRIM_STEPS_SCALE / MARGIN_SCALE) * ccoFreq / margin[config.ccoRange];
                    uint64_t mlt = TRIM_STEPS_SCALE + trimSteps[config.ccoRange];
                    uint64_t res = mlt;

                    config.cco_Freq = 0U;

                    while(res < cmp)
                    {
                        res *= mlt;
                        res /= TRIM_STEPS_SCALE;
                        config.cco_Freq++;
                    }
                }
            }
            
        /* 9. Compute the settling count, using a 1 usec settling time. Use a constant if the WCO is the FLL source. */
            {
                uint64_t fref = (6000ULL * inputFreq) / config.refDiv;
                uint32_t divval = CY_SYSCLK_DIV_ROUNDUP(inputFreq, 1000000UL);
                uint32_t altval = CY_SYSCLK_DIV_ROUNDUP(divval * fref, 6000000UL) + 1UL;

                config.settlingCount = wcoSource ? 200U : (uint16_t)
                          ((outputFreq < fref) ? divval :
                            ((divval > altval) ? divval : altval));
            }
        /* Configure FLL based on calculated values */
            retVal = Cy_SysClk_FllManualConfigure(&config);
        }
        else /* if not, bypass output mode */
        {
            CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
        }
    }

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllManualConfigure
****************************************************************************//**
*
* Manually configures the FLL based on user inputs.
*
* \param config \ref cy_stc_fll_manual_config_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - FLL not configured because it is enabled
*
* \note
* Call this function after changing the FLL input frequency, for example if
* \ref Cy_SysClk_ClkPathSetSource() is called.
*
* \note
* Do not call this function when the FLL is enabled. If it is called, then this function
* returns immediately with an CY_SYSCLK_INVALID_STATE return value and no register updates.
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note 
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the FLL is the source of CLK_HF0 and the FLL frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the FLL is the source of CLK_HF0 and the FLL frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_FllManualConfigure
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_FllManualConfigure(const cy_stc_fll_manual_config_t *config)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_SUCCESS;

    CY_ASSERT_L1(config != NULL);
    /* check for errors */
    if (_FLD2BOOL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS_CLK_FLL_CONFIG)) /* if enabled */
    {
        retVal = CY_SYSCLK_INVALID_STATE;
    }
    else /* no errors */
    {
        /* update CLK_FLL_CONFIG register with 2 parameters; FLL_ENABLE is already 0 */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->fllMult <= (SRSS_CLK_FLL_CONFIG_FLL_MULT_Msk >> SRSS_CLK_FLL_CONFIG_FLL_MULT_Pos));

        SRSS_CLK_FLL_CONFIG = _VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_MULT, config->fllMult) |
                             _BOOL2FLD(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, config->enableOutputDiv);

        /* update CLK_FLL_CONFIG2 register with 2 parameters */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->refDiv <= (SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Msk >> SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Pos));
        CY_ASSERT_L1(config->lockTolerance <= (SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Msk >> SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Pos));

        SRSS_CLK_FLL_CONFIG2 = _VAL2FLD(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, config->refDiv) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, config->lockTolerance);

        /* update CLK_FLL_CONFIG3 register with 4 parameters */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->igain <= (SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Pos));
        CY_ASSERT_L1(config->pgain <= (SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Pos));
        CY_ASSERT_L1(config->settlingCount <= (SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Msk >> SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Pos));

        SRSS_CLK_FLL_CONFIG3 = _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, config->igain) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, config->pgain) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, config->settlingCount) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, config->outputMode);

        /* update CLK_FLL_CONFIG4 register with 1 parameter; preserve other bits */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(CY_SYSCLK_FLL_IS_CCO_RANGE_VALID(config->ccoRange));
        CY_ASSERT_L1(config->cco_Freq <= (SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Msk >> SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Pos));

        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG4, SRSS_CLK_FLL_CONFIG4_CCO_RANGE, (uint32_t)(config->ccoRange));
        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG4, SRSS_CLK_FLL_CONFIG4_CCO_FREQ, (uint32_t)(config->cco_Freq));
    }

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllGetConfiguration
****************************************************************************//**
*
* Reports the FLL configuration settings.
*
* \param config \ref cy_stc_fll_manual_config_t
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_FllGetConfiguration
*
*******************************************************************************/
void Cy_SysClk_FllGetConfiguration(cy_stc_fll_manual_config_t *config)
{
    CY_ASSERT_L1(config != NULL);
    /* read 2 parameters from CLK_FLL_CONFIG register */
    uint32_t tempReg = SRSS_CLK_FLL_CONFIG;
    config->fllMult         = _FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_MULT, tempReg);
    config->enableOutputDiv = _FLD2BOOL(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, tempReg);
    /* read 2 parameters from CLK_FLL_CONFIG2 register */
    tempReg = SRSS_CLK_FLL_CONFIG2;
    config->refDiv          = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, tempReg);
    config->lockTolerance   = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, tempReg);
    /* read 4 parameters from CLK_FLL_CONFIG3 register */
    tempReg = SRSS_CLK_FLL_CONFIG3;
    config->igain           = (uint8_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, tempReg);
    config->pgain           = (uint8_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, tempReg);
    config->settlingCount   = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, tempReg);
    config->outputMode      = (cy_en_fll_pll_output_mode_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, tempReg);
    /* read 1 parameter from CLK_FLL_CONFIG4 register */
    config->ccoRange        = (cy_en_fll_cco_ranges_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG4_CCO_RANGE, SRSS_CLK_FLL_CONFIG4);
    config->cco_Freq        = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG4_CCO_FREQ, SRSS_CLK_FLL_CONFIG4);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllEnable
****************************************************************************//**
*
* Enables the FLL. The FLL should be configured before calling this function.
*
* \param timeoutus Amount of time in micro seconds to wait for FLL to lock.
* If lock doesn't occur, the FLL is stopped. To avoid waiting for lock, set this to 0
* and manually check for lock using \ref Cy_SysClk_FllLocked.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for FLL lock
*
* \note
* While waiting for the FLL to lock, the FLL bypass mode is set to \ref CY_SYSCLK_FLLPLL_OUTPUT_INPUT.
* After the FLL is locked, the FLL bypass mdoe is then set to \ref CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT.
*
* \note 
* Call \ref SystemCoreClockUpdate after calling this function  
* if it affects the CLK_HF0 frequency.
*
* \note 
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the FLL is the source of CLK_HF0 and the CLK_HF0 frequency is increasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_FllEnable
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_FllEnable(uint32_t timeoutus)
{
    bool zeroTimeout = (0UL == timeoutus);

    /* first set the CCO enable bit */
    SRSS_CLK_FLL_CONFIG4 |= SRSS_CLK_FLL_CONFIG4_CCO_ENABLE_Msk;

    /* Wait until CCO is ready */
    for (; (!_FLD2BOOL(SRSS_CLK_FLL_STATUS_CCO_READY, SRSS_CLK_FLL_STATUS)) && /* if cco_ready == 0 */
           (0UL != timeoutus);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }

    /* Set the FLL bypass mode to FLL_REF */
    CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);

    /* Set the FLL enable bit, if CCO is ready */
    if (zeroTimeout || (0UL != timeoutus))
    {
        SRSS_CLK_FLL_CONFIG |= SRSS_CLK_FLL_CONFIG_FLL_ENABLE_Msk;
    }

    /* now do the timeout wait for FLL_STATUS, bit LOCKED */
    for (; (!Cy_SysClk_FllLocked()) && /* if locked == 0 */
           (0UL != timeoutus);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }

    if (zeroTimeout || (0UL != timeoutus))
    { 
        /* Set the FLL bypass mode to FLL_OUT (ignoring lock indicator). */
        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
    }
    else
    {
        /* If lock doesn't occur, FLL is stopped. */
        (void)Cy_SysClk_FllDisable();
    }

    return ((zeroTimeout || (0UL != timeoutus)) ? CY_SYSCLK_SUCCESS : CY_SYSCLK_TIMEOUT);
}
/** \} group_sysclk_fll_funcs */


/* ========================================================================== */
/* ===========================    PLL SECTION    ============================ */
/* ========================================================================== */

/* PLL OUTPUT_DIV bitfield allowable range */
#define CY_SYSCLK_PLL_MIN_OUTPUT_DIV   (2UL)
#define CY_SYSCLK_PLL_MAX_OUTPUT_DIV   (16UL)

/* PLL REFERENCE_DIV bitfield allowable range */
#define CY_SYSCLK_PLL_MIN_REF_DIV      (1UL)
#define CY_SYSCLK_PLL_MAX_REF_DIV      (18UL)

/* PLL FEEDBACK_DIV bitfield allowable ranges, LF and normal modes */
#define CY_SYSCLK_PLL_MIN_FB_DIV_LF    (19UL)
#define CY_SYSCLK_PLL_MAX_FB_DIV_LF    (56UL)
#define CY_SYSCLK_PLL_MIN_FB_DIV_NORM  (22UL)
#define CY_SYSCLK_PLL_MAX_FB_DIV_NORM  (112UL)

/* PLL FEEDBACK_DIV bitfield allowable range selection */
#define CY_SYSCLK_PLL_MIN_FB_DIV       ((config->lfMode) ? CY_SYSCLK_PLL_MIN_FB_DIV_LF : CY_SYSCLK_PLL_MIN_FB_DIV_NORM)
#define CY_SYSCLK_PLL_MAX_FB_DIV       ((config->lfMode) ? CY_SYSCLK_PLL_MAX_FB_DIV_LF : CY_SYSCLK_PLL_MAX_FB_DIV_NORM)

/* PLL Fvco range allowable ranges, LF and normal modes */
#define CY_SYSCLK_PLL_MIN_FVCO_LF      (170000000UL)
#define CY_SYSCLK_PLL_MAX_FVCO_LF      (200000000UL)
#define CY_SYSCLK_PLL_MIN_FVCO_NORM    (200000000UL)
#define CY_SYSCLK_PLL_MAX_FVCO_NORM    (400000000UL)
/* PLL Fvco range selection */
#define CY_SYSCLK_PLL_MIN_FVCO         ((config->lfMode) ? CY_SYSCLK_PLL_MIN_FVCO_LF : CY_SYSCLK_PLL_MIN_FVCO_NORM)
#define CY_SYSCLK_PLL_MAX_FVCO         ((config->lfMode) ? CY_SYSCLK_PLL_MAX_FVCO_LF : CY_SYSCLK_PLL_MAX_FVCO_NORM)

/* PLL input and output frequency limits */
#define CY_SYSCLK_PLL_MIN_IN_FREQ  (4000000UL)
#define CY_SYSCLK_PLL_MAX_IN_FREQ  (64000000UL)
#define CY_SYSCLK_PLL_MIN_OUT_FREQ (CY_SYSCLK_PLL_MIN_FVCO / CY_SYSCLK_PLL_MAX_OUTPUT_DIV)
#define CY_SYSCLK_PLL_MAX_OUT_FREQ (150000000UL)

/**
* \addtogroup group_sysclk_pll_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_PllConfigure
****************************************************************************//**
*
* Configures a given PLL.
* The configuration formula used is:
*   Fout = pll_clk * (P / Q / div_out), where:
*     Fout is the desired output frequency
*     pll_clk is the frequency of the input source
*     P is the feedback divider. Its value is in bitfield FEEDBACK_DIV.
*     Q is the reference divider. Its value is in bitfield REFERENCE_DIV.
*     div_out is the reference divider. Its value is in bitfield OUTPUT_DIV.
*
* \param clkPath Selects which PLL to configure. 1 is the first PLL; 0 is invalid.
*
* \param config \ref cy_stc_pll_config_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - PLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - Invalid clock path number, or input or desired output frequency is out of valid range
*
* \note
* Call this function after changing the PLL input frequency, for example if
* \ref Cy_SysClk_ClkPathSetSource() is called.
*
* \note
* Do not call this function when the PLL is enabled. If it is called, then this function
* returns immediately with an error return value and no register updates.
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note 
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the PLL is the source of CLK_HF0 and the PLL frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the PLL is the source of CLK_HF0 and the PLL frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PllConfigure
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_PllConfigure(uint32_t clkPath, const cy_stc_pll_config_t *config)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_SUCCESS;

    if (((config->inputFreq)  < CY_SYSCLK_PLL_MIN_IN_FREQ)  || (CY_SYSCLK_PLL_MAX_IN_FREQ  < (config->inputFreq)) ||
        ((config->outputFreq) < CY_SYSCLK_PLL_MIN_OUT_FREQ) || (CY_SYSCLK_PLL_MAX_OUT_FREQ < (config->outputFreq)))
    {
        retVal = CY_SYSCLK_BAD_PARAM;
    }
    else
    { 
        cy_stc_pll_manual_config_t manualConfig;
        manualConfig.feedbackDiv = 0U;
        manualConfig.referenceDiv = 0U;
        manualConfig.outputDiv = 0U;

        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (config->outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            /* for each possible value of OUTPUT_DIV and REFERENCE_DIV (Q), try
               to find a value for FEEDBACK_DIV (P) that gives an output frequency
               as close as possible to the desired output frequency. */
            uint32_t p, q, out;
            uint32_t foutBest = 0UL; /* to ensure at least one pass through the for loops below */

            /* REFERENCE_DIV (Q) selection */
            for (q = CY_SYSCLK_PLL_MIN_REF_DIV; (q <= CY_SYSCLK_PLL_MAX_REF_DIV) && (foutBest != (config->outputFreq)); q++)
            {
                /* FEEDBACK_DIV (P) selection */
                for (p = CY_SYSCLK_PLL_MIN_FB_DIV; (p <= CY_SYSCLK_PLL_MAX_FB_DIV) && (foutBest != (config->outputFreq)); p++)
                {
                    /* Calculate the intermediate Fvco, and make sure that it's in range. */
                    uint32_t fvco = (uint32_t)(((uint64_t)(config->inputFreq) * (uint64_t)p) / (uint64_t)q);
                    if ((CY_SYSCLK_PLL_MIN_FVCO <= fvco) && (fvco <= CY_SYSCLK_PLL_MAX_FVCO))
                    {
                        /* OUTPUT_DIV selection */
                        for (out = CY_SYSCLK_PLL_MIN_OUTPUT_DIV; (out <= CY_SYSCLK_PLL_MAX_OUTPUT_DIV) && (foutBest != (config->outputFreq)); out++)
                        {
                            /* Calculate what output frequency will actually be produced. 
                               If it's closer to the target than what we have so far, then save it. */
                            uint32_t fout = ((p * config->inputFreq) / q) / out;
                            if ((uint32_t)abs((int32_t)fout - (int32_t)(config->outputFreq)) <
                                (uint32_t)abs((int32_t)foutBest - (int32_t)(config->outputFreq)))
                            {
                                foutBest = fout;
                                manualConfig.feedbackDiv  = (uint8_t)p;
                                manualConfig.referenceDiv = (uint8_t)q;
                                manualConfig.outputDiv    = (uint8_t)out;
                            }
                        }
                    }
                }
            }
            /* exit loops if foutBest equals outputFreq */
        } /* if not, bypass output mode */

        /* configure PLL based on calculated values */
        manualConfig.lfMode     = config->lfMode;
        manualConfig.outputMode = config->outputMode;
        retVal = Cy_SysClk_PllManualConfigure(clkPath, &manualConfig);

    } /* if no error */

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PllManualConfigure
****************************************************************************//**
*
* Manually configures a PLL based on user inputs.
*
* \param clkPath Selects which PLL to configure. 1 is the first PLL; 0 is invalid.
*
* \param config \ref cy_stc_pll_manual_config_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - PLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \note
* Call this function after changing the PLL input frequency; for example if
* \ref Cy_SysClk_ClkPathSetSource() is called.
*
* \note
* Do not call this function when the PLL is enabled. If it is called, then this function
* returns immediately with an error return value and no register updates.
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the PLL is the source of CLK_HF0 and the PLL frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the PLL is the source of CLK_HF0 and the PLL frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PllManualConfigure
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_PllManualConfigure(uint32_t clkPath, const cy_stc_pll_manual_config_t *config)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_SUCCESS;
    clkPath--; /* to correctly access PLL config registers structure */
    /* check for errors */
    if (clkPath >= CY_SRSS_NUM_PLL) /* invalid clock path number */
    {
        retVal = CY_SYSCLK_BAD_PARAM;
    }
    else if (_FLD2BOOL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS_CLK_PLL_CONFIG[clkPath]))
    {
        retVal = CY_SYSCLK_INVALID_STATE;
    }
    /* valid divider bitfield values */
    else if ((config->outputDiv    < CY_SYSCLK_PLL_MIN_OUTPUT_DIV) || (CY_SYSCLK_PLL_MAX_OUTPUT_DIV < config->outputDiv)    ||
             (config->referenceDiv < CY_SYSCLK_PLL_MIN_REF_DIV)    || (CY_SYSCLK_PLL_MAX_REF_DIV    < config->referenceDiv) ||
             (config->feedbackDiv  < CY_SYSCLK_PLL_MIN_FB_DIV)     || (CY_SYSCLK_PLL_MAX_FB_DIV     < config->feedbackDiv))
    {
         retVal = CY_SYSCLK_BAD_PARAM;
    }
    else /* no errors */
    {
        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the frequency parameters. */
        if (config->outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            SRSS_CLK_PLL_CONFIG[clkPath] =
                _VAL2FLD(SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV,  config->feedbackDiv)  |
                _VAL2FLD(SRSS_CLK_PLL_CONFIG_REFERENCE_DIV, config->referenceDiv) |
                _VAL2FLD(SRSS_CLK_PLL_CONFIG_OUTPUT_DIV,    config->outputDiv)    |
                _VAL2FLD(SRSS_CLK_PLL_CONFIG_PLL_LF_MODE,   config->lfMode);
        }

        CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[clkPath], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, (uint32_t)config->outputMode);
    }

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PllGetConfiguration
****************************************************************************//**
*
* Reports configuration settings for a PLL.
*
* \param clkPath Selects which PLL to report. 1 is the first PLL; 0 is invalid.
*
* \param config \ref cy_stc_pll_manual_config_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL data successfully reported<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PllGetConfiguration
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_PllGetConfiguration(uint32_t clkPath, cy_stc_pll_manual_config_t *config)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    clkPath--; /* to correctly access PLL config and status register structures */
    if (clkPath < CY_SRSS_NUM_PLL)
    {
        uint32_t tempReg = SRSS_CLK_PLL_CONFIG[clkPath];
        config->feedbackDiv  = (uint8_t)_FLD2VAL(SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV,  tempReg);
        config->referenceDiv = (uint8_t)_FLD2VAL(SRSS_CLK_PLL_CONFIG_REFERENCE_DIV, tempReg);
        config->outputDiv    = (uint8_t)_FLD2VAL(SRSS_CLK_PLL_CONFIG_OUTPUT_DIV,    tempReg);
        config->lfMode       =         _FLD2BOOL(SRSS_CLK_PLL_CONFIG_PLL_LF_MODE,   tempReg);
        config->outputMode   = (cy_en_fll_pll_output_mode_t)_FLD2VAL(SRSS_CLK_PLL_CONFIG_BYPASS_SEL, tempReg);
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PllEnable
****************************************************************************//**
*
* Enables the PLL. The PLL should be configured before calling this function.
*
* \param clkPath Selects which PLL to enable. 1 is the first PLL; 0 is invalid.
*
* \param timeoutus amount of time in microseconds to wait for the PLL to lock.
* If the lock doesn't occur, PLL is stopped. To avoid waiting for lock, set this to 0
* and manually check for lock using \ref Cy_SysClk_PllLocked.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for PLL lock<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \note 
* Call \ref SystemCoreClockUpdate after this function calling 
* if it affects the CLK_HF0 frequency.
*
* \note 
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is decreasing.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PllEnable
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_PllEnable(uint32_t clkPath, uint32_t timeoutus)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    clkPath--; /* to correctly access PLL config and status registers structures */
    if (clkPath < CY_SRSS_NUM_PLL)
    {
        /* first set the PLL enable bit */
        SRSS_CLK_PLL_CONFIG[clkPath] |= SRSS_CLK_PLL_CONFIG_ENABLE_Msk;

        /* now do the timeout wait for PLL_STATUS, bit LOCKED */
        for (; (0UL == (SRSS_CLK_PLL_STATUS_LOCKED_Msk & SRSS_CLK_PLL_STATUS[clkPath])) &&
               (0UL != timeoutus);
             timeoutus--)
        {
            Cy_SysLib_DelayUs(1U);
        }
        retVal = ((timeoutus == 0UL) ? CY_SYSCLK_TIMEOUT : CY_SYSCLK_SUCCESS);
    }
    return (retVal);
}
/** \} group_sysclk_pll_funcs */


/* ========================================================================== */
/* ====================    Clock Measurement section    ===================== */
/* ========================================================================== */
/* Slow control register default value */
#define TST_DDFT_SLOW_CTL_DEFAULT_VAL      (0x00001F1FUL)

/* Fast control register */
#define TST_DDFT_FAST_CTL_REG              (*(volatile uint32_t *) 0x40260104U)

/* Slow control register default value */
#define TST_DDFT_FAST_CTL_DEFAULT_VAL      (0x00003D3DUL)

/* Define for select signal outputs in slow clock */
#define SRSS_CLK_OUTPUT_SLOW_MASK  ((uint32_t) SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Msk | \
                                               SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Msk)

/* Define for select signal outputs in fast clock */
#define SRSS_CLK_OUTPUT_FAST_MASK  ((uint32_t) SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Msk | \
                                               SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Msk)

/* Cy_SysClk_StartClkMeasurementCounters() input parameter saved for use later in other functions. */
static uint32_t clk1Count1;

/* These variables act as locks to prevent collisions between clock measurement and entry into
   Deep Sleep mode. See Cy_SysClk_DeepSleep(). */
static bool clkCounting = false;
static bool preventCounting = false;

/**
* \addtogroup group_sysclk_calclk_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_StartClkMeasurementCounters
****************************************************************************//**
*
* Assigns clocks to the clock measurement counters, and starts counting. The counters
* let you measure a clock frequency using another clock as a reference. There are two
* counters.
*
* - One counter (counter1), which is clocked by clock1, is loaded with an initial
*   value and counts down to zero.
* - The second counter (counter2), which is clocked by clock2, counts up until 
*   the first counter reaches zero.
*
* Either clock1 or clock2 can be a reference clock; the other clock becomes the
* measured clock. The reference clock frequency is always known.<br>
* After calling this function, call \ref Cy_SysClk_ClkMeasurementCountersDone()
* to determine when counting is done; that is, counter1 has counted down to zero.
* Then call \ref Cy_SysClk_ClkMeasurementCountersGetFreq() to calculate the frequency
* of the measured clock.
*
* \param clock1 The clock for counter1
*
* \param count1 The initial value for counter1, from which counter1 counts down to zero.
*
* \param clock2 The clock for counter2
*
* \return Error / status code:<br>
* CY_SYSCLK_INVALID_STATE if already doing a measurement<br>
* CY_SYSCLK_BAD_PARAM if invalid clock input parameter<br>
* else CY_SYSCLK_SUCCESS
*
* \note The counters are both 24-bit, so the maximum value of count1 is 0xFFFFFF.
* If clock2 frequency is greater than clock1, make sure that count1 is low enough
* that counter2 does not overflow before counter1 reaches zero.
* \note The time to complete a measurement is count1 / clock1 frequency.
* \note The clocks for both counters must have a nonzero frequency, or
* \ref Cy_SysClk_ClkMeasurementCountersGetFreq() incorrectly reports the result of the
* previous  measurement.
* \note Do not enter a device low power mode (Sleep, Deep Sleep) while doing a measurement;
* the measured clock frequency may not be accurate.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_StartClkMeasurementCounters
*
*******************************************************************************/
cy_en_sysclk_status_t Cy_SysClk_StartClkMeasurementCounters(cy_en_meas_clks_t clock1, uint32_t count1, cy_en_meas_clks_t clock2)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;

    uint32_t clkOutputSlowVal = 0UL;
    uint32_t clkOutputFastVal = 0UL;
    
    uint32_t clkOutputSlowMask = 0UL;
    uint32_t clkOutputFastMask = 0UL;
    
    /* Prepare values for measurement control registers */

    /* Connect the indicated clocks to the respective counters:

       if clock1 is a slow clock,
         select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL0, and SRSS_CLK_OUTPUT_FAST.FAST_SEL0 = SLOW_SEL0
       else if clock1 is a fast clock,
         select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL0,
       else error, do nothing and return.

       if clock2 is a slow clock,
         select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL1, and SRSS_CLK_OUTPUT_FAST.FAST_SEL1 = SLOW_SEL1
       else if clock2 is a fast clock,
         select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL1,
       else error, do nothing and return.
    */
    if ((clock1 < CY_SYSCLK_MEAS_CLK_LAST_CLK) && (clock2 < CY_SYSCLK_MEAS_CLK_LAST_CLK) &&
        (count1 <= (SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Msk >> SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Pos)))
    {
        /* Disallow entry into Deep Sleep mode while counting. */
        clkCounting = true;

        if (clock1 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
        { /* slow clock */
            clkOutputSlowVal |= _VAL2FLD(SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0, (uint32_t)clock1);
            clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, 7UL/*slow_sel0 output*/);

            clkOutputSlowMask |= SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Msk;
            clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;
        }
        else
        { /* fast clock */
            if (clock1 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
            { /* ECO, EXT, ALTHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, (uint32_t)clock1);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;
            }
            else
            { /* PATH or CLKHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, (((uint32_t)clock1 >> 8) & 0xFUL) /*use enum bits [11:8]*/);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;

                if (clock1 < CY_SYSCLK_MEAS_CLK_CLKHFS)
                { /* PATH select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_PATH_SEL0, ((uint32_t)clock1 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Msk;
                }
                else
                { /* CLKHF select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0, ((uint32_t)clock1 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Msk;
                }
            }
        } /* clock1 fast clock */

        if (clock2 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
        { /* slow clock */
            clkOutputSlowVal |= _VAL2FLD(SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1, (uint32_t)clock2);
            clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, 7UL/*slow_sel1 output*/);

            clkOutputSlowMask |= SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Msk;
            clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;
        }
        else
        { /* fast clock */
            if (clock2 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
            { /* ECO, EXT, ALTHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, (uint32_t)clock2);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;
            }
            else
            { /* PATH or CLKHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, (((uint32_t)clock2 >> 8) & 0xFUL) /*use enum bits [11:8]*/);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;

                if (clock2 < CY_SYSCLK_MEAS_CLK_CLKHFS)
                { /* PATH select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_PATH_SEL1, ((uint32_t)clock2 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Msk;
                }
                else
                { /* CLKHF select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1, ((uint32_t)clock2 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Msk;
                }
            }
        } /* clock2 fast clock */

        /* Acquire the IPC to prevent changing of the shared resources at the same time */
        while(0U == (IPC_STRUCT_ACQUIRE_SUCCESS_Msk & REG_IPC_STRUCT_ACQUIRE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT))))
        {
            /* Wait until the IPC structure is released by another process */
        }

        if ((!preventCounting) /* don't start a measurement if about to enter Deep Sleep mode */  ||
            (_FLD2VAL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS_CLK_CAL_CNT1) != 0ul/*1 = done*/))
        {
            /* Set default values for counters measurement control registers */
            SRSS_TST_DDFT_SLOW_CTL_REG = TST_DDFT_SLOW_CTL_DEFAULT_VAL;
            SRSS_TST_DDFT_FAST_CTL_REG = TST_DDFT_FAST_CTL_DEFAULT_VAL;

            SRSS_CLK_OUTPUT_SLOW = ((SRSS_CLK_OUTPUT_SLOW & ((uint32_t) ~clkOutputSlowMask)) | clkOutputSlowVal);
            SRSS_CLK_OUTPUT_FAST = ((SRSS_CLK_OUTPUT_FAST & ((uint32_t) ~clkOutputFastMask)) | clkOutputFastVal);

            /* Save this input parameter for use later, in other functions.
               No error checking is done on this parameter */
            clk1Count1 = count1;

            /* Counting starts when counter1 is written with a nonzero value */
            SRSS_CLK_CAL_CNT1 = clk1Count1;

            retVal = CY_SYSCLK_SUCCESS;
        }

        /* Release the IPC */
        REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 0U;
    }

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkMeasurementCountersGetFreq
****************************************************************************//**
*
* Calculates the frequency of the indicated measured clock (clock1 or clock2).
* 
* - If clock1 is the measured clock, its frequency is:<br>
*   clock1 frequency = (count1 / count2) * clock2 frequency
* - If clock2 is the measured clock, its frequency is:<br>
*   clock2 frequency = (count2 / count1) * clock1 frequency
*
* Call this function only after counting is done; see \ref Cy_SysClk_ClkMeasurementCountersDone().
*
* \param measuredClock False (0) if the measured clock is clock1; true (1)
* if the measured clock is clock2.
*
* \param refClkFreq The reference clock frequency (clock1 or clock2).
*
* \return The frequency of the measured clock, in Hz.
* \warning The function returns zero, if during measurement device was in the 
* Deep Sleep or partially blocking flash operation occurred. It means that 
* current measurement is not valid and you should call the 
* Cy_SysClk_StartClkMeasurementCounters() function once again.
*
* \funcusage
* Refer to the Cy_SysClk_StartClkMeasurementCounters() function usage.
*
*******************************************************************************/
uint32_t Cy_SysClk_ClkMeasurementCountersGetFreq(bool measuredClock, uint32_t refClkFreq)
{
    uint32_t retVal = 0UL;
    bool isMeasurementValid = false;

    /* Done counting; allow entry into Deep Sleep mode */
    clkCounting = false;

    /* Acquire the IPC to prevent changing of the shared resources at the same time */
    while(0U == (IPC_STRUCT_ACQUIRE_SUCCESS_Msk & REG_IPC_STRUCT_ACQUIRE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT))))
    {
        /* Wait until the IPC structure is released by another process */
    }

    /* Check whether the device was in the Deep Sleep mode or the flash partially blocked while the
    *  operation was done
    */
    isMeasurementValid = ((SRSS_TST_DDFT_SLOW_CTL_REG == TST_DDFT_SLOW_CTL_DEFAULT_VAL) && 
                          (SRSS_TST_DDFT_FAST_CTL_REG == TST_DDFT_FAST_CTL_DEFAULT_VAL));

    retVal = _FLD2VAL(SRSS_CLK_CAL_CNT2_CAL_COUNTER2, SRSS_CLK_CAL_CNT2);
    /* Release the IPC */
    REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 0U;

    if (isMeasurementValid && (0UL != retVal))
    {
        if (!measuredClock)
        {   /* clock1 is the measured clock */
            retVal = (uint32_t)CY_SYSCLK_DIV_ROUND((uint64_t)clk1Count1 * (uint64_t)refClkFreq, (uint64_t)retVal);
        }
        else
        {   /* clock2 is the measured clock */
            retVal = (uint32_t)CY_SYSCLK_DIV_ROUND((uint64_t)retVal * (uint64_t)refClkFreq, (uint64_t)clk1Count1);
        }
    }
    else
    {
        /* Return zero value to indicate invalid measurement */
        retVal = 0UL;
    }
    
    return (retVal);
}
/** \} group_sysclk_calclk_funcs */


/* ========================================================================== */
/* ==========================    TRIM SECTION    ============================ */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_trim_funcs
* \{
*/

/*******************************************************************************
* Function Name: Cy_SysClk_IloTrim
****************************************************************************//**
*
* Trims the ILO to be as close to 32,768 Hz as possible.
*
* \param iloFreq current ILO frequency. Call \ref Cy_SysClk_StartClkMeasurementCounters
* and other measurement functions to obtain the current frequency of the ILO.
*
* \return Change in trim value - 0 if done; that is, no change in trim value.
*
* \note The watchdog timer (WDT) must be unlocked before calling this function.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_IloTrim
*
*******************************************************************************/
/** \cond INTERNAL */
/* target frequency */
#define CY_SYSCLK_ILO_TARGET_FREQ  (32768UL)
/* Nominal trim step size is 1.5% of "the frequency". Using the target frequency. */
#define CY_SYSCLK_ILO_TRIM_STEP    (CY_SYSCLK_DIV_ROUND(CY_SYSCLK_ILO_TARGET_FREQ * 15UL, 1000UL))
/** \endcond */

int32_t Cy_SysClk_IloTrim(uint32_t iloFreq)
{
    uint32_t diff;
    bool sign = false;

    if(iloFreq > (CY_SYSCLK_ILO_TARGET_FREQ + CY_SYSCLK_ILO_TRIM_STEP))
    {
        diff = iloFreq - CY_SYSCLK_ILO_TARGET_FREQ;
    }
    else if (iloFreq < (CY_SYSCLK_ILO_TARGET_FREQ - CY_SYSCLK_ILO_TRIM_STEP))
    {
        diff = CY_SYSCLK_ILO_TARGET_FREQ - iloFreq;
        sign = true;
    }
    else
    {
        diff = 0UL;
    }

    /* Do nothing if iloFreq is already within one trim step from the target */
    if(0UL != diff)
    {
        /* Get current trim value */
        uint32_t trim = _FLD2VAL(SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM, SRSS_CLK_TRIM_ILO_CTL);
        
        diff = CY_SYSCLK_DIV_ROUND(diff, CY_SYSCLK_ILO_TRIM_STEP);
            
        if(sign)
        {
            trim += diff;
        }
        else
        {
            trim -= diff;
        }

        /* Update the trim value */
        CY_REG32_CLR_SET(SRSS_CLK_TRIM_ILO_CTL, SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM, trim);
    }
    
    return (sign ? (int32_t)diff : (0L - (int32_t)diff));
}

/*******************************************************************************
* Function Name: Cy_SysClk_PiloTrim
****************************************************************************//**
*
* Trims the PILO to be as close to 32,768 Hz as possible.
*
* \param piloFreq current PILO frequency. Call \ref Cy_SysClk_StartClkMeasurementCounters
* and other measurement functions to obtain the current frequency of the PILO.
*
* \return Change in trim value; 0 if done, that is, no change in trim value.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PiloTrim
*
*******************************************************************************/
/** \cond INTERNAL */
/* target frequency */
#define CY_SYSCLK_PILO_TARGET_FREQ  (32768UL)
/* nominal trim step size */
#define CY_SYSCLK_PILO_TRIM_STEP        (5UL)
/** \endcond */

int32_t Cy_SysClk_PiloTrim(uint32_t piloFreq)
{
    uint32_t diff;
    bool sign = false;

    if(piloFreq > (CY_SYSCLK_PILO_TARGET_FREQ + CY_SYSCLK_PILO_TRIM_STEP))
    {
        diff = piloFreq - CY_SYSCLK_PILO_TARGET_FREQ;
    }
    else if (piloFreq < (CY_SYSCLK_PILO_TARGET_FREQ - CY_SYSCLK_PILO_TRIM_STEP))
    {
        diff = CY_SYSCLK_PILO_TARGET_FREQ - piloFreq;
        sign = true;
    }
    else
    {
        diff = 0UL;
    }

    /* Do nothing if piloFreq is already within one trim step from the target */
    if(0UL != diff)
    {
        /* Get current trim value */
        uint32_t trim = Cy_SysClk_PiloGetTrim();

        diff = CY_SYSCLK_DIV_ROUND(diff, CY_SYSCLK_PILO_TRIM_STEP);

        if(sign)
        {/* piloFreq too low. Increase the trim value. */
            trim += diff;
            if (trim >= SRSS_CLK_PILO_CONFIG_PILO_FFREQ_Msk) /* limit overflow */
            {
                trim = SRSS_CLK_PILO_CONFIG_PILO_FFREQ_Msk;
            }
        }
        else
        {/* piloFreq too high. Decrease the trim value. */
            trim -= diff;
            if ((int32_t)trim < 0) /* limit underflow */
            {
                trim = 0UL;
            }
        }

        /* Update the trim value */
        Cy_SysClk_PiloSetTrim(trim);
    }

    return (sign ? (int32_t)diff : (0L - (int32_t)diff));
}
/** \} group_sysclk_trim_funcs */


/* ========================================================================== */
/* ======================    POWER MANAGEMENT SECTION    ==================== */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_pm_funcs
* \{
*/
/** \cond INTERNAL */
/* timeout count for use in function Cy_SysClk_DeepSleepCallback() is sufficiently large for ~1 second */
#define TIMOUT_US 1000000UL
/** \endcond */

/*******************************************************************************
* Function Name: Cy_SysClk_DeepSleepCallback
****************************************************************************//**
*
* Callback function to be used when entering chip Deep Sleep mode. This function is
* applicable for when either the FLL or the PLL is enabled. It performs the following:
*
* 1. Before entering Deep Sleep, the clock configuration is saved in SRAM. If the
*    FLL/PLL source is the ECO, then the source is updated to the IMO.
* 2. Upon wakeup from Deep Sleep, the function restores the configuration and 
*    waits for the FLL/PLL to regain their frequency locks.
*
* The function prevents entry into Deep Sleep mode if the measurement counters
* are currently counting; see \ref Cy_SysClk_StartClkMeasurementCounters.
*
* This function can be called during execution of \ref Cy_SysPm_CpuEnterDeepSleep.
* To do so, register this function as a callback before calling
* \ref Cy_SysPm_CpuEnterDeepSleep - specify \ref CY_SYSPM_DEEPSLEEP as the callback
* type and call \ref Cy_SysPm_RegisterCallback.
*
* \note This function is recommended to be the last callback that is registered.
* Doing so minimizes the time spent on low power mode entry and exit. In the case
* where the ECO sources the FLL/PLL, this also allows the ECO to stabilize before
* reconnecting it to the FLL or PLL.
*
* \param callbackParams
* structure with the syspm callback parameters,
* see \ref cy_stc_syspm_callback_params_t.
*
* \param mode
* Callback mode, see \ref cy_en_syspm_callback_mode_t
*
* \return Error / status code; see \ref cy_en_syspm_status_t. Pass if not doing
* a clock measurement, otherwise Fail. Timeout if timeout waiting for FLL or a PLL
* to regain its frequency lock.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_DeepSleepCallback
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysClk_DeepSleepCallback(cy_stc_syspm_callback_params_t * callbackParams, cy_en_syspm_callback_mode_t mode)
{
    /* bitmapped paths and roots that may be affected by FLL or PLL being sourced by ECO */
    static uint16_t changedSourcePaths;

    cy_en_syspm_status_t retVal = CY_SYSPM_FAIL;

    if (callbackParams != NULL)
    {
        retVal = CY_SYSPM_SUCCESS;
        
        switch (mode)
        {
            case CY_SYSPM_CHECK_READY:
                /* Don't allow entry into Deep Sleep mode if currently measuring a frequency. */
                if (clkCounting)
                {
                    retVal = CY_SYSPM_FAIL;
                }
                else 
                {
                    /* Indicating that we can go into Deep Sleep.
                     * Prevent starting a new clock measurement until
                     * after we've come back from Deep Sleep. 
                     */
                    preventCounting = true;
                }
                break;
            
            case CY_SYSPM_CHECK_FAIL:
                preventCounting = false;
                break;
        
            case CY_SYSPM_BEFORE_TRANSITION:
                {
                    uint32_t fllpll; /* 0 = FLL, all other values = a PLL */

                    /* initialize record of changed paths */
                    changedSourcePaths = 0U;

                    /* for FLL and each PLL, */
                    for (fllpll = 0UL; fllpll <= CY_SRSS_NUM_PLL; fllpll++)
                    {
                        /* If FLL or PLL is enabled, */
                        if (0UL != ((fllpll == 0UL) ? (_FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS_CLK_FLL_CONFIG)) : 
                                                      (_FLD2VAL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS_CLK_PLL_CONFIG[fllpll - 1UL]))))
                        {
                            /* and the FLL or PLL has ECO as a source, */
                            if (Cy_SysClk_ClkPathGetSource(fllpll) == CY_SYSCLK_CLKPATH_IN_ECO)
                            {
                                /* Change this FLL or PLL source to IMO */
                                (void)Cy_SysClk_ClkPathSetSource(fllpll, CY_SYSCLK_CLKPATH_IN_IMO);
                                /* keep a record that this FLL or PLL's source was changed from ECO */
                                changedSourcePaths |= (uint16_t)(1UL << fllpll);
                            }
                            
                            /* Set the FLL/PLL bypass mode to 2 */
                            if(fllpll == 0UL)
                            {
                                CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, (uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
                            }
                            else
                            {
                                CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[fllpll - 1UL], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, (uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
                            }
                        }
                    }
                }
                break;

        /* After return from Deep Sleep, for each FLL and PLL, if needed, restore the source to ECO.
           And block until the FLL or PLL has regained its frequency locks. */
            case CY_SYSPM_AFTER_TRANSITION:
                /* if any FLL/PLL was sourced by the ECO, timeout wait for the ECO to become fully stabilized again. */
                if (0U != changedSourcePaths)
                {
                    uint32_t timeoutus = TIMOUT_US;

                    /* Wait for CY_SYSCLK_ECOSTAT_STABLE */
                    for (; (CY_SYSCLK_ECOSTAT_STABLE != Cy_SysClk_EcoGetStatus()) && (0UL != timeoutus); timeoutus--)
                    {
                        Cy_SysLib_DelayUs(1U);
                    }
                    
                    if (0UL == timeoutus)
                    {
                        retVal = CY_SYSPM_TIMEOUT;
                    }
                    else
                    {
                        uint32_t fllpll; /* 0 = FLL, all other values = PLL */

                        for (fllpll = 0UL; fllpll <= CY_SRSS_NUM_PLL; fllpll++)
                        {
                            /* If FLL or PLL is enabled, */
                            if (0UL != ((fllpll == 0UL) ? (_FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS_CLK_FLL_CONFIG)) : 
                                                          (_FLD2VAL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS_CLK_PLL_CONFIG[fllpll - 1UL]))))
                            {
                                uint32_t timeoutus = TIMOUT_US;
                                
                                /* check the record that this FLL or PLL's source was changed from ECO */
                                if ((changedSourcePaths & (uint16_t)(1UL << fllpll)) != 0U)
                                {
                                    /* Change this FLL or PLL source back to ECO */
                                    (void)Cy_SysClk_ClkPathSetSource(fllpll, CY_SYSCLK_CLKPATH_IN_ECO);
                                }
                                
                                /* Timeout wait for FLL or PLL to regain lock. */
                                for (; ((fllpll == 0UL) ? Cy_SysClk_FllLocked() : Cy_SysClk_PllLocked(fllpll)) && (0UL != timeoutus); timeoutus--)
                                {
                                    Cy_SysLib_DelayUs(1U);
                                }
                                
                                if (0UL == timeoutus)
                                {
                                    retVal = CY_SYSPM_TIMEOUT;
                                }
                                else
                                {
                                    /* Set the FLL/PLL bypass mode to OUTPUT */
                                    if(fllpll == 0UL)
                                    {
                                        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, (uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
                                    }
                                    else
                                    {
                                        CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[fllpll - 1UL], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, (uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
                                    }
                                }
                            }
                        }
                    }
                }
                
                /* Allow clock measurement. */
                preventCounting = false;
                break;
        }
    }
    return retVal;
}
/** \} group_sysclk_pm_funcs */


/* ========================================================================== */
/* =====================    clk_peripherals SECTION    ====================== */
/* ========================================================================== */


/**
* \addtogroup group_sysclk_clk_peripheral_funcs
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetFrequency
****************************************************************************//**
*
* Reports the frequency of the output of a given peripheral divider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
* \return The frequency, in Hz.
*
* \note
* The reported frequency may be zero, which indicates unknown. This happens if
* the source input is clk_ext, ECO, clk_althf, dsi_out, or clk_altlf.
*
* \funcusage
* \snippet sysclk/1.20/snippet/main.c snippet_Cy_SysClk_PeriphGetFrequency
*
*******************************************************************************/
uint32_t Cy_SysClk_PeriphGetFrequency(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    /* variables holding intermediate clock frequencies, dividers and FLL/PLL settings */
    bool     enabled = false;      /* FLL or PLL enable status; n/a for direct */
    uint32_t freq = 0UL;           /* path (FLL, PLL, or direct) frequency, in Hz, 0 = unknown frequency */
    uint32_t rootDiv = 0UL;        /* root prescaler (1/2/4/8) */
    uint32_t periDiv = 0UL;        /* predivider to clk_peri */
    uint32_t fDiv = 0UL;           /* FLL/PLL multiplier/feedback divider */
    uint32_t rDiv = 0UL;           /* FLL/PLL reference divider */
    uint32_t oDiv = 0UL;           /* FLL/PLL output divider */
    uint32_t integer = 0UL;        /* Integer part of peripheral divider */
    uint32_t frac = 0UL;           /* Fractional part of peripheral divider */
    cy_en_clkhf_in_sources_t    path   = Cy_SysClk_ClkHfGetSource(0UL); /* path input for root 0 (clkHf[0]) */
    cy_en_clkpath_in_sources_t  source = Cy_SysClk_ClkPathGetSource((uint32_t)path); /* source input for path (FLL, PLL, or direct) */

    CY_ASSERT_L1(((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    ||
                 ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   ||
                 ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) ||
                 ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)));

    /* get the frequency of the source, i.e., the path mux input */
    switch(source)
    {
        case CY_SYSCLK_CLKPATH_IN_IMO: /* IMO frequency is fixed at 8 MHz */
            freq = CY_SYSCLK_IMO_FREQ;
            break;
        case CY_SYSCLK_CLKPATH_IN_ILO: /* ILO, WCO and PILO frequencies are nominally 32.768 kHz */
        case CY_SYSCLK_CLKPATH_IN_WCO:
        case CY_SYSCLK_CLKPATH_IN_PILO:
            freq = CY_SYSCLK_ILO_FREQ;
            break;
        default:
            /* don't know the frequency of clk_ext, ECO, clk_althf, dsi_out, or clk_altlf */
            freq = 0UL; /* unknown frequency */
            break;
    }

    if (path == CY_SYSCLK_CLKHF_IN_CLKPATH0) /* FLL? (always path 0) */
    {
        cy_stc_fll_manual_config_t fllCfg = {0UL,0U,CY_SYSCLK_FLL_CCO_RANGE0,false,0U,0U,0U,0U,CY_SYSCLK_FLLPLL_OUTPUT_AUTO,0U};
        Cy_SysClk_FllGetConfiguration(&fllCfg);
        enabled = (_FLD2BOOL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS_CLK_FLL_CONFIG)) && (fllCfg.outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
        fDiv = fllCfg.fllMult;
        rDiv = fllCfg.refDiv;
        oDiv = (fllCfg.enableOutputDiv) ? 2UL : 1UL;
    }
    else if ((uint32_t)path <= CY_SRSS_NUM_PLL) /* PLL? (always path 1...N)*/
    {
        cy_stc_pll_manual_config_t pllcfg = {0U,0U,0U,false,CY_SYSCLK_FLLPLL_OUTPUT_AUTO};
        (void)Cy_SysClk_PllGetConfiguration((uint32_t)path, &pllcfg);
        enabled = (_FLD2BOOL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS_CLK_PLL_CONFIG[(uint32_t)path - 1UL])) && (pllcfg.outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
        fDiv = pllcfg.feedbackDiv;
        rDiv = pllcfg.referenceDiv;
        oDiv = pllcfg.outputDiv;
    }
    else
    {
        /* Direct select path  */
    }

    if (enabled) /* if FLL or PLL enabled and not bypassed */
    {
        freq = (uint32_t)(((uint64_t)freq * (uint64_t)fDiv) /
                          ((uint64_t)rDiv * (uint64_t)oDiv));
    }
    
    /* get the prescaler value for root 0, or clkHf[0]: 1/2/4/8 */
    rootDiv = 1UL << (uint32_t)Cy_SysClk_ClkHfGetDivider(0UL);

    /* get the predivider value for clkHf[0] to clk_peri */
    periDiv = 1UL + (uint32_t)Cy_SysClk_ClkPeriGetDivider();

    /* Divide the path input frequency down and return the result.
       Stepping through the following code shows the frequency at each stage.
    */
    freq /= (rootDiv * periDiv); /* clk_peri frequency */

    /* get the divider value for clk_peri to the selected peripheral clock */
    switch(dividerType)
    {
        case CY_SYSCLK_DIV_8_BIT:
        case CY_SYSCLK_DIV_16_BIT:
            integer = (uint32_t)Cy_SysClk_PeriphGetDivider(dividerType, dividerNum);
            /* frac = 0 means it is an integer divider */
            break;
        case CY_SYSCLK_DIV_16_5_BIT:
        case CY_SYSCLK_DIV_24_5_BIT:
            (void)Cy_SysClk_PeriphGetFracDivider(dividerType, dividerNum, &integer, &frac);
            break;
        default:
            break;
    }

    /* For fractional dividers, the divider is (int + 1) + frac/32.
     * Use the fractional value to round the divider to the nearest integer.
     */
    freq /= (integer + 1UL + ((frac >= 16UL) ? 1UL : 0UL)); /* peripheral divider output frequency */

    return freq;
}
/** \} group_sysclk_clk_peripheral_funcs */


/* [] END OF FILE */

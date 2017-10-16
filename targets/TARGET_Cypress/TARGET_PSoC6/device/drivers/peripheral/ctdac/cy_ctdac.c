/***************************************************************************//**
* \file cy_ctdac.c
* \version 1.0
*
* Provides the public functions for the API for the CTDAC driver.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ctdac/cy_ctdac.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Static function to configure the clock */
static void Cy_CTDAC_ConfigureClock(cy_en_ctdac_update_t updateMode, cy_en_divider_types_t dividerType,
                                           uint32_t dividerNum, uint32_t dividerIntValue, uint32_t dividerFracValue);

const cy_stc_ctdac_fast_config_t Cy_CTDAC_Fast_VddaRef_UnbufferedOut =
{
    /*.refSource      */ CY_CTDAC_REFSOURCE_VDDA,
    /*.outputBuffer   */ CY_CTDAC_OUTPUT_UNBUFFERED,
};

const cy_stc_ctdac_fast_config_t Cy_CTDAC_Fast_VddaRef_BufferedOut =
{
    /*.refSource      */ CY_CTDAC_REFSOURCE_VDDA,
    /*.outputBuffer   */ CY_CTDAC_OUTPUT_BUFFERED,
};

const cy_stc_ctdac_fast_config_t Cy_CTDAC_Fast_OA1Ref_UnbufferedOut =
{
    /*.refSource      */ CY_CTDAC_REFSOURCE_EXTERNAL,
    /*.outputBuffer   */ CY_CTDAC_OUTPUT_UNBUFFERED,
};

const cy_stc_ctdac_fast_config_t Cy_CTDAC_Fast_OA1Ref_BufferedOut =
{
    /*.refSource      */ CY_CTDAC_REFSOURCE_EXTERNAL,
    /*.outputBuffer   */ CY_CTDAC_OUTPUT_BUFFERED,
};

/*******************************************************************************
* Function Name: Cy_CTDAC_Init
****************************************************************************//**
*
* Initialize all CTDAC configuration registers
*
* \param base
* Pointer to structure describing registers
*
* \param config
* Pointer to structure containing configuration data
*
* \return cy_en_ctdac_status_t
* Status of initialization, CY_CTDAC_SUCCESS or CY_CTDAC_BAD_PARAM
*
*******************************************************************************/
cy_en_ctdac_status_t Cy_CTDAC_Init(CTDAC_Type *base, const cy_stc_ctdac_config_t *config)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != config);

    cy_en_ctdac_status_t result;
    uint32_t ctdacCtrl = CY_CTDAC_DEINIT;
    uint32_t setSwitch = CY_CTDAC_DEINIT;
    uint32_t clearSwitch = CY_CTDAC_DEINIT;

    if ((NULL == base) || (NULL == config))
    {
       result = CY_CTDAC_BAD_PARAM;
    }
    else
    {

        CY_ASSERT_L3(CY_CTDAC_REFSOURCE(config->refSource));
        CY_ASSERT_L3(CY_CTDAC_FORMAT(config->formatMode));
        CY_ASSERT_L3(CY_CTDAC_UPDATE(config->updateMode));
        CY_ASSERT_L3(CY_CTDAC_DEGLITCH(config->deglitchMode));
        CY_ASSERT_L3(CY_CTDAC_OUTPUTMODE(config->outputMode));
        CY_ASSERT_L3(CY_CTDAC_OUTPUTBUFFER(config->outputBuffer));
        CY_ASSERT_L3(CY_CTDAC_DEEPSLEEP(config->deepSleep));
        CY_ASSERT_L2(CY_CTDAC_DEGLITCHCYCLES(config->deglitchCycles));

        /* Handle the deglitch counts */
        ctdacCtrl |= (config->deglitchCycles << CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Pos) & CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Msk;

        /* Handle the deglitch mode */
        ctdacCtrl |= (uint32_t)config->deglitchMode;

        /* Handle the update mode */
        if ((config->updateMode == CY_CTDAC_UPDATE_STROBE_EDGE_IMMEDIATE) \
        || (config->updateMode == CY_CTDAC_UPDATE_STROBE_EDGE_SYNC) \
        || (config->updateMode == CY_CTDAC_UPDATE_STROBE_LEVEL))
        {
            ctdacCtrl |= CTDAC_CTDAC_CTRL_DSI_STROBE_EN_Msk;
        }

        if (config->updateMode == CY_CTDAC_UPDATE_STROBE_LEVEL)
        {
            ctdacCtrl |= CTDAC_CTDAC_CTRL_DSI_STROBE_LEVEL_Msk;
        }

        /* Handle the sign format */
        ctdacCtrl |= (uint32_t)config->formatMode;

        /* Handle the Deep Sleep mode */
        ctdacCtrl |= (uint32_t)config->deepSleep;

        /* Handle the output mode */
        ctdacCtrl |= (uint32_t)config->outputMode;

        /* Handle enable bit */
        ctdacCtrl |= ((uint32_t)config->enable) << CTDAC_CTDAC_CTRL_ENABLED_Pos;

        /* Handle the reference source */
        switch(config->refSource)
        {
        case CY_CTDAC_REFSOURCE_VDDA:

            /* Close the CVD switch to use Vdda as the reference source */
            setSwitch |= CTDAC_CTDAC_SW_CTDD_CVD_Msk;
            break;
        case CY_CTDAC_REFSOURCE_EXTERNAL:
        default:
            clearSwitch |= CTDAC_CTDAC_SW_CLEAR_CTDD_CVD_Msk;
            break;
        }

        /* Handle the output buffer switch CO6 */
        switch(config->outputBuffer)
        {
        case CY_CTDAC_OUTPUT_UNBUFFERED:

            /* Close the CO6 switch to send output to a direct pin unbuffered */
            setSwitch |= CTDAC_CTDAC_SW_CTDO_CO6_Msk;
            break;
        case CY_CTDAC_OUTPUT_BUFFERED:
        default:
            clearSwitch |= CTDAC_CTDAC_SW_CTDO_CO6_Msk;
            break;
        }

        base->INTR_MASK         = (uint32_t)config->enableInterrupt << CTDAC_INTR_VDAC_EMPTY_Pos;
        base->CTDAC_SW          = setSwitch;
        base->CTDAC_SW_CLEAR    = clearSwitch;
        base->CTDAC_VAL         = (((uint32_t)config->value) << CTDAC_CTDAC_VAL_VALUE_Pos) & CTDAC_CTDAC_VAL_VALUE_Msk;
        base->CTDAC_VAL_NXT     = (((uint32_t)config->nextValue) << CTDAC_CTDAC_VAL_NXT_VALUE_Pos) & CTDAC_CTDAC_VAL_NXT_VALUE_Msk;

        if (config->configClock)
        {
            Cy_CTDAC_ConfigureClock(config->updateMode, config->dividerType, config->dividerNum, config->dividerIntValue, config->dividerFracValue);
        }

        base->CTDAC_CTRL        = ctdacCtrl;
        result                  = CY_CTDAC_SUCCESS;
    }

    return result;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_DeInit
****************************************************************************//**
*
* Reset CTDAC registers back to power on reset defaults.
*
* Does not disable or configure the clock. Call \ref Cy_CTDAC_Init or \ref Cy_CTDAC_FastInit
* to configure the CTDAC and clock.
*
* \param base
* Pointer to structure describing registers
*
* \param deInitRouting
* If true, all switches are reset to their default state.
* If false, switch registers are untouched.
*
* \return cy_en_ctdac_status_t
* Status of initialization, CY_CTDAC_SUCCESS, or CY_CTDAC_BAD_PARAM
*
*******************************************************************************/
cy_en_ctdac_status_t Cy_CTDAC_DeInit(CTDAC_Type *base, bool deInitRouting)
{
    CY_ASSERT_L1(NULL != base);

    cy_en_ctdac_status_t result;

    if (NULL == base)
    {
       result = CY_CTDAC_BAD_PARAM;
    }
    else
    {
        base->CTDAC_CTRL        = CY_CTDAC_DEINIT;
        base->INTR_MASK         = CY_CTDAC_DEINIT;
        base->CTDAC_VAL         = CY_CTDAC_DEINIT;
        base->CTDAC_VAL_NXT     = CY_CTDAC_DEINIT;

        if (deInitRouting)
        {
            base->CTDAC_SW_CLEAR = CY_CTDAC_DEINIT;
        }

        result                  = CY_CTDAC_SUCCESS;
    }

    return result;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_FastInit
****************************************************************************//**
*
* Initializes the CTDAC to one of the common use modes.
* These provide a quick and easy method of configuring the CTDAC when using
* the PDL driver for device configuration.
*
* The other configuration options are set to:
*   - .formatMode       = CY_CTDAC_FORMAT_UNSIGNED
*   - .updateMode       = CY_CTDAC_UPDATE_BUFFERED_WRITE
*   - .deglitchMode     = CY_CTDAC_DEGLITCHMODE_NONE
*   - .outputMode       = CY_CTDAC_OUTPUT_VALUE
*   - .deepSleep        = CY_CTDAC_DEEPSLEEP_DISABLE
*   - .deglitchCycles   = CY_CTDAC_DEINIT
*   - .value            = CY_CTDAC_UNSIGNED_MID_CODE_VALUE
*   - .nextValue        = CY_CTDAC_UNSIGNED_MID_CODE_VALUE
*   - .enableInterrupt  = true
*   - .configClock      = true
*   - .dividerType      = CY_CTDAC_FAST_CLKCFG_TYPE
*   - .dividerNum       = CY_CTDAC_FAST_CLKCFG_NUM
*   - .dividerInitValue = CY_CTDAC_FAST_CLKCFG_DIV
*   - .dividerFracValue = CY_CTDAC_DEINIT
*
* \param base
* Pointer to structure describing registers
*
* \param config
* Pointer to structure containing configuration data for quick initialization
* Use one of the provided structures or define your own.
* - \ref Cy_CTDAC_Fast_VddaRef_UnbufferedOut
* - \ref Cy_CTDAC_Fast_VddaRef_BufferedOut
* - \ref Cy_CTDAC_Fast_OA1Ref_UnbufferedOut
* - \ref Cy_CTDAC_Fast_OA1Ref_BufferedOut
*
* \return cy_en_ctdac_status_t
* Status of initialization, CY_CTDAC_SUCCESS or CY_CTDAC_BAD_PARAM
*
*******************************************************************************/
cy_en_ctdac_status_t Cy_CTDAC_FastInit(CTDAC_Type *base, const cy_stc_ctdac_fast_config_t *config)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != config);

    cy_en_ctdac_status_t result;
    uint32_t ctdacCtrl;
    uint32_t setSwitch = CY_CTDAC_DEINIT;
    uint32_t clearSwitch = CY_CTDAC_DEINIT;

    if ((NULL == base) || (NULL == config))
    {
       result = CY_CTDAC_BAD_PARAM;
    }
    else
    {
        CY_ASSERT_L3(CY_CTDAC_REFSOURCE(config->refSource));
        CY_ASSERT_L3(CY_CTDAC_OUTPUTBUFFER(config->outputBuffer));

        ctdacCtrl = (uint32_t) CY_CTDAC_DEGLITCHMODE_NONE \
                    | (uint32_t) CY_CTDAC_UPDATE_BUFFERED_WRITE \
                    | (uint32_t) CY_CTDAC_FORMAT_UNSIGNED \
                    | (uint32_t) CY_CTDAC_DEEPSLEEP_DISABLE \
                    | (uint32_t) CY_CTDAC_OUTPUT_VALUE;

        /* Handle the reference source */
        switch(config->refSource)
        {
        case CY_CTDAC_REFSOURCE_VDDA:

            /* Close the CVD switch to use Vdda as the reference source */
            setSwitch |= CTDAC_CTDAC_SW_CTDD_CVD_Msk;
            break;
        case CY_CTDAC_REFSOURCE_EXTERNAL:
        default:
            clearSwitch |= CTDAC_CTDAC_SW_CLEAR_CTDD_CVD_Msk;
            break;
        }

        /* Handle the output buffer switch CO6 */
        switch(config->outputBuffer)
        {
        case CY_CTDAC_OUTPUT_UNBUFFERED:

            /* Close the CO6 switch to send output to a direct pin unbuffered */
            setSwitch |= CTDAC_CTDAC_SW_CTDO_CO6_Msk;
            break;
        case CY_CTDAC_OUTPUT_BUFFERED:
        default:
            clearSwitch |= CTDAC_CTDAC_SW_CTDO_CO6_Msk;
            break;
        }

        base->INTR_MASK         = CTDAC_INTR_VDAC_EMPTY_Msk;
        base->CTDAC_SW          = setSwitch;
        base->CTDAC_SW_CLEAR    = clearSwitch;
        base->CTDAC_VAL         = CY_CTDAC_UNSIGNED_MID_CODE_VALUE;
        base->CTDAC_VAL_NXT     = CY_CTDAC_UNSIGNED_MID_CODE_VALUE;

        /* For fast configuration, the DAC clock is the Peri clock divided by 100 */
        Cy_CTDAC_ConfigureClock(CY_CTDAC_UPDATE_BUFFERED_WRITE, CY_CTDAC_FAST_CLKCFG_TYPE, CY_CTDAC_FAST_CLKCFG_NUM, CY_CTDAC_FAST_CLKCFG_DIV, CY_CTDAC_DEINIT);

        base->CTDAC_CTRL        = ctdacCtrl;
        result                  = CY_CTDAC_SUCCESS;
    }

    return result;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_ConfigureClock
****************************************************************************//**
*
* Private function for configuring the CTDAC clock based on the desired
* update mode. This function is called by \ref Cy_CTDAC_Init.
*
* \param updateMode
* Update mode value. See \ref cy_en_ctdac_update_t for values.
*
* \return None
*
*******************************************************************************/
static void Cy_CTDAC_ConfigureClock(cy_en_ctdac_update_t updateMode, cy_en_divider_types_t dividerType,
                                           uint32_t dividerNum, uint32_t dividerIntValue, uint32_t dividerFracValue)
{
    if (updateMode == CY_CTDAC_UPDATE_DIRECT_WRITE)
    { /* In direct mode, there is not a clock */
    }
    else if(updateMode == CY_CTDAC_UPDATE_STROBE_EDGE_IMMEDIATE)
    {

        /* In this mode, the Peri Clock is divided by 1 to give a constant logic high on the CTDAC clock. */
        (void)Cy_SysClk_PeriphDisableDivider(dividerType, dividerNum);

        (void)Cy_SysClk_PeriphAssignDivider(PCLK_PASS_CLOCK_CTDAC, dividerType, dividerNum);

        if ((dividerType == CY_SYSCLK_DIV_8_BIT) || (dividerType == CY_SYSCLK_DIV_16_BIT))
        {
            (void)Cy_SysClk_PeriphSetDivider(dividerType, dividerNum, CY_CTDAC_STROBE_EDGE_IMMEDIATE_DIV);
        }
        else
        {
            (void)Cy_SysClk_PeriphSetFracDivider(dividerType, dividerNum, CY_CTDAC_STROBE_EDGE_IMMEDIATE_DIV, CY_CTDAC_STROBE_EDGE_IMMEDIATE_DIV_FRAC);
        }

        (void)Cy_SysClk_PeriphEnableDivider(dividerType, dividerNum);
    }
    else
    {

        /* All other modes, require a CTDAC clock configured to the desired user frequency */
        (void)Cy_SysClk_PeriphDisableDivider(dividerType, dividerNum);

        (void)Cy_SysClk_PeriphAssignDivider(PCLK_PASS_CLOCK_CTDAC, dividerType, dividerNum);

        if ((dividerType == CY_SYSCLK_DIV_8_BIT) || (dividerType == CY_SYSCLK_DIV_16_BIT))
        {
            (void)Cy_SysClk_PeriphSetDivider(dividerType, dividerNum, dividerIntValue);
        }
        else
        {
            (void)Cy_SysClk_PeriphSetFracDivider(dividerType, dividerNum, dividerIntValue, dividerFracValue);
        }
        (void)Cy_SysClk_PeriphEnableDivider(dividerType, dividerNum);
    }

}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetSignMode
****************************************************************************//**
*
* Sets whether to interpret the DAC value as signed or unsigned.
* In unsigned mode, the DAC value register is used without any decoding.
* In signed  mode, the MSB is inverted by adding 0x800 to the DAC value.
* This converts the lowest signed number, 0x800, to the lowest unsigned
* number, 0x000.
*
* \param base
* Pointer to structure describing registers
*
* \param formatMode
* Mode can be signed or unsigned. See \ref cy_en_ctdac_format_t for values.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetSignMode(CTDAC_Type *base, cy_en_ctdac_format_t formatMode)
{
    CY_ASSERT_L3(CY_CTDAC_FORMAT(formatMode));

    uint32_t ctdacCtrl;

    /* Clear the CTDAC_MODE bits */
    ctdacCtrl = base->CTDAC_CTRL & ~CTDAC_CTDAC_CTRL_CTDAC_MODE_Msk;

    base->CTDAC_CTRL = ctdacCtrl | (uint32_t)formatMode;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetDeepSleepMode
****************************************************************************//**
*
* Sets whether to allow the DAC hardware to continue to stay powered in
* Deep Sleep mode.
*
* \param base
* Pointer to structure describing registers
*
* \param deepSleep
* Enabled or disabled. See \ref cy_en_ctdac_deep_sleep_t for values.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetDeepSleepMode(CTDAC_Type *base, cy_en_ctdac_deep_sleep_t deepSleep)
{
    CY_ASSERT_L3(CY_CTDAC_DEEPSLEEP(deepSleep));

    uint32_t ctdacCtrl;

    ctdacCtrl = base->CTDAC_CTRL & ~CTDAC_CTDAC_CTRL_DEEPSLEEP_ON_Msk;

    base->CTDAC_CTRL = ctdacCtrl | (uint32_t)deepSleep;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetOutputMode
****************************************************************************//**
*
* Set the output mode to one of the supported options.
*
* \param base
* Pointer to structure describing registers
*
* \param outputMode
* Output drive mode. See \ref cy_en_ctdac_output_mode_t for values.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetOutputMode(CTDAC_Type *base, cy_en_ctdac_output_mode_t outputMode)
{
    CY_ASSERT_L3(CY_CTDAC_OUTPUTMODE(outputMode));

    uint32_t ctdacCtrl;

    /* Clear out the three affected bits */
    ctdacCtrl = base->CTDAC_CTRL & ~(CTDAC_CTDAC_CTRL_OUT_EN_Msk | CTDAC_CTDAC_CTRL_DISABLED_MODE_Msk | CTDAC_CTDAC_CTRL_CTDAC_RANGE_Msk);

    base->CTDAC_CTRL = ctdacCtrl | (uint32_t)outputMode;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetDeglitchMode
****************************************************************************//**
*
* Set the deglitch mode or disable deglitching completely.
*
* \param base
* Pointer to structure describing registers
*
* \param deglitchMode
* Deglitching mode selection. See \ref cy_en_ctdac_deglitch_t for values.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetDeglitchMode(CTDAC_Type *base, cy_en_ctdac_deglitch_t deglitchMode)
{
    CY_ASSERT_L3(CY_CTDAC_DEGLITCH(deglitchMode));

    uint32_t ctdacCtrl;

    /* Clear out DEGLITCH_CO6 and DEGLITCH_C0S bits */
    ctdacCtrl = base->CTDAC_CTRL & ~(CTDAC_CTDAC_CTRL_DEGLITCH_COS_Msk | CTDAC_CTDAC_CTRL_DEGLITCH_CO6_Msk);

    base->CTDAC_CTRL = ctdacCtrl | (uint32_t)deglitchMode;
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetDeglitchCycles
****************************************************************************//**
*
* Set the number of deglitch cycles (0 to 63) that will be used.
* To calculate the deglitch time, (DEGLITCH_CNT + 1) / PERI_CLOCK_FREQ.
* The optimal deglitch time is 700 ns.
*
* \param base
* Pointer to structure describing registers
*
* \param deglitchCycles
* Number of cycles to deglitch
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetDeglitchCycles(CTDAC_Type *base, uint32_t deglitchCycles)
{
    CY_ASSERT_L2(CY_CTDAC_DEGLITCHCYCLES(deglitchCycles));

    uint32_t ctdacCtrl;

    ctdacCtrl = (base->CTDAC_CTRL) & ~CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Msk;

    base->CTDAC_CTRL = ctdacCtrl | ((deglitchCycles << CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Pos) & CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Msk);
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetRef
****************************************************************************//**
*
* Set the CTDAC reference source to Vdda or an external reference.
* The external reference must come from Opamp1 of the CTB.
*
* \param base
* Pointer to structure describing registers
*
* \param refSource
* The reference source. See \ref cy_en_ctdac_ref_source_t for values.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetRef(CTDAC_Type *base, cy_en_ctdac_ref_source_t refSource)
{
    CY_ASSERT_L3(CY_CTDAC_REFSOURCE(refSource));

    switch(refSource)
    {
    case CY_CTDAC_REFSOURCE_VDDA:

        /* Close the CVD switch to use Vdda as the reference source */
        base->CTDAC_SW |= CTDAC_CTDAC_SW_CTDD_CVD_Msk;
        break;
    case CY_CTDAC_REFSOURCE_EXTERNAL:
    default:
        base->CTDAC_SW_CLEAR = CTDAC_CTDAC_SW_CLEAR_CTDD_CVD_Msk;
        break;
    }
}

/*******************************************************************************
* Function Name: Cy_CTDAC_SetAnalogSwitch
****************************************************************************//**
*
* Provides firmware control of the CTDAC switches. Each call to this function
* can open a set of switches or close a set of switches.
*
* This is an advanced function. The switches will be managed by the reference
* source and output mode selections when initializing the hardware.
*
* \param base
* Pointer to structure describing registers
*
* \param switchMask
* The mask of the switches to either open or close.
* The switches can be found in the \ref cy_en_ctdac_switches_t enum.
*
* For example, to close both CVD and CO6 switches:
*   switchMask = CY_CTDAC_SWITCH_CVD_MASK | CY_CTDAC_SWITCH_CO6_MASK;
*
* \param state
* Open or close the switches, see enum \ref cy_en_ctdac_switch_state_t.
*
* \return None
*
*******************************************************************************/
void Cy_CTDAC_SetAnalogSwitch(CTDAC_Type *base, uint32_t switchMask, cy_en_ctdac_switch_state_t state)
{
    CY_ASSERT_L2(CY_CTDAC_SWITCHMASK(switchMask));
    CY_ASSERT_L3(CY_CTDAC_SWITCHSTATE(state));

    switch(state)
    {
    case CY_CTDAC_SWITCH_CLOSE:
        base->CTDAC_SW |= switchMask;
        break;
    case CY_CTDAC_SWITCH_OPEN:
    default:

        /* Unlike the close case, do not OR the register. Set 1 to clear.*/
        base->CTDAC_SW_CLEAR = switchMask;
        break;
    }
}

/*******************************************************************************
* Function Name: Cy_CTDAC_DeepSleepCallback
****************************************************************************//**
*
* Callback to prepare the CTDAC before entering and after exiting Deep Sleep
* mode. If deglitching is used, it is disabled before entering Deep Sleep
* to ensure the deglitch switches are closed. This is needed only
* if the CTDAC will be enabled in DeepSleep. Upon wakeup, deglitching will
* be re-enabled if it was previously used.
*
* \param callbackParams
* Pointer to structure of type \ref cy_stc_syspm_callback_params_t
*
* \return cy_en_syspm_status_t
* See \ref cy_en_syspm_status_t
*
*******************************************************************************/
cy_en_syspm_status_t Cy_CTDAC_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams)
{
    /** Static variable preserved between function calls.
    * Tracks the state of the deglitch mode before sleep so that it can be re-enabled after wakeup */
    static uint32_t deglitchModeBeforeSleep;

    cy_en_syspm_status_t returnValue = CY_SYSPM_SUCCESS;

    CTDAC_Type *ctdacBase = (CTDAC_Type *)callbackParams->base;

    if (CY_SYSPM_BEFORE_TRANSITION == callbackParams->mode)
    { /* Actions that should be done before entering the Deep Sleep mode */

        /* Store the state of the deglitch switches before turning deglitch off */
        deglitchModeBeforeSleep = ctdacBase->CTDAC_CTRL & (CTDAC_CTDAC_CTRL_DEGLITCH_CO6_Msk | CTDAC_CTDAC_CTRL_DEGLITCH_COS_Msk);

        /* Turn deglitch off before entering Deep Sleep */
        ctdacBase->CTDAC_CTRL &= ~(CTDAC_CTDAC_CTRL_DEGLITCH_CO6_Msk | CTDAC_CTDAC_CTRL_DEGLITCH_COS_Msk);
    }
    else if (CY_SYSPM_AFTER_TRANSITION == callbackParams->mode)
    { /* Actions that should be done after exiting the Deep Sleep mode */

        /* Re-enable the deglitch mode that was configured before Deep Sleep entry */
        ctdacBase->CTDAC_CTRL |= deglitchModeBeforeSleep;
    }
    else
    { /* Does nothing in other modes */
    }

    return returnValue;
}

#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */


/***************************************************************************//**
* \file cy_hal_system.c
*
* \brief
* Provides a high level interface for interacting with the Cypress power 
* management and system clock configuration. This interface abstracts out the 
* chip specific details. If any chip specific functionality is necessary, or 
* performance is critical the low level functions can be used directly.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cy_hal_system.h"

cy_rslt_t cy_system_sleep(void)
{
    return (cy_rslt_t) Cy_SysPm_CpuEnterSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
}


cy_rslt_t cy_system_deepsleep(void)
{
    return (cy_rslt_t) Cy_SysPm_CpuEnterDeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
}



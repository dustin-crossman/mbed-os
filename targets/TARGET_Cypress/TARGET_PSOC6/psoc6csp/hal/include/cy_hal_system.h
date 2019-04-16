/***************************************************************************//**
* \file cy_hal_system.h
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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Enter a critical section */
#define core_util_critical_section_enter() \
    uint32_t _last_irq_status_ = Cy_SysLib_EnterCriticalSection()
/** Exit a critical section */
#define core_util_critical_section_exit() \
     Cy_SysLib_ExitCriticalSection(_last_irq_status_)
	 
/** Send the device to sleep
 *
 * The processor is setup ready for sleep, and sent to sleep using __WFI(). In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs.

 * @return Returns CY_RSLT_SUCCESS if the processor successfully entered and exited sleep, 
 * otherwise error
 */
cy_rslt_t cy_system_sleep(void);

/** Send the device to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and clocks. All state
 * is still maintained.

 * @return Returns CY_RSLT_SUCCESS if the processor successfully entered and exited sleep, 
 * otherwise error
 */
cy_rslt_t cy_system_deepsleep(void);

/** Register the specified handler with the power manager to be notified when the power
 *  state changes.
 *
 * @param[in] handler The handler to notify on power transitions
 * @return The status of the register_callback request
 */
cy_rslt_t cy_system_register_callback(cy_stc_syspm_callback_t *handler);

/** Removes the specified handler from the power manager so no future notification are made.
 *
 * @param[in] handler The handler to remove from receiving notifications
 * @return The status of the unregister_callback request
 */
cy_rslt_t cy_system_unregister_callback(cy_stc_syspm_callback_t const *handler);

/** Sets the specified clock's frequency and enables it.
 *  This will turn on any additional clocks needed to drive this.
 * 
 * @param[in]  clock        The high frequency clock to configure
 * @param[in]  frequency_hz The frequency to run the clock at
 * @return The status of the clock_frequency request
 */
cy_rslt_t cy_system_clock_frequency(uint8_t clock, uint32_t frequency_hz);

/** Divides the clock driving the CM4 from what system clock 0 is 
 *  running at
 *
 * @param[in]  divider The amount to divide the system clock by
 * @return The status of the cm4_divider request
 */
cy_rslt_t cy_system_cm4_divider(uint8_t divider);

/** Divides the clock driving the peripherals from what system 
 *  clock 0 is running at
 *
 * @param[in]  divider The amount to divide the system clock by
 * @return The status of the peri_divider request
 */
cy_rslt_t cy_system_peri_divider(uint8_t divider);

/** Divides the clock driving the CM0 from what peri clock is 
 *  running at
 *
 * @param[in]  divider The amount to divide the peri clock by
 * @return The status of the cm0_divider request
 */
cy_rslt_t cy_system_cm0_divider(uint8_t divider);

#if defined(__cplusplus)
}
#endif

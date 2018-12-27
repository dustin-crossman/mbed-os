/*******************************************************************************
* File Name: cy_hal_hwmgr.h
*
* Description:
* Provides a high level interface for managing hardware resources. This is
* used to track what hardware blocks are already being used so they are not over
* allocated.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_sysclk.h"

typedef enum
{
    CY_RSC_ADC,
    CY_RSC_CAN,
    CY_RSC_CLOCK_DIVIDER,
    CY_RSC_CRC,
    CY_RSC_DAC,
    CY_RSC_DMA,
    CY_RSC_FLASH,
    CY_RSC_GPIO,
    CY_RSC_I2S,
    CY_RSC_LPCOMP,
    CY_RSC_OPAMP,
    CY_RSC_PDM_PCM,
    CY_RSC_QSPI,
    CY_RSC_RNG,
    CY_RSC_RTC,
    CY_RSC_SCB,
    CY_RSC_SDHC,
    CY_RSC_SMART_IO,
    CY_RSC_TIMER,
    CY_RSC_TCPWM,
} cy_resource_t;

typedef struct
{
    cy_resource_t type;
    uint8_t inst_num;
} cy_resource_inst_t;

typedef enum
{
    CY_TCPWM_16_BIT   = 1U, /**< TCPWM is 16 bits wide */
    CY_TCPWM_32_BIT   = 1U, /**< TCPWM is 32 bits wide */
} cy_tcpwm_size_t;

#define CYCSP_HWMGR_RSLT_USED (CY_RSLT_CREATE(CY_RSLT_ERROR, CY_RSLT_MODULE_CHIP_HWMGR, 0))

/** Initializes the hardware manager to keep track of what resources are being used.
 *
 * @return The status of the init request
 */
cy_rslt_t cy_hwmgr_init();

/** Reserve the specified resource and provide a reference to it.
 *
 * @param[out] obj  The resource object that was reserved
 * @param[in]  type The type of hardware block to reserve
 * @param[in]  inst The instance number of hardware to reserve
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_reserve(cy_resource_inst_t* obj, cy_resource_t type, uint8_t inst_num);

/** Free the specified resource to allow it to be used by something else.
 *
 * @param[in/out] obj The resource object to free
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_free(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an ADC resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_adc(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an CAN resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_can(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an Clock Divider resource and provide a reference to it.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  div           The type of divider to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_clock(cy_resource_inst_t* obj, cy_en_divider_types_t div, bool accept_larger);

/** Allocate (pick and reserve) an CRC resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_crc(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an DAC resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_dac(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an DMA resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_dma(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an Flash resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_flash(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a GPIO resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_gpio(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an I2S resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_i2s(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an LPCOMP resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_lpcomp(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an OpAmp resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_opamp(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a PDM/PCM resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_pdmpcm(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a QSPI resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_qspi(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a RNG resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_rng(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an RTC resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_rtc(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an SCB resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_scb(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an SDHC resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_sdhc(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an SmartIO resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_smartio(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a WDT resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_timer(cy_resource_inst_t* obj);

/** Allocate (pick and reserve) a TCPWM resource and provide a reference to it.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  width         The size of TCPWM to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_tcpwm(cy_resource_inst_t* obj, cy_tcpwm_size_t width, bool accept_larger);


#if defined(__cplusplus)
}
#endif

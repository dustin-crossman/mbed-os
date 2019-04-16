/***************************************************************************//**
* \file cy_hal_hwmgr.h
*
* \brief
* Provides a high level interface for managing hardware resources. This is
* used to track what hardware blocks are already being used so they are not over
* allocated.
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

/** Enum to in indicate which module an errors occurred in. */
enum cy_rslt_module_chip
{
    CY_RSLT_MODULE_CHIP_HWMGR = CY_RSLT_MODULE_CHIP_BASE, //!< An error occurred in hardware management module
    CY_RSLT_MODULE_COMP,                                  //!< An error occurred in comparator module
    CY_RSLT_MODULE_COUNTER,                               //!< An error occurred in counter module
    CY_RSLT_MODULE_CRC,                                   //!< An error occurred in crypto CRC module 
    CY_RSLT_MODULE_DAC,                                   //!< An error occurred in DAC module
    CY_RSLT_MODULE_DMA,                                   //!< An error occurred in DMA module
    CY_RSLT_MODULE_FLASH,                                 //!< An error occurred in flash module
    CY_RSLT_MODULE_GPIO,                                  //!< An error occurred in GPIO module
    CY_RSLT_MODULE_I2C,                                   //!< An error occurred in I2C module
    CY_RSLT_MODULE_I2S,                                   //!< An error occurred in I2S module
    CY_RSLT_MODULE_INTERCONNECT,                          //!< An error occurred in Interconnct module
    CY_RSLT_MODULE_LPCOMP,                                //!< An error occurred in LPComp module
    CY_RSLT_MODULE_OPAMP,                                 //!< An error occurred in OpAmp module
    CY_RSLT_MODULE_PDMPCM,                                //!< An error occurred in PDM/PCM module
    CY_RSLT_MODULE_PWM,                                   //!< An error occurred in PWM module
    CY_RSLT_MODULE_QSPI,                                  //!< An error occurred in QSPI module
    CY_RSLT_MODULE_RNG,                                   //!< An error occurred in RNG module
    CY_RSLT_MODULE_RTC,                                   //!< An error occurred in RTC module
    CY_RSLT_MODULE_SDHC,                                  //!< An error occurred in SDHC module
    CY_RSLT_MODULE_SDIO,                                  //!< An error occurred in SDIO module
    CY_RSLT_MODULE_SMARTIO,                               //!< An error occurred in SmartIO module
    CY_RSLT_MODULE_SPI,                                   //!< An error occurred in SPI module
    CY_RSLT_MODULE_SYSTEM,                                //!< An error occurred in System module
    CY_RSLT_MODULE_UART,                                  //!< An error occurred in UART module
    CY_RSLT_MODULE_WDT,                                   //!< An error occurred in WDT module
};

/** The requested resource type is invalid */
#define CY_RSLT_ERR_CSP_HWMGR_INVALID (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_HWMGR, 0))
/** The requested resource is already in use */
#define CY_RSLT_ERR_CSP_HWMGR_INUSE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_HWMGR, 1))
/** No resources of the requested type are available */
#define CY_RSLT_ERR_CSP_HWMGR_NONE_FREE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CHIP_HWMGR, 2))

/** Attempt to free a resource that was not used */
#define CY_RSLT_WARN_CSP_HWMGR_UNUSED (CY_RSLT_CREATE(CY_RSLT_TYPE_WARNING, CY_RSLT_MODULE_CHIP_HWMGR, 50))


/** Initializes the hardware manager to keep track of what resources are being used.
 *
 * @return The status of the init request
 */
static inline cy_rslt_t cy_hwmgr_init() 
{ 
    return CY_RSLT_SUCCESS;
}

/** Reserve the specified resource.
 *
 * @param[in] obj  The resource object that should be reserved
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_reserve(const cy_resource_inst_t* obj);

/** Free the specified resource to allow it to be used by something else.
 *
 * @param[in,out] obj The resource object to free
 * @return The status of the free request
 */
cy_rslt_t cy_hwmgr_free(const cy_resource_inst_t* obj);

/** Allocate (pick and reserve) an Clock Divider resource and provide a reference to it.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  div           The type of divider to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_clock(cy_clock_divider_t* obj, cy_clock_divider_types_t div, bool accept_larger);

/** Free the specified Clock Divider resource and allow it be used by something else.
 *
 * @param[in] obj           The resource object that was allocated
 * @return The status of the free request
 */
cy_rslt_t cy_hwmgr_free_clock(cy_clock_divider_t* obj);

/** Allocate (pick and reserve) an DMA resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cy_hwmgr_allocate_dma(cy_resource_inst_t* obj);

/** Marks the specified resource as having already been configured (eg: it doesn't need to be configured again).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @return The status of the set request
 */
cy_rslt_t cy_hwmgr_set_configured(cy_resource_t type, uint8_t block, uint8_t channel);

/** Marks the specified resource as not having already been configured (eg: it still needs to be configured before being used).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @return The status of the set request
 */
cy_rslt_t cy_hwmgr_set_unconfigured(cy_resource_t type, uint8_t block, uint8_t channel);

/** Checks to see if the specified resource has already been configured (eg: it doesn't need to be configured again).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @param[out]  isConfigured Whether the block is currently configured
 * @return The status of the check
 */
cy_rslt_t cy_hwmgr_is_configured(cy_resource_t type, uint8_t block, uint8_t channel, bool* isConfigured);

#if defined(__cplusplus)
}
#endif

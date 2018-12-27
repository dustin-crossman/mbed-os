/*******************************************************************************
* File Name: cy_hal_smartio.h
*
* Description:
* Provides a high level interface for interacting with the Cypress SmartIO. 
* This is a wrapper around the lower level PDL API.
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

#define TR_REGS (3)

enum lut_cfg_t
{
    DU_OUT = 0,
    LUT0_OUT = 0,
    LUT1_OUT = 1,
    LUT2_OUT = 2,
    LUT3_OUT = 3,
    LUT4_OUT = 4,
    LUT5_OUT = 5,
    LUT6_OUT = 6,
    LUT7_OUT = 7,
    CHIP0 = 8,
    CHIP4 = 8,
    CHIP1 = 9,
    CHIP5 = 9,
    CHIP2 = 10,
    CHIP6 = 10,
    CHIP3 = 11,
    CHIP7 = 11,
    IO0 = 12,
    IO4 = 12,
    IO1 = 13,
    IO5 = 13,
    IO2 = 14,
    IO6 = 14,
    IO3 = 15,
    IO7 = 15,
}

/** Initialize the SmartIO peripheral
 *
 * @param[out] obj  The smartio object to initialize
 * @param[in]  port The port number to initialize
 * @return The status of the init request
 */
cy_rslt_t smartio_init(smartio_t *obj, uint8_t port);

/** Uninitialize the SmartIO peripheral
 *
 * @param[in,out] obj The smartio object
 * @return The status of the free request
 */
cy_rslt_t smartio_free(smartio_t *obj);

/** Update which connections bypass the SmartIO block
 *
 * @param[in,out] obj  The smartio object
 * @param[in]     mask Bit mask for which lines to bypass (1=bypass, 0=routed)
 * @return The status of the bypass request
 */
cy_rslt_t smartio_bypass(smartio_t *obj, uint8_t mask);

/** Update the LUT configurations for the SmartIO routes
 *
 * @param[in] obj The smartio object
 * @param[in] lut The LUT to update configuration for
 * @param[in] lut The TR configuration settings for the LUT
 * @param[in] lut The LUT output mapping
 * @return The status of the lut request
 */
cy_rslt_t smartio_lut(smartio_t *obj, utin8_t lut, lut_cfg_t cfg[TR_REGS], uint8_t map);

#if defined(__cplusplus)
}
#endif





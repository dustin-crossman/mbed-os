/*******************************************************************************
* File Name: cy_hal_crc.h
*
* Description:
* Provides a high level interface for interacting with the Cypress CRC 
* generator. This is a wrapper around the lower level PDL API.
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

enum crc_algorithm_t
{
    CRC3_ROHC,
    CRC4_ITU,
    CRC5_EPC,
    CRC5_ITU,
    ...
    CRC32_XFER,
}

/** Initialize the CRC generator.
 *
 * @param[out] obj       The CRC generator object
 * @param[in]  algorithm The CRC algorithm to use for computations
 * @return The status of the init request
 */
cy_rslt_t crc_init(crc_t *obj, crc_algorithm_t algorithm);

/** Release the CRC generator.
 *
 * @param[in,out] obj The CRC generator object
 * @return The status of the free request
 */
cy_rslt_t crc_free(crc_t *obj);

/** Compute a CRC.
 *
 * @param[in] obj    The CRC generator object
 * @param[in] data   The data to compute a CRC over
 * @param[in] length The number of bytes of data to process
 * @return The status of the compute request
 */
cy_rslt_t crc_compute(const crc_t *obj, const uint8_t *data, size_t length);

#if defined(__cplusplus)
}
#endif
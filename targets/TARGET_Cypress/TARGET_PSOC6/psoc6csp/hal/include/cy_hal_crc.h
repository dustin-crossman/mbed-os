/***************************************************************************//**
* \file cy_hal_crc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress CRC. 
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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

/** Invalid argument */
#define CY_RSLT_CSP_ERR_CRC_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_CRC, 0))

/** CRC algorithm parameters */
typedef struct
{
    uint32_t width; //!< Bit width of the CRC
    uint32_t polynomial; //!< The polynomial to use
    uint32_t lfsrInitState; //!< Initial state of the LFSR
    uint32_t dataXor; //!< Byte mask to xor with the data
    uint32_t remXor; //!< Mask to xor with the remainder.
    /**
     * The order in which data should be processed. 
     * If 0, data is processed MSB first.
     * If 1, data is processed LSB first.
     */
    uint8_t dataReverse;
    uint8_t remReverse; //!< If 1, the remainder is reversed. If 0, it is not.
} crc_algorithm_t;

/** Initialize the CRC generator. This function reserves the CRYPTO block for CRC calculations.
 *
 * @param[out] obj  The CRC generator object
 * @return          The status of the init request.
 */
cy_rslt_t cy_crc_init(cy_crc_t *obj);

/** Release the CRC generator.
 *
 * @param[in,out] obj The CRC generator object
 * @return            The status of the free request
 */
cy_rslt_t cy_crc_free(cy_crc_t *obj);

/** Initializes a CRC calculation.
 *
 * @param[in,out] obj       The CRC generator object
 * @param[in] algorithm     The CRC algorithm to use for computations
 * @return The status of the compute request
 */
__STATIC_INLINE cy_rslt_t cy_crc_start(cy_crc_t *obj, const crc_algorithm_t *algorithm);

/** Computes the CRC for the given data. This function can be called multiple times to 
 * provide addtional data. This CRC generator will compute the CRC for including all data
 * that was provided during previous calls.
 *
 * @param[in] obj    The CRC generator object
 * @param[in] data   The data to compute a CRC over
 * @param[in] length The number of bytes of data to process
 * @return The status of the compute request
 */
__STATIC_INLINE cy_rslt_t cy_crc_compute(const cy_crc_t *obj, const uint8_t *data, size_t length);

/** Provides final result for a CRC calculation. This will compute the CRC for all data that 
 * was provided when during the diffrent calls to cy_crc_compute.
 *
 * @param[in]  obj The CRC generator object
 * @param[out] crc The computed CRC
 * @return The status of the compute request
 */
__STATIC_INLINE cy_rslt_t cy_crc_finish(const cy_crc_t *obj, uint32_t *crc);

#if defined(__cplusplus)
}
#endif
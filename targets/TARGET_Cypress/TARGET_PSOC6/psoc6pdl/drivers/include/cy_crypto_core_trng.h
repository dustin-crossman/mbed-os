/***************************************************************************//**
* \file cy_crypto_core_trng.h
* \version 2.20
*
* \brief
*  This file provides provides constant and parameters
*  for the API of the TRNG in the Crypto block driver.
*
********************************************************************************
* Copyright 2016-2018 Cypress Semiconductor Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_TRNG_H)
#define CY_CRYPTO_CORE_TRNG_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_TR == 1)

#include "cy_crypto_core_trng_v1.h"
#include "cy_crypto_core_trng_v2.h"

typedef cy_en_crypto_status_t (*cy_crypto_trng_func_t)(CRYPTO_Type *base,
                                             uint32_t  GAROPol,
                                             uint32_t  FIROPol,
                                             uint32_t  max,
                                             uint32_t *randomNum);

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Trng
****************************************************************************//**
*
* Generates a True Random Number and returns it in the
* cfContext->trngNumPtr.
*
* This function available for Server side only.
*
* This function is internal and should not to be called directly by user software
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param GAROPol
* The polynomial for the programmable Galois ring oscillator.
*
* \param FIROPol
* The polynomial for the programmable Fibonacci ring oscillator.
*
* \param max
* The maximum length of a random number, in the range [0, 32] bits.
*
* \param randomNum
* The pointer to a generated true random number. Must be 4-byte aligned.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Trng(CRYPTO_Type *base,
                                             uint32_t  GAROPol,
                                             uint32_t  FIROPol,
                                             uint32_t  max,
                                             uint32_t *randomNum)
{
    cy_en_crypto_status_t result;
    if (cy_device->cryptoVersion == 1u)
    {
        result = Cy_Crypto_Core_V1_Trng(base, GAROPol, FIROPol, max, randomNum);
    }
    else
    {
        result = Cy_Crypto_Core_V2_Trng(base, GAROPol, FIROPol, max, randomNum);
    }
    return (result);
}


#endif /* #if (CPUSS_CRYPTO_TR == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_TRNG_H) */


/* [] END OF FILE */

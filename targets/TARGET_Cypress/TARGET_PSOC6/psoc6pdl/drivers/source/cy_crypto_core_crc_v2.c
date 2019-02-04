/***************************************************************************//**
* \file cy_crypto_core_crc_v2.c
* \version 2.20
*
* \brief
*  This file provides the source code for CRC API
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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


#include "cy_crypto_common.h"
#include "cy_crypto_core_crc_v2.h"
#include "cy_crypto_core_hw_v2.h"
#include "cy_syslib.h"


#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_CRC == 1)


/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_Init
****************************************************************************//**
*
* Initializes CRC calculation.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param polynomial
* The polynomial (specified using 32 bits) used in the computing CRC.
*
* \param dataReverse
* The order in which data bytes are processed. 0 - MSB first; 1- LSB first.
*
* \param dataXor
* The byte mask for XORing data
*
* \param remReverse
* A remainder reverse: 0 means the remainder is not reversed. 1 means reversed.
*
* \param remXor
* Specifies a mask with which the LFSR32 register is XORed to produce a remainder.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_Init(CRYPTO_Type *base,
                                        uint32_t polynomial,
                                        uint32_t dataReverse,
                                        uint32_t dataXor,
                                        uint32_t remReverse,
                                        uint32_t remXor)
{

    /* Specifies the bit order in which a data Byte is processed
     * (reversal is performed after XORing):
     *                                       '0': Most significant bit (bit 1) first.
     *                                       '1': Least significant bit (bit 0) first. */
    REG_CRYPTO_CRC_CTL(base) = (uint32_t)( (_VAL2FLD(CRYPTO_V2_CRC_CTL_DATA_REVERSE, dataReverse)) |
                                           (_VAL2FLD(CRYPTO_V2_CRC_CTL_REM_REVERSE,  remReverse)) );

    /* Specifies a byte mask with which each data byte is XORed.
     * The XOR is performed before data reversal. */
    REG_CRYPTO_CRC_DATA_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_DATA_CTL_DATA_XOR, dataXor));

    /* CRC polynomial. The polynomial is represented WITHOUT the high order bit
     * (this bit is always assumed '1'). */
    REG_CRYPTO_CRC_POL_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_POL_CTL_POLYNOMIAL, polynomial));

    /*Specifies a mask with which the CRC_LFSR_CTL.LFSR32 register is XORed to produce a remainder.
     * The XOR is performed before remainder reversal. */
    REG_CRYPTO_CRC_REM_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_REM_CTL_REM_XOR, remXor));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc
****************************************************************************//**
*
* Performs CRC calculation on a message.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param crc
* The pointer to a computed CRC value. Must be 4-byte aligned.
*
* \param data
* The pointer to the message whose CRC is being computed.
*
* \param dataSize
* The size of a message in bytes.
*
* \param lfsrInitState
* The initial state of the LFSR.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc(CRYPTO_Type *base,
                                        uint32_t *crc,
                                        void      const *data,
                                        uint32_t  dataSize,
                                        uint32_t  lfsrInitState)
{
    /* Fill the FIFO with the instruction parameters */
    Cy_Crypto_Core_V2_FFStart(base, CY_CRYPTO_V2_RB_FF_LOAD0, (uint8_t const *)data, dataSize);

    /* A state of 32-bit Linear Feedback Shift Registers (LFSR) used to implement CRC. */
    REG_CRYPTO_RESULT(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_RESULT_DATA, lfsrInitState));

    /* Issue the CRC instruction */
    Cy_Crypto_Core_V2_Run(base, CY_CRYPTO_V2_CRC_OPC);

    /* Wait until CRC instruction is complete */
    Cy_Crypto_Core_V2_Sync(base);

    /* Copy the result from the CRC_REM_RESULT register */
    *crc = (uint32_t)_FLD2VAL(CRYPTO_V2_CRC_REM_RESULT_REM, REG_CRYPTO_CRC_REM_RESULT(base));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_CalcInit
****************************************************************************//**
*
* Initializes CRC calculation.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param width
* The CRC width in bits.
*
* \param polynomial
* The polynomial (specified using 32 bits) used in the computing CRC.
*
* \param dataReverse
* The order in which data bytes are processed. 0 - MSB first; 1- LSB first.
*
* \param dataXor
* The byte mask for XORing data
*
* \param remReverse
* A remainder reverse: 0 means the remainder is not reversed. 1 means reversed.
*
* \param remXor
* Specifies a mask with which the LFSR32 register is XORed to produce a remainder.
*
* \param lfsrInitState
* The initial state of the LFSR.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_CalcInit(CRYPTO_Type *base,
                                        uint32_t width,
                                        uint32_t polynomial,
                                        uint32_t dataReverse,
                                        uint32_t dataXor,
                                        uint32_t remReverse,
                                        uint32_t remXor,
                                        uint32_t lfsrInitState)
{
    CY_ASSERT_L1((width >= 1) && (width <=32u));

    /* Specifies the bit order in which a data Byte is processed
     * (reversal is performed after XORing):
     *                                       '0': Most significant bit (bit 1) first.
     *                                       '1': Least significant bit (bit 0) first. */
    REG_CRYPTO_CRC_CTL(base) = (uint32_t)( (_VAL2FLD(CRYPTO_V2_CRC_CTL_DATA_REVERSE, dataReverse)) |
                                           (_VAL2FLD(CRYPTO_V2_CRC_CTL_REM_REVERSE,  remReverse)) );

    /* Specifies a byte mask with which each data byte is XORed.
     * The XOR is performed before data reversal. */
    REG_CRYPTO_CRC_DATA_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_DATA_CTL_DATA_XOR, dataXor));

    /* CRC polynomial. The polynomial is represented WITHOUT the high order bit
     * (this bit is always assumed '1'). */
    REG_CRYPTO_CRC_POL_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_POL_CTL_POLYNOMIAL, polynomial << (32u - width)));

    /*Specifies a mask with which the CRC_LFSR_CTL.LFSR32 register is XORed to produce a remainder.
     * The XOR is performed before remainder reversal. */
    REG_CRYPTO_CRC_REM_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CRC_REM_CTL_REM_XOR, remXor << (32u - width)));

    /* A state of 32-bit Linear Feedback Shift Registers (LFSR) used to implement CRC. */
    REG_CRYPTO_RESULT(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_RESULT_DATA, lfsrInitState << (32u - width)));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_CalcStart
****************************************************************************//**
*
* Prepares CRC calculation by setting an initial seeds value.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param width
* The CRC width in bits.
*
* \param lfsrInitState
* The initial state of the LFSR.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_CalcStart(CRYPTO_Type *base, uint32_t width, uint32_t  lfsrInitState)
{
    CY_ASSERT_L1((width >= 1) && (width <=32u));

    /* A state of 32-bit Linear Feedback Shift Registers (LFSR) used to implement CRC. */
    REG_CRYPTO_RESULT(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_RESULT_DATA, lfsrInitState << (32u - width)));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_CalcPartial
****************************************************************************//**
*
* Performs CRC calculation of a message part.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param data
* The pointer to the message whose CRC is being computed.
*
* \param dataSize
* The size of a message in bytes.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_CalcPartial(CRYPTO_Type *base,
                                        void const *data, uint32_t  dataSize)
{
    /* Fill the FIFO with the instruction parameters */
    Cy_Crypto_Core_V2_FFStart(base, CY_CRYPTO_V2_RB_FF_LOAD0, (uint8_t const *)data, dataSize);

    /* Issue the CRC instruction */
    Cy_Crypto_Core_V2_Run(base, CY_CRYPTO_V2_CRC_OPC);

    /* Wait until CRC instruction is complete */
    Cy_Crypto_Core_V2_Sync(base);

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_CalcFinish
****************************************************************************//**
*
* Finalizes CRC calculation.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param width
* The CRC width in bits.
*
* \param crc
* The pointer to a computed CRC value. Must be 4-byte aligned.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_CalcFinish(CRYPTO_Type *base, uint32_t width, uint32_t *crc)
{
    CY_ASSERT_L1((width >= 1) && (width <=32u));

    uint32_t calculatedCrc;

    /* Copy the result from the CRC_REM_RESULT register */
    calculatedCrc = (uint32_t)_FLD2VAL(CRYPTO_V2_CRC_REM_RESULT_REM, REG_CRYPTO_CRC_REM_RESULT(base));

    /* Note: Calculated CRC value is MSB aligned and should be shifted WHEN CRC_DATA_REVERSE is zero. */
    if (_FLD2VAL(CRYPTO_V2_CRC_CTL_REM_REVERSE, REG_CRYPTO_CRC_CTL(base)) == 0u)
    {
        calculatedCrc = calculatedCrc >> (32u - width);
    }

    *crc = calculatedCrc;

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Crc_Calc
****************************************************************************//**
*
* Performs CRC calculation on a message.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param width
* The CRC width in bits.
*
* \param crc
* The pointer to a computed CRC value. Must be 4-byte aligned.
*
* \param data
* The pointer to the message whose CRC is being computed.
*
* \param dataSize
* The size of a message in bytes.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Crc_Calc(CRYPTO_Type *base,
                                        uint32_t  width,
                                        uint32_t *crc,
                                        void      const *data,
                                        uint32_t  dataSize)
{
    CY_ASSERT_L1((width >= 1) && (width <=32u));

    uint32_t calculatedCrc;

    /* Fill the FIFO with the instruction parameters */
    Cy_Crypto_Core_V2_FFStart(base, CY_CRYPTO_V2_RB_FF_LOAD0, (uint8_t const *)data, dataSize);

    /* Issue the CRC instruction */
    Cy_Crypto_Core_V2_Run(base, CY_CRYPTO_V2_CRC_OPC);

    /* Wait until CRC instruction is complete */
    Cy_Crypto_Core_V2_Sync(base);

    /* Copy the result from the CRC_REM_RESULT register */
    calculatedCrc = (uint32_t)_FLD2VAL(CRYPTO_V2_CRC_REM_RESULT_REM, REG_CRYPTO_CRC_REM_RESULT(base));

    /* Note: Calculated CRC value is MSB aligned and should be shifted WHEN CRC_DATA_REVERSE is zero. */
    if (_FLD2VAL(CRYPTO_V2_CRC_CTL_REM_REVERSE, REG_CRYPTO_CRC_CTL(base)) == 0u)
    {
        calculatedCrc = calculatedCrc >> (32u - width);
    }

    *crc = calculatedCrc;

    return (CY_CRYPTO_SUCCESS);
}

#endif /* #if (CPUSS_CRYPTO_CRC == 1) */

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */

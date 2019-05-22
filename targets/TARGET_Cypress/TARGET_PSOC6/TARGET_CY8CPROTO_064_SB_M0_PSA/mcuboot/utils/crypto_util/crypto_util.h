/***************************************************************************//**
* \file crypto_util.h
* \version 1.0
*
* \brief
*  This is the header file complementing the source of
*  helper/service functions needed for SecureBoot cryptography.
*
********************************************************************************
* \copyright
* Copyright 2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifndef CY_BOOTLDR_CRYPTO_UTIL_H_
#define CY_BOOTLDR_CRYPTO_UTIL_H_

#include "cy_sysint.h"

#define CY_BOOTLDR_KEY_PRIV_LEN (32u)

int Cy_ParseImport_DERKey(uint8_t *derKey, uint32_t size, uint8_t *parsedKey);

#endif /* CY_BOOTLDR_CRYPTO_UTIL_H_ */

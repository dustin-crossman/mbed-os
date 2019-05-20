/***************************************************************************//**
* \file crypto_util.c
* \version 1.0
*
* \brief
*  This is the source code implementing helper/service function needed for SecureBoot cryptography.
*
*******************************************************************************
* \copyright
* Copyright 2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "crypto_util.h"
#include "mbedtls/ecp.h"
#include "mbedtls/asn1.h"
#include "mbedtls/pk.h"

#if defined (MCUBOOT_USE_FLASHBOOT_CRYPTO)
#else /* defined (MCUBOOT_USE_FLASHBOOT_CRYPTO) */
/*
 * Parses an unencrypted PKCS#8 encoded private key
 *
 * Notes:
 *
 * - This function does not own the key buffer. It is the
 *   responsibility of the caller to take care of zeroizing
 *   and freeing it after use.
 *
 * - The function is responsible for freeing the provided
 *   PK context on failure.
 *
 */
extern int pk_parse_key_pkcs8_unencrypted_der(mbedtls_pk_context *pk,
												const unsigned char* key,
												size_t keylen );

/*******************************************************************************
* Function Name: Cy_ParseImport_DERKey
****************************************************************************//**
* Parses ASN1/DER-encoded private key into binary stream, compatible with
* psa_import_key()
*
* \param derKey		DER-encoded key.
* \param size		Size of input data.
* \param parsedKey	Parsed binary stream.
*
* \return Returns success (0) in case of parsing performed correctly.
*******************************************************************************/
int Cy_ParseImport_DERKey(uint8_t *derKey, uint32_t size, uint8_t *parsedKey)
{
	int rc = 0;

	uint8_t i;
	uint8_t *tmp_key_ptr;

	mbedtls_pk_context pk;
	mbedtls_ecp_keypair *tmp_keypair;

	mbedtls_pk_init(&pk);

	rc = pk_parse_key_pkcs8_unencrypted_der(&pk, derKey, size);

	if(0 == rc)
	{
		tmp_keypair = (mbedtls_ecp_keypair *)pk.pk_ctx;
		tmp_key_ptr = (uint8_t *)(tmp_keypair->d.p);

		for(i=0; i<CY_BOOTLDR_KEY_PRIV_LEN; i++)
		{
			parsedKey[i] = tmp_key_ptr[CY_BOOTLDR_KEY_PRIV_LEN-1-i];
		}
	}
	return rc;
}
#endif /* defined (MCUBOOT_USE_FLASHBOOT_CRYPTO) */

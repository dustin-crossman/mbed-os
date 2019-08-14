/***************************************************************************//**
* \file ecdh_kdf_process.c
* \version 1.0
*
* \brief
*  This is the source code implementing ECDH/HKDF scheme secret data interchange.
*
*******************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifdef MCUBOOT_HAVE_ASSERT_H
#include "mcuboot_config/mcuboot_assert.h"
#else
#include <assert.h>
#endif
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "ecdh_kdf_process.h"
#include "flashboot_psacrypto.h"

#define PSA_CRYPTO_LEGACY_OFFSET (35u)

encrypted_image_header enc_img_hdr;

/**************************************************************************/

const char *crypto_header_info =
{
	"_handshake_data_"
};

const char *crypto_header_salt =
{
	"0123456701234567"
};

#if defined (MCUBOOT_USE_FLASHBOOT_CRYPTO)
int Cy_ECDH_ExtractDerivedKey(header_crypto_data *crypto_data,
                                uint8_t key_id_pair,
                                uint8_t key_id_peer_pub,
                                uint8_t *key_data)
{
    int rc = 0;
    psa_status_t status = PSA_SUCCESS;
    
    fb_psa_key_type_t publicKeyType;
    
    size_t keyBits;
    uint8_t *publicKey = NULL;
    size_t publicKeyLength;
    
    fb_psa_crypto_generator_t generator = PSA_CRYPTO_GENERATOR_INIT;
    fb_psa_algorithm_t alg = PSA_ALG_ECDH(PSA_ALG_HKDF(PSA_ALG_SHA_256));
    
    /* AES key length + IV length; bits */
    uint32_t outSize = (CY_BOOTLDR_AES128_KEY_LEN + CY_BOOTLDR_AES128_IV_LEN);

    /* read from encrypted_image_header */
    unsigned char *label = crypto_data->info;
    unsigned char *salt = crypto_data->salt;
    size_t label_length = CY_IMG_CRYPTO_BLK_SIZE;
    size_t salt_length = CY_IMG_CRYPTO_BLK_SIZE;
    
    if(status == PSA_SUCCESS)
    {
        status = fb_psa_get_key_information( key_id_peer_pub,
                                              &publicKeyType,
                                              &keyBits );
    }
    if(status == PSA_SUCCESS)
    {
        publicKeyLength = PSA_KEY_EXPORT_MAX_SIZE( publicKeyType, keyBits ) + PSA_CRYPTO_LEGACY_OFFSET;
        publicKey = malloc(publicKeyLength);
        if(publicKey == NULL)
        {
            status = PSA_ERROR_INSUFFICIENT_MEMORY;
        }
        else
        {
            status = fb_psa_export_public_key( key_id_peer_pub,
                                            publicKey,
                                            publicKeyLength,
                                            &publicKeyLength );
        }
    }

    if(status == PSA_SUCCESS)
    {
        status = fb_psa_key_agreement_salt_label( &generator,
                                                key_id_pair,
									            publicKey,
									            publicKeyLength,
									            alg ,
									            salt,
									            salt_length,
									            label,
									            label_length);
    }

    free( publicKey );

    if(status == PSA_SUCCESS)
    {
        status = fb_psa_generator_read( &generator, key_data, outSize );
    }

    fb_psa_generator_abort( &generator );

    if(PSA_SUCCESS != status)
    {
    	rc = status;
    }
    
    return rc;
}
#else /* defined (MCUBOOT_USE_FLASHBOOT_CRYPTO) */
int Cy_ECDH_ExtractDerivedKey(header_crypto_data *crypto_data,
                                uint8_t key_id_pair,
                                uint8_t key_id_peer_pub,
                                uint8_t *key_data)
{
    int rc = 0;
    psa_status_t status = PSA_SUCCESS;
    
    psa_key_type_t publicKeyType;
    
    size_t keyBits;
    uint8_t *publicKey = NULL;
    size_t publicKeyLength;
    
    psa_crypto_generator_t generator = PSA_CRYPTO_GENERATOR_INIT;
    psa_algorithm_t alg = PSA_ALG_ECDH(PSA_ALG_HKDF(PSA_ALG_SHA_256));
    
    /* AES key length + IV length; bits */
    uint32_t outSize = (CY_BOOTLDR_AES128_KEY_LEN + CY_BOOTLDR_AES128_IV_LEN);

    /* read from encrypted_image_header */
    unsigned char *label = crypto_data->info;
    unsigned char *salt = crypto_data->salt;
    size_t label_length = CY_IMG_CRYPTO_BLK_SIZE;
    size_t salt_length = CY_IMG_CRYPTO_BLK_SIZE;
    
    if(status == PSA_SUCCESS)
    {
        status = psa_get_key_information( key_id_peer_pub,
                                              &publicKeyType,
                                              &keyBits );
    }
    if(status == PSA_SUCCESS)
    {
        publicKeyLength = PSA_KEY_EXPORT_MAX_SIZE( publicKeyType, keyBits ) + PSA_CRYPTO_LEGACY_OFFSET;
        publicKey = malloc(publicKeyLength);
        if(publicKey == NULL)
        {
            status = PSA_ERROR_INSUFFICIENT_MEMORY;
        }
        else
        {
            status = psa_export_public_key( key_id_peer_pub,
                                            publicKey,
                                            publicKeyLength,
                                            &publicKeyLength );
        }
    }

    if(status == PSA_SUCCESS)
    {
        status = psa_key_agreement_salt_label( &generator,
                                                key_id_pair,
									            publicKey,
									            publicKeyLength,
									            alg ,
									            salt,
									            salt_length,
									            label,
									            label_length);
    }

    free( publicKey );

    if(status == PSA_SUCCESS)
    {
        status = psa_generator_read( &generator, key_data, outSize );
    }

    psa_generator_abort( &generator );

    if(PSA_SUCCESS != status)
    {
    	rc = status;
    }
    
    return rc;
}
#endif /* defined (MCUBOOT_USE_FLASHBOOT_CRYPTO) */

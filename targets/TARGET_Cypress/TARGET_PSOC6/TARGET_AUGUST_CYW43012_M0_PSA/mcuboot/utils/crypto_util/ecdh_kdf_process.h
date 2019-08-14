/***************************************************************************//**
* \file ecdh_kdf_process.h
* \version 1.0
*
* \brief
*  This is the header file complementign the source for ECDH/HKDF scheme 
*  secret data interchange.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifndef PRJ_CM0P_ECDH_KDF_PROCESS_H_
#define PRJ_CM0P_ECDH_KDF_PROCESS_H_

#include "bootutil/image.h"
#include "mcuboot_config/mcuboot_config.h"

#include "mbedtls/asn1.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"

#include "psa/crypto.h"
#include "flashboot_psacrypto.h"

#define CY_BOOTLDR_DEV_PRIV_DER_KEY_ID	(1)
#define CY_BOOTLDR_DEV_PRIV_SIG_KEY_ID	(2)
#define CY_BOOTLDR_CY_PUB_KEY_ID        (3)
#define CY_BOOTLDR_HSM_PUB_KEY_ID       (4)
#define CY_BOOTLDR_OEM_PUB_KEY_ID       (5)
#define CY_BOOTLDR_PRIV_DERIVE_KEY_ID   (9)
#define CY_BOOTLDR_HDR_KEY_ID           (11) /* SecureFlashBoot (FB_KEY_SLOT_STATIC_MAX+1) */
#define CY_BOOTLDR_AES_IMG_KEY_ID       ((CY_BOOTLDR_HDR_KEY_ID)+(1))

#define CY_IMG_CRYPTO_BLK_SIZE	(16)
#define CY_IMG_KEY_SIZE_BYTES 	(40)

#define CY_BOOTLDR_AES128_KEY_LEN    (16)
#define CY_BOOTLDR_AES128_IV_LEN     (16)

typedef struct
{
    /**
    * The crypto data wrapped by ECDH/HKDF:
    * - AES128 key [16]
    * - AES128 IV [16]
    * - PKCS#7 pad [16]
    */
    uint8_t encrypted_secret[3*CY_IMG_CRYPTO_BLK_SIZE];

    uint8_t salt[CY_IMG_CRYPTO_BLK_SIZE]; /* seed */
    uint8_t info[CY_IMG_CRYPTO_BLK_SIZE]; /* label */
}header_crypto_data;

/** Image header.  All fields are in little endian byte order. */
typedef struct {
    uint32_t ih_magic;
    uint32_t ih_load_addr;
    uint16_t ih_hdr_size; /* Size of image header (bytes). */
    uint16_t _pad1;
    uint32_t ih_img_size; /* Does not include header. */
    uint32_t ih_flags;    /* IMAGE_F_[...]. */
    struct image_version ih_ver;
    uint32_t _pad2;

    /* cybootloader header crypto data */
    header_crypto_data crypto_data;
}encrypted_image_header;

int Cy_ECDH_ExtractDerivedKey(header_crypto_data *crypto_data, uint8_t key_id_pair, uint8_t key_id_peer_pub, uint8_t *key_data);

#endif /* PRJ_CM0P_ECDH_KDF_PROCESS_H_ */

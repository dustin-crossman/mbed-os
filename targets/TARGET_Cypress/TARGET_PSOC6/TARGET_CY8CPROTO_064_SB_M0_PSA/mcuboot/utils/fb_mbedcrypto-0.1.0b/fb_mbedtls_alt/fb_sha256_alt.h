/***************************************************************************//**
* \file fb_sha256_alt.h
*
* \brief
* header file - wrapper for mbedtls SHA256 HW acceleration
*
* \note
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(FB_SHA256_ALT_H)
#define FB_SHA256_ALT_H

#include "cy_crypto_common.h"

#if defined(FB_MBEDTLS_SHA256_ALT)

typedef struct fb_mbedtls_sha256_context {
    fb_cy_stc_crypto_sha_state_t hashState;           /* Structure used by CY Crypto Driver   */
    fb_cy_stc_crypto_v1_sha256_buffers_t shaBuffers;  /* Structure used by CY Crypto Driver   */
}
fb_mbedtls_sha256_context;

#endif /* FB_MBEDTLS_SHA256_ALT */

#endif /* (FB_SHA256_ALT_H) */

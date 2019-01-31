/*
 *  mbed Microcontroller Library
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019 Cypress Semiconductor Corporation
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     sha512_alt.c
 *
 * \brief    Source file - wrapper for mbedtls SHA512 HW acceleration
 *
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA512_C)

#include "mbedtls/sha512.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_SHA512_ALT)

void mbedtls_sha512_init( mbedtls_sha512_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_free( mbedtls_sha512_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_clone( mbedtls_sha512_context *dst, const mbedtls_sha512_context *src )
{
    *dst = *src;
}

/*
 * SHA-512 context setup
 */
int mbedtls_sha512_starts_ret( mbedtls_sha512_context *ctx, int is384)
{
    return cy_hw_sha_start(&ctx->currBlockLen,
                           &ctx->hashState,
                           ( is384 == 0 ) ? CY_CRYPTO_MODE_SHA512 : CY_CRYPTO_MODE_SHA384,
                           &ctx->shaBuffers);
}

/*
 * SHA-512 process buffer
 */
int mbedtls_sha512_update_ret( mbedtls_sha512_context *ctx, const unsigned char *input, size_t ilen )
{
    return cy_hw_sha_update(&ctx->currBlockLen, &ctx->hashState, input, ilen, (uint8_t *)ctx->shaBuffers.block);
}

/*
 * SHA-512 final digest
 */
int mbedtls_sha512_finish_ret( mbedtls_sha512_context *ctx, unsigned char output[64] )
{
     return cy_hw_sha_finish(&ctx->hashState, output, (uint8_t *)ctx->shaBuffers.block, ctx->currBlockLen);
}

int mbedtls_internal_sha512_process( mbedtls_sha512_context *ctx, const unsigned char data[128] )
{
    return cy_hw_sha_process(&ctx->hashState, data);
}

#endif /* MBEDTLS_SHA512_ALT */

#endif /* MBEDTLS_SHA512_C */

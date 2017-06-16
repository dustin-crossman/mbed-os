/*
 *  sha1_alt.c for SHA1 HASH
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
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
 *
 */
#include "mbedtls/sha1.h"
#if defined(MBEDTLS_SHA1_ALT)
#include "mbedtls/platform.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}

void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();

}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL )
        return;

	/* Force the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_FORCE_RESET();

    /* Release the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_RELEASE_RESET();

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    *dst = *src;
}

void mbedtls_sha1_starts( mbedtls_sha1_context *ctx )
{
    /* Deinitializes the HASH peripheral */
    if (HAL_HASH_DeInit(&ctx->hhash_sha1) == HAL_ERROR) {
        // error found to be returned
        return;
    }

    /* HASH Configuration */
    ctx->hhash_sha1.Init.DataType = HASH_DATATYPE_8B;
    if (HAL_HASH_Init(&ctx->hhash_sha1) == HAL_ERROR) {
        // error found to be returned
        return;
    }
}

void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[MBEDTLS_SHA1_BLOCK_SIZE] )
{
	HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *) data, MBEDTLS_SHA1_BLOCK_SIZE);
}

void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen )
{
    unsigned char i=0;
    int currentlen = ilen;
    /* store mechanism to handle 64 bytes per 64 bytes */
    if (currentlen == 0){ // change HW status is size if 0
        HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *) input, ilen);
    }
    while ((currentlen+ctx->sbuf_len) >=MBEDTLS_SHA1_BLOCK_SIZE) {
        if (ctx->sbuf_len ==0) { /* straight forward */
            mbedtls_sha1_process(ctx, input+(i*MBEDTLS_SHA1_BLOCK_SIZE));
        } else {
            unsigned char tmp = ctx->sbuf_len;
            memcpy(ctx->sbuf+tmp, input+(i*MBEDTLS_SHA1_BLOCK_SIZE),MBEDTLS_SHA1_BLOCK_SIZE-tmp);
            mbedtls_sha1_process(ctx, ctx->sbuf);
            if ((currentlen-(MBEDTLS_SHA1_BLOCK_SIZE-tmp)) < tmp) {
                ctx->sbuf_len = currentlen-(MBEDTLS_SHA1_BLOCK_SIZE-tmp);
            }
            memcpy(ctx->sbuf,input+(i+1)*MBEDTLS_SHA1_BLOCK_SIZE-tmp, ctx->sbuf_len);
        }
        currentlen -= MBEDTLS_SHA1_BLOCK_SIZE;
        i++;
    }
    if (currentlen >0) {
        /* Store the remaining <64 values */
        memcpy(ctx->sbuf+ctx->sbuf_len, input+(i*MBEDTLS_SHA1_BLOCK_SIZE), currentlen);
        ctx->sbuf_len += currentlen;
    }
}

void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] )
{

    if (ctx->sbuf_len > 0) {
        HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, ctx->sbuf, ctx->sbuf_len);
    }
    mbedtls_zeroize(ctx->sbuf, MBEDTLS_SHA1_BLOCK_SIZE);
    ctx->sbuf_len = 0;
    __HAL_HASH_START_DIGEST();

    if (HAL_HASH_SHA1_Finish(&ctx->hhash_sha1, output, 10)){
        // error code to be returned
    }
}

#endif /*MBEDTLS_SHA1_ALT*/

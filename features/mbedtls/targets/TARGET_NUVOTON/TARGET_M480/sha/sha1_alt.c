/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbedtls/sha1.h"

#if defined(MBEDTLS_SHA1_C)
#if defined(MBEDTLS_SHA1_ALT)

#include "crypto-misc.h"
#include "nu_bitutil.h"
#include "string.h"

/* Choose SHA S/W or H/W context and initialize it
 * 
 * try_hw:
 *   0: Initialize S/W context
 *   1: Try acquiring SHA H/W resource first and initialize its H/W context if successful. If failed, initialize S/W context.
 */
static void mbedtls_sha1_init_internal(mbedtls_sha1_context *ctx, int try_hw)
{
    if (try_hw && crypto_sha_acquire()) {
        ctx->active_ctx = &ctx->hw_ctx;
        mbedtls_sha1_hw_init(&ctx->hw_ctx);
    } else {
        ctx->active_ctx = &ctx->sw_ctx;
        mbedtls_sha1_sw_init(&ctx->sw_ctx);
    }
}

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    mbedtls_sha1_init_internal(ctx, 1);
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->active_ctx == &ctx->hw_ctx) {
        mbedtls_sha1_hw_free(&ctx->hw_ctx);
        crypto_sha_release();
    } else if (ctx->active_ctx == &ctx->sw_ctx) {
        mbedtls_sha1_sw_free(&ctx->sw_ctx);
    }
    ctx->active_ctx = NULL;
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src)
{
    // Corner case: Destination/source contexts are the same
    if (dst == src) {
        return;
    }

    // If dst is H/W context, we need to change it to S/W context first before cloning to.
    if (dst->active_ctx == &dst->hw_ctx) {
        mbedtls_sha1_free(dst);
        // Force S/W context
        mbedtls_sha1_init_internal(dst, 0);
    }

    if (src->active_ctx == &src->hw_ctx) {
        // Clone S/W ctx from H/W ctx
        dst->sw_ctx.total[0] = src->hw_ctx.total;
        dst->sw_ctx.total[1] = 0;
        {
            unsigned char output[20];
            crypto_sha_getinternstate(output, sizeof (output));
            unsigned char *output_pos = output;
            unsigned char *output_end = output + (sizeof (output) / sizeof (output[0]));
            uint32_t *state_pos = (uint32_t *) &(dst->sw_ctx.state[0]);
            while (output_pos != output_end) {
                *state_pos ++ = nu_get32_be(output_pos);
                output_pos += 4;
            }
        }
        memcpy(dst->sw_ctx.buffer, src->hw_ctx.buffer, src->hw_ctx.buffer_left);
        if (src->hw_ctx.buffer_left == src->hw_ctx.blocksize) {
            mbedtls_sha1_sw_process(&dst->sw_ctx, dst->sw_ctx.buffer);
        }
    } else if (src->active_ctx == &src->sw_ctx) {
        // Clone S/W ctx from S/W ctx
        dst->sw_ctx = src->sw_ctx;
    }
}

/*
 * SHA-1 context setup
 */
void mbedtls_sha1_starts(mbedtls_sha1_context *ctx)
{
    if (ctx->active_ctx == &ctx->hw_ctx) {
        mbedtls_sha1_hw_starts(&ctx->hw_ctx);
    } else if (ctx->active_ctx == &ctx->sw_ctx) {
        mbedtls_sha1_sw_starts(&ctx->sw_ctx);
    }
}

/*
 * SHA-1 process buffer
 */
void mbedtls_sha1_update(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx->active_ctx == &ctx->hw_ctx) {
        mbedtls_sha1_hw_update(&ctx->hw_ctx, input, ilen);
    } else if (ctx->active_ctx == &ctx->sw_ctx) {
        mbedtls_sha1_sw_update(&ctx->sw_ctx, input, ilen);
    }
}

/*
 * SHA-1 final digest
 */
void mbedtls_sha1_finish(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    if (ctx->active_ctx == &ctx->hw_ctx) {
        mbedtls_sha1_hw_finish(&ctx->hw_ctx, output);
    } else if (ctx->active_ctx == &ctx->sw_ctx) {
        mbedtls_sha1_sw_finish(&ctx->sw_ctx, output);
    }
}

void mbedtls_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    if (ctx->active_ctx == &ctx->hw_ctx) {
        mbedtls_sha1_hw_process(&ctx->hw_ctx, data);
    } else if (ctx->active_ctx == &ctx->sw_ctx) {
        mbedtls_sha1_sw_process(&ctx->sw_ctx, data);
    }
}

#endif /* MBEDTLS_SHA1_ALT */
#endif /* MBEDTLS_SHA1_C */

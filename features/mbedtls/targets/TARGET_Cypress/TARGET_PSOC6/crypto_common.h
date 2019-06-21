/*
 * mbed Microcontroller Library
 * Copyright (c) 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

/**
 * \file    crypto_common.h
 * \version 1.0
 *
 * \brief   Header file for common mbedtls acceleration functions
 *
 */

#if !defined(CRYPTO_COMMON_H)
#define CRYPTO_COMMON_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/ecp.h"

#include "cy_crypto_core_sha.h"

void cy_hw_sha_init (void *ctx, uint32_t ctxSize);

int cy_hw_sha_start (cy_stc_crypto_sha_state_t *hashState,
                     cy_en_crypto_sha_mode_t shaMode,
                     void *shaBuffers);

int cy_hw_sha_update(cy_stc_crypto_sha_state_t *hashState,
				     const uint8_t *in, uint32_t inlen);

int cy_hw_sha_finish(cy_stc_crypto_sha_state_t *hashState, uint8_t *output);

void cy_hw_sha_clone(void *ctxDst, const void *ctxSrc, uint32_t ctxSize,
                     cy_stc_crypto_sha_state_t *hashStateDst, void *shaBuffersDst);

int cy_hw_sha_free(cy_stc_crypto_sha_state_t *hashState);

int cy_hw_sha_process(cy_stc_crypto_sha_state_t *hashState, const uint8_t *in);

#endif /* (CRYPTO_COMMON_H) */

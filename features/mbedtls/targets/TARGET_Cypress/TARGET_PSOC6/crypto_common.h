/*
 * mbed Microcontroller Library
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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
#include "cy_crypto_core_ecc.h"

int cy_hw_sha_start (uint32_t *currBlockLen, cy_stc_crypto_sha_state_t *hashState,
					 cy_en_crypto_sha_mode_t shaMode, void *shaBuffers);

int cy_hw_sha_update(uint32_t *currBlockLen, cy_stc_crypto_sha_state_t *hashState,
				     const uint8_t *in, uint32_t inlen, uint8_t *shaTempBlock);

int cy_hw_sha_finish(cy_stc_crypto_sha_state_t *hashState, uint8_t *output,
                     uint8_t *shaTempBlock, uint32_t currBlockLen);

int cy_hw_sha_process(cy_stc_crypto_sha_state_t *hashState, const uint8_t *in);

cy_en_crypto_ecc_curve_id_t get_dp_idx(mbedtls_ecp_group_id gid);

#endif /* (CRYPTO_COMMON_H) */

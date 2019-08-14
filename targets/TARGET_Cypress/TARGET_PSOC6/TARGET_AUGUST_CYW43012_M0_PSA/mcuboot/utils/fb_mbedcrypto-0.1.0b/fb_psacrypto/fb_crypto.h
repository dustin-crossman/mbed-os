/**
 * \file fb_crypto.h
 * \brief Platform Security Architecture cryptography module
 */
/*
 *  Copyright (C) 2019, Cypress Semiconductor
 *  Copyright (C) 2018, ARM Limited, All Rights Reserved
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

#ifndef FB_PSA_CRYPTO_H
#define FB_PSA_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Function return status.
 *
 * This is either #PSA_SUCCESS (which is zero), indicating success,
 * or a nonzero value indicating that an error occurred. Errors are
 * encoded as one of the \c PSA_ERROR_xxx values defined here.
 */
typedef int32_t fb_psa_status_t;

/** \brief Encoding of a cryptographic algorithm.
 *
 * For algorithms that can be applied to multiple key types, this type
 * does not encode the key type. For example, for symmetric ciphers
 * based on a block cipher, #psa_algorithm_t encodes the block cipher
 * mode and the padding mode while the block cipher itself is encoded
 * via #psa_key_type_t.
 */
typedef uint32_t fb_psa_algorithm_t;

/** \brief Encoding of a key type.
 */
typedef uint32_t fb_psa_key_type_t;

/** \brief Encoding of permitted usage on a key. */
typedef uint32_t fb_psa_key_usage_t;

/** The type of the state data structure for multipart cipher operations.
 *
 * This is an implementation-defined \c struct. Applications should not
 * make any assumptions about the content of this structure except
 * as directed by the documentation of a specific implementation. */
typedef struct fb_psa_cipher_operation_s fb_psa_cipher_operation_t;

/** The type of the state data structure for generators.
 *
 * Before calling any function on a generator, the application must
 * initialize it by any of the following means:
 * - Set the structure to all-bits-zero, for example:
 *   \code
 *   psa_crypto_generator_t generator;
 *   memset(&generator, 0, sizeof(generator));
 *   \endcode
 * - Initialize the structure to logical zero values, for example:
 *   \code
 *   psa_crypto_generator_t generator = {0};
 *   \endcode
 * - Initialize the structure to the initializer #PSA_CRYPTO_GENERATOR_INIT,
 *   for example:
 *   \code
 *   psa_crypto_generator_t generator = PSA_CRYPTO_GENERATOR_INIT;
 *   \endcode
 * - Assign the result of the function psa_crypto_generator_init()
 *   to the structure, for example:
 *   \code
 *   psa_crypto_generator_t generator;
 *   generator = psa_crypto_generator_init();
 *   \endcode
 *
 * This is an implementation-defined \c struct. Applications should not
 * make any assumptions about the content of this structure except
 * as directed by the documentation of a specific implementation.
 */
typedef struct fb_psa_crypto_generator_s fb_psa_crypto_generator_t;

/** The type of the state data structure for multipart hash operations.
 *
 * This is an implementation-defined \c struct. Applications should not
 * make any assumptions about the content of this structure except
 * as directed by the documentation of a specific implementation. */
typedef struct fb_psa_hash_operation_s fb_psa_hash_operation_t;

/** The type of the key policy data structure.
 *
 * This is an implementation-defined \c struct. Applications should not
 * make any assumptions about the content of this structure except
 * as directed by the documentation of a specific implementation. */
typedef struct fb_psa_key_policy_s fb_psa_key_policy_t;

#ifdef __cplusplus
}
#endif

#endif /* FB_PSA_CRYPTO_H */

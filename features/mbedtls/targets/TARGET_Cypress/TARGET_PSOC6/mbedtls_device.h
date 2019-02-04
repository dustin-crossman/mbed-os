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

#ifndef MBEDTLS_DEVICE_H
#define MBEDTLS_DEVICE_H

/* SHA alternate implementations are disabled: concurrent hardware accelerated
   operation of SHA block is not currently supported */
// #define MBEDTLS_SHA1_C
// #define MBEDTLS_SHA1_ALT
// #define MBEDTLS_SHA256_ALT
// #define MBEDTLS_SHA512_ALT

/* AES alternate implementation is disabled: concurrent hardware accelerated
   operation of AES block is not currently supported */
// #define MBEDTLS_AES_ALT
// #define MBEDTLS_CIPHER_MODE_CBC
// #define MBEDTLS_CIPHER_MODE_CFB
// #define MBEDTLS_CIPHER_MODE_CTR

/* ECP alternate implementation is disabled: only NIST-P curves
   are currently supported, no software fallback for other curves */
// #define MBEDTLS_ECP_ALT
// #define MBEDTLS_ECP_DP_SECP192R1_ENABLED
// #define MBEDTLS_ECP_DP_SECP224R1_ENABLED
// #define MBEDTLS_ECP_DP_SECP256R1_ENABLED
// #define MBEDTLS_ECP_DP_SECP384R1_ENABLED
// #define MBEDTLS_ECP_DP_SECP521R1_ENABLED

#endif /* MBEDTLS_DEVICE_H */

/*
 *  ECDSA sign, verify and key generation functions
 *
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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"

#include <string.h>
#include <stdlib.h>

#include "cy_crypto_core_ecc.h"
#include "cy_crypto_core_vu.h"

#include "psoc6_utils.h"
#include "crypto_common.h"

#include <stdio.h>

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message (SEC1 4.1.3)
 * Obviously, compared to SEC1 4.1.3, we skip step 4 (hash message)
 */
/**
 * \brief           This function computes the ECDSA signature of a
 *                  previously-hashed message.
 *
 * \note            The deterministic version is usually preferred.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated
 *                  as defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.3, step 5.
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group.
 * \param r         The first output integer.
 * \param s         The second output integer.
 * \param d         The private signing key.
 * \param buf       The message hash.
 * \param blen      The length of \p buf.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX
 *                  or \c MBEDTLS_MPI_XXX error code on failure.
 */
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret;
    uint8_t *sig = NULL;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
	int bytesize;
	uint8_t *tmp_b = NULL;
	uint8_t tmp_k[CY_CRYPTO_ECC_MAX_BYTE_SIZE];
    cy_en_crypto_status_t ecdsa_sig_status;

    printf("mbedtls_ecdsa_sign: enter\n");

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if ( grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure d is in range 1..n-1 */
    if ( mbedtls_mpi_cmp_int( d, 1 ) < 0 || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    printf("f_rng() check)\n");
    if (f_rng == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    printf("buf check)\n");
    if (buf == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    printf("get_dp_idx())\n");
    key.curveID = get_dp_idx(grp->id);

    if (key.curveID == CY_CRYPTO_ECC_ECP_NONE)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    cy_reserve_crypto(CY_CRYPTO_VU_HW);

    printf("Cy_Crypto_Core_ECC_GetCurveParams())\n");
	dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);

	bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

    printf("malloc() key.k)\n");
	key.k = malloc(bytesize);
    MBEDTLS_MPI_CHK((key.k == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, key.k, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(key.k, bytesize);

    printf("malloc() sig)\n");
    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    printf("malloc() tmp_b)\n");
    tmp_b = malloc(blen + 1u);
    MBEDTLS_MPI_CHK((tmp_b == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    memcpy(tmp_b, buf, blen);
    Cy_Crypto_Core_InvertEndianness(tmp_b, blen);

    f_rng(p_rng, tmp_k, bytesize);
    Cy_Crypto_Core_InvertEndianness(tmp_k, bytesize);

    printf("Cy_Crypto_Core_ECC_SignHash())\n");
    ecdsa_sig_status = Cy_Crypto_Core_ECC_SignHash(CRYPTO, tmp_b, blen, sig, &key, tmp_k);

    MBEDTLS_MPI_CHK((ecdsa_sig_status == CY_CRYPTO_SUCCESS) ? 0 : MBEDTLS_ERR_ECP_HW_ACCEL_FAILED);

    printf("convert results)\n");
    Cy_Crypto_Core_InvertEndianness(sig, bytesize);
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( r, sig, bytesize ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( s, sig + bytesize, bytesize ) );

cleanup:
    cy_free_crypto(CY_CRYPTO_VU_HW);

    if (key.k != NULL)
    {
    	free(key.k);
    }
    if (sig != NULL)
    {
    	free(sig);
    }
    if (tmp_b != NULL)
    {
        free(tmp_b);
    }

    printf("mbedtls_ecdsa_sign: exit\n");

    return( ret );
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                  const unsigned char *buf, size_t blen,
                  const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s)
{
    int ret;
	uint8_t stat;
    uint8_t *point_arr = NULL;
    uint8_t *sig = NULL;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
    int bytesize;
	size_t olen;
	uint8_t *tmp_b = NULL;
    cy_en_crypto_status_t ecdsa_ver_status;

    printf("mbedtls_ecdsa_verify: enter\n");

    printf("buf check)\n");
    if (buf == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    key.curveID = get_dp_idx(grp->id);

    MBEDTLS_MPI_CHK( (key.curveID == CY_CRYPTO_ECC_ECP_NONE) ? MBEDTLS_ERR_ECP_BAD_INPUT_DATA : 0);

    cy_reserve_crypto(CY_CRYPTO_VU_HW);

	dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);
	bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

    point_arr = malloc(2 * bytesize + 1);
    MBEDTLS_MPI_CHK((point_arr == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);
    key.pubkey.x  = point_arr + 1;
    key.pubkey.y  = point_arr + bytesize + 1;

    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( r, sig, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( s, sig + bytesize, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, point_arr, 2 * bytesize + 1) );
    Cy_Crypto_Core_InvertEndianness(key.pubkey.x, bytesize);
    Cy_Crypto_Core_InvertEndianness(key.pubkey.y, bytesize);

    tmp_b = malloc(blen + 1u);
    MBEDTLS_MPI_CHK((tmp_b == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    memcpy(tmp_b, buf, blen);
    Cy_Crypto_Core_InvertEndianness(tmp_b, blen);

    ecdsa_ver_status = Cy_Crypto_Core_ECC_VerifyHash(CRYPTO, sig, tmp_b, blen, &stat, &key);

    MBEDTLS_MPI_CHK((ecdsa_ver_status != CY_CRYPTO_SUCCESS) ? MBEDTLS_ERR_ECP_HW_ACCEL_FAILED : 0);

    MBEDTLS_MPI_CHK((stat == 1) ? 0 : MBEDTLS_ERR_ECP_VERIFY_FAILED);

cleanup:
    cy_free_crypto(CY_CRYPTO_VU_HW);

    if (point_arr != NULL)
    {
    	free(point_arr);
    }
    if (sig != NULL)
    {
    	free(sig);
    }
    if (tmp_b != NULL)
    {
        free(tmp_b);
    }

    printf("mbedtls_ecdsa_verify: exit\n");

    return( ret );
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#if defined(MBEDTLS_ECDSA_GENKEY_ALT)
/*
 * Generate key pair
 */
int mbedtls_ecdsa_genkey( mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid,
                  int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret;
	cy_stc_crypto_ecc_key key;
	cy_stc_crypto_ecc_dp_type *dp;
	int bytesize;
    uint8_t *point_arr = NULL;
    cy_en_crypto_status_t key_gen_status;

    printf("mbedtls_ecdsa_genkey: enter\n");

    cy_en_crypto_ecc_curve_id_t curveId = get_dp_idx(gid);

    MBEDTLS_MPI_CHK((curveId == CY_CRYPTO_ECC_ECP_NONE) ? MBEDTLS_ERR_ECP_BAD_INPUT_DATA : 0);

    cy_reserve_crypto(CY_CRYPTO_VU_HW);

	dp = Cy_Crypto_Core_ECC_GetCurveParams(curveId);

	bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

	point_arr = malloc(3 * bytesize);
    MBEDTLS_MPI_CHK((point_arr == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    key.k         = point_arr;
    key.pubkey.x  = point_arr + bytesize;
    key.pubkey.y  = point_arr + 2 * bytesize;

    key_gen_status = Cy_Crypto_Core_ECC_MakeKeyPair(CRYPTO, curveId, &key, f_rng, p_rng);

    MBEDTLS_MPI_CHK((key_gen_status != CY_CRYPTO_SUCCESS) ? MBEDTLS_ERR_ECP_HW_ACCEL_FAILED : 0);

    Cy_Crypto_Core_InvertEndianness(key.k, bytesize);
    Cy_Crypto_Core_InvertEndianness(key.pubkey.x, bytesize);
    Cy_Crypto_Core_InvertEndianness(key.pubkey.y, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &ctx->d, key.k, bytesize ) );

    /* mbedtls_ecp_point_read_binary needs 4 before point data array */
    point_arr[bytesize - 1] = 0x04;

    MBEDTLS_MPI_CHK( mbedtls_ecp_point_read_binary( &ctx->grp, &ctx->Q, point_arr + bytesize - 1, 2 * bytesize + 1 ) );

cleanup:
    cy_free_crypto(CY_CRYPTO_VU_HW);

    if (point_arr != NULL)
    {
    	free(point_arr);
    }

    printf("mbedtls_ecdsa_genkey: exit\n");

	return (ret);
}
#endif /* MBEDTLS_ECDSA_GENKEY_ALT */

#endif /* MBEDTLS_ECDSA_C */

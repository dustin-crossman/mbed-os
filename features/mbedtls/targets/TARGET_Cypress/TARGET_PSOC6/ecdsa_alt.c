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

/*
 * \file     ecdsa_alt.c
 * \version  1.0
 *
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/platform_util.h"

#include <string.h>
#include <stdlib.h>

#include "cy_crypto_core_ecc.h"
#include "cy_crypto_core_vu.h"

#include "psoc6_utils.h"

#if defined(MBEDTLS_ECDSA_SIGN_ALT)

#define ciL                 (sizeof(mbedtls_mpi_uint))       /* chars in limb  */
#define biL                 (ciL << 3)                       /* bits  in limb  */
#define BITS_TO_LIMBS(i)    ((i) / biL + ((i) % biL != 0))

/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi( const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                       const unsigned char *buf, size_t blen )
{
    int ret;
    size_t n_size = ( grp->nbits + 7 ) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( x, buf, use_size ) );

    if( use_size * 8 > grp->nbits ) {
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( x, use_size * 8 - grp->nbits ) );
    }

    /* While at it, reduce modulo N */
    if( mbedtls_mpi_cmp_mpi( x, &grp->N ) >= 0 ) {
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( x, x, &grp->N ) );
    }

cleanup:
    return( ret );
}

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
    mbedtls_mpi e, t;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
	size_t bytesize;
    uint8_t *sig = NULL;
	uint8_t *tmp_b = NULL;
	uint8_t  tmp_k[CY_CRYPTO_ECC_MAX_BYTE_SIZE];
    cy_en_crypto_status_t ecdsa_sig_status;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if ( grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure d is in range 1..n-1 */
    if ( mbedtls_mpi_cmp_int( d, 1 ) < 0 || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    if (f_rng == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if (buf == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    key.curveID = cy_get_dp_idx(grp->id);

    if (key.curveID == CY_CRYPTO_ECC_ECP_NONE)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    mbedtls_mpi_init( &e ); mbedtls_mpi_init( &t );

    cy_reserve_crypto(CY_CRYPTO_VU_HW);

	dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);

	bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

	key.k = malloc(bytesize);
    MBEDTLS_MPI_CHK((key.k == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, key.k, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(key.k, bytesize);

    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    tmp_b = malloc(CY_CRYPTO_MAX(blen, bytesize) + 1);
    MBEDTLS_MPI_CHK((tmp_b == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    /*
     * Step 5: derive MPI from hashed message
     */
    MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &e, tmp_b, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(tmp_b, bytesize);
    /* ----------------- */

    /* make a PMSN value -> get_priv_key() */
    MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( &t, bytesize, f_rng, p_rng ) );

    if( bytesize * 8 > grp->nbits ) {
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &t, 8 * bytesize - grp->nbits ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &t, tmp_k, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(tmp_k, bytesize);
    /* ----------------- */

    ecdsa_sig_status = Cy_Crypto_Core_ECC_SignHash(CRYPTO, tmp_b, bytesize, sig, &key, tmp_k);

    MBEDTLS_MPI_CHK((ecdsa_sig_status == CY_CRYPTO_SUCCESS) ? 0 : MBEDTLS_ERR_ECP_HW_ACCEL_FAILED);

    Cy_Crypto_Core_InvertEndianness(sig, bytesize);
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( r, sig, bytesize ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( s, sig + bytesize, bytesize ) );

cleanup:
    cy_free_crypto(CY_CRYPTO_VU_HW);

    mbedtls_mpi_free( &e ); mbedtls_mpi_free( &t );

    if (key.k != NULL)
    {
        mbedtls_platform_zeroize(key.k, bytesize);
    	free(key.k);
    }
    if (sig != NULL)
    {
        mbedtls_platform_zeroize(sig, 2 * bytesize);
    	free(sig);
    }
    if (tmp_b != NULL)
    {
        mbedtls_platform_zeroize(tmp_b, CY_CRYPTO_MAX(blen, bytesize) + 1);
        free(tmp_b);
    }

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
    mbedtls_mpi e;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
    size_t bytesize;
	size_t olen;
    uint8_t *sig = NULL;
	uint8_t *tmp_b = NULL;
    uint8_t *point_arr = NULL;
    cy_en_crypto_status_t ecdsa_ver_status;

    if (buf == NULL)
       return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    key.curveID = cy_get_dp_idx(grp->id);

    MBEDTLS_MPI_CHK( (key.curveID == CY_CRYPTO_ECC_ECP_NONE) ? MBEDTLS_ERR_ECP_BAD_INPUT_DATA : 0);

    mbedtls_mpi_init( &e );

    cy_reserve_crypto(CY_CRYPTO_VU_HW);

	dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);
	bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

    point_arr = malloc(2 * bytesize + 1u);
    MBEDTLS_MPI_CHK((point_arr == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);
    key.pubkey.x  = point_arr + 1u;
    key.pubkey.y  = point_arr + bytesize + 1u;

    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( r, sig, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( s, sig + bytesize, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, point_arr, 2 * bytesize + 1) );
    Cy_Crypto_Core_InvertEndianness(key.pubkey.x, bytesize);
    Cy_Crypto_Core_InvertEndianness(key.pubkey.y, bytesize);

    tmp_b = malloc(CY_CRYPTO_MAX(blen, bytesize) + 1);
    MBEDTLS_MPI_CHK((tmp_b == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    /*
     * Step 5: derive MPI from hashed message
     */
    MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &e, tmp_b, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(tmp_b, bytesize);
    /* ----------------- */

    ecdsa_ver_status = Cy_Crypto_Core_ECC_VerifyHash(CRYPTO, sig, tmp_b, bytesize, &stat, &key);

    MBEDTLS_MPI_CHK((ecdsa_ver_status != CY_CRYPTO_SUCCESS) ? MBEDTLS_ERR_ECP_HW_ACCEL_FAILED : 0);

    MBEDTLS_MPI_CHK((stat == 1) ? 0 : MBEDTLS_ERR_ECP_VERIFY_FAILED);

cleanup:
    cy_free_crypto(CY_CRYPTO_VU_HW);

    mbedtls_mpi_free( &e );

    if (point_arr != NULL)
    {
        mbedtls_platform_zeroize(point_arr, 2 * bytesize + 1u);
    	free(point_arr);
    }
    if (sig != NULL)
    {
        mbedtls_platform_zeroize(sig, 2 * bytesize);
    	free(sig);
    }
    if (tmp_b != NULL)
    {
        mbedtls_platform_zeroize(tmp_b, CY_CRYPTO_MAX(blen, bytesize) + 1u);
        free(tmp_b);
    }

    return( ret );
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#endif /* MBEDTLS_ECDSA_C */

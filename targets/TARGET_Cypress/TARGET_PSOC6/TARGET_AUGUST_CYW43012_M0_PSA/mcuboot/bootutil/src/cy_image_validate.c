/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
 /***************************************************************************//**
* \copyright
*
* (c) 2018-2019, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
*
* This software is a port of the open source MCUBoot project.
* This file was modified to fit Cypress Secure Bootloader design.
*
* Portions of this software, including source code, documentation and related
* materials ("Software"), are owned by Cypress Semiconductor
* Corporation or one of its subsidiaries ("Cypress") and is protected by
* and subject to worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-
* exclusive, non-transferable license to copy, modify, and compile the
* Software source code solely for use in connection with Cypress's
* integrated circuit products. Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. Cypress reserves the right to make
* changes to the Software without notice. Cypress does not assume any
* liability arising out of the application or use of the Software or any
* product or circuit described in the Software. Cypress does not
* authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*
********************************************************************************/

#ifdef MCUBOOT_HAVE_ASSERT_H
#include "mcuboot_config/mcuboot_assert.h"
#else
#include <assert.h>
#endif
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

#include "hal/hal_flash.h"

#include <flash_map_backend/flash_map_backend.h>

#include "bootutil/image.h"
#include "bootutil/cy_sha256.h"
#include "bootutil/sign_key.h"

#include "mcuboot_config/mcuboot_config.h"

#ifdef MCUBOOT_SIGN_RSA
#include "mbedtls/rsa.h"
#endif
#if defined(MCUBOOT_SIGN_EC) || defined(MCUBOOT_SIGN_EC256)
#include "mbedtls/ecdsa.h"
#endif
#include "mbedtls/asn1.h"

#include "bootutil_priv.h"

#include "cy_jwt_policy.h"

/*
 * Compute SHA256 over the image.
 */
static int
bootutil_img_hash(struct image_header *hdr, const struct flash_area *fap,
                  uint8_t *tmp_buf, uint32_t tmp_buf_sz,
                  uint8_t *hash_result, uint8_t *seed, int seed_len)
{
    uint32_t blk_sz;
    uint32_t size;
    uint32_t off;
    int rc;
    fb_psa_hash_operation_t psa_op;
    psa_status_t psa_ret;

    psa_ret = bootutil_sha256_init(&psa_op);

    if(0 == psa_ret)
    {
        /* in some cases (split image) the hash is seeded with data from
         * the loader image */
        if (seed && (seed_len > 0)) {
        	psa_ret = bootutil_sha256_update(&psa_op, seed, seed_len);
        }

        if(0 == psa_ret)
        {
			/*
			 * Hash is computed over image header and image itself. No TLV is
			 * included ATM.
			 */
			size = hdr->ih_img_size + hdr->ih_hdr_size;
			for (off = 0; off < size; off += blk_sz) {
				blk_sz = size - off;
				if (blk_sz > tmp_buf_sz) {
					blk_sz = tmp_buf_sz;
				}
				rc = flash_area_read(fap, off, tmp_buf, blk_sz);
				if (rc) {
					return rc;
				}
				psa_ret = bootutil_sha256_update(&psa_op, tmp_buf, blk_sz);
				if(0 != psa_ret)
				{
					break;
				}
			}
			psa_ret = bootutil_sha256_finish(&psa_op, hash_result);
        }
    }
    return (int)psa_ret;
}

/*
 * Currently, we only support being able to verify one type of
 * signature, because there is a single verification function that we
 * call.  List the type of TLV we are expecting.  If we aren't
 * configured for any signature, don't define this macro.
 */
#if defined(MCUBOOT_SIGN_EC256)
#    define EXPECTED_SIG_TLV IMAGE_TLV_ECDSA256
#    define EXPECTED_SIG_LEN(x) ((x) >= 72) /* oids + 2 * 32 bytes */
#endif

#ifdef EXPECTED_SIG_TLV
int
bootutil_find_key(uint8_t *keyhash, uint8_t keyhash_len)
{
	fb_psa_hash_operation_t psa_op;
	psa_status_t psa_ret = -1;

    int i = 0;
    const struct bootutil_key *key;
    uint8_t hash[PSA_HASH_SIZE(PSA_ALG_SHA_256)];

    assert(keyhash_len <= PSA_HASH_SIZE(PSA_ALG_SHA_256));

    if(bootutil_keys[0].key == NULL)
    {/* skip [0] if go through PSA key storage */
    	i = 1;
    }

    for (; i < bootutil_key_cnt; i++) {
        key = &bootutil_keys[i];
        
        if (NULL != key->key)
        {
            psa_ret = bootutil_sha256_init(&psa_op);
            if(0 == psa_ret)
            {
                psa_ret = bootutil_sha256_update(&psa_op, key->key, *key->len);
            }		
            if(0 == psa_ret)
            {
                psa_ret = bootutil_sha256_finish(&psa_op, hash);
            }
            if(0 == psa_ret)
            {
                if (!memcmp(hash, keyhash, keyhash_len)) {
                    return i;
                }
                else
                {
                    psa_ret = -1;
                }
            }
            else
            {
                psa_ret = -1;
            }
        }
    }
    return psa_ret;
}
#endif

extern bnu_policy_t bnu_policy;
int cy_bootutil_find_key(int slot)
{
    int key = 0;

    if(0 == slot)
    {
        key = bnu_policy.bnu_img_policy.boot_auth[0];
    }
    else if(1 == slot)
    {
        key = bnu_policy.bnu_img_policy.upgrade_auth[0];
    }

    return key;
}

#if defined(MCUBOOT_USE_FLASHBOOT_CRYPTO)
/*
 * Verify the integrity of the image.
 * Return non-zero if image could not be validated/does not validate.
 */
int
bootutil_img_validate(struct image_header *hdr, const struct flash_area *fap,
                      uint8_t *tmp_buf, uint32_t tmp_buf_sz,
                      uint8_t *seed, int seed_len, uint8_t *out_hash,
                      int slot)
{
    uint32_t off;
    uint32_t end;
    int sha256_valid = 0;
    struct image_tlv_info info;
#ifdef EXPECTED_SIG_TLV
    int valid_signature = 0;
    int key_id = -1;
#endif /* EXPECTED_SIG_TLV */
    struct image_tlv tlv;
    uint8_t buf[256];
    uint8_t hash[32];
    int rc;

    rc = bootutil_img_hash(hdr, fap, tmp_buf, tmp_buf_sz, hash,
                           seed, seed_len);
    if (rc) {
        return rc;
    }

    if (out_hash) {
        memcpy(out_hash, hash, 32);
    }

    /* The TLVs come after the image. */
    /* After image there are TLVs. */
    off = hdr->ih_img_size + hdr->ih_hdr_size;

    rc = flash_area_read(fap, off, &info, sizeof(info));
    if (rc) {
        return rc;
    }
    if (info.it_magic != IMAGE_TLV_INFO_MAGIC) {
        return -1;
    }
    end = off + info.it_tlv_tot;
    off += sizeof(info);

    /*
     * Traverse through all of the TLVs, performing any checks we know
     * and are able to do.
     */
    for (; off < end; off += sizeof(tlv) + tlv.it_len) {
        rc = flash_area_read(fap, off, &tlv, sizeof tlv);
        if (rc) {
            return rc;
        }

        if (tlv.it_type == IMAGE_TLV_SHA256) {
            /*
             * Verify the SHA256 image hash.  This must always be
             * present.
             */
            if (tlv.it_len != sizeof(hash)) {
                return -1;
            }
            rc = flash_area_read(fap, off + sizeof(tlv), buf, sizeof hash);
            if (rc) {
                return rc;
            }
            if (memcmp(hash, buf, sizeof(hash))) {
                return -1;
            }

            sha256_valid = 1;
#ifdef EXPECTED_SIG_TLV
        } else if (tlv.it_type == IMAGE_TLV_KEYHASH) {
            /*
             * Determine which key we should be checking.
             */
            key_id = cy_bootutil_find_key(slot);
        } else if (tlv.it_type == EXPECTED_SIG_TLV) {
            /* Ignore this signature if it is out of bounds. */
//            if (key_id < 0 || key_id >= bootutil_key_cnt) {
//                key_id = -1;
//                continue;
//            }
            if (!EXPECTED_SIG_LEN(tlv.it_len) || tlv.it_len > sizeof(buf)) {
                return -1;
            }
            rc = flash_area_read(fap, off + sizeof(tlv), buf, tlv.it_len);
            if (rc) {
                return -1;
            }
            rc = cy_bootutil_verify_sig(hash, sizeof(hash), buf, tlv.it_len, key_id);
            if (rc == 0) {
                valid_signature = 1;
            }
            key_id = -1;
#endif /* EXPECTED_SIG_TLV */
        }
    }

    if (!sha256_valid) {
        return -1;
    }

#ifdef EXPECTED_SIG_TLV
    if (!valid_signature) {
        return -1;
    }
#endif /* EXPECTED_SIG_TLV */

    return 0;
}

#else

/*
 * Verify the integrity of the image.
 * Return non-zero if image could not be validated/does not validate.
 */
int
bootutil_img_validate(struct image_header *hdr, const struct flash_area *fap,
                      uint8_t *tmp_buf, uint32_t tmp_buf_sz,
                      uint8_t *seed, int seed_len, uint8_t *out_hash)
{
    uint32_t off;
    uint32_t end;
    int sha256_valid = 0;
    struct image_tlv_info info;
#ifdef EXPECTED_SIG_TLV
    int valid_signature = 0;
    int key_id = -1;
#endif
    struct image_tlv tlv;
    uint8_t buf[256];
    uint8_t hash[PSA_HASH_SIZE(PSA_ALG_SHA_256)];
    int rc;

    rc = bootutil_img_hash(hdr, fap, tmp_buf, tmp_buf_sz, hash,
                           seed, seed_len);
    if (rc) {
        return rc;
    }

    if (out_hash) {
        memcpy(out_hash, hash, PSA_HASH_SIZE(PSA_ALG_SHA_256));
    }

    /* The TLVs come after the image. */
    /* After image there are TLVs. */
    off = hdr->ih_img_size + hdr->ih_hdr_size;

    rc = flash_area_read(fap, off, &info, sizeof(info));
    if (rc) {
        return rc;
    }
    if (info.it_magic != IMAGE_TLV_INFO_MAGIC) {
        return -1;
    }
    end = off + info.it_tlv_tot;
    off += sizeof(info);

    /*
     * Traverse through all of the TLVs, performing any checks we know
     * and are able to do.
     */
    for (; off < end; off += sizeof(tlv) + tlv.it_len) {
        rc = flash_area_read(fap, off, &tlv, sizeof tlv);
        if (rc) {
            return rc;
        }

        if (tlv.it_type == IMAGE_TLV_SHA256) {
            /*
             * Verify the SHA256 image hash.  This must always be
             * present.
             */
            if (tlv.it_len != sizeof(hash)) {
                return -1;
            }
            rc = flash_area_read(fap, off + sizeof(tlv), buf, sizeof hash);
            if (rc) {
                return rc;
            }
            if (memcmp(hash, buf, sizeof(hash))) {
                return -1;
            }

            sha256_valid = 1;
#ifdef EXPECTED_SIG_TLV
        } else if (tlv.it_type == IMAGE_TLV_KEYHASH) {
            /*
             * Determine which key we should be checking.
             */
            if (tlv.it_len > PSA_HASH_SIZE(PSA_ALG_SHA_256)) {
                return -1;
            }
            rc = flash_area_read(fap, off + sizeof tlv, buf, tlv.it_len);
            if (rc) {
                return rc;
            }
            key_id = bootutil_find_key(buf, tlv.it_len);
            /*
             * The key may not be found, which is acceptable.  There
             * can be multiple signatures, each preceded by a key.
             */
        } else if (tlv.it_type == EXPECTED_SIG_TLV) {
            /* Ignore this signature if it is out of bounds. */
            if (key_id < 0 || key_id >= bootutil_key_cnt) {
                key_id = -1;
                continue;
            }
            if (!EXPECTED_SIG_LEN(tlv.it_len) || tlv.it_len > sizeof(buf)) {
                return -1;
            }
            rc = flash_area_read(fap, off + sizeof(tlv), buf, tlv.it_len);
            if (rc) {
                return -1;
            }
            rc = bootutil_verify_sig(hash, sizeof(hash), buf, tlv.it_len, key_id);
            if (rc == 0) {
                valid_signature = 1;
            }
            key_id = -1;
#endif
        }
    }

    if (!sha256_valid) {
        return -1;
    }

#ifdef EXPECTED_SIG_TLV
    if (!valid_signature) {
        return -1;
    }
#endif

    return 0;
}

#endif /* MCUBOOT_USE_FLASHBOOT_CRYPTO */

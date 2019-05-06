/*
 *
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cy_syslib.h"
#include "cy_ipc_drv.h"

#include "psa/crypto.h"
#include "flashboot_psacrypto.h"

/** PSA crypto SysCall opcode */
#define PSACRYPTO_SYSCALL_OPCODE        (0x35UL << 24UL)

/** PSA crypto function code */
#define CY_FB_SYSCALL_PSA_ASYMETRIC_VERIFY          (0)
#define CY_FB_SYSCALL_PSA_EXPORT_PUBLIC_KEY         (1)
#define CY_FB_SYSCALL_PSA_GET_KEY_INFO              (2)
#define CY_FB_SYSCALL_PSA_KEY_AGREEMENT             (3)
#define CY_FB_SYSCALL_PSA_GENERATOR_READ            (4)
#define CY_FB_SYSCALL_PSA_GENERATOR_ABORT           (5)
#define CY_FB_SYSCALL_PSA_KEY_POLICY_INIT           (6)
#define CY_FB_SYSCALL_PSA_KEY_POLICY_SET_USAGE      (7)
#define CY_FB_SYSCALL_PSA_SET_KEY_POLICY            (8)
#define CY_FB_SYSCALL_PSA_IMPORT_KEY                (9)
#define CY_FB_SYSCALL_PSA_DESTROY_KEY               (10)
#define CY_FB_SYSCALL_PSA_CIPHER_DECRYPT_SETUP      (11)
#define CY_FB_SYSCALL_PSA_CIPHER_IV                 (12)
#define CY_FB_SYSCALL_PSA_CIPHER_UPDATE             (13)
#define CY_FB_SYSCALL_PSA_CIPHER_FINISH             (14)
#define CY_FB_SYSCALL_PSA_GENERATE_RANDOM           (15)
#define CY_FB_SYSCALL_PSA_HASH_SETUP                (16)
#define CY_FB_SYSCALL_PSA_HASH_UPDATE               (17)
#define CY_FB_SYSCALL_PSA_HASH_FINISH               (18)

/** PSA crypto SysCall return codes */
#define CY_FB_SYSCALL_SUCCESS           (0xA0000000UL)

/** Timeout values */
#define PSACRYPTO_SYSCALL_TIMEOUT_SHORT (15000UL)
#define PSACRYPTO_SYSCALL_TIMEOUT_LONG  (2000000000UL)

/**
 *
 */
psa_status_t Cy_SysCall_Psa(uint32_t *syscallCmd)
{
    psa_status_t status = PSA_SUCCESS;
    uint32_t timeout = 0;

    IPC->STRUCT[CY_IPC_CHAN_SYSCALL].DATA =  (uint32_t)syscallCmd;

    while(((IPC->STRUCT[CY_IPC_CHAN_SYSCALL].ACQUIRE &
            IPC_STRUCT_ACQUIRE_SUCCESS_Msk) == 0) &&
            (timeout < PSACRYPTO_SYSCALL_TIMEOUT_SHORT))
    {
        ++timeout;
    }

    if(timeout < PSACRYPTO_SYSCALL_TIMEOUT_SHORT)
    {
        timeout = 0;

        IPC->STRUCT[CY_IPC_CHAN_SYSCALL].NOTIFY = 1;

        while(((IPC->STRUCT[CY_IPC_CHAN_SYSCALL].LOCK_STATUS &
                IPC_STRUCT_ACQUIRE_SUCCESS_Msk) != 0) &&
                (timeout < PSACRYPTO_SYSCALL_TIMEOUT_LONG))
        {
            ++timeout;
        }

        if(timeout < PSACRYPTO_SYSCALL_TIMEOUT_LONG)
        {
            if(CY_FB_SYSCALL_SUCCESS != syscallCmd[0])
            {
                status = syscallCmd[0];
            }
        }
        else
        {
            status = PSA_ERROR_UNKNOWN_ERROR;
        }
    }
    else
    {
        status = PSA_ERROR_UNKNOWN_ERROR;
    }
    return status;
}

/**
 *
 */
psa_status_t fb_psa_asymmetric_verify( fb_psa_key_slot_t key,
                                    fb_psa_algorithm_t alg,
                                    const uint8_t *hash,
                                    size_t hash_length,
                                    const uint8_t *signature,
                                    size_t signature_length )
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_ASYMETRIC_VERIFY<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;
    syscall_param[1] = alg;
    syscall_param[2] = (uint32_t)hash;
    syscall_param[3] = hash_length;
    syscall_param[4] = (uint32_t)signature;
    syscall_param[5] = signature_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_cipher_decrypt_setup(fb_psa_cipher_operation_t *operation,
                                        fb_psa_key_slot_t key,
                                        fb_psa_algorithm_t alg)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_CIPHER_DECRYPT_SETUP<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = key;
    syscall_param[2] = alg;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_cipher_finish(fb_psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_CIPHER_FINISH<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = (uint32_t)output;
    syscall_param[2] = output_size;
    syscall_param[3] = (uint32_t)output_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_cipher_set_iv(fb_psa_cipher_operation_t *operation,
                                    const unsigned char *iv,
                                    size_t iv_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_CIPHER_IV<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = (uint32_t)iv;
    syscall_param[2] = iv_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_cipher_update(fb_psa_cipher_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length,
                                unsigned char *output,
                                size_t output_size,
                                size_t *output_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_CIPHER_UPDATE<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = (uint32_t)input;
    syscall_param[2] = input_length;
    syscall_param[3] = (uint32_t)output;
    syscall_param[4] = output_size;
    syscall_param[5] = (uint32_t)output_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

//psa_status_t fb_psa_crypto_init(void)
//{
//    psa_status_t status = PSA_SUCCESS;
//// TODO: not implemented in FB
////    uint32_t syscall_param[6];
////    uint32_t syscall_cmd[2];
////
////    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_<<8);
////    syscall_cmd[1] = (uint32_t)syscall_param;
////
////    status = Cy_SysCall_Psa(syscall_cmd);
//
//    return status;
//}

psa_status_t fb_psa_destroy_key(fb_psa_key_slot_t key)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_DESTROY_KEY<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

//psa_status_t fb_psa_generate_random(uint8_t *output,
//                                    size_t output_size)
//{
//    psa_status_t status = PSA_SUCCESS;
//
//    uint32_t syscall_param[6];
//    uint32_t syscall_cmd[2];
//
//    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_GENERATE_RANDOM<<8);
//    syscall_cmd[1] = (uint32_t)syscall_param;
//
//    syscall_param[0] = (uint32_t)output;
//    syscall_param[1] = output_size;
//
//    status = Cy_SysCall_Psa(syscall_cmd);
//
//    return status;
//}

psa_status_t fb_psa_generator_abort(fb_psa_crypto_generator_t *generator)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_GENERATOR_ABORT<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)generator;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_generator_read(fb_psa_crypto_generator_t *generator,
                                    uint8_t *output,
                                    size_t output_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_param[6];
    uint32_t syscall_cmd[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_GENERATOR_READ<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)generator;
    syscall_param[1] = (uint32_t)output;
    syscall_param[2] = output_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_hash_setup(fb_psa_hash_operation_t *operation,
                                fb_psa_algorithm_t alg)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[2];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_HASH_SETUP<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = alg;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_hash_update(fb_psa_hash_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[3];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_HASH_UPDATE<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = (uint32_t)input;
    syscall_param[2] = input_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_hash_finish(fb_psa_hash_operation_t *operation,
                                uint8_t *hash,
                                size_t hash_size,
                                size_t *hash_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[4];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_HASH_FINISH<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)operation;
    syscall_param[1] = (uint32_t)hash;
    syscall_param[2] = hash_size;
    syscall_param[3] = (uint32_t)hash_length;

    status = Cy_SysCall_Psa(syscall_cmd);
    /* TBD */
    return status;
}

psa_status_t fb_psa_import_key(fb_psa_key_slot_t key,
                                fb_psa_key_type_t type,
                                const uint8_t *data,
                                size_t data_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[4];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_IMPORT_KEY<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;
    syscall_param[1] = type;
    syscall_param[2] = (uint32_t)data;
    syscall_param[3] = data_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

//psa_status_t fb_psa_key_derivation(fb_psa_crypto_generator_t *generator,
//                                    fb_psa_key_slot_t key,
//                                    fb_psa_algorithm_t alg,
//                                    const uint8_t *salt,
//                                    size_t salt_length,
//                                    const uint8_t *label,
//                                    size_t label_length,
//                                    size_t capacity)
//{
//    psa_status_t status = PSA_SUCCESS;
//
////    uint32_t syscall_cmd[2];
////    uint32_t syscall_param[4];
////
////    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_<<8);
////    syscall_cmd[1] = (uint32_t)syscall_param;
////
////
////    status = Cy_SysCall_Psa(syscall_cmd);
//
//    return status;
//}

void fb_psa_key_policy_init(fb_psa_key_policy_t *policy)
{
    uint32_t syscall_cmd[2];
    uint32_t syscall_param[3];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_KEY_POLICY_INIT<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)policy;

    (void)Cy_SysCall_Psa(syscall_cmd);
}

void fb_psa_key_policy_set_usage(fb_psa_key_policy_t *policy,
                                  fb_psa_key_usage_t usage,
                                  fb_psa_algorithm_t alg)
{
    uint32_t syscall_cmd[2];
    uint32_t syscall_param[3];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_KEY_POLICY_SET_USAGE<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)policy;
    syscall_param[1] = usage;
    syscall_param[2] = alg;

    (void)Cy_SysCall_Psa(syscall_cmd);
}

psa_status_t fb_psa_set_key_policy( fb_psa_key_slot_t key,
                                const fb_psa_key_policy_t *policy )
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[3];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_SET_KEY_POLICY<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;
    syscall_param[1] = (uint32_t)policy;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}


psa_status_t fb_psa_get_key_information(fb_psa_key_slot_t key,
                                     fb_psa_key_type_t *type,
                                     size_t *bits)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[3];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_GET_KEY_INFO<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;
    syscall_param[1] = (uint32_t)type;
    syscall_param[2] = (uint32_t)bits;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_export_public_key(fb_psa_key_slot_t key,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[4];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_EXPORT_PUBLIC_KEY<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = key;
    syscall_param[1] = (uint32_t)data;
    syscall_param[2] = data_size;
    syscall_param[3] = (uint32_t)data_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

psa_status_t fb_psa_key_agreement_salt_label( fb_psa_crypto_generator_t *generator,
                                fb_psa_key_slot_t private_key,
                                const uint8_t *peer_key,
                                size_t peer_key_length,
                                fb_psa_algorithm_t alg,
                                const uint8_t *salt,
                                size_t salt_length,
                                const uint8_t *label,
                                size_t label_length)
{
    psa_status_t status = PSA_SUCCESS;

    uint32_t syscall_cmd[2];
    uint32_t syscall_param[9];

    syscall_cmd[0] = PSACRYPTO_SYSCALL_OPCODE + (CY_FB_SYSCALL_PSA_KEY_AGREEMENT<<8);
    syscall_cmd[1] = (uint32_t)syscall_param;

    syscall_param[0] = (uint32_t)generator;
    syscall_param[1] = private_key;
    syscall_param[2] = (uint32_t)peer_key;
    syscall_param[3] = peer_key_length;
    syscall_param[4] = alg;
    syscall_param[5] = (uint32_t)salt;
    syscall_param[6] = salt_length;
    syscall_param[7] = (uint32_t)label;
    syscall_param[8] = label_length;

    status = Cy_SysCall_Psa(syscall_cmd);

    return status;
}

/* Copyright 2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/*
 * THIS FILE MUST BE USED WHEN BUILDING FOR NON-SECURE ZONE.
 * IT WILL CREATE STRUCTURES OF INPUT VARIABLES AND PASS TO VENEER_TABLE.C
 * THE ACTUAL IMPLEMENTATION WILL BE IN VENEER_TABLE.C
 * VENEER_TABLE.C WILL BE RUNNING IN SECURE ZONE WHILE THIS FILE WILL BE IN NON-SECURE ZONE
 */

#include <nxLog_App.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "psa/crypto.h"
#include "psa/crypto_se_driver.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa_alt.h"
#include "psa_alt_utils.h"
#include "psa_crypto_storage.h"
#include "sss_psa_alt.h"
#include "veneer_helper.h"
#include "veneer_table.h"

/*
 * TODO: Marshall input / use specific structure and call Secure Function
 */

psa_status_t psa_alt_driver_init(psa_drv_se_context_t *drv_context, void *persistent_data, psa_key_lifetime_t lifetime)
{
    /* Marshall input / use specific structure and call Secure Function
     * return veneer_psa_alt_driver_init(<marshalled input>);
     */

    psa_alt_driver_init_struct_t psa_alt_driver_init_struct = {
        .drv_context = drv_context, .persistent_data = persistent_data, .lifetime = lifetime};

    return veneer_psa_alt_driver_init(&psa_alt_driver_init_struct);
}

psa_status_t psa_alt_allocate_key(psa_drv_se_context_t *drv_context,
    void *persistent_data,
    const psa_key_attributes_t *attributes,
    psa_key_creation_method_t method,
    psa_key_slot_number_t *key_slot)
{
    psa_alt_allocate_key_struct_t psa_alt_allocate_key_struct = {.drv_context = drv_context,
        .persistent_data                                                      = persistent_data,
        .attributes                                                           = attributes,
        .method                                                               = method,
        .key_slot                                                             = key_slot};
    return veneer_psa_alt_allocate_key(&psa_alt_allocate_key_struct);
}

psa_status_t psa_alt_validate_slot_number(psa_drv_se_context_t *drv_context,
    void *persistent_data,
    const psa_key_attributes_t *attributes,
    psa_key_creation_method_t method,
    psa_key_slot_number_t key_slot)
{
    psa_alt_validate_slot_number_struct_t psa_alt_validate_slot_number_struct = {.drv_context = drv_context,
        .persistent_data                                                                      = persistent_data,
        .attributes                                                                           = attributes,
        .method                                                                               = method,
        .key_slot                                                                             = key_slot};

    return veneer_psa_alt_validate_slot_number(&psa_alt_validate_slot_number_struct);
}

psa_status_t psa_alt_import_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    const psa_key_attributes_t *attributes,
    const uint8_t *data,
    size_t data_length,
    size_t *bits)
{
    psa_alt_import_key_struct_t psa_alt_import_key_struct = {.drv_context = drv_context,
        .key_slot                                                         = key_slot,
        .attributes                                                       = attributes,
        .data                                                             = data,
        .data_length                                                      = data_length,
        .bits                                                             = bits};

    return veneer_psa_alt_import_key(&psa_alt_import_key_struct);
}

psa_status_t psa_alt_generate_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    const psa_key_attributes_t *attributes,
    uint8_t *pubkey,
    size_t pubkey_size,
    size_t *pubkey_length)
{
    psa_alt_generate_key_struct_t psa_alt_generate_key_struct = {drv_context = drv_context,
        key_slot                                                             = key_slot,
        attributes                                                           = attributes,
        pubkey                                                               = pubkey,
        pubkey_size                                                          = pubkey_size,
        pubkey_length                                                        = pubkey_length};

    return veneer_psa_alt_generate_key(&psa_alt_generate_key_struct);
}

psa_status_t psa_alt_destroy_key(
    psa_drv_se_context_t *drv_context, void *persistent_data, psa_key_slot_number_t key_slot)
{
    psa_alt_destroy_key_struct_t psa_alt_destroy_key_struct = {
        .drv_context = drv_context, .persistent_data = persistent_data, .key_slot = key_slot};

    return veneer_psa_alt_destroy_key(&psa_alt_destroy_key_struct);
}

psa_status_t psa_alt_export_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key,
    uint8_t *p_data,
    size_t data_size,
    size_t *p_data_length)
{
    psa_alt_export_key_struct_t psa_alt_export_key_struct = {.drv_context = drv_context,
        .key                                                              = key,
        .p_data                                                           = p_data,
        .data_size                                                        = data_size,
        .p_data_length                                                    = p_data_length};

    return veneer_psa_alt_export_key(&psa_alt_export_key_struct);
}

psa_status_t psa_alt_cipher_setup(psa_drv_se_context_t *drv_context,
    void *op_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t algorithm,
    psa_encrypt_or_decrypt_t direction)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_cipher_set_iv(void *op_context, const uint8_t *p_iv, size_t iv_length)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_cipher_update(void *op_context,
    const uint8_t *p_input,
    size_t input_size,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_cipher_finish(void *op_context, uint8_t *p_output, size_t output_size, size_t *p_output_length)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_cipher_abort(void *op_context)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_cipher_ecb(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t algorithm,
    psa_encrypt_or_decrypt_t direction,
    const uint8_t *p_input,
    size_t input_size,
    uint8_t *p_output,
    size_t output_size)
{
    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t psa_alt_asymmetric_sign_digest(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_hash,
    size_t hash_length,
    uint8_t *p_signature,
    size_t signature_size,
    size_t *p_signature_length)
{
    psa_alt_asymmetric_sign_digest_struct_t psa_alt_asymmetric_sign_digest_struct = {.drv_context = drv_context,
        .key_slot                                                                                 = key_slot,
        .alg                                                                                      = alg,
        .p_hash                                                                                   = p_hash,
        .hash_length                                                                              = hash_length,
        .p_signature                                                                              = p_signature,
        .signature_size                                                                           = signature_size,
        .p_signature_length                                                                       = p_signature_length};

    return veneer_psa_alt_asymmetric_sign_digest(&psa_alt_asymmetric_sign_digest_struct);
}

psa_status_t psa_alt_asymmetric_verify_digest(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_hash,
    size_t hash_length,
    const uint8_t *p_signature,
    size_t signature_length)
{
    psa_alt_asymmetric_verify_digest_struct_t psa_alt_asymmetric_verify_digest_struct = {.drv_context = drv_context,
        .key_slot                                                                                     = key_slot,
        .alg                                                                                          = alg,
        .p_hash                                                                                       = p_hash,
        .hash_length                                                                                  = hash_length,
        .p_signature                                                                                  = p_signature,
        .signature_length = signature_length};

    return veneer_psa_alt_asymmetric_verify_digest(&psa_alt_asymmetric_verify_digest_struct);
}

psa_status_t psa_alt_asymmetric_encrypt(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_input,
    size_t input_length,
    const uint8_t *p_salt,
    size_t salt_length,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length)
{
    psa_alt_asymmetric_encrypt_struct_t psa_alt_asymmetric_encrypt_struct = {.drv_context = drv_context,
        .key_slot                                                                         = key_slot,
        .alg                                                                              = alg,
        .p_input                                                                          = p_input,
        .input_length                                                                     = input_length,
        .p_salt                                                                           = p_salt,
        .salt_length                                                                      = salt_length,
        .p_output                                                                         = p_output,
        .output_size                                                                      = output_size,
        .p_output_length                                                                  = p_output_length};

    return veneer_psa_alt_asymmetric_encrypt(&psa_alt_asymmetric_encrypt_struct);
}

psa_status_t psa_alt_asymmetric_decrypt(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_input,
    size_t input_length,
    const uint8_t *p_salt,
    size_t salt_length,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length)
{
    psa_alt_asymmetric_decrypt_struct_t psa_alt_asymmetric_decrypt_struct = {.drv_context = drv_context,
        .key_slot                                                                         = key_slot,
        .alg                                                                              = alg,
        .p_input                                                                          = p_input,
        .input_length                                                                     = input_length,
        .p_salt                                                                           = p_salt,
        .salt_length                                                                      = salt_length,
        .p_output                                                                         = p_output,
        .output_size                                                                      = output_size,
        .p_output_length                                                                  = p_output_length};

    return veneer_psa_alt_asymmetric_decrypt(&psa_alt_asymmetric_decrypt_struct);
}

psa_status_t psa_alt_store_its_file(psa_storage_uid_t uid, uint8_t *data, size_t dataLen)
{
    return veneer_psa_alt_store_its_file(uid, data, dataLen);
}

psa_status_t psa_alt_read_its_file(psa_storage_uid_t uid, uint8_t *data, size_t *dataLen)
{
    return veneer_psa_alt_read_its_file(uid, data, dataLen);
}

psa_status_t psa_alt_remove_its_file(psa_storage_uid_t uid)
{
    return veneer_psa_alt_remove_its_file(uid);
}

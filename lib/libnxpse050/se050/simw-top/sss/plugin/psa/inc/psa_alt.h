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

#ifndef _PSA_ALT_H_
#define _PSA_ALT_H_

#include "psa/crypto.h"
#include "psa/crypto_se_driver.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa_crypto_its.h"
#include "psa_crypto_se.h"

/************************************************************************
 * Definitions
 ************************************************************************/

#define PSA_ALT_SE05X_LIFETIME (PSA_CRYPTO_SE_DRIVER_ITS_UID_BASE + 0x50)
/* PSA_CRYPTO_SE_DRIVER_ITS_UID_BASE = 0xfffffe00 */
/* Transaction file : PSA_CRYPTO_ITS_TRANSACTION_UID = 0xffffff74 */

/* Driver keystore file is defined as File Permissions (32-bit) (PSA_CRYPTO_SE_DRIVER_ITS_UID_BASE (32-bit) + lifetime) */
#define PSA_ALT_TRANSACTION_FILE 0x1FFFFFFE
#define PSA_ALT_LIFETIME_FILE 0x1FFFFFFF
#define PSA_ALT_OBJECT_FILE_START 0x20000000
#define PSA_ALT_OBJECT_FILE_MASK 0xF0000000
#define PSA_ALT_OBJECT_FILE_END 0x2FFFFFFF
#define PSA_ALT_OBJECT_START 0x30000000
#define PSA_ALT_OBJECT_END 0x3FFFFFFF

#define PSA_KEY_ID_TO_ALT_OBJECT_ID(id) ((id & (~PSA_ALT_OBJECT_FILE_MASK)) | PSA_ALT_OBJECT_START)

/** The driver initialization function.
 *
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_driver_init(psa_drv_se_context_t *drv_context, void *persistent_data, psa_key_lifetime_t lifetime);

/** psa_drv_se_key_management_t APIs */

/** Function that allocates a slot for a key.
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_allocate_key(psa_drv_se_context_t *drv_context,
    void *persistent_data,
    const psa_key_attributes_t *attributes,
    psa_key_creation_method_t method,
    psa_key_slot_number_t *key_slot);

/** Function that checks the validity of a slot for a key.
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_validate_slot_number(psa_drv_se_context_t *drv_context,
    void *persistent_data,
    const psa_key_attributes_t *attributes,
    psa_key_creation_method_t method,
    psa_key_slot_number_t key_slot);

/** Function that performs a key import operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_import_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    const psa_key_attributes_t *attributes,
    const uint8_t *data,
    size_t data_length,
    size_t *bits);

/** Function that performs a generation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_generate_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    const psa_key_attributes_t *attributes,
    uint8_t *pubkey,
    size_t pubkey_size,
    size_t *pubkey_length);

/** Function that performs a key destroy operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_destroy_key(
    psa_drv_se_context_t *drv_context, void *persistent_data, psa_key_slot_number_t key_slot);

/** Function that performs a key export operation
 *  Function that performs a public key export operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_export_key(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key,
    uint8_t *p_data,
    size_t data_size,
    size_t *p_data_length);

/** psa_drv_se_mac_t APIs */
/** psa_drv_se_cipher_t APIs */

/** A function that provides the cipher setup function for a
 *  secure element driver
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_setup(psa_drv_se_context_t *drv_context,
    void *op_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t algorithm,
    psa_encrypt_or_decrypt_t direction);

/** A function that sets the initialization vector (if
 *  necessary) for an secure element cipher operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_set_iv(void *op_context, const uint8_t *p_iv, size_t iv_length);

/** A function that continues a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_update(void *op_context,
    const uint8_t *p_input,
    size_t input_size,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length);

/** A function that completes a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_finish(void *op_context, uint8_t *p_output, size_t output_size, size_t *p_output_length);

/** A function that aborts a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_abort(void *op_context);

/** A function that performs the ECB block mode for secure element
 *  cipher operations
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_cipher_ecb(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t algorithm,
    psa_encrypt_or_decrypt_t direction,
    const uint8_t *p_input,
    size_t input_size,
    uint8_t *p_output,
    size_t output_size);

/** psa_drv_se_aead_t APIs */
/** psa_drv_se_asymmetric_t APIs */

/**
 * A function that signs a hash or short message with a private key in
 * a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */

psa_status_t psa_alt_asymmetric_sign_digest(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_hash,
    size_t hash_length,
    uint8_t *p_signature,
    size_t signature_size,
    size_t *p_signature_length);

/**
 * A function that verifies the signature a hash or short message using
 * an asymmetric public key in a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_asymmetric_verify_digest(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_hash,
    size_t hash_length,
    const uint8_t *p_signature,
    size_t signature_length);

/**
 * A function that encrypts a short message with an asymmetric public
 * key in a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_asymmetric_encrypt(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_input,
    size_t input_length,
    const uint8_t *p_salt,
    size_t salt_length,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length);

/**
 * A function that decrypts a short message with an asymmetric private
 * key in a secure element.
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t psa_alt_asymmetric_decrypt(psa_drv_se_context_t *drv_context,
    psa_key_slot_number_t key_slot,
    psa_algorithm_t alg,
    const uint8_t *p_input,
    size_t input_length,
    const uint8_t *p_salt,
    size_t salt_length,
    uint8_t *p_output,
    size_t output_size,
    size_t *p_output_length);

/** psa_drv_se_key_derivation_t APIs */

psa_status_t psa_alt_store_its_file(psa_storage_uid_t uid, uint8_t *data, size_t dataLen);

psa_status_t psa_alt_read_its_file(psa_storage_uid_t uid, uint8_t *data, size_t *dataLen);

psa_status_t psa_alt_remove_its_file(psa_storage_uid_t uid);

#endif //_PSA_ALT_H_

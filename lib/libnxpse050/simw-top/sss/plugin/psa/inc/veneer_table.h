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

#ifndef _VENEER_PSA_ALT_H_
#define _VENEER_PSA_ALT_H_

#include "psa/crypto.h"
#include "psa/crypto_se_driver.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa_crypto_its.h"
#include "psa_crypto_se.h"
#include "veneer_helper.h"

/************************************************************************
 * Definitions
 ************************************************************************/

void DbgConsole_Printf_NSE(char const *s);

/** The driver initialization function.
 *
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_driver_init(psa_alt_driver_init_struct_t *veneer_psa_alt_driver_init_struct);

// /** psa_drv_se_key_management_t APIs */

/** Function that allocates a slot for a key.
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_allocate_key(psa_alt_allocate_key_struct_t *veneer_psa_alt_allocate_key_struct);

/** Function that checks the validity of a slot for a key.
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_validate_slot_number(
    psa_alt_validate_slot_number_struct_t *veneer_psa_alt_validate_slot_number_struct);

/** Function that performs a key import operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_import_key(psa_alt_import_key_struct_t *veneer_psa_alt_import_key_struct);

/** Function that performs a generation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_generate_key(psa_alt_generate_key_struct_t *veneer_psa_alt_generate_key_struct);

/** Function that performs a key destroy operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_destroy_key(psa_alt_destroy_key_struct_t *veneer_psa_alt_destroy_key_struct);

/** Function that performs a key export operation
 *  Function that performs a public key export operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_export_key(psa_alt_export_key_struct_t *veneer_psa_alt_export_key_struct);

/** psa_drv_se_mac_t APIs */
/** psa_drv_se_cipher_t APIs */

/** A function that provides the cipher setup function for a
 *  secure element driver
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_setup(psa_alt_cipher_setup_struct_t *veneer_psa_alt_cipher_setup_struct);

/** A function that sets the initialization vector (if
 *  necessary) for an secure element cipher operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_set_iv(psa_alt_cipher_set_iv_struct_t *veneer_psa_alt_cipher_set_iv_struct);

/** A function that continues a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_update(psa_alt_cipher_update_struct_t *veneer_psa_alt_cipher_update_struct);

/** A function that completes a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_finish(psa_alt_cipher_finish_struct_t *veneer_psa_alt_cipher_finish_struct);

/** A function that aborts a previously started secure element cipher
 *  operation
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_abort(void *op_context);

/** A function that performs the ECB block mode for secure element
 *  cipher operations
 *  Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_cipher_ecb(psa_alt_cipher_ecb_struct_t *veneer_psa_alt_cipher_ecb_struct);

/** psa_drv_se_aead_t APIs */
/** psa_drv_se_asymmetric_t APIs */

/**
 * A function that signs a hash or short message with a private key in
 * a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */

psa_status_t veneer_psa_alt_asymmetric_sign_digest(
    psa_alt_asymmetric_sign_digest_struct_t *veneer_psa_alt_asymmetric_sign_digest_struct);

/**
 * A function that verifies the signature a hash or short message using
 * an asymmetric public key in a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_asymmetric_verify_digest(
    psa_alt_asymmetric_verify_digest_struct_t *veneer_psa_alt_asymmetric_verify_digest_struct);

/**
 * A function that encrypts a short message with an asymmetric public
 * key in a secure element
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_asymmetric_encrypt(
    psa_alt_asymmetric_encrypt_struct_t *veneer_psa_alt_asymmetric_encrypt_struct);

/**
 * A function that decrypts a short message with an asymmetric private
 * key in a secure element.
 * Refer to mbed-crypto\include\psa\crypto_se_driver.h
 */
psa_status_t veneer_psa_alt_asymmetric_decrypt(
    psa_alt_asymmetric_decrypt_struct_t *veneer_psa_alt_asymmetric_decrypt_struct);

/** psa_drv_se_key_derivation_t APIs */

psa_status_t veneer_psa_alt_store_its_file(psa_storage_uid_t uid, uint8_t *data, size_t dataLen);

psa_status_t veneer_psa_alt_read_its_file(psa_storage_uid_t uid, uint8_t *data, size_t *dataLen);

psa_status_t veneer_psa_alt_remove_its_file(psa_storage_uid_t uid);

#endif //_VENEER_PSA_ALT_H_

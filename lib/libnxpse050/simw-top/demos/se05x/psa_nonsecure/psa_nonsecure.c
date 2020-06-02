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
 * THIS FILE WILL BE IN NON-SECURE ZONE
 */

#include <nxEnsure.h>
#include <nxLog_App.h>
#include <stdio.h>
#include <string.h>

#include "ex_sss_boot.h"
#include "ksdk_mbedtls.h"
#include "psa/crypto.h"
#include "psa/crypto_se_driver.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa_alt.h"
#include "veneer_table.h"
#define PRINTF_NSE DbgConsole_Printf_NSE

void SystemInit(void)
{
}

int main(int argc, const char *argv[])
{
    psa_status_t status = PSA_SUCCESS;

    /* Lifetime will identify the scope of any object
     * (Where the object is stored)
     * We can use a value which will be specific to an SE.
     * This value can be validated in the driver implementation.
     * Currently we are not validating the lifetime value for any object / driver initialization.
     * Lifetime can be considered as driver ID. It will be used to look for the assigned function pointers.
     */

    psa_key_lifetime_t lifetime = 50;
    psa_drv_se_t driver;
    psa_key_handle_t key_handle = 0;
    const psa_algorithm_t alg   = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH);
    size_t key_bits             = 1024;

    /*
     * The Most significant nibble will be masked out in the provided keyID.
     * The keyID range is 0x30000000 - 0x3FFFFFFF
     * PSA maintains 4 type of objects :
     *
     *      PSA_ALT_TRANSACTION_FILE -  This is a temporary file created at the time of any operation.
     *                                  It will be deleted after the operation.
     *                                  (Fixed KeyID - 0x1FFFFFFE)
     *
     *      PSA_ALT_LIFETIME_FILE -     This is SE specific file which can contain SE(driver) specific
     *                                  persistent data
     *                                  (Fixed KeyID - 0x1FFFFFFF)
     *
     *      PSA_ALT_OBJECT_FILE -       This file corresponds to any object we create inside the SE. It
     *                                  will store data about policy, supported algorithms, etc.
     *                                  (keyID range is 0x20000000 - 0x2FFFFFFF)
     *
     *      PSA_ALT_OBJECT -            This is the actual object created inside the SE.
     *                                  (keyID range is 0x30000000 - 0x3FFFFFFF)
     */

    psa_key_id_t key_id = 0x10181001;

    /* Assign SE driver structures with available PSA ALT APIs */

    /* doc:start:psa-assign-fp */
    psa_drv_se_key_management_t key_mgmt_drv;
    psa_drv_se_asymmetric_t asymm_drv;

    memset(&key_mgmt_drv, 0, sizeof(psa_drv_se_key_management_t));
    memset(&asymm_drv, 0, sizeof(psa_drv_se_asymmetric_t));
    key_mgmt_drv.p_generate             = &psa_alt_generate_key;
    key_mgmt_drv.p_allocate             = &psa_alt_allocate_key;
    key_mgmt_drv.p_export               = &psa_alt_export_key;
    key_mgmt_drv.p_destroy              = &psa_alt_destroy_key;
    key_mgmt_drv.p_import               = &psa_alt_import_key;
    key_mgmt_drv.p_validate_slot_number = &psa_alt_validate_slot_number;

    asymm_drv.p_sign    = &psa_alt_asymmetric_sign_digest;
    asymm_drv.p_verify  = &psa_alt_asymmetric_verify_digest;
    asymm_drv.p_encrypt = &psa_alt_asymmetric_encrypt;
    asymm_drv.p_decrypt = &psa_alt_asymmetric_decrypt;

    memset(&driver, 0, sizeof(driver));
    driver.hal_version    = PSA_DRV_SE_HAL_VERSION;
    driver.p_init         = &psa_alt_driver_init;
    driver.key_management = &key_mgmt_drv;
    driver.asymmetric     = &asymm_drv;
    /* doc:end:psa-assign-fp */

    /* First register SE Driver so that it is initialized in psa_crypto_init before performing any operation
     * Maximum of 4 drivers can be registered
     */

    LOG_I("Registering SE Driver");

    status = psa_register_se_driver(lifetime, &driver);
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);
    status = psa_crypto_init();
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);

    /* doc:start:psa-generate-key */
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes,
        PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_EXPORT | PSA_KEY_USAGE_SIGN_HASH |
            PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_lifetime(&attributes, lifetime);
    psa_set_key_id(&attributes, key_id);

    LOG_I("Generating RSA-%d key", key_bits);

    status = psa_generate_key(&attributes, &key_handle);
    /* doc:end:psa-generate-key */
    ENSURE_OR_GO_CLEANUP(status == PSA_SUCCESS);

    status = psa_open_key(key_id, &key_handle);
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);

    uint8_t data[256]  = {0};
    size_t data_length = 0;

    status = psa_export_key(key_handle, data, sizeof(data), &data_length);
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);

    /*
     *  Sign verify example with algorithm PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256)
     */

    LOG_I(
        "Performing Sign-Verify operation with "
        "PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256)");

    /* doc:start:psa-sign-verify */
    uint8_t hash[32]       = {1};
    size_t hashLen         = sizeof(hash);
    uint8_t signature[256] = {0};
    size_t sigLen          = sizeof(signature);

    status = psa_sign_hash(
        key_handle, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256), hash, hashLen, signature, sizeof(signature), &sigLen);
    if (status != 0) {
        LOG_E("Signing failed");
        goto cleanup;
    }
    else {
        LOG_I("Signing success");
    }

    status = psa_verify_hash(key_handle, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256), hash, hashLen, signature, sigLen);
    if (status != 0) {
        LOG_E("Verification failed");
        goto cleanup;
    }
    else {
        LOG_I("Verification success");
    }
    /* doc:end:psa-sign-verify */

cleanup:
    status = psa_open_key(key_id, &key_handle);
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);

    status = psa_destroy_key(key_handle);
    ENSURE_OR_GO_EXIT(status == PSA_SUCCESS);

exit:

    LOG_I("Example finished");
    return 0;
}

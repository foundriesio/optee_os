/* Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_ecc_curves_values.h>
#include <se05x_tlv.h>
#include <stdio.h>
#include <string.h>

#include "ex_sss_auth.h"

//#include "certificate.h"

static ex_sss_boot_ctx_t gex_sss_gen_cert;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_gen_cert)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

#define ECC_KEY_BIT_LEN 256

/* doc:start:auth-obj */
#if defined(EX_SE050_AUTH_UserID) || defined(EX_SE050_AUTH_UserID_PlatfSCP03) //UserID Session
#define EX_LOCAL_OBJ_AUTH_ID EX_SSS_AUTH_SE05X_UserID_AUTH_ID
#elif defined(EX_SE050_AUTH_NONE) || defined(EX_SE050_AUTH_PlatfSCP03) //No auth
#define EX_LOCAL_OBJ_AUTH_ID EX_SSS_AUTH_SE05X_NONE_AUTH_ID
#elif defined(EX_SE050_AUTH_AESKey) || defined(EX_SE050_AUTH_AESKey_PlatfSCP03) //AESKey
#define EX_LOCAL_OBJ_AUTH_ID EX_SSS_AUTH_SE05X_APPLETSCP_AUTH_ID
#elif defined(EX_SE050_AUTH_ECKey) || defined(EX_SE050_AUTH_ECKey_PlatfSCP03) //ECKey session
#define EX_LOCAL_OBJ_AUTH_ID EX_SSS_AUTH_SE05X_ECKEY_ECDSA_AUTH_ID
#endif
/* doc:end:auth-obj */

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    LOG_I(
        "This example is to demonstrate the use of policies for secure "
        "objects");
    sss_status_t status                = kStatus_SSS_Fail;
    uint8_t digest[32]                 = "Hello World";
    size_t digestLen                   = sizeof(digest);
    uint8_t signature[ECC_KEY_BIT_LEN] = {0};
    size_t signatureLen                = sizeof(signature);
    sss_asymmetric_t asymm;
    sss_object_t object;

    sss_algorithm_t algorithm;
    sss_mode_t mode;
    uint32_t keyId = MAKE_TEST_ID(__LINE__);
    size_t keylen  = ECC_KEY_BIT_LEN / 8;

    algorithm = kAlgorithm_SSS_SHA256;
    mode      = kMode_SSS_Sign;
    status    = sss_key_object_init(&object, &pCtx->ks);
    if (status != kStatus_SSS_Success) {
        LOG_E("sss_key_object_init Failed!!!");
        return status;
    }

    status = sss_key_object_allocate_handle(
        &object, keyId, kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P, keylen, kKeyObject_Mode_Persistent);
    if (status != kStatus_SSS_Success) {
        LOG_E("key_object_allocate_handle Failed!!!");
        return status;
    }

    /* clang-format off */
    /* doc:start:allow-policy-sign */

    /*Logic to pass sign policy*/
    const int enable = 1;

    /* doc:start:allow-policy-sign-part1 */
    /* Policies for key */
    const sss_policy_u key_withPol = {
        .type = KPolicy_Asym_Key,
        /*Authentication object based on SE05X_AUTH*/
        .auth_obj_id = EX_LOCAL_OBJ_AUTH_ID,
        .policy = {
            /*Asymmetric key policy*/
            .asymmkey = {
                /*Policy for sign*/
                .can_Sign = enable,
                /*Policy for verify*/
                .can_Verify = 1,
                /*Policy for encrypt*/
                .can_Encrypt = 1,
                /*Policy for decrypt*/
                .can_Decrypt = 1,
                /*Policy for Key Derivation*/
                .can_KD = 1,
                /*Policy for wrapped object*/
                .can_Wrap = 1,
                /*Policy to re-write object*/
                .can_Write = 1,
                /*Policy for reading object*/
                .can_Read = 1,
                /*Policy to use object for attestation*/
                .can_Attest = 1,
            }
        }
    };

    /* Common rules */
    const sss_policy_u common = {
        .type = KPolicy_Common,
        /*Authentication object based on SE05X_AUTH*/
        .auth_obj_id = EX_LOCAL_OBJ_AUTH_ID,
        .policy = {
        .common = {
        /*Secure Messaging*/
        .req_Sm = 0,
        /*Policy to Delete object*/
        .can_Delete = 1,
        /*Forbid all operations on object*/
        .forbid_All = 0,
    }
    }
    };

    /* create policy set */
    sss_policy_t policy_for_ec_key = {
        .nPolicies = 2,
        .policies = { &key_withPol, &common }
    };
    /* doc:end:allow-policy-sign-part1 */

    /* doc:start:allow-policy-sign-part2 */
    status = sss_key_store_generate_key(
        &pCtx->ks,
        &object,
        ECC_KEY_BIT_LEN,
        &policy_for_ec_key);
    /* doc:end:allow-policy-sign-part2 */

    /* doc:end:allow-policy-sign */

    /* clang-format on */

    if (status != kStatus_SSS_Success) {
        LOG_E("Key Store Generate Key Failed!!!");
        return status;
    }

    /* asymmetric Sign */
    status = sss_asymmetric_context_init(&asymm, &pCtx->session, &object, algorithm, mode);
    if (status != kStatus_SSS_Success) {
        LOG_E("Asymmetric Init Context Failed!!!");
        return status;
    }

    status = sss_asymmetric_sign_digest(&asymm, digest, digestLen, signature, &signatureLen);

    if (enable) {
        if (status != kStatus_SSS_Success) {
            LOG_E("Signing Failed, Must have passed!!!");
            goto exit;
        }
        LOG_I("Signing was succesful");
        LOG_I("Example Success");
        goto exit;
    }
    else {
        if (status == kStatus_SSS_Success) {
            LOG_E(
                "Signing policy not respected. Test with Sign Disabled Policy "
                "Failed!!!");
            status = kStatus_SSS_Fail;
            goto exit;
        }
        LOG_I("Signing was not allowed due to policy.");
        LOG_I("Example Success");
        status = kStatus_SSS_Success;
        goto exit;
    }
exit:
    return status;
}

/**
 * @file se05x_MfDfInit.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * File has functions That setup SE05X for MIFARE DESFire EV2 examples.
 * Please note Rdlib support is not needed for this file.
 */

/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ex_sss.h"
#include "fsl_sss_se05x_types.h"
#include "nxLog_App.h"
#include "se05x_MfDfInit.h"
#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */
#define KEY_BIT_LEN 128
/* clang-format off */
/* doc:start:mif-key-values */
const uint8_t oldKeyValue[KEY_BIT_LEN / 8] = {  0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00 };

const uint8_t newKeyValue[KEY_BIT_LEN / 8] = {  0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x01 };
/* doc:end:mif-key-values */
/* clang-format on */
/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */


/* *****************************************************************************************************************
* Private Functions Prototypes
* ***************************************************************************************************************** */

sss_status_t InitialSetupSe050(sss_key_store_t *pkeyStore, uint32_t keyId, uint32_t derivedkeyId)
{

    sss_status_t sssStatus = kStatus_SSS_Success;

    sss_key_part_t keyPart = kSSS_KeyPart_Default;
    sss_cipher_type_t cipherType = kSSS_CipherType_AES;
    size_t keyByteLenMax = KEY_BIT_LEN / 8;
    sss_object_t oldKey;
    sss_object_t newKey;

        sssStatus = sss_key_object_init(&oldKey, pkeyStore);
        if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj initialization failed");
            goto exit;
        }

        sssStatus = sss_key_object_init(&newKey, pkeyStore);
        if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj initialization failed");
            goto exit;
        }

    sssStatus = sss_key_object_allocate_handle(&oldKey,
        keyId,
        keyPart,
        cipherType,
        keyByteLenMax,
        kKeyObject_Mode_Persistent);

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj alllocation failed");
        goto exit;
    }

    sssStatus = sss_key_object_allocate_handle(&newKey,
        derivedkeyId,
        keyPart,
        cipherType,
        keyByteLenMax,
        kKeyObject_Mode_Persistent);

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj alllocation failed");
        goto exit;
    }

    const sss_policy_u key_withPol2 = {
        .type = KPolicy_Sym_Key,
        .auth_obj_id = 0, //0x11223346,
        .policy = {
        .symmkey = {
        .can_Desfire_Auth = 1,
        .can_Desfire_Dump = 1,
    }
    }
    };
    const sss_policy_u common = {
        .type = KPolicy_Common,
        .auth_obj_id = 0, //0x11223344,
        .policy = {
        .common = {
        .can_Delete = 1,
    }
    }
    };
    sss_policy_t policy_for_ec_key = {
        .nPolicies = 2,
        .policies = {  &key_withPol2, &common}
    };

    sssStatus = sss_key_store_set_key(
        pkeyStore, &oldKey, oldKeyValue, KEY_BIT_LEN / 8, KEY_BIT_LEN, &policy_for_ec_key, sizeof(policy_for_ec_key));

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Storing failed");
        goto exit;
    }

    sssStatus = sss_key_store_set_key(
        pkeyStore, &newKey, newKeyValue, KEY_BIT_LEN / 8, KEY_BIT_LEN, &policy_for_ec_key, sizeof(policy_for_ec_key));

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Storing failed");
        goto exit;
    }
    LOG_I("SE050 prepared successfully for MIFARE DESFire EV2 examples");
exit:
    return sssStatus;
}


sss_status_t SubsequentSetupSe050(sss_key_store_t *pkeyStore, uint32_t keyId, uint32_t derivedkeyId)
{

    sss_status_t sssStatus = kStatus_SSS_Success;
    sss_object_t oldKey;
    sss_object_t newKey;

    sssStatus = sss_key_object_init(&oldKey, pkeyStore);
    if (sssStatus != kStatus_SSS_Success) {
        LOG_E("Key Obj initialization failed");
        goto exit;
    }
   sssStatus = sss_key_object_init(&newKey, pkeyStore);
    if (sssStatus != kStatus_SSS_Success) {
       LOG_E("Key Obj initialization failed");
        goto exit;
    }

    sssStatus = sss_key_object_get_handle(&oldKey,
        keyId);

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj getting handle failed");
        goto exit;
    }

    sssStatus = sss_key_object_get_handle(&newKey,
        derivedkeyId);

    if (sssStatus != kStatus_SSS_Success) {
           LOG_E("Key Obj getting handle failed");
        goto exit;
    }

exit:
    return sssStatus;
}

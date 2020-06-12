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

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <nxEnsure.h>
#include <nxLog_App.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mfa_utils.h"
#include "smCom.h"
#include "sm_apdu.h"
#ifdef SMCOM_JRCP_V2
#include <smComJRCP.h>
#endif

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

void initialise_allocate_key_object(sss_object_t *Key,
    sss_key_store_t *ks,
    uint32_t keyId,
    sss_key_part_t keyPart,
    sss_cipher_type_t cipherType,
    size_t keyByteLenMax,
    uint32_t options)
{
    sss_status_t status;
    status = sss_key_object_init(Key, ks);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    LOG_D("Allocating KeyID=%Xh(%d) type=%d in %d",
        keyId,
        keyId,
        cipherType,
        ks->session->subsystem);

    status = sss_key_object_allocate_handle(
        Key, keyId, keyPart, cipherType, keyByteLenMax, options);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    return;
cleanup:
    LOG_E("Failurein initialise_allocate_key_object");
}

/* This Function will initialise, allocate and set the key in keystore,
for one sss_object in provided key store*/
void tst_asymm_alloc_and_set_key(sss_object_t *keyObject,
    sss_key_store_t *ks,
    sss_key_part_t keyPart,
    sss_cipher_type_t cipherType,
    uint32_t keyId,
    const uint8_t *key,
    size_t keyByteLen,
    size_t keyBitLen,
    uint32_t options)
{
    sss_status_t status;

    /*
     * KeyPair -> RSA_CRT       -> >=2048
     * Public  -> RSA, RSA_CRT  -> >=2048
     */
    if (cipherType == kSSS_CipherType_RSA ||
        cipherType == kSSS_CipherType_RSA_CRT) {
        if (keyBitLen < 2048) {
            LOG_W("Can not inject RSA <2048 in FIPS Mode");
        }
        if ((keyPart == kSSS_KeyPart_Pair || keyPart == kSSS_KeyPart_Private) &&
            cipherType == kSSS_CipherType_RSA) {
            LOG_W("Can not inject Plain RSA in FIPS Mode");
        }
    }
    if (keyBitLen < 224 && keyPart != kSSS_KeyPart_Default) {
        LOG_W("No SECP192R1 in FIPS Mode");
    }
    if (cipherType == kSSS_CipherType_EC_TWISTED_ED ||
        cipherType == kSSS_CipherType_EC_MONTGOMERY ||
        cipherType == kSSS_CipherType_EC_BARRETO_NAEHRIG) {
        LOG_W("Curve not supported in FIPS Mode");
    }

    initialise_allocate_key_object(
        keyObject, ks, keyId, keyPart, cipherType, keyByteLen, options);

    status = sss_key_store_set_key(
        ks, keyObject, key, keyByteLen, keyBitLen, NULL, 0);
    ENSURE_OR_GO_CLEANUP(kStatus_SSS_Success == status);

    return;
cleanup:
    LOG_E("ERROR in Alloc and Set Key");
    return;
}

void printMALSStatusCode(mal_status_t mal_status)
{
    switch (mal_status) {
    case kStatus_MAL_Success:
        printf("MALS Status: SUCCESS");
        break;
    case kStatus_MAL_ERR_General:
        printf("MALS Status: FAILED");
        break;
    case kStatus_MAL_ERR_COM:
        printf("MALS Status: COMMUNICATION FAILED");
        break;
    case kStatus_MAL_ERR_DoReRun:
        printf("MALS Status: Do Rerun");
        break;
    case kStatus_MAL_ERR_NotApplicable:
        printf("MALS Status: Not Applicable");
        break;
    case kStatus_MAL_ERR_DoRecovery:
        printf("MALS Status: Do Recovery");
        break;
    case kStatus_MAL_ERR_Fatal:
        printf("MALS Status: Fatal Error");
        break;
    default:
        printf("MALS Status: UNKNOWN");
        break;
    }
    printf("\n");
}

void print_SSS_StatusCode(sss_status_t sss_stat)
{
    switch (sss_stat) {
    case kStatus_SSS_Success:
        printf("SSS Status: SUCCESS");
        break;
    case kStatus_SSS_Fail:
        printf("SSS Status: FAILED");
        break;
    default:
        printf("SSS Status: UNKNOWN");
        break;
    }
    printf("\n");
}

void print_hex_contents(
    const char *contentsName, uint8_t *contents, size_t contentsLen)
{
    printf("%s: ", contentsName);
    size_t i;
    for (i = 0; i < contentsLen; i++) {
        printf("%02x", contents[i]);
    }
    printf("\n");
}

uint8_t *hexstr_to_bytes(const char *str, size_t *len)
{
    if ((strlen(str) % 2) != 0) {
        printf("Invalid length");
        return NULL;
    }

    *len = strlen(str) / 2;
    uint8_t *res = (uint8_t *)malloc(*len);

    const char *pos = str;
    for (size_t count = 0; count < *len; count++) {
        if (sscanf(pos, "%2hhx", &res[count]) < 1) {
            free(res);
            return NULL;
        }
        pos += 2;
    }
    return res;
}

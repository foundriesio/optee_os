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

#include "mfa_process.h"

#include <nxEnsure.h>
#include <nxLog_App.h>

#include "mfa_utils.h"
#include "nxp_iot_agent.h"
#include "nxp_iot_agent_keystore_sss_se05x.h"
#include "smCom.h"

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define MALS_GETDATA_BUF_SIZE 256
#define MALS_UID_SIZE 18
#define MALS_GETDATA_CA_IDENTIFIER_TAG 0x42
#define MALS_GETDATA_CA_KEY_IDENTIFIER_TAG 0x45
#define MALS_GETDATA_CA_IDENTIFIER_TAGLEN 0x10
#define MALS_GETDATA_CA_KEY_IDENTIFIER_TAGLEN 0x08

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

mal_agent_ctx_t g_mal_agent_load_ctx = {0};
ex_sss_boot_ctx_t gfeature_app_mals_boot_ctx;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

void mfa_process_loadpkg(const char *pkgname)
{
    mal_status_t status = kStatus_MAL_ERR_Fatal;
    FILE *fp = NULL;
    int n = 0;
    size_t numbytes;
    size_t tempbytes;
    uint8_t towrite = 0;
    uint8_t *buffer;

    fp = fopen(pkgname, "rb");
    if (fp == NULL) {
        printf("File not found");
        exit(2);
    }
    fseek(fp, 0L, SEEK_END);
    numbytes = ftell(fp);

    buffer = (uint8_t *)malloc(
        numbytes + 4); /* Total size + varient length encoding */
    if (buffer == NULL) {
        printf("malloc failed !!!");
        exit(2);
    }
    tempbytes = numbytes;
    while (tempbytes) {
        /* put encoded varient length directly into buffer */
        towrite = tempbytes & 0x7f;
        tempbytes >>= 7;
        if (tempbytes) {
            buffer[n++] = towrite | 0x80;
        }
        else {
            buffer[n++] = towrite;
            break;
        }
    }
    fseek(fp, 0L, SEEK_SET);
    fread(buffer + n, numbytes, 1, fp);

    // TODO: Use signature buffer as paramters.
    status = mal_agent_load_package(
        &g_mal_agent_load_ctx, buffer, numbytes + n, NULL, 0);
    printMALSStatusCode(status);
    free(buffer);
}

void mfa_process_getuid()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t uid[MALS_GETDATA_BUF_SIZE];
    size_t uidLen = MALS_GETDATA_BUF_SIZE;
    sss_status = mals_get_UUID(&g_mal_agent_load_ctx, uid, &uidLen);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        print_hex_contents((const char *)"uid", uid, uidLen);
    }
}

void mfa_process_getappcontents(const char *appAid)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t *aidhex = NULL;
    size_t aidhexLen = 0;
    uint8_t cardContent[MALS_GETDATA_BUF_SIZE];
    size_t cardContentLen = MALS_GETDATA_BUF_SIZE;

    if (appAid) {
        aidhex = hexstr_to_bytes(appAid, &aidhexLen);
        if (aidhex == NULL) {
            printf("invalid hexstr in [%s]\n", aidhex);
            return;
        }
    }

    sss_status = mals_get_SEAppInfoRAW(&g_mal_agent_load_ctx,
        aidhex,
        (uint8_t)aidhexLen,
        cardContent,
        &cardContentLen);
    print_SSS_StatusCode(sss_status);
    free(aidhex);
}

void mfa_process_getpkgcontents(const char *pkgAid)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t *aidhex = NULL;
    size_t aidhexLen = 0;
    uint8_t cardContent[MALS_GETDATA_BUF_SIZE];
    size_t cardContentLen = MALS_GETDATA_BUF_SIZE;

    if (pkgAid) {
        aidhex = hexstr_to_bytes(pkgAid, &aidhexLen);
        if (aidhex == NULL) {
            printf("invalid hexstr in [%s]\n", aidhex);
            return;
        }
    }
    sss_status = mals_get_SEPkgInfoRAW(&g_mal_agent_load_ctx,
        aidhex,
        (uint8_t)aidhexLen,
        cardContent,
        &cardContentLen);
    print_SSS_StatusCode(sss_status);
    free(aidhex);
}

void mfa_process_getPbkeyId()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t pbKey[MALS_GETDATA_BUF_SIZE];
    size_t pbKeyLen = MALS_GETDATA_BUF_SIZE;

    sss_status = mals_get_Publickey(&g_mal_agent_load_ctx, pbKey, &pbKeyLen);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        print_hex_contents((const char *)"PbkeyId", pbKey, pbKeyLen);
    }
}

void mfa_process_malsgetversion()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t appletVer[MALS_GETDATA_BUF_SIZE];
    size_t appletVerLen = MALS_GETDATA_BUF_SIZE;
    sss_status =
        mals_get_AppletVersion(&g_mal_agent_load_ctx, appletVer, &appletVerLen);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        print_hex_contents((const char *)"appletVer", appletVer, appletVerLen);
    }
}

void mfa_process_getsignature(const char *filename)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    FILE *fp = NULL;
    uint8_t sig[MALS_GETDATA_BUF_SIZE];
    size_t sigLen = MALS_GETDATA_BUF_SIZE;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("File open failed");
        return;
    }
    sss_status =
        mals_get_SignatureofLastScript(&g_mal_agent_load_ctx, sig, &sigLen);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        fwrite(sig, 1, sigLen, fp);
    }
}

void mfa_process_checkTear()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    mal_tearDown_status_t tearStatus;
    sss_status = mals_check_Tear(&g_mal_agent_load_ctx, &tearStatus);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        printf("Tear Status: %d\n", tearStatus);
    }
}

void mfa_process_checkUpgradeProgress()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    mal_upgradeProgress_status_t upgradeStatus;

    sss_status =
        mals_check_AppletUpgradeProgress(&g_mal_agent_load_ctx, &upgradeStatus);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        printf("Upgrade Status: %d\n", upgradeStatus);
    }
}

void mfa_process_getENCIdentifier()
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t rspBuf[MALS_GETDATA_BUF_SIZE] = {0};
    size_t rspBufLen = sizeof(rspBuf);
    sss_status =
        mals_get_ENCIdentifier(&g_mal_agent_load_ctx, rspBuf, &rspBufLen);
    print_SSS_StatusCode(sss_status);
    if (sss_status == kStatus_SSS_Success) {
        print_hex_contents((const char *)"EnckeyId", rspBuf, rspBufLen);
    }
}

static sss_status_t mfa_process_getData(mal_agent_ctx_t *pContext,
    uint8_t tag_P2,
    const uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *pRspBuf,
    size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    U32 respstat;

    uint8_t getDataCmd[MALS_GET_DATA_CMD_BUF_LEN] = {
        0x80, // CLA '80' / '00' GlobalPlatform / ISO / IEC
        0xCA, // INS 'CA' GET DATA(IDENTIFY)
        0x00, // P1 '00' High order tag value
        0x00, // P2  - proprietary data coming from respective function
        0x00, // Lc is Le'00' Case 2 command
    };
    U16 getDataCmdLen = 5;
    U32 getDataRspLen;

    /* Copy the appropriate tag coming from respective function at P2 */
    getDataCmd[3] = tag_P2;

    if (!(cmdBufLen < (255 - 5))) {
        goto cleanup;
    }

    if (cmdBufLen > 0) {
        getDataCmdLen += (U16)cmdBufLen;
        getDataCmdLen++;
        memcpy(&getDataCmd[5], cmdBuf, cmdBufLen);
        getDataCmd[4] /* Lc */ = (uint8_t)cmdBufLen;
    }

    if (!(*pRspBufLen > 0)){
        goto cleanup;
    }

    getDataRspLen = (U32)*pRspBufLen;

#ifdef MAL_AGENT_CHANNEL_1
    if (pContext->n_logical_channel == MAL_AGENT_CHANNEL_0) {
        LOG_E("It is not permitted to use MALS APIs in Channel 0");
        LOG_E(
            "Before calling this API, MALS Context should have been "
            "initialized.");
        goto cleanup;
    }
    else {
        getDataCmd[0] = getDataCmd[0] | MAL_AGENT_CHANNEL_1;
    }
#endif

    respstat = smCom_TransceiveRaw(pContext->pS05x_Ctx->conn_ctx,
        (uint8_t *)getDataCmd,
        getDataCmdLen,
        pRspBuf,
        &getDataRspLen);
    if (respstat != SM_OK) {
        LOG_E("Could not get requested Data!!!");
        goto cleanup;
    }

    if (pRspBuf[getDataRspLen - 2] == 0x90 &&
        pRspBuf[getDataRspLen - 1] == 0x00) {
        /* 0x9000*/
        sss_stat = kStatus_SSS_Success;
    }
    else {
        sss_stat = kStatus_SSS_Fail;
    }

    if (*pRspBufLen > getDataRspLen) {
        *pRspBufLen = (size_t)getDataRspLen;
    }
    else {
        LOG_E("InSufficient Buffer passed!!!");
        sss_stat = kStatus_SSS_Fail;
    }

cleanup:
    return sss_stat;
}

void mfa_process_getCAIdentifier()
{
    sss_status_t status = kStatus_SSS_Fail;
    sss_status_t sss_status = kStatus_SSS_Fail;
    smStatus_t retStatus = SM_NOT_OK;
    uint8_t rspBuf[24] = {0};
    size_t rspBufLen = sizeof(rspBuf);

    // P2 '0x46' Check Tear during script execution
    sss_status =
        mfa_process_getData(&g_mal_agent_load_ctx, MALS_GETDATA_CA_IDENTIFIER_TAG, NULL, 0, rspBuf, &rspBufLen);

    status = mal_agent_session_close(&g_mal_agent_load_ctx);
    if (status != kStatus_SSS_Success) {
        LOG_E("MAL close session failed!!!");
    }

    print_SSS_StatusCode(sss_status);
    if ((sss_status == kStatus_SSS_Success) && (rspBufLen > 2)) {
        sss_status = kStatus_SSS_Fail;
        retStatus = (rspBuf[rspBufLen - 2] << 8) | (rspBuf[rspBufLen - 1]);
        if ((retStatus == SM_OK) && (rspBuf[0] == MALS_GETDATA_CA_IDENTIFIER_TAG) &&
            (rspBuf[1] == MALS_GETDATA_CA_IDENTIFIER_TAGLEN)) {
            print_hex_contents((const char *)"CA-SEMS Identifier", &rspBuf[2], rspBuf[1]);
            sss_status = kStatus_SSS_Success;
        }
    }

    return;
}

void mfa_process_getCAKeyIdentifier()
{
    sss_status_t status = kStatus_SSS_Fail;
    sss_status_t sss_status = kStatus_SSS_Fail;
    smStatus_t retStatus = SM_NOT_OK;
    uint8_t rspBuf[24] = {0};
    size_t rspBufLen = sizeof(rspBuf);

    // P2 '0x46' Check Tear during script execution
    sss_status =
        mfa_process_getData(&g_mal_agent_load_ctx, MALS_GETDATA_CA_KEY_IDENTIFIER_TAG, NULL, 0, rspBuf, &rspBufLen);

    status = mal_agent_session_close(&g_mal_agent_load_ctx);
    if (status != kStatus_SSS_Success) {
        LOG_E("MAL close session failed!!!");
    }

    print_SSS_StatusCode(sss_status);
    if ((sss_status == kStatus_SSS_Success) && (rspBufLen > 2)) {
        sss_status = kStatus_SSS_Fail;
        retStatus = (rspBuf[rspBufLen - 2] << 8) | (rspBuf[rspBufLen - 1]);
        if ((retStatus == SM_OK) && (rspBuf[0] == MALS_GETDATA_CA_KEY_IDENTIFIER_TAG) &&
            (rspBuf[1] == MALS_GETDATA_CA_KEY_IDENTIFIER_TAGLEN)) {
            print_hex_contents((const char *)"CA-SEMS Key Identifier", &rspBuf[2], rspBuf[1]);
            sss_status = kStatus_SSS_Success;
        }
    }

    return;
}

void mfa_process_getPkgVerion(const char *pkgAid)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    uint8_t *aidhex = NULL;
    size_t aidhexLen = 0;
	mals_SEAppInfoList_t getAppInfoList[50];
	size_t i;
    size_t getAppInfoListLen = sizeof(getAppInfoList) / sizeof(getAppInfoList[0]);

    if (pkgAid) {
        aidhex = hexstr_to_bytes(pkgAid, &aidhexLen);
        if (aidhex == NULL) {
            printf("invalid hexstr in [%s]\n", aidhex);
            return;
        }
    }

    sss_status = mals_get_SEPkgInfo(&g_mal_agent_load_ctx,
		aidhex,
		(uint8_t)aidhexLen,
		getAppInfoList,
		&getAppInfoListLen);
	print_SSS_StatusCode(sss_status);

    for (i = 0; i < getAppInfoListLen; i++) {
		if (getAppInfoList[i].pLoadFileVersionNumber) {
            LOG_MAU8_I("Version Number",
            getAppInfoList[i].pLoadFileVersionNumber,
            getAppInfoList[i].LoadFileVersionNumberLen);
        }
    }

    free(aidhex);
}

/* ************************************************************************** */
/* Static function definitions                                               */
/* ************************************************************************** */

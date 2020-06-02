/*
* Copyright 2019-2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include <nxLog_App.h>

#include "global_platf.h"
#include "mal_api.h"
#include "nxEnsure.h"
#include "smCom.h"
#include "string.h"

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
#define MALS_GETDATA_UUID_TAG 0xFE
#define MALS_GETDATA_UUID_TAGLEN 0x18

#define MALS_GETDATA_PUBLICKEY_TAG 0xC4

#define MALS_GETDATA_APP_INFO 0xC2

#define MALS_GETDATA_PACKAGE_INFO 0xC3

#define MALS_GETDATA_CHECKTEAR_TAG 0x46
#define MALS_GETDATA_CHECKTEAR_TAGLEN 0x01

#define MALS_GETDATA_SIGNATURE_LAST_SCRIPT_TAG 0x47

#define MALS_GETDATA_APPLET_VERSION_TAG 0xDE

#define MALS_GETDATA_UPGRADE_PROGRESS_TAG 0xC1
#define MALS_GETDATA_ENC_ID_TAG 0x43

#define MALS_GETDATA_UPGRADE_PROGRESS_TAG 0xC1
#define ELF_UPGRADE_INFO_TAG 0xA1
#define ELF_UPGRADE_SESSION_STATUS_TAG 0x90

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */
#if SSS_HAVE_SE05X
static sss_status_t mals_get_Data(mal_agent_ctx_t *pContext, uint8_t tag, uint8_t *pRspBuf, size_t *pRspBufLen);

static sss_status_t mals_verify_GetDataResponse(uint8_t tag, uint8_t *pRspBuf, size_t *pRspBufLen);

static sss_status_t mals_parse_AppletUpgradeProgressResp(uint8_t *pRspBuf,
    size_t *pRspBufLen,
    mal_upgradeProgress_status_t *pUpgradeStatus,
    mal_recovery_status_t *pRecoveryStatus);

static sss_status_t mals_get_Data_BUF(mal_agent_ctx_t *pContext,
    uint8_t tag_P1,
    uint8_t tag_P2,
    const uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *pRspBuf,
    size_t *pRspBufLen);

static sss_status_t mals_get_Data_BUF_Recursive(mal_agent_ctx_t *pContext,
    uint8_t tag_P2,
    const uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *pRspBuf,
    size_t *pRspBufLen);

static void mals_parse_card_contents(mals_SEAppInfoList_t *pAppInfo);

static sss_status_t mals_get_SEInfo_Int(mal_agent_ctx_t *pContext,
    uint8_t tag_p2,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen);
#endif

#if SSS_HAVE_SE05X
sss_status_t mals_get_UUID(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);

    // P2 '0xFE' MALS UUID
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_UUID_TAG, pRspBuf, pRspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_verify_GetDataResponse(MALS_GETDATA_UUID_TAG, pRspBuf, pRspBufLen);
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_get_Publickey(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);

    // P2 '0xC4' MALS PUBLICKEY
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_PUBLICKEY_TAG, pRspBuf, pRspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_verify_GetDataResponse(MALS_GETDATA_PUBLICKEY_TAG, pRspBuf, pRspBufLen);
    }
cleanup:
    return sss_stat;
}

static sss_status_t mals_get_SEInfoRAW_Int(mal_agent_ctx_t *pContext,
    uint8_t tag,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    uint8_t *pRspBuf,
    size_t *pRspBufLen)
{
    sss_status_t sss_stat                     = kStatus_SSS_Fail;
    uint8_t cmdBuf[MALS_GET_DATA_CMD_BUF_LEN] = {0x4F, 00};
    uint8_t cmdBufLen                         = 2;

    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);
    ENSURE_OR_GO_CLEANUP(*pRspBufLen > 0);

    if (searchAidLen > 0) {
        //         4F  Len  searchAid
        cmdBufLen = 1 + 1 + searchAidLen;
        cmdBuf[1] = searchAidLen;
        memcpy(&cmdBuf[2], searchAID, searchAidLen);

        sss_stat = mals_get_Data_BUF(pContext, 0x00, tag, cmdBuf, cmdBufLen, pRspBuf, pRspBufLen);
    }
    else {
        sss_stat = mals_get_Data_BUF_Recursive(pContext, tag, cmdBuf, cmdBufLen, pRspBuf, pRspBufLen);
    }

    if (sss_stat == kStatus_SSS_Success) {
        /* Good */
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_get_SEAppInfoRAW(
    mal_agent_ctx_t *pContext, const uint8_t *searchAID, uint8_t searchAidLen, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    return mals_get_SEInfoRAW_Int(pContext, MALS_GETDATA_APP_INFO, searchAID, searchAidLen, pRspBuf, pRspBufLen);
}

sss_status_t mals_get_SEPkgInfoRAW(
    mal_agent_ctx_t *pContext, const uint8_t *searchAID, uint8_t searchAidLen, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    return mals_get_SEInfoRAW_Int(pContext, MALS_GETDATA_PACKAGE_INFO, searchAID, searchAidLen, pRspBuf, pRspBufLen);
}

sss_status_t mals_get_SEAppInfo(mal_agent_ctx_t *pContext,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen)
{
    return mals_get_SEInfo_Int(pContext, MALS_GETDATA_APP_INFO, searchAID, searchAidLen, pAppInfo, pAppInfoLen);
}

sss_status_t mals_get_SEPkgInfo(mal_agent_ctx_t *pContext,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen)
{
    return mals_get_SEInfo_Int(pContext, MALS_GETDATA_PACKAGE_INFO, searchAID, searchAidLen, pAppInfo, pAppInfoLen);
}

static sss_status_t mals_get_SEInfo_Int(mal_agent_ctx_t *pContext,
    uint8_t tag_p2,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen)
{
    sss_status_t sss_stat                     = kStatus_SSS_Fail;
    uint8_t cmdBuf[MALS_GET_DATA_CMD_BUF_LEN] = {0x4F, 00};
    uint8_t cmdBufLen                         = 2;
    uint8_t tag_P1                            = 0x00;
    uint8_t i                                 = 0;
    size_t lastResponseLen                    = MALS_GET_DATA_CMD_BUF_LEN;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pAppInfo != NULL);
    ENSURE_OR_GO_CLEANUP(*pAppInfoLen > 0);

    if (searchAidLen > 0) {
        //         4F  Len  searchAid
        cmdBufLen = 1 + 1 + searchAidLen;
        cmdBuf[1] = searchAidLen;
        memcpy(&cmdBuf[2], searchAID, searchAidLen);
    }

    while (lastResponseLen > 2) {
        if (*pAppInfoLen > i) {
            memset(&pAppInfo[i], 0, sizeof(pAppInfo[i]));
            pAppInfo[i].rspBufLen = MALS_GET_DATA_CMD_BUF_LEN;
            sss_stat              = mals_get_Data_BUF(
                pContext, tag_P1, tag_p2, cmdBuf, cmdBufLen, pAppInfo[i].rspBuf, &pAppInfo[i].rspBufLen);

            lastResponseLen = pAppInfo[i].rspBufLen;
            if (sss_stat == kStatus_SSS_Success) {
                if (pAppInfo[i].rspBufLen > 2) {
                    mals_parse_card_contents(&pAppInfo[i]);
                    i++;
                }
            }
            else {
                /* Fail */
                goto cleanup;
            }
        }
        else {
            LOG_E("InSufficient Buffer passed!!!");
            sss_stat = kStatus_SSS_Fail;
            goto cleanup;
        }
        /* For the next iteration make p1 to 0x01 */
        tag_P1 = 0x01;
    }
    *pAppInfoLen = i + 1;
cleanup:
    return sss_stat;
}

sss_status_t mals_check_Tear(mal_agent_ctx_t *pContext, mal_tearDown_status_t *pTearStatus)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    smStatus_t retStatus  = SM_NOT_OK;
    uint8_t rspBuf[16]    = {0};
    size_t rspBufLen      = sizeof(rspBuf);
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    // P2 '0x46' Check Tear during script execution
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_CHECKTEAR_TAG, rspBuf, &rspBufLen);
    if ((sss_stat == kStatus_SSS_Success) && (rspBufLen > 2)) {
        sss_stat  = kStatus_SSS_Fail;
        retStatus = (rspBuf[rspBufLen - 2] << 8) | (rspBuf[rspBufLen - 1]);
        if ((retStatus == SM_OK) && (rspBuf[0] == MALS_GETDATA_CHECKTEAR_TAG) &&
            (rspBuf[1] == MALS_GETDATA_CHECKTEAR_TAGLEN)) {
            sss_stat = kStatus_SSS_Success;
            if (rspBuf[2] == 0) {
                *pTearStatus = mal_notear;
            }
            else if (rspBuf[2] == 1) {
                *pTearStatus = mal_tear;
            }
        }
    }
    else {
        sss_stat = kStatus_SSS_Fail;
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_get_SignatureofLastScript(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    size_t getDataRspLen  = 0;
    smStatus_t retStatus  = SM_NOT_OK;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);
    // P2 '0x47' _Get Signature of executed script
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_SIGNATURE_LAST_SCRIPT_TAG, pRspBuf, pRspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = kStatus_SSS_Fail;
        /* Handle no tear signature is 0 bytes */
        if (*pRspBufLen == 4) {
            getDataRspLen = *pRspBufLen;
            retStatus     = (pRspBuf[getDataRspLen - 2] << 8) | (pRspBuf[getDataRspLen - 1]);
            if (retStatus == SM_OK) {
                if ((pRspBuf[0] == MALS_GETDATA_SIGNATURE_LAST_SCRIPT_TAG) && (pRspBuf[1] == 0x00)) {
                    memset(pRspBuf, 0, *pRspBufLen);
                    *pRspBufLen = 0x00;
                    sss_stat    = kStatus_SSS_Success;
                }
            }
        }
        else {
            sss_stat = mals_verify_GetDataResponse(MALS_GETDATA_SIGNATURE_LAST_SCRIPT_TAG, pRspBuf, pRspBufLen);
        }
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_get_AppletVersion(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);

    // P2 '0xDE' MALS Applet Version
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_APPLET_VERSION_TAG, pRspBuf, pRspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_verify_GetDataResponse(MALS_GETDATA_APPLET_VERSION_TAG, pRspBuf, pRspBufLen);
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_check_AppletUpgradeProgress(mal_agent_ctx_t *pContext, mal_upgradeProgress_status_t *pUpgradeStatus)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    uint8_t rspBuf[256]   = {0};
    size_t rspBufLen      = sizeof(rspBuf);
    mal_recovery_status_t recoveryStatus;

    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    // P2 '0xC1' MALS Applet Upgrade Progress
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_UPGRADE_PROGRESS_TAG, rspBuf, &rspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_parse_AppletUpgradeProgressResp(rspBuf, &rspBufLen, pUpgradeStatus, &recoveryStatus);
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_check_AppletRecoveryStatus(mal_agent_ctx_t *pContext, mal_recovery_status_t *pRecoveryStatus)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    uint8_t rspBuf[256]   = {0};
    size_t rspBufLen      = sizeof(rspBuf);
    mal_upgradeProgress_status_t upgradeStatus;

    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    // P2 '0xC1' MALS Applet Upgrade Progress
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_UPGRADE_PROGRESS_TAG, rspBuf, &rspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_parse_AppletUpgradeProgressResp(rspBuf, &rspBufLen, &upgradeStatus, pRecoveryStatus);
    }
cleanup:
    return sss_stat;
}

sss_status_t mals_get_ENCIdentifier(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(pContext != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBuf != NULL);
    ENSURE_OR_GO_CLEANUP(pRspBufLen != NULL);

    // P2 '0x43' MALS ENC Identifier
    sss_stat = mals_get_Data(pContext, MALS_GETDATA_ENC_ID_TAG, pRspBuf, pRspBufLen);
    if (sss_stat == kStatus_SSS_Success) {
        sss_stat = mals_verify_GetDataResponse(MALS_GETDATA_ENC_ID_TAG, pRspBuf, pRspBufLen);
    }
cleanup:
    return sss_stat;
}

static void mals_parse_card_contents(mals_SEAppInfoList_t *pAppInfo)
{
    int i = 0;

    if ((pAppInfo->rspBuf[i]) == 0xE3) {
        /* Tag E3 */
        i = 2;
    }
    else {
        LOG_E("Wrong tag parsed incorrect information");
        return;
    }

    if (pAppInfo->rspBuf[i++] == 0x4F) {
        /* 0x4F Aid tag*/
        pAppInfo->AIDLen = pAppInfo->rspBuf[i++];
        pAppInfo->pAID   = &pAppInfo->rspBuf[i];
        i += pAppInfo->AIDLen;
    }

    if (pAppInfo->rspBuf[i] == 0x9F && pAppInfo->rspBuf[++i] == 0x70) {
        /* 9F70 Life Cycle tag */
        i += 1;
        pAppInfo->LifeCycleState = pAppInfo->rspBuf[++i];
        i += 1;
    }

    if (pAppInfo->rspBuf[i] == 0xC5) {
        /* 0xC5 Privilages tag*/
        i += 1;
        pAppInfo->PriviledgesLen = pAppInfo->rspBuf[i++];
        pAppInfo->pPriviledges   = &pAppInfo->rspBuf[i];
        i += pAppInfo->PriviledgesLen;
    }

    if (pAppInfo->rspBuf[i] == 0xC4) {
        /* 0xC4 Load file aid tag*/
        i += 1;
        pAppInfo->LoadFileAIDLen = pAppInfo->rspBuf[i++];
        pAppInfo->pLoadFileAID   = &pAppInfo->rspBuf[i];
        i += pAppInfo->LoadFileAIDLen;
    }

    if (pAppInfo->rspBuf[i] == 0xCE) {
        /* 0xCE Load File Version Number tag*/
        i += 1;
        pAppInfo->LoadFileVersionNumberLen = pAppInfo->rspBuf[i++];
        pAppInfo->pLoadFileVersionNumber   = &pAppInfo->rspBuf[i];
        i += pAppInfo->LoadFileVersionNumberLen;
    }

    if (pAppInfo->rspBuf[i] == 0xCC) {
        /* 0xCC Security Domain AID tag*/
        i += 1;
        pAppInfo->SecurityDomainAIDLen = pAppInfo->rspBuf[i++];
        pAppInfo->pSecurityDomainAID   = &pAppInfo->rspBuf[i];
        i += pAppInfo->SecurityDomainAIDLen;
    }
    return;
}

static sss_status_t mals_get_Data(mal_agent_ctx_t *pContext, uint8_t tag_P2, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    const uint8_t tag_P1 = 0x00;
    return mals_get_Data_BUF(pContext, tag_P1, tag_P2, NULL, 0, pRspBuf, pRspBufLen);
}

static sss_status_t mals_get_Data_BUF(mal_agent_ctx_t *pContext,
    uint8_t tag_P1,
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

    /* Copy the appropriate tag coming from respective function at P1 & P2 */
    getDataCmd[2] = tag_P1;
    getDataCmd[3] = tag_P2;

    ENSURE_OR_GO_CLEANUP(cmdBufLen < (255 - 5));

    if (cmdBufLen > 0) {
        getDataCmdLen += (U16)cmdBufLen;
        getDataCmdLen++;
        memcpy(&getDataCmd[5], cmdBuf, cmdBufLen);
        getDataCmd[4] /* Lc */ = (uint8_t)cmdBufLen;
    }

    ENSURE_OR_GO_CLEANUP(*pRspBufLen > 0);

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

    respstat = smCom_TransceiveRaw(
        pContext->pS05x_Ctx->conn_ctx, (uint8_t *)getDataCmd, getDataCmdLen, pRspBuf, &getDataRspLen);
    if (respstat != SM_OK) {
        LOG_E("Could not get requested Data!!!");
        goto cleanup;
    }

    if (pRspBuf[getDataRspLen - 2] == 0x90 && pRspBuf[getDataRspLen - 1] == 0x00) {
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

/*
 * NOTE 2: App/Pkg AID can be present full/partial or empty. When all the app/pkg are to be
 * retrieved, provide empty AID. Refer to the command format provided in the above table. Stop when
 * SW 9000/6A82/6A83 is received without response data. Ref: GET STATUS command in section
 * 11.4 of GP Card Spec v1.3 for response information.
 * only for Full package info or full app info (cmdBufLen == 2)
 */

static sss_status_t mals_get_Data_BUF_Recursive(mal_agent_ctx_t *pContext,
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
    U32 getDataCompleteRspLen = 0;

    /* Copy the appropriate tag coming from respective function at P2 */
    getDataCmd[3] = tag_P2;

    ENSURE_OR_GO_CLEANUP(cmdBufLen < (255 - 5));

    if (cmdBufLen > 0) {
        getDataCmdLen += (U16)cmdBufLen;
        getDataCmdLen++;
        memcpy(&getDataCmd[5], cmdBuf, cmdBufLen);
        getDataCmd[4] /* Lc */ = (uint8_t)cmdBufLen;
    }
    else {
        sss_stat = kStatus_SSS_Fail;
        goto cleanup;
    }

    ENSURE_OR_GO_CLEANUP(*pRspBufLen > 0);

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
    while (getDataRspLen > 2) {
        respstat = smCom_TransceiveRaw(pContext->pS05x_Ctx->conn_ctx,
            (uint8_t *)getDataCmd,
            getDataCmdLen,
            pRspBuf + getDataCompleteRspLen,
            &getDataRspLen);
        if (respstat != SM_OK) {
            LOG_E("Could not get requested Data!!!");
            goto cleanup;
        }
        if (getDataRspLen > 2) {
            getDataCompleteRspLen += getDataRspLen;
            if (pRspBuf[getDataCompleteRspLen - 2] == 0x90 && pRspBuf[getDataCompleteRspLen - 1] == 0x00) {
                /* 0x9000*/
                sss_stat = kStatus_SSS_Success;
            }
            else {
                sss_stat = kStatus_SSS_Fail;
                goto cleanup;
            }

            if (*pRspBufLen > getDataCompleteRspLen) {
                getDataRspLen = (U32)*pRspBufLen - getDataRspLen;
            }
            else {
                LOG_E("InSufficient Buffer passed!!!");
                sss_stat = kStatus_SSS_Fail;
                goto cleanup;
            }
        }
        else if (getDataRspLen == 2) {
            if (pRspBuf[getDataCompleteRspLen - 2] == 0x90 && pRspBuf[getDataCompleteRspLen - 1] == 0x00) {
                /* 0x9000*/
                sss_stat = kStatus_SSS_Success;
            }
            else if (pRspBuf[getDataCompleteRspLen - 2] == 0x6A && pRspBuf[getDataCompleteRspLen - 1] == 0x82) {
                /* 0x6A82*/
                sss_stat = kStatus_SSS_Success;
            }
            else if (pRspBuf[getDataCompleteRspLen - 2] == 0x6A && pRspBuf[getDataCompleteRspLen - 1] == 0x83) {
                /* 0x6A83*/
                sss_stat = kStatus_SSS_Success;
            }
            else {
                sss_stat = kStatus_SSS_Fail;
                goto cleanup;
            }
            *pRspBufLen = (size_t)getDataCompleteRspLen;
        }

        /* For the next iteration make p1 to 0x01 */
        getDataCmd[2] = 0x01;
    }

cleanup:
    return sss_stat;
}

static sss_status_t mals_verify_GetDataResponse(uint8_t tag_P2, uint8_t *pRspBuf, size_t *pRspBufLen)
{
    sss_status_t sss_stat = kStatus_SSS_Fail;
    size_t getDataRspLen  = 0;
    smStatus_t retStatus  = SM_NOT_OK;
    if (*pRspBufLen > 2) {
        getDataRspLen = *pRspBufLen;
        retStatus     = (pRspBuf[getDataRspLen - 2] << 8) | (pRspBuf[getDataRspLen - 1]);
        if (retStatus == SM_OK) {
            if (pRspBuf[0] == tag_P2) {
                if (pRspBuf[1] > 0) {
                    *pRspBufLen = pRspBuf[1];
                    memmove(pRspBuf, pRspBuf + 2, pRspBuf[1]);
                    sss_stat = kStatus_SSS_Success;
                    LOG_MAU8_D("Response:-", pRspBuf, *pRspBufLen);
                }
                else {
                    memset(pRspBuf, 0, *pRspBufLen);
                    *pRspBufLen = 0;
                }
            }
        }
    }
    return sss_stat;
}

static sss_status_t mals_parse_AppletUpgradeProgressResp(uint8_t *pRspBuf,
    size_t *pRspBufLen,
    mal_upgradeProgress_status_t *pUpgradeStatus,
    mal_recovery_status_t *pRecoveryStatus)
{
    sss_status_t sss_stat              = kStatus_SSS_Fail;
    smStatus_t retStatus               = SM_NOT_OK;
    size_t bufIndex                    = 0;
    size_t length_upgrade_confirmation = 0;
    size_t length_upgrade_session_info = 0;

    /* AMD-H Manage ELF Upgrad Response Table 4-6 */
    /* Format:
        *   Length of Upgrade Confirmation
        *   Upgrade Confirmation
        *   Length of ELF Upgrade Session Info
        *   ELF Upgrade Session Info
        */
    /* Length of Upgrade Confirmation */
    if (pRspBuf[bufIndex] <= 0x7FU) {
        length_upgrade_confirmation = *pRspBuf;
        bufIndex += 1 + length_upgrade_confirmation; // 1 Byte and Length of itself
    }
    else if (pRspBuf[bufIndex] == 0x81) {
        length_upgrade_confirmation = *(pRspBuf + 1);
        bufIndex += (1 + 1) + length_upgrade_confirmation; // 2 Bytes and Length of itself
    }
    else if (pRspBuf[bufIndex] == 0x82) {
        length_upgrade_confirmation = *(pRspBuf + 1);
        length_upgrade_confirmation = (length_upgrade_confirmation << 8) | *(pRspBuf + 2);
        bufIndex += (1 + 2) + length_upgrade_confirmation; // 3 Bytes and Length of itself
    }
    else {
        LOG_E("Length of Upgrade Confirmation Unsupported!!!");
        goto cleanup;
    }

    /* Length of ELF Upgrade Session Info */
    if (pRspBuf[bufIndex] <= 0x7FU) {
        length_upgrade_session_info = *(pRspBuf + bufIndex);
        bufIndex += 1; // 1 Byte and Length of itself
    }
    else if (pRspBuf[bufIndex] == 0x81) {
        length_upgrade_session_info = *(pRspBuf + 1);
        bufIndex += (1 + 1); // 2 Bytes and Length of itself
    }
    else if (pRspBuf[bufIndex] == 0x82) {
        length_upgrade_session_info = *(pRspBuf + 1);
        length_upgrade_session_info = (length_upgrade_session_info << 8) | *(pRspBuf + 2);
        bufIndex += (1 + 2); // 3 Bytes and Length of itself
    }
    else {
        LOG_E("Length of ELF Upgrade Session Info Unsupported!!!");
        goto cleanup;
    }

    if (length_upgrade_session_info + bufIndex > *pRspBufLen) {
        LOG_E("Invalid Length of ELF Upgrade Session Info!!!");
        goto cleanup;
    }

    retStatus =
        (pRspBuf[length_upgrade_session_info + bufIndex] << 8) | (pRspBuf[length_upgrade_session_info + bufIndex + 1]);

    /* ELF Upgrade Session Info */
    if (pRspBuf[bufIndex] == ELF_UPGRADE_INFO_TAG) {
        /* Length of Tag  0xA1*/
        bufIndex++;
        /* Length of value */
        bufIndex++;
        if (pRspBuf[bufIndex] == ELF_UPGRADE_SESSION_STATUS_TAG) {
            /* Length of Tag  0x90*/
            bufIndex++;
            /* Length of value */
            bufIndex++;
            if ((pRspBuf[bufIndex] == 0x00) || (pRspBuf[bufIndex] == 0x01)) {
                *pUpgradeStatus  = mal_upgrade_not_inProgress;
                *pRecoveryStatus = mal_recovery_not_started;
            }
            else if (pRspBuf[bufIndex] == 0x04) {
                *pUpgradeStatus  = mal_upgrade_inProgress;
                *pRecoveryStatus = mal_recovery_started;
            }
            else {
                *pUpgradeStatus  = mal_upgrade_inProgress;
                *pRecoveryStatus = mal_recovery_not_started;
            }

            if (retStatus == SM_OK) {
                sss_stat = kStatus_SSS_Success;
            }
        }
    }
cleanup:
    return sss_stat;
}

#endif /* #if SSS_HAVE_SE05X */

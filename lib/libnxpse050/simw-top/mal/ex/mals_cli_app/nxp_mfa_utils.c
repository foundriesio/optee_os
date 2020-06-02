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

#include "nxp_mfa_utils.h"

#include <nxEnsure.h>
#include <nxLog_App.h>
#include <stdlib.h>
#include <string.h>

#include "mfa_utils.h"
#include "smCom.h"
#include "sm_apdu.h"
#ifdef SMCOM_JRCP_V2
#include <smComJRCP.h>
#endif
#include "mal_api.h"
#include <stdio.h>

#if RJCT_VCOM
static uint32_t vcom_set_reset_timer(uint32_t time_ms);
#endif

void prepareTear(uint32_t tear_time)
{
    uint32_t st = 0;
#if AX_EMBEDDED
#if SSS_HAVE_SE05X
#if FSL_FEATURE_SOC_PIT_COUNT > 0
    tear_time /= 10;
    LOG_W("Starting the %d usec Timer to reset the IC", tear_time);
    se_pit_SetTimer(tear_time);
    st = SMCOM_OK;
#endif
#endif
#elif RJCT_VCOM
    tear_time /= 10;
    st = vcom_set_reset_timer(tear_time);
#elif SMCOM_JRCP_V2
    st = smComJRCP_Reset(NULL, tear_time);
#endif
}

#if RJCT_VCOM
static uint32_t vcom_set_reset_timer(uint32_t time_ms)
{
    U8 txBuf[6] = {0};
    U16 txLen = sizeof(txBuf);
    U8 rxBuf[MAX_APDU_BUF_LENGTH];
    U32 rxBufLen = sizeof(rxBuf);
    U32 st = 0;
    txBuf[0] = 0xFF;                   //propritery set timer CLA byte
    txBuf[1] = 0x0B;                   // INS does not matter can be anything
    txBuf[2] = (time_ms >> 24) & 0xFF; // P1 time
    txBuf[3] = (time_ms >> 16) & 0xFF; // P2 time
    txBuf[4] = (time_ms >> 8) & 0xFF;  // P3 time
    txBuf[5] = time_ms & 0xFF;         // P4 time

    st = smCom_TransceiveRaw(NULL, txBuf, txLen, rxBuf, &rxBufLen);
    if (st != SMCOM_OK) {
        LOG_E("Transceive failed returns : %d\r\n", st);
        return st;
    }
    return st;
}
#endif

void mfa_process_loadpkg_with_tear(const char *pkgname, uint32_t tear_time)
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

    if (tear_time > 0) {
        prepareTear(tear_time);
    }

    // TODO: Use signature buffer as paramters.
    status = mal_agent_load_package(
        &g_mal_agent_load_ctx, buffer, numbytes + n, NULL, 0);
    printMALSStatusCode(status);
    free(buffer);
}
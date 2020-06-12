/*
* Copyright 2020 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#include "se05x_ImportExternalObjectCreate.h"

#include <ex_sss_boot.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <nxScp03_Apis.h>
#include <nxScp03_Const.h>
#include <se05x_APDU.h>
#include <se05x_tlv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smCom.h"

static ex_sss_boot_ctx_t gex_sss_import_external_create;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_import_external_create)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 1
#define EX_SSS_BOOT_SKIP_SELECT_APPLET 0

#include <ex_sss_main_inc.h>

void usage()
{
    LOG_W(
        "\n\
    Usage:\n\
    se05x_ImportExternalObjectDo.exe \n\
        -file <Input file containing raw APDU>");
    return;
}

static sss_status_t parse_command_line_args(int argc, const char **argv);

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status;
    int argc                           = gex_sss_argc;
    const char **argv                  = gex_sss_argv;
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pCtx->session;
    Se05xSession_t *pSe05xSessionCtx   = &se05x_session->s_ctx;
    int cmdBufLen_len                  = 0;
    uint8_t rsp[200];
    size_t rspLen                               = sizeof(rsp);
    tlvHeader_t hdr                             = {0};
    uint8_t hasLe                               = 0;
    uint8_t raw_command[SE05X_MAX_BUF_SIZE_CMD] = {0};
    size_t raw_command_size                     = sizeof(raw_command);
    size_t cmdBufLen                            = 0;
    uint8_t *pcmdBuf                            = NULL;
    smStatus_t sm_status                        = SM_NOT_OK;

    /* Parse commandline arguments */
    status = parse_command_line_args(argc, argv);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    if (input_filename != NULL) {
        FILE *fp = NULL;
        fp       = fopen(input_filename, "rb");
        if (fp == NULL) {
            LOG_E("Cannot open file");
            return kStatus_SSS_Fail;
        }
        size_t bytes_read = fread((void *)raw_command, raw_command_size, 1, fp);
        raw_command_size  = bytes_read;
        fclose(fp);
    }

    memcpy(&hdr, raw_command, sizeof(tlvHeader_t));
    cmdBufLen = (size_t)raw_command[sizeof(tlvHeader_t)];
    if (cmdBufLen == 0x00) {
        cmdBufLen     = (size_t)((raw_command[sizeof(tlvHeader_t) + 1] << 8) + (raw_command[sizeof(tlvHeader_t) + 2]));
        cmdBufLen_len = 2;
    }
    pcmdBuf = &raw_command[sizeof(tlvHeader_t) + 1 + cmdBufLen_len];

    sm_status = pSe05xSessionCtx->fp_TXn(pSe05xSessionCtx, &hdr, pcmdBuf, cmdBufLen, rsp, &rspLen, hasLe);
    status    = (sm_status == SM_OK) ? kStatus_SSS_Success : kStatus_SSS_Fail;

exit:
    return status;
}

static sss_status_t parse_command_line_args(int argc, const char **argv)
{
    sss_status_t status              = kStatus_SSS_Success;
    bool input_file_parameter_passed = false;

    for (int j = 1; j < argc; j++) {
        if (strcmp(argv[j], "-file") == 0) {
            input_file_parameter_passed = true;
            j++;
            input_filename = (char *)argv[j];
        }
    }
    if (!input_file_parameter_passed) {
        LOG_E("Input file parameter not passed");
        status = kStatus_SSS_Fail;
        usage();
    }

    return status;
}

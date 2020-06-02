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

#include <mal_api.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <stdio.h>
#include <string.h>

#include "mfa_process.h"
#include "mfa_process_tst.h"
#include "mfa_utils.h"
#if AX_EMBEDDED
#include "se_pit_config.h"
#endif
#if SMCOM_JRCP_V2
#include "smComJRCP.h"
#endif

#if NXP_INTERNAL_IMPL
#include "nxp_mfa_utils.h"
#endif

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

#define IS_ARGV_1(VALUE_LONG) \
    (0 == STRNICMP(argv[1], (VALUE_LONG), sizeof((VALUE_LONG))))

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static void usage(const char *program_name);
void parseArgsAndProcess(int argc, const char *argv[]);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

int main(int argc, const char *argv[])
{
    sss_status_t status;
    const char *portName;
    const char *program_name;

    program_name = argv[0];
    if (argc < 2) {
        usage(program_name);
        exit(0);
    }
    if (!((IS_ARGV_1("--testIoTPreUpgrade")) ||
            (IS_ARGV_1("--testIoTPostUpgrade")))) {
        gfeature_app_mals_boot_ctx.se05x_open_ctx.skip_select_applet = TRUE;
    }

    status = ex_sss_boot_connectstring(0, NULL, &portName);
    if (kStatus_SSS_Success != status) {
        printf("ex_sss_boot_connectstring Failed");
        return 1;
    }

    status = ex_sss_boot_open(&gfeature_app_mals_boot_ctx, portName);
    if (kStatus_SSS_Success != status) {
        printf("ex_sss_session_open Failed");
        return 1;
    }

    status = ex_sss_kestore_and_object_init(&gfeature_app_mals_boot_ctx);
    if (kStatus_SSS_Success != status) {
        printf("ex_sss_kestore_and_object_init Failed");
        return 1;
    }

    if (!((IS_ARGV_1("--testIoTPreUpgrade")) ||
            (IS_ARGV_1("--testIoTPostUpgrade")) ||
            (IS_ARGV_1("--testapplet")))) {
        status = mal_agent_init_context(
            &g_mal_agent_load_ctx, &gfeature_app_mals_boot_ctx.session);
        ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

        status = mal_agent_session_open(&g_mal_agent_load_ctx);
        ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    }

    parseArgsAndProcess(argc, argv);

    if (!((IS_ARGV_1("--testIoTPreUpgrade")) ||
            (IS_ARGV_1("--testIoTPostUpgrade")) ||
            (IS_ARGV_1("--testapplet")))) {
        mal_agent_session_close(&g_mal_agent_load_ctx);
    }

    ex_sss_session_close(&gfeature_app_mals_boot_ctx);
    return 0;
cleanup:
    LOG_E("Failure in main()");
    return 1;
}

void parseArgsAndProcess(int argc, const char *argv[])
{
    const char *api_param1;
    const char *api_param2;
    const char *program_name;
    program_name = argv[0];

    api_param1 = argv[2];
    api_param2 = argv[3];

    if (IS_ARGV_1("--loadpkg")) {
        mfa_process_loadpkg(api_param1);
    }
#if NXP_INTERNAL_IMPL
    else if (IS_ARGV_1("--loadpkgwt")) {
        uint32_t tear_time = 0;
        if (argc > 3 && api_param2) {
            tear_time = atoi(api_param2);
        }
        mfa_process_loadpkg_with_tear(api_param1, tear_time);
    }
#endif
    else if (IS_ARGV_1("--getuid")) {
        mfa_process_getuid();
    }
    else if (IS_ARGV_1("--getappcontents")) {
        mfa_process_getappcontents(api_param1);
    }
    else if (IS_ARGV_1("--getpkgcontents")) {
        mfa_process_getpkgcontents(api_param1);
    }
    else if (IS_ARGV_1("--getPbkeyId")) {
        mfa_process_getPbkeyId();
    }
    else if (IS_ARGV_1("--malsgetversion")) {
        mfa_process_malsgetversion();
    }
    else if (IS_ARGV_1("--getsignature")) {
        mfa_process_getsignature(api_param1);
    }
    else if (IS_ARGV_1("--checkTear")) {
        mfa_process_checkTear();
    }
    else if (IS_ARGV_1("--checkUpgradeProgress")) {
        mfa_process_checkUpgradeProgress();
    }
    else if (IS_ARGV_1("--getENCIdentifier")) {
        mfa_process_getENCIdentifier();
    }
    else if (IS_ARGV_1("--testapplet")) {
        mfa_process_testapplet(api_param1, api_param2);
    }
    else if (IS_ARGV_1("--testIoTPreUpgrade")) {
        mfa_process_testIoTPreUpgrade();
    }
    else if (IS_ARGV_1("--testIoTPostUpgrade")) {
        mfa_process_testIoTPostUpgrade();
    }
    else if (IS_ARGV_1("--checkCAIdentifier")) {
        mfa_process_getCAIdentifier();
    }
    else if (IS_ARGV_1("--checkCAKeyIdentifier")) {
        mfa_process_getCAKeyIdentifier();
    }
	else if (IS_ARGV_1("--getpkgversion")) {
        mfa_process_getPkgVerion(api_param1);
    }
    //#if (SMCOM_JRCP_V2)
    //    else if (IS_ARGV_1("--prepareTear")) {
    //        mfa_process_prepareTear(api_param1);
    //    }
    //#endif
    else {
        usage(program_name);
    }
}

static void usage(const char *program_name)
{
    LOG_I("Give one of Following Options for Mals Test");
    LOG_I(
        "%s [--loadpkg] path-to-Mals-applet-package-binary-file", program_name);
#if NXP_INTERNAL_IMPL
    LOG_I("%s [--loadpkgwt] path-to-Mals-applet-package-binary-file tear_time",
        program_name);
#endif
    LOG_I("%s [--getuid]", program_name);
    LOG_I(
        "%s [--getappcontents] File-Name-to-store-card-contents "
        "optional-app-aid",
        program_name);
    LOG_I(
        "%s [--getpkgcontents] File-Name-to-store-card-contents "
        "optional-pkg-aid",
        program_name);
    LOG_I("%s [--getPbkeyId]", program_name);
    LOG_I("%s [--malsgetversion]", program_name);
    LOG_I("%s [--getsignature] File-Name-to-store-signature", program_name);
    LOG_I("%s [--checkTear]", program_name);
    LOG_I("%s [--checkUpgradeProgress]", program_name);
    LOG_I("%s [--getENCIdentifier]", program_name);
    LOG_I("%s [--testapplet] applet-aid, apdu-command", program_name);
    LOG_I(
        "%s [--prepareTear] No-of-instruction-bytes-before-tear", program_name);
}

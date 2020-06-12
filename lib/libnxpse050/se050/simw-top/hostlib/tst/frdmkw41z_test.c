/**
 * @file frdmkw41z_rng_test.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2018 NXP
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
 * Get random data using frdmkw41z board
 */

#include "fsl_sss_ftr.h"
#ifdef AX_EMBEDDED
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"
#include <stdio.h>
#include <stdlib.h>
#include <fsl_sss_api.h>
#include <fsl_sss_sscp.h>
#include "nxLog_App.h"
#include "ax_api.h"

#include "ksdk_mbedtls.h"
#include "sm_timer.h"

#include "ex_sss.h"

#include "se05x_tlv.h"
#include "se05x_APDU.h"
#include "fsl_sss_se05x_types.h"

#define RND_KEY_LEN 32


sss_rng_context_t rng;

sss_status_t rng_get_random()
{
    sss_status_t status;
    int i;
    uint8_t rndData[RND_KEY_LEN] = {
        0,
    };
    LOG_I("Starting rng init");
    status = sss_rng_context_init(&rng, &gSSSExCtx.currentSession /* Session */);
    if (status != kStatus_SSS_Success) {
        LOG_E("rng init failed.");
    }
    else {
        LOG_I("rng init completed");
    }

    for (i = 0; i < 10; i++) {
        LOG_I("Starting rng get random");
        status = sss_rng_get_random(&rng, rndData, sizeof(rndData));
        if (status != kStatus_SSS_Success) {
            LOG_E("rng get random failed.");
        }
        else {
            LOG_I("rng get random completed");
            LOG_MAU8_I("Random Data", rndData, sizeof(rndData));
        }
    }

    return status;
}


sss_status_t se050x_ex_i2cm()
{
    smStatus_t status;
    se050_I2CM_cmd_t TLV[2];
    uint8_t Green_ON[] = {0xFF, 0x01};

    TLV[0].type = se050_I2CM_Configure;
    TLV[0].cmd.cfg.I2C_addr = 0x30;
    TLV[0].cmd.cfg.I2C_baudRate = 0x00;

    TLV[1].type = se050_I2CM_Write;
    TLV[1].cmd.w.writeLength = sizeof(Green_ON);
    TLV[1].cmd.w.writebuf = Green_ON;

    status = Se05x_i2c_master_txn(&gSSSExCtx.currentSession, &TLV[0], ARRAY_SIZE(TLV));
    return status;
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    sss_status_t status;

    /*Initialize to bring up the board */
#if defined(FRDM_KW41Z)
    BOARD_BootClockRUN();
#endif

#ifdef FREEDOM
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    LED_BLUE_INIT(1);
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);
    LED_RED_ON();
    LED_BLUE_ON();
#endif

#if defined(IMX_RT)

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#endif

    sm_initSleep();
    CRYPTO_InitHardware();

    /*Open Session*/
#if SSS_HAVE_SSS
    LOG_I("Starting Open Session");
    status = ex_SessionOpen(0); /*0-> No Debug Reset*/
    if (status != kStatus_SSS_Success) {
        LOG_E("Open Session failed.");
        return status;
    }
    else {
        LOG_I("Open Session completed");
    }

    /*Get Random data */
    // rng_get_random();

    se050x_ex_i2cm();

    /*Close session */
    LOG_I("Starting Close Session");
    ex_SessionClose();
    LOG_I("Close Session completed");
#endif

    /* Never exit */
    while (1) {
    }
}

#endif //#if AX_EMBEDDED

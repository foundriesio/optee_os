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

/* THIS IS THE MAIN CODE TO BE RUN FROM SECURE ZONE
 *
 */

#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif

#include <nxLog_App.h>

#include "arm_cmse.h"
#include "ax_reset.h"
#include "board.h"
#include "clock_config.h"
#include "ex_sss_boot.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "ksdk_mbedtls.h"
#include "nxEnsure.h"
#include "pin_mux.h"
#include "sm_timer.h"
#include "sss_psa_alt.h"
#include "tzm_config.h"
#include "veneer_table.h"

#define NON_SECURE_START 0x00038000

typedef void (*funcptr_ns)(void) __attribute__((cmse_nonsecure_call));

/* Session Open from driver->p_init API */
int main()
{
    funcptr_ns ResetHandler_ns;

    /* Init board hardware. */
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM8 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    PRINTF("Hello from secure world (simw)!\r\n");

#if defined(MBEDTLS) || defined(MBEDCRYPTO)
    CRYPTO_InitHardware();
#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif /* SHA */
#endif /* defined(MBEDTLS) */

    sm_initSleep();

    /* Set non-secure main stack (MSP_NS) */
    __TZ_set_MSP_NS(*((uint32_t *)(NON_SECURE_START)));

    /* Set non-secure vector table */
    SCB_NS->VTOR = NON_SECURE_START;

    /* Get non-secure reset handler */
    ResetHandler_ns = (funcptr_ns)(*((uint32_t *)((NON_SECURE_START) + 4U)));

    /* Call non-secure application */
    PRINTF("Entering normal world.\r\n");
    /* Jump to normal world */
    ResetHandler_ns();
    while (1) {
        /* This point should never be reached */
    }
}

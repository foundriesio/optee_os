/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2017
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/*
 *  _   ___   _______    _   _ ______ _____    _____           _          _ _
 * | \ | \ \ / /  __ \  | \ | |  ____/ ____|  / ____|         | |        (_) |
 * |  \| |\ V /| |__) | |  \| | |__ | |      | |     ___   ___| | ___ __  _| |_
 * | . ` | > < |  ___/  | . ` |  __|| |      | |    / _ \ / __| |/ / '_ \| | __|
 * | |\  |/ . \| |      | |\  | |   | |____  | |___| (_) | (__|   <| |_) | | |_
 * |_| \_/_/ \_\_|      |_| \_|_|    \_____|  \_____\___/ \___|_|\_\ .__/|_|\__|
 *                                                                 | |
 *                                                                 |_| Example On PC
 *
 * Delegates for handling of GPIOs
 *
 * $Date$
 * $Author$
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif

#include "np_Delegate_Platform.h"
#include <phPlatform_Delegate.h>
#include "phUcBal.h"
#include "phUcBal_GPIO.h"
#include "phUcBal_Protocol.h"
#include "ph_TypeDefs.h"

#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define MAX_WAIT_INTERVAL_MS 500

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

static phStatus_t __stdcall DelegateWaitFor_IRQHigh(void *pCtx);
static phStatus_t __stdcall DelegateWaitFor_BusyLow(void *pCtx);
static phStatus_t __stdcall DelegateWaitFor_BusyHigh(void *pCtx);
static phStatus_t WaitForPin(
    NPPlatformCtx_t *pPfCtx, phUcBal_GPIO_Name_t the_gpio, uint8_t waitForHigh);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

int np_Delegate_Platform_Init(NPPlatformCtx_t *pPfCtx)
{
    phPlatform_DelegateInit(pPfCtx,
        &DelegateWaitFor_IRQHigh,
        &DelegateWaitFor_BusyLow,
        &DelegateWaitFor_BusyHigh);

    return 0;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t __stdcall DelegateWaitFor_IRQHigh(void *pCtx)
{
    return WaitForPin((NPPlatformCtx_t *)pCtx, PHUCBAL_GPIO_NAME_IRQ, 1);
}
static phStatus_t __stdcall DelegateWaitFor_BusyLow(void *pCtx)
{
    return WaitForPin((NPPlatformCtx_t *)pCtx, PHUCBAL_GPIO_NAME_BUSY, 0);
}
static phStatus_t __stdcall DelegateWaitFor_BusyHigh(void *pCtx)
{
    return WaitForPin((NPPlatformCtx_t *)pCtx, PHUCBAL_GPIO_NAME_BUSY, 0);
}

static phStatus_t WaitForPin(
    NPPlatformCtx_t *pPfCtx, phUcBal_GPIO_Name_t the_gpio, uint8_t waitForHigh)
{
    phUcBal_Cmd_t cmd = {CLA_GPIO};
    phUcBal_Rsp_t rsp = {0};
    uint16_t wRxLength = 0;
    bool timedOut = FALSE;
    int nextSleepDuration = 1;
    int totalSleepDuration = 1;

    cmd.header.CLA = CLA_GPIO;
    cmd.header.INS = GPIO_INS_GetV;
    cmd.header.P1 = (uint8_t)the_gpio;

    do {
        phbalReg_Exchange(pPfCtx->pSerialWin,
            0,
            cmd.buf,
            PH_UCBAL_PROTOCOL_HEADER_SIZE,
            PH_UCBAL_PROTOCOL_HEADER_SIZE + 1,
            rsp.buf,
            &wRxLength);
        if (rsp.header.Response[0] == waitForHigh) {
            break;
        }
        Sleep(nextSleepDuration++);
        totalSleepDuration += nextSleepDuration;
        if (totalSleepDuration > MAX_WAIT_INTERVAL_MS) {
            timedOut = TRUE;
        }
    } while (!timedOut);

    if (timedOut)
        return PH_ERR_IO_TIMEOUT;
    else
        return PH_ERR_SUCCESS;
}

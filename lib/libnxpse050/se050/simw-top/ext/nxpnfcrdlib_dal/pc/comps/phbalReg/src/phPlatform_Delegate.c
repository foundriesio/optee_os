/*
 *         Copyright (c), NXP Semiconductors Bangalore / India
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
 * Platform file to abstract HOST and front-end.
 * $Author$
 * $Revision$
 * $Date$
 *
 */

#include <ph_Status.h>
#include <ph_NxpBuild.h>

#ifdef NXPBUILD__PH_PLATFORM_DELEGATE

#include <phDriver.h>
#include "phPlatform_Delegate.h"
#include <phOsal.h>


static phPlatform_Delegate_t gppphPlatform_Delegate;

phStatus_t phPlatform_DelegateInit(
    void * pCtx,
    fpphPlatform_DelegateWaitForPIN  fpWaitForIRQHigh,
    fpphPlatform_DelegateWaitForPIN  fpWaitForBusyLow,
    fpphPlatform_DelegateWaitForPIN fpWaitForBusyHigh
)
{
    gppphPlatform_Delegate.pCtx = pCtx;
    gppphPlatform_Delegate.fpWaitForIRQHigh = fpWaitForIRQHigh;
    gppphPlatform_Delegate.fpWaitForBusyLow = fpWaitForBusyLow;
    gppphPlatform_Delegate.fpWaitForBusyHigh = fpWaitForBusyHigh;
    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_DelegateWaitForIRQHigh() {
    if ( NULL != gppphPlatform_Delegate.fpWaitForIRQHigh) {
        return gppphPlatform_Delegate.fpWaitForIRQHigh(gppphPlatform_Delegate.pCtx);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_DelegateWaitForBusyLow() {
    if ( NULL != gppphPlatform_Delegate.fpWaitForBusyLow) {
        return gppphPlatform_Delegate.fpWaitForBusyLow(gppphPlatform_Delegate.pCtx);
    }
    return PH_ERR_SUCCESS;
}
phStatus_t phPlatform_DelegateWaitForBusyHigh() {
    if ( NULL != gppphPlatform_Delegate.fpWaitForBusyHigh) {
        return gppphPlatform_Delegate.fpWaitForBusyHigh(gppphPlatform_Delegate.pCtx);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack) {
    uint32_t sleep_ms = 0;
    switch (eTimerUnit)
    {
    case  PH_DRIVER_TIMER_SECS:
        sleep_ms = dwTimePeriod * 1000;
        break;
    case  PH_DRIVER_TIMER_MICRO_SECS:
        sleep_ms = dwTimePeriod / 1000;
        break;
    case  PH_DRIVER_TIMER_MILLI_SECS:
        sleep_ms = dwTimePeriod;
        break;
    default:
        break;
    }
    Sleep(sleep_ms);
    return PH_ERR_SUCCESS;
}

phStatus_t phDriver_TimerStop(void) {
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */

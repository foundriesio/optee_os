/*
 * phPlatform_LpcPort.h
 *
 *  Created on: Apr 11, 2016
 *      Author: nxp69678
 */

#ifndef PHPLATFORM_DELEGATE_H
#define PHPLATFORM_DELEGATE_H

#include <ph_Status.h>

#ifdef NXPBUILD__PH_PLATFORM_DELEGATE

typedef phStatus_t ( __stdcall * fpphPlatform_DelegateWaitForPIN) (
    void * pCtx
);

typedef struct {
    void * pCtx;
    fpphPlatform_DelegateWaitForPIN fpWaitForIRQHigh;
    fpphPlatform_DelegateWaitForPIN fpWaitForBusyLow;
    fpphPlatform_DelegateWaitForPIN fpWaitForBusyHigh;
} phPlatform_Delegate_t;

phStatus_t phPlatform_DelegateInit(
    void * pCtx,
    fpphPlatform_DelegateWaitForPIN fpWaitForIRQHigh,
    fpphPlatform_DelegateWaitForPIN fpWaitForBusyLow,
    fpphPlatform_DelegateWaitForPIN fpWaitForBusyHigh);

phStatus_t phPlatform_DelegateWaitForIRQHigh(void);

phStatus_t phPlatform_DelegateWaitForBusyLow(void);

phStatus_t phPlatform_DelegateWaitForBusyHigh(void);

#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */
#endif /* PHPLATFORM_DELEGATE_H */

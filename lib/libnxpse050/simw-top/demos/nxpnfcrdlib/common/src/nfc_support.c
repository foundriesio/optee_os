/**
 * @file nfc_support.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2019 NXP
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
 * File contains functions for adding NFC support for SE05X MIFARE DESFire EV2 examples.
 */
/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <phTMIUtils.h>
#include <phalMfdfEv2.h>
#include <phalVca.h>
#include <stdio.h>
#include <phbalReg.h>

#ifdef NXPBUILD__PHBAL_REG_DELEGATE
#include "np_Delegate_Bal.h"
#endif
#ifdef NXPBUILD__PHBAL_PLATFORM_DELEGATE
#include "np_Delegate_Platform.h"
#endif

#include "np_NxpNfcRdLib.h"
#include "np_TypeA.h"
#include "phApp_Init.h"
#include "phExMfDfCrypto.h"
#include "phNfcLib.h"
#include "ph_Status.h"
#include "phOsal.h"
#include "nfc_support.h"
#include "nxLog_App.h"
#include <string.h>
#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */


/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
/** This is the comport and the front end being used
*/
const char *com_port = "COM33";
const char *front_end = "RC663";

extern NPNxpNfcRdLibCtx_t gRdLibCtx;

/* *****************************************************************************************************************
* Private Functions Prototypes
* ***************************************************************************************************************** */
/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */
phStatus_t np_ActivateL4(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;
    uint8_t aUid[10] = {0};
    uint8_t uidLen = 0;
    uint8_t bSak;
    uint8_t bMoreCardsAvailable;
    PH_CHECK_SUCCESS_FCT(status, phhalHw_FieldReset(pRdCtx->pHal));

    status = phpalI14443p3a_ActivateCard(&pRdCtx->sType3APal,
        NULL,
        0,
        aUid,
        &uidLen,
        &bSak,
        &bMoreCardsAvailable);
    if (status != PH_ERR_SUCCESS) {
        LOG_E("Could not detect any TYPE A Card.\n");
        return status;
    }
    LOG_MAU8_I("UID after L3 Activation", aUid, sizeof(aUid));

    if (bSak == 0x20) // DESFIre
    {
        uint8_t pAts[256] = {0};
        uint8_t PH_MEMLOC_REM bCidEnabled;
        uint8_t PH_MEMLOC_REM bCid;
        uint8_t PH_MEMLOC_REM bNadSupported;
        uint8_t PH_MEMLOC_REM bFwi;
        uint8_t PH_MEMLOC_REM bFsdi;
        uint8_t PH_MEMLOC_REM bFsci;

        // activate till level 4
        PH_CHECK_SUCCESS_FCT(status,
            phpalI14443p4a_ActivateCard(&pRdCtx->sType4APal,
                0x08, // 256 bytes buffer
                0,    // CID = 0
                (uint8_t)PHHAL_HW_RF_DATARATE_106,
                (uint8_t)PHHAL_HW_RF_DATARATE_106,
                pAts));
        LOG_MAU8_I("ATS after L4 Activation", pAts, pAts[0]);
        PH_CHECK_SUCCESS_FCT(status,
            phpalI14443p4a_GetProtocolParams(&pRdCtx->sType4APal,
                &bCidEnabled,
                &bCid,
                &bNadSupported,
                &bFwi,
                &bFsdi,
                &bFsci));
        PH_CHECK_SUCCESS_FCT(status,
            phpalI14443p4_SetProtocol(&pRdCtx->sType4Pal,
                bCidEnabled,
                bCid,
                bNadSupported,
                pRdCtx->sType4Pal.bNad,
                bFwi,
                bFsdi,
                bFsci));

    }
    return status;
}


void nfcInit(NPNxpNfcRdLibCtx_t** ppRdLibCtx)
{
    *ppRdLibCtx = &gRdLibCtx;
}

phStatus_t nfcConnect(NPNxpNfcRdLibCtx_t* pRdLibCtx)
{
    phStatus_t status = PH_ERR_SUCCESS;
#if (FRDM_K64F || LPC_55x || IMX_RT)
#ifdef PH_PLATFORM_HAS_ICFRONTEND
    PH_CHECK_SUCCESS_FCT(
        status,phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t)));
        pRdLibCtx->pBal = &sBalParams;
        pHal = &pRdLibCtx->sHal_Rc663;
    PH_CHECK_SUCCESS_FCT(status,phApp_Configure_IRQ());
 #endif
#endif//(FRDM_K64F || LPC_55x || IMX_RT)
#ifdef NXPBUILD__PHBAL_REG_DELEGATE
    PH_CHECK_SUCCESS_FCT(
        status, np_NxpNfcRdLib_CheckComPortName(com_port, &(pRdLibCtx->sBalCtx)));
#endif

#ifdef NXPBUILD__PH_PLATFORM_DELEGATE
    PH_CHECK_SUCCESS_FCT(
        status, np_NxpNfcRdLib_CheckFrontEndName(front_end, pRdLibCtx));
#else
    pRdLibCtx->selectedHal = NPSelectedHal_RC663;
#endif

    PH_CHECK_SUCCESS_FCT(status, np_NxpNfcRdLib_Init(pRdLibCtx));
    PH_CHECK_SUCCESS_FCT(status, np_mfdf_Init(pRdLibCtx));
    PH_CHECK_SUCCESS_FCT(status, np_ActivateL4(pRdLibCtx));
    return status;
}



phStatus_t nfcDisconnect(NPNxpNfcRdLibCtx_t* pRdLibCtx)
{
    phStatus_t status = PH_ERR_SUCCESS;
        PH_CHECK_SUCCESS_FCT(status, np_NxpNfcRdLib_DeInit(pRdLibCtx));
#ifdef NXPBUILD__PHBAL_REG_DELEGATE
    PH_CHECK_SUCCESS_FCT(status, phbalReg_ClosePort(pRdLibCtx->pBal));
#endif
    return status;
}

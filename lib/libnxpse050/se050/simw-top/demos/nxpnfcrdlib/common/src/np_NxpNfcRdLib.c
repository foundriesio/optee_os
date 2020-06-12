/* Copyright 2017,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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
* Initialize and manage the stack of NxpNfcRdLib
*
* $Date$
* $Author$
* $Revision$
*/

/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <np_NxpNfcRdLib.h>
#include <stdio.h>
#include <string.h>
#include <np_TypeA.h>
#include <ph_Status.h>
#include <phhalHw.h>
#include "nxLog_App.h"

extern    phStatus_t phhalHw_Rc663_DeInit(
    phhalHw_Rc663_DataParams_t * pDataParams
    );

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */

#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#define SET_FE(STR_NAME, ENUM_NAME)                                 \
    do {                                                            \
        if (0 == _strnicmp(STR_NAME, szFeName, sizeof(STR_NAME))) { \
            pRdCtx->selectedHal = (ENUM_NAME);                      \
            return PH_ERR_SUCCESS;                                  \
        }                                                           \
    } while (0)

#define SET_COM(USER_NAME, VALID_NAME)                                    \
    do {                                                                  \
        if (0 == _strnicmp(USER_NAME, szComPort, sizeof(USER_NAME))) {    \
            strncpy(pBalCtx->portName, (VALID_NAME), sizeof(VALID_NAME)); \
            return PH_ERR_SUCCESS;                                        \
        }                                                                 \
    } while (0)

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

static void printUsage(const char *exe_name);

#ifdef NXPBUILD__PHBAL_REG_DELEGATE
static phStatus_t initBal_DelegateVCOM(NPNxpNfcRdLibCtx_t *pRdCtx);
#endif

static phStatus_t initHal_CLRC663(NPNxpNfcRdLibCtx_t *pRdCtx);

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

phStatus_t np_NxpNfcRdLib_CheckUsage(int argc, const char *exe_name)
{
    if (argc == 3)
        return PH_ERR_SUCCESS;
    else {
        printUsage(exe_name);
        return PH_ERR_INVALID_PARAMETER;
    }
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127) // C4127: conditional expression is constant
#endif

#ifdef NXPBUILD__PHBAL_REG_DELEGATE
phStatus_t np_NxpNfcRdLib_CheckComPortName(
    const char *szComPort, NPDelegateBalCtx_t *pBalCtx)
{
    SET_COM("COM1", "COM1");
    SET_COM("COM2", "COM2");
    SET_COM("COM3", "COM3");
    SET_COM("COM4", "COM4");
    SET_COM("COM5", "\\\\.\\COM5");
    SET_COM("COM6", "\\\\.\\COM6");
    SET_COM("COM7", "\\\\.\\COM7");
    SET_COM("COM8", "\\\\.\\COM8");
    SET_COM("COM9", "\\\\.\\COM9");
    SET_COM("COM10", "\\\\.\\COM10");
    SET_COM("COM11", "\\\\.\\COM11");
    SET_COM("COM12", "\\\\.\\COM12");
    SET_COM("COM13", "\\\\.\\COM13");
    SET_COM("COM14", "\\\\.\\COM14");
    SET_COM("COM15", "\\\\.\\COM15");
    SET_COM("COM16", "\\\\.\\COM16");
    SET_COM("COM17", "\\\\.\\COM17");
    SET_COM("COM18", "\\\\.\\COM18");
    SET_COM("COM19", "\\\\.\\COM19");
    SET_COM("COM20", "\\\\.\\COM20");
    SET_COM("COM21", "\\\\.\\COM21");
    SET_COM("COM22", "\\\\.\\COM22");
    SET_COM("COM23", "\\\\.\\COM23");
    SET_COM("COM24", "\\\\.\\COM24");
    SET_COM("COM25", "\\\\.\\COM25");
    SET_COM("COM26", "\\\\.\\COM26");
    SET_COM("COM27", "\\\\.\\COM27");
    SET_COM("COM28", "\\\\.\\COM28");
    SET_COM("COM29", "\\\\.\\COM29");
    SET_COM("COM30", "\\\\.\\COM30");
    SET_COM("COM31", "\\\\.\\COM31");
    SET_COM("COM32", "\\\\.\\COM32");
    SET_COM("COM33", "\\\\.\\COM33");
    SET_COM("COM34", "\\\\.\\COM34");
    SET_COM("COM35", "\\\\.\\COM35");
    SET_COM("COM36", "\\\\.\\COM36");
    SET_COM("COM37", "\\\\.\\COM37");
    SET_COM("COM38", "\\\\.\\COM38");
    SET_COM("COM39", "\\\\.\\COM39");
    SET_COM("COM40", "\\\\.\\COM40");
    SET_COM("\\\\.\\COM5", "\\\\.\\COM5");
    SET_COM("\\\\.\\COM6", "\\\\.\\COM6");
    SET_COM("\\\\.\\COM7", "\\\\.\\COM7");
    SET_COM("\\\\.\\COM8", "\\\\.\\COM8");
    SET_COM("\\\\.\\COM9", "\\\\.\\COM9");
    SET_COM("\\\\.\\COM10", "\\\\.\\COM10");
    SET_COM("\\\\.\\COM11", "\\\\.\\COM11");
    SET_COM("\\\\.\\COM12", "\\\\.\\COM12");
    SET_COM("\\\\.\\COM13", "\\\\.\\COM13");
    SET_COM("\\\\.\\COM14", "\\\\.\\COM14");
    SET_COM("\\\\.\\COM15", "\\\\.\\COM15");
    SET_COM("\\\\.\\COM16", "\\\\.\\COM16");
    SET_COM("\\\\.\\COM17", "\\\\.\\COM17");
    SET_COM("\\\\.\\COM18", "\\\\.\\COM18");
    SET_COM("\\\\.\\COM19", "\\\\.\\COM19");
    SET_COM("\\\\.\\COM20", "\\\\.\\COM20");
    SET_COM("\\\\.\\COM21", "\\\\.\\COM21");
    SET_COM("\\\\.\\COM22", "\\\\.\\COM22");
    SET_COM("\\\\.\\COM23", "\\\\.\\COM23");
    SET_COM("\\\\.\\COM24", "\\\\.\\COM24");
    SET_COM("\\\\.\\COM25", "\\\\.\\COM25");
    SET_COM("\\\\.\\COM26", "\\\\.\\COM26");
    SET_COM("\\\\.\\COM27", "\\\\.\\COM27");
    SET_COM("\\\\.\\COM28", "\\\\.\\COM28");
    SET_COM("\\\\.\\COM29", "\\\\.\\COM29");
    SET_COM("\\\\.\\COM30", "\\\\.\\COM30");
    SET_COM("\\\\.\\COM31", "\\\\.\\COM31");
    SET_COM("\\\\.\\COM32", "\\\\.\\COM32");
    SET_COM("\\\\.\\COM33", "\\\\.\\COM33");
    SET_COM("\\\\.\\COM34", "\\\\.\\COM34");
    SET_COM("\\\\.\\COM35", "\\\\.\\COM35");
    SET_COM("\\\\.\\COM36", "\\\\.\\COM36");
    SET_COM("\\\\.\\COM37", "\\\\.\\COM37");
    SET_COM("\\\\.\\COM38", "\\\\.\\COM38");
    SET_COM("\\\\.\\COM39", "\\\\.\\COM39");
    SET_COM("\\\\.\\COM40", "\\\\.\\COM40");

    return PH_ERR_INVALID_PARAMETER;
}

phStatus_t np_NxpNfcRdLib_CheckFrontEndName(
    const char *szFeName, NPNxpNfcRdLibCtx_t *pRdCtx)
{
    pRdCtx->selectedHal = NPSelectedHal_Unknown;
    SET_FE("RC663", NPSelectedHal_RC663);
    SET_FE("CLRC663", NPSelectedHal_RC663);
    SET_FE("CL663", NPSelectedHal_RC663);

    return PH_ERR_INVALID_PARAMETER;
}
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/** Initialisze the rdlib stack */
phStatus_t np_NxpNfcRdLib_Init(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;

    //phOsal_Init();

#ifdef NXPBUILD__PHBAL_REG_DELEGATE
    PH_CHECK_SUCCESS_FCT(status, initBal_DelegateVCOM(pRdCtx));
#endif

    if (pRdCtx->selectedHal == NPSelectedHal_RC663) {
        PH_CHECK_SUCCESS_FCT(status, initHal_CLRC663(pRdCtx));
    }

    PH_CHECK_SUCCESS_FCT(status,
        phhalHw_ApplyProtocolSettings(
            pRdCtx->pHal, PHHAL_HW_CARDTYPE_ISO14443A));

    // PH_CHECK_SUCCESS_FCT(status, np_TypeA_Init(pRdCtx));

    return PH_ERR_SUCCESS;
}

phStatus_t np_NxpNfcRdLib_DeInit(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;

    PH_CHECK_SUCCESS_FCT(status,
        phhalHw_Rc663_DeInit(&pRdCtx->sHal_Rc663));

    return PH_ERR_SUCCESS;
}

/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */

static void printUsage(const char *exe_name)
{
    LOG_E("Error in usage of this example exe.\n");
    LOG_E("Usage:\n\t%s <COMPort> <RC663>\n", exe_name);
}

#ifdef NXPBUILD__PHBAL_REG_DELEGATE
static phStatus_t initBal_DelegateVCOM(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;
    np_Delegate_Bal_Init(&pRdCtx->sBalCtx);
    pRdCtx->pBal = (void *)&pRdCtx->sBalCtx.sBalDelegate;

    pRdCtx->sPlatformCtx.pSerialWin = &pRdCtx->sBalCtx.sBalSerialWin;
    np_Delegate_Platform_Init(&pRdCtx->sPlatformCtx);

    PH_CHECK_SUCCESS_FCT(status, phbalReg_OpenPort(pRdCtx->pBal));
    return status;
}

#endif

static phStatus_t initHal_CLRC663(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;
    PH_CHECK_SUCCESS_FCT(status,
        phbalReg_SetConfig(pRdCtx->pBal,
            PHHAL_HW_CONFIG_BAL_CONNECTION,
            PHHAL_HW_BAL_CONNECTION_SPI));

    PH_CHECK_SUCCESS_FCT(status,
        phhalHw_Rc663_Init(&pRdCtx->sHal_Rc663,
            (uint16_t)sizeof(pRdCtx->sHal_Rc663),
            pRdCtx->pBal,
            (uint8_t *)gkphhalHw_Rc663_LoadConfig,
            pRdCtx->gTxBuffer,
            NP_HAL_TX_BUF_SIZE,
            pRdCtx->gRxBuffer,
            NP_HAL_RX_BUF_SIZE));

    pRdCtx->pHal = &pRdCtx->sHal_Rc663;
    return status;
}

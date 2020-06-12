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
 * Delegates for handling of SPI Communicaiton.
 *
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
#include "np_Delegate_Bal.h"
#include <phbalReg.h>
#include <phbalReg_SerialWin.h>
#include "phUcBal.h"
#include "phUcBal_Protocol.h"
#include "phUcBal_TransReceive.h"
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

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

static phStatus_t __stdcall BAL_Delegate_OpenPort(void *pCtx);
static phStatus_t __stdcall BAL_Delegate_ClosePort(void *pCtx);
static phStatus_t __stdcall BAL_Delegate_Exchange(void *pCtx,
    uint16_t wOption,
    uint8_t *pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t *pRxBuffer,
    uint16_t *pRxLength);
static phStatus_t __stdcall BAL_Delegate_SetConfig(
    void *pCtx, uint16_t wConfig, uint16_t wValue);
static phStatus_t __stdcall BAL_Delegate_GetConfig(
    void *pCtx, uint16_t wConfig, uint16_t *pValue);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

int np_Delegate_Bal_Init(NPDelegateBalCtx_t *pBalCtx)
{
    phbalReg_Delegate_Init(&pBalCtx->sBalDelegate,
        sizeof(pBalCtx->sBalDelegate),
        pBalCtx,
        &BAL_Delegate_OpenPort,
        &BAL_Delegate_ClosePort,
        &BAL_Delegate_Exchange,
        &BAL_Delegate_SetConfig,
        &BAL_Delegate_GetConfig);
    return 0;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t __stdcall BAL_Delegate_OpenPort(void *pCtx)
{
    NPDelegateBalCtx_t *pBalCtx = (NPDelegateBalCtx_t *)pCtx;

    phStatus_t status = PH_ERR_SUCCESS;

    if (pBalCtx->sBalSerialWin.pComHandle == NULL) {
        PH_CHECK_SUCCESS_FCT(status,
            phbalReg_SerialWin_Init(&pBalCtx->sBalSerialWin,
                sizeof(pBalCtx->sBalSerialWin),
                sizeof(pBalCtx->aSerialWinBuffer),
                pBalCtx->aSerialWinBuffer));

        PH_CHECK_SUCCESS_FCT(status,
            phbalReg_SetPort(
                &pBalCtx->sBalSerialWin, (uint8_t *)pBalCtx->portName));

        PH_CHECK_SUCCESS_FCT(
            status, phbalReg_OpenPort(&pBalCtx->sBalSerialWin));

        PH_CHECK_SUCCESS_FCT(status,
            phbalReg_SetConfig(&pBalCtx->sBalSerialWin,
                PHBAL_REG_SERIALWIN_CONFIG_BITRATE,
                PHBAL_REG_SERIALWIN_VALUE_BITRATE_115200));
    }
    return status;
}

static phStatus_t __stdcall BAL_Delegate_ClosePort(void *pCtx)
{
    NPDelegateBalCtx_t *pBalCtx = (NPDelegateBalCtx_t *)pCtx;

    phStatus_t status = PH_ERR_SUCCESS;
    if (NULL != pBalCtx->sBalSerialWin.pComHandle)
        status = phbalReg_ClosePort(&pBalCtx->sBalSerialWin);

    return status;
}

static phStatus_t __stdcall BAL_Delegate_Exchange(void *pCtx,
    uint16_t wOption,
    uint8_t *pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t *pRxBuffer,
    uint16_t *pRxLength)
{
    NPDelegateBalCtx_t *pBalCtx = (NPDelegateBalCtx_t *)pCtx;

    phUcBal_Cmd_t cmd = {0};
    phUcBal_Rsp_t rsp = {0};
    cmd.header.CLA = CLA_TransReceive;
    if (wRxBufSize == 0) {
        cmd.header.INS = TransReceive_INS_Tx;
    }
    else if (wTxLength == 0) {
        cmd.header.INS = TransReceive_INS_Rx;
    }
    else {
        cmd.header.INS = TransReceive_INS_TRx;
    }
    cmd.header.Lc = wTxLength;
    cmd.header.P1 = (byte)wRxBufSize;
    cmd.header.P2 = (byte)(wRxBufSize >> 8);

    memcpy(cmd.header.Payload, pTxBuffer, wTxLength);

    phbalReg_Exchange(&pBalCtx->sBalSerialWin,
        0,
        cmd.buf,
        PH_UCBAL_PROTOCOL_HEADER_SIZE + wTxLength,
        PH_UCBAL_PROTOCOL_HEADER_SIZE + wRxBufSize,
        rsp.buf,
        pRxLength);
    if (*pRxLength >= PH_UCBAL_PROTOCOL_HEADER_SIZE) {
        *pRxLength -= PH_UCBAL_PROTOCOL_HEADER_SIZE;
    }
    if (*pRxLength > 0) {
        memcpy(pRxBuffer, rsp.header.Response, *pRxLength);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t __stdcall BAL_Delegate_SetConfig(
    void *pCtx, uint16_t wConfig, uint16_t wValue)
{
    NPDelegateBalCtx_t *pBalCtx = (NPDelegateBalCtx_t *)pCtx;
    phStatus_t status;

    if (PHHAL_HW_CONFIG_BAL_CONNECTION == wConfig) {
        pBalCtx->sBalDelegate.bBalType = (uint8_t)wValue;
        status = PH_ERR_SUCCESS;
    }
    else {
        status = phbalReg_SetConfig(&pBalCtx->sBalSerialWin, wConfig, wValue);
    }
    return status;
}

phStatus_t __stdcall BAL_Delegate_GetConfig(
    void *pCtx, uint16_t wConfig, uint16_t *pValue)
{
    NPDelegateBalCtx_t *pBalCtx = (NPDelegateBalCtx_t *)pCtx;

    phStatus_t status = PH_ERR_SUCCESS;

    if (PHHAL_HW_CONFIG_BAL_CONNECTION == wConfig) {
        *pValue = pBalCtx->sBalDelegate.bBalType;
        status = PH_ERR_SUCCESS;
    }
    else {
        status = phbalReg_GetConfig(&pBalCtx->sBalSerialWin, wConfig, pValue);
    }

    return status;
}

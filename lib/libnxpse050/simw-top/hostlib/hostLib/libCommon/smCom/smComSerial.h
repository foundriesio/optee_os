/**
 * @file smComSerial.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2020 NXP
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
 *
 *****************************************************************************/

#ifndef _SCCOMSERIAL_H_
#define _SCCOMSERIAL_H_

#include "smCom.h"

#ifdef __cplusplus
extern "C" {
#endif

U32 smComVCom_Open(void** vcom_ctx, const char *pComPortString);
U32 smComVCom_Close(void* conn_ctx);
U32 smComVCom_Transceive(void* conn_ctx, apdu_t *pApdu);
U32 smComVCom_TransceiveRaw(void* conn_ctx, U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen);

U32 smComVCom_SetState(void* conn_ctx);
U32 smComVCom_GetATR(void* conn_ctx, U8 *pAtr, U16 *atrLen);

#ifdef __cplusplus
}
#endif
#endif //_SCCOMSERIAL_H_

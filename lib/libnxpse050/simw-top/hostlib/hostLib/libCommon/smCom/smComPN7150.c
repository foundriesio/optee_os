/* Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef HOSTLIB_HOSTLIB_LIBCOMMON_SMCOM_SMCOMPN7150_C_
#define HOSTLIB_HOSTLIB_LIBCOMMON_SMCOM_SMCOMPN7150_C_

#ifdef SMCOM_PN7150

#include "smComPN7150.h"
#include "smCom.h"
#include "Nfc.h"
#include "sm_apdu.h"
#include <assert.h>

#define NX_LOG_ENABLE_SMCOM_DEBUG 1
#include "nxLog_smCom.h"
#include "nxEnsure.h"

static U32 smComPN7150_Transceive(apdu_t *pApdu);
static U32 smComPN7150_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen);

U16 smComPN7150_Open(U8 mode, U8 seqCnt, U8 *atr, U16 *atrLen)
{
    smCom_Init(&smComPN7150_Transceive, &smComPN7150_TransceiveRaw);
    return SMCOM_OK;
}

static U32 smComPN7150_Transceive(apdu_t *pApdu)
{
    U32 rxLen = MAX_APDU_BUF_LENGTH;
    U32 status = SMCOM_SND_FAILED;

    ENSURE_OR_GO_EXIT(pApdu != NULL);

    status = smComPN7150_TransceiveRaw(pApdu->pBuf, pApdu->buflen, pApdu->pBuf, &rxLen);

    pApdu->rxlen = rxLen;
    // reset offset for subsequent response parsing
    //pApdu->offset = 0;

exit:
    return status;
}
static U32 smComPN7150_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen)
{
    unsigned char RespSize;
    U32 rv = SMCOM_COM_FAILED;

    ENSURE_OR_GO_EXIT(pRxLen != NULL);

    RespSize = *pRxLen;
    LOG_MAU8_D("Tx>", pTx, txLen);
    bool status = NxpNci_ReaderTagCmd(pTx, txLen, pRx, &RespSize);
    *pRxLen = RespSize;
    LOG_MAU8_D("<Rx", pRx, RespSize);
    if (status == 0)
        return SMCOM_OK;
    else
        return SMCOM_COM_FAILED;
exit:
    return rv;
}

#endif /* SMCOM_PN7150 */

#endif /* HOSTLIB_HOSTLIB_LIBCOMMON_SMCOM_SMCOMPN7150_C_ */

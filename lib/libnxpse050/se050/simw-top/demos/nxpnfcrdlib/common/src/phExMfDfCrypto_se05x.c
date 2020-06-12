/**
 * @file phExMfDfCrypto_se05x.c
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
 * File has functions for nfc desfire EV2 crypto implementation using SE05x
 */

#include <phExMfDfCrypto.h>
#include <se05x_APDU.h>
#include <phApp_Init.h>
#include <phCryptoSym.h>
#include <ph_Status.h>


#include "phalMfdfEv2_Int.h"
#include "phalMfdfEv2_Sw_Int.h"
#include "phalMfdfEv2_Sw.h"
#include <phCryptoRng.h>
#include <phalMfdfEv2.h>
#include <phalVca.h>
#include "ex_sss.h"
#include "phTools.h"
#include "nxLog_App.h"
#include "phExMfDfCrypto_se05x.h"
#include "se05x_MfDfInit.h"



extern uint8_t bAfcApp[3];


/* *****************************************************************************************************************
* Private Functions Prototypes
* ***************************************************************************************************************** */
static phStatus_t loadSessionKey(sss_se05x_session_t * pSession,
    phalMfdfEv2_Sw_DataParams_t * pDataParams, uint8_t bKeyNoCard);



static phStatus_t phalMfdfEv2_AuthenticateEv2_Se05x(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2,
    sss_se05x_session_t * pSession,
    uint8_t bFirstAuth,
    uint16_t wOption,
    uint32_t dwSe050KeyUID,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen,
    uint8_t bLenPcdCapsIn,
    uint8_t *pPcdCapsIn,
    uint8_t *pPcdCapsOut,
    uint8_t *pPdCapsOut
);

static phStatus_t phalMfdfEv2_ChangeKeyEv2_Se05x(
    sss_se05x_session_t * pSession,
    phalMfdfEv2_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint32_t dwSe050OldKeyUID,
    uint16_t wOldKeyVer,
    uint32_t dwSe050NewKeyUID,
    uint16_t wNewKeyVer,
    uint8_t bKeySetNo,
    uint8_t bKeyNoCardAuth,
    uint8_t bKeyNoCardChange,
    uint8_t * pDivInput,
    uint8_t bDivLen
);

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

phStatus_t phEx_Auth_Se05xEV2(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2, sss_se05x_session_t * pSession, uint32_t objId, uint16_t wOption, uint8_t bKeyNoCard)
{
    phStatus_t status;
    uint8_t bLenPcdCapsIn =
        6; /**< [In] Length of PcdCapsIn. Always zero for following authentication */

    /* clang-format off */
    uint8_t bPcdCapsIn[6] = { 0x00,  0x00, 0x00, 0x00, 0x00, 0x00
                    };   /**< [In] PCD Capabilities. Upto 6 bytes. */
   /* clang-format on */

    uint8_t bPcdCapsOut[6]; /**< [Out] PCD Capabilities. 6 bytes. */
    uint8_t bPdCapsOut[6];  /**< [Out] PD Capabilities. 6 bytes. */


    /* Select the AFC Application */
    status = phalMfdfEv2_SelectApplication(pAlMfdfEv2,
        0x00, /* [In] bOption-0x00 for single and 0x01 for second application */
        bAfcApp, /* [In] 3 byte AID, LSB First */
        0x00); /* [In] Second AID...NA in this case; we do not need this as bOption is 0x00 */
    CHECK_SUCCESS(status);
    LOG_I("Select the AFC Application Successful\n");

    LOG_I("attempting to authenticate with cardkey = %d and Se0Obj ID = %d", bKeyNoCard, objId);


    /* EV2 First Authenticate */
    status = phalMfdfEv2_AuthenticateEv2_Se05x(
        pAlMfdfEv2, /* [In] Pointer to parameters data structure */
        pSession,
        0x01,       /* first authentication */
        PHAL_MFDFEV2_NO_DIVERSIFICATION, /* [In] Diversification option */
        objId,                                   /*dwSe050KeyUID*/
        bKeyNoCard,                            /* [In] Key number on Card */
        NULL,                            /* [In] Diversification input */
        0,                               /* [In] Diversification input length */
        bLenPcdCapsIn, /* [In] Length of PcdCapsIn. Always zero for following authentication */
        bPcdCapsIn,  /* [In] PCD Capabilities. Upto 6 bytes. */
        bPcdCapsOut, /* [Out] PCD Capabilities. 6 bytes. */
        bPdCapsOut); /* [Out] PD Capabilities. 6 bytes. */

    CHECK_SUCCESS(status);
    LOG_I(" EV2 First Authenticate  Successful\n");


    bLenPcdCapsIn = 0;
    /* EV2 Following Authenticate */
    status = phalMfdfEv2_AuthenticateEv2_Se05x(
        pAlMfdfEv2, /* [In] Pointer to parameters data structure */
        pSession,
        0x00,       /* Nonfirst Authentication*/
        PHAL_MFDFEV2_NO_DIVERSIFICATION, /* [In] Diversification option */
        objId,                                   /*dwSe050KeyUID*/
        bKeyNoCard,                            /* [In] Key number on Card */
        NULL,                            /* [In] Diversification input */
        0,                               /* [In] Diversification input length */
        bLenPcdCapsIn, /* [In] Length of PcdCapsIn. Always zero for following authentication */
        bPcdCapsIn,  /* [In] PCD Capabilities. Upto 6 bytes. */
        bPcdCapsOut, /* [Out] PCD Capabilities. 6 bytes. */
        bPdCapsOut); /* [Out] PD Capabilities. 6 bytes. */
    CHECK_SUCCESS(status);
    LOG_I(" EV2 Following Authenticate  Successful\n");
    LOG_I("Authenticated with cardkey = %d and Se0Obj ID = %d", bKeyNoCard, objId);

    return status;
}


phStatus_t phEx_ChangeKey_Se05xEV2(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2,
    sss_se05x_session_t * pSession,
    uint32_t se05x_Auth_keyid,
    uint32_t se05x_Change_Currentkeyid,
    uint32_t se05x_Change_derivedkeyid,
    uint16_t wOption,
    uint8_t bKeyNoCardAuth,
    uint8_t bKeyNoCardChange,
    uint8_t bKeyVerCardChange,
    uint8_t * pDivInput,
    uint8_t bDivLen
)
{
    phStatus_t status;

    status = phEx_Auth_Se05xEV2(pAlMfdfEv2,
        pSession,
        se05x_Auth_keyid,
        PHAL_MFDFEV2_NO_DIVERSIFICATION,
        bKeyNoCardAuth);
    CHECK_SUCCESS(status);

    LOG_I("attempting to change cardkey = %d from  Old Se050ObjID= %d to new Se050ObjID= %d ", bKeyNoCardChange, se05x_Change_Currentkeyid, se05x_Change_derivedkeyid);

    status = phalMfdfEv2_ChangeKeyEv2_Se05x(
    pSession,
    pAlMfdfEv2,
    wOption,
    se05x_Change_Currentkeyid,      //dwSe050OldKeyUID
    0x00,       //wOldKeyVer
    se05x_Change_derivedkeyid,   //dwSe050NewKeyUID
    bKeyVerCardChange,              //wNewKeyVer
    0,              //bKeySetNo
    bKeyNoCardAuth,             //bKeyNoCardAuth
    bKeyNoCardChange,           //bKeyNoCardChange
    pDivInput,
    bDivLen
);
    CHECK_SUCCESS(status);
    LOG_I(" Change Key for card key %d is Successful to Se050ObjID= %d \n", bKeyNoCardChange, se05x_Change_derivedkeyid);
    return status;
}

phStatus_t phEx_KillAuth_Se05xEV2( sss_se05x_session_t * pSession, phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2, uint8_t bCard)
{
    smStatus_t smStatus;
    phStatus_t status;
    status = phalMfdfEv2_Sw_ResetAuthentication(pAlMfdfEv2);
    CHECK_SUCCESS(status);
    LOG_I(" Auth session is reset in software ");
    smStatus = Se05x_API_DFKillAuthentication(&pSession->s_ctx);
    if (smStatus != SM_OK) {
        LOG_E("SE050 failed to kill the authentication");
        return PH_ERR_INTERNAL_ERROR;
    }
    LOG_I(" Auth session is killed in SE ");
    return PH_ERR_SUCCESS;
}



/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */

static phStatus_t phalMfdfEv2_AuthenticateEv2_Se05x(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2,
    sss_se05x_session_t * pSession,
    uint8_t bFirstAuth,
    uint16_t wOption,
    uint32_t dwSe050KeyUID,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen,
    uint8_t bLenPcdCapsIn,
    uint8_t *pPcdCapsIn,
    uint8_t *pPcdCapsOut,
    uint8_t *pPdCapsOut
)
{
    phStatus_t rdlibStatus = PH_ERR_INTERNAL_ERROR;
    smStatus_t    smStatus;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phalMfdfEv2_Sw_DataParams_t * pDataParams = pAlMfdfEv2;
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE + 1U];
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[60U];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bRndLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    uint8_t *   PH_MEMLOC_REM pRecv = NULL;

    uint8_t lEKx_RandAB[32];
    size_t lEKx_RandABLen = 32;

    /* Reset the states and buffers in case. */
    if(0U != (bFirstAuth))
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfEv2_Sw_ResetAuthentication(pDataParams));

    /* Set the current authentication status to NOT AUTHENTICATED i.e., invalid key number */
    pDataParams->bKeyNo = 0xFF;


    /********************************************************************************/
    /* Step-1    Send the cmd and receive the encrypted RndB from the Card */
    /********************************************************************************/
    if (0U != (bFirstAuth))
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFEV2_CMD_AUTHENTICATE_EV2_FIRST;
    }
    else
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFEV2_CMD_AUTHENTICATE_EV2_NON_FIRST;
    }
    bCmdBuff[wCmdLen++] = bKeyNoCard; /* key number card */
    if (0U != (bFirstAuth))
    {
        /* Maximum frame size of card is 64 bytes, so the data should be of max 57 bytes */
        bLenPcdCapsIn = (bLenPcdCapsIn > 57U) ? 57 : bLenPcdCapsIn;

        bCmdBuff[wCmdLen++] = bLenPcdCapsIn; /* PCD Caps In length */
                                             /* PCD Caps In */
        (void)memcpy(&bCmdBuff[wCmdLen], pPcdCapsIn, bLenPcdCapsIn);
        wCmdLen += bLenPcdCapsIn;
    }

    rdlibStatus = phalMfdfEv2_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
    );
    if ((rdlibStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        if (pDataParams->bAuthMode != PHAL_MFDFEV2_NOT_AUTHENTICATED)
        {
            phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
        }

        if ((rdlibStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
        }
        else
        {
            return rdlibStatus;
        }
    }
    if (wRxlen != bRndLen)
    {
        phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
    }

    /* Store the unencrypted RndB */
    (void)memcpy(bRndB, pRecv, bRndLen);

    LOG_MAU8_I("\n CARD =====> SE050   16-byte Ek(RndB)  = \n", bRndB, bRndLen);

    /********************************************************************************/
    /*END  of Step-1    Send the cmd and receive the encrypted RndB from the Card */
    /********************************************************************************/

    /********************************************************************************/
    /*Step 2 Do a first auth from SE050*/
    /********************************************************************************/
    if (0U != (bFirstAuth))
    {
        smStatus = Se05x_API_DFAuthenticateFirstPart1(
            &(pSession->s_ctx),
            dwSe050KeyUID,
            bRndB,
            bRndLen,
            lEKx_RandAB,
            &lEKx_RandABLen);
    }
    else
    {
        smStatus = Se05x_API_DFAuthenticateNonFirstPart1(
            &(pSession->s_ctx),
            dwSe050KeyUID,
            bRndB,
            bRndLen,
            lEKx_RandAB,
            &lEKx_RandABLen);
    }


        if (smStatus != SM_OK) {
           LOG_E(" Auth First/Non-First failed");
            return PH_ERR_INTERNAL_ERROR;
        }

        LOG_MAU8_I("\n  CARD <====== SE050  E(Kx, RandA || RandB') = \n", lEKx_RandAB, lEKx_RandABLen);

    /********************************************************************************/
    /*end of Step2 Do a first auth form SE050*/
    /********************************************************************************/

    bCmdBuff[0] = PHAL_MFDFEV2_RESP_ADDITIONAL_FRAME;
    wCmdLen = (uint16_t)(lEKx_RandABLen + 1U);
    (void)memcpy(&bCmdBuff[1], lEKx_RandAB, lEKx_RandABLen);

    /* Get the encrypted TI || RndA' || PDCaps || PCDCaps into bWorkBuffer */
    PH_CHECK_SUCCESS_FCT(rdlibStatus, phalMfdfEv2_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
    ));

    LOG_MAU8_I("\n  CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) = \n", pRecv, wRxlen);

    /* If First Auth, then 32 bytes of data is obtained after exchange */
    if (0U != (bFirstAuth))
    {
        if (wRxlen != (2u * bRndLen))
        {
            phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
        }

        lEKx_RandABLen = 0x12;
        smStatus = Se05x_API_DFAuthenticateFirstPart2(
            &(pSession->s_ctx),
            pRecv,
            wRxlen,
            lEKx_RandAB,
            &lEKx_RandABLen);

        if (smStatus != SM_OK) {
           LOG_E(" Auth First Part 2 failed");
            return PH_ERR_INTERNAL_ERROR;
        }

        LOG_MAU8_I("\n  CARD <====== SE050  E(Kx, RandA || RandB') = \n", lEKx_RandAB, lEKx_RandABLen);
    }
    else   /*Auth is Non First*/
    {
        /* If Auth is Non First, then 16 bytes of data is expected */
        if (wRxlen != bRndLen)
        {
            phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
        }

        smStatus = Se05x_API_DFAuthenticateNonFirstPart2(
            &(pSession->s_ctx),
            pRecv,
            wRxlen);

        if (smStatus != SM_OK) {
           LOG_E(" Auth Non-First Part 2 failed");
           return PH_ERR_INTERNAL_ERROR;
        }
    }

    rdlibStatus = loadSessionKey(pSession, pDataParams, bKeyNoCard);
    return rdlibStatus;
}


static phStatus_t phalMfdfEv2_ChangeKeyEv2_Se05x(
    sss_se05x_session_t * pSession,
    phalMfdfEv2_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint32_t dwSe050OldKeyUID,
    uint16_t wOldKeyVer,
    uint32_t dwSe050NewKeyUID,
    uint16_t wNewKeyVer,
    uint8_t bKeySetNo,
    uint8_t bKeyNoCardAuth,
    uint8_t bKeyNoCardChange,
    uint8_t * pDivInput,
    uint8_t bDivLen
)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[51];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[16];

    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

#ifdef RDR_LIB_PARAM_CHECK
    uint8_t     PH_MEMLOC_REM bAppId[3] = { 0x00, 0x00, 0x00 };
#endif
    uint8_t *   PH_MEMLOC_REM pRecv = NULL;

    smStatus_t smStatus;


#ifdef RDR_LIB_PARAM_CHECK
    /* Change key should also take care of changing other keys at PICC level like
    * the Proximity check key,
    * VCA keys
    * Transaction MAC key
    *
    */
    if (memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
    {
        /* Only if seleted Aid is 0x000000, PICC level key change is targeted. */
        if (((bKeyNoCardChange & 0x3FU) != 0x00U) &&                                                                        /* PICC Master key */
            ((bKeyNoCardChange & 0x3FU) != 0x10U) && ((bKeyNoCardChange & 0x3FU) != 0x11U) && ((bKeyNoCardChange & 0x3FU) != 0x12U) &&      /* PICC DAMAuthKey, DAMMACKey, DAMEncKey */
            ((bKeyNoCardChange & 0x3FU) != 0x20U) && ((bKeyNoCardChange & 0x3FU) != 0x21U) && ((bKeyNoCardChange & 0x3FU) != 0x22U) && ((bKeyNoCardChange & 0x3FU) != 0x23U) &&   /* PICC VCConfigurationKey, VCProximityKey, VCSelectMACKey, VCSelectENCKey */
            ((bKeyNoCardChange & 0x3FU) != 0x30U) && ((bKeyNoCardChange & 0x3FU) != 0x31U))                                                           /* PICC VCPollingEncKey, VCPollingMACKey */
        {
            /* Invalid card key number supplied */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFEV2);
        }
        else if (((bKeyNoCardChange & 0xC0U) != 0x80U) && ((bKeyNoCardChange & 0xC0U) != 0x40U) && ((bKeyNoCardChange & 0xC0U) != 0x00U))
        {
            /* Invalid card key number supplied */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFEV2);
        }
        else
        {
            /*Do Nothing. This is for PRQA compliance */
        }
    }
    else
    {
        if ((bKeyNoCardChange & 0x7fU) > 0x0D
            && ((bKeyNoCardChange & 0x3FU) != 0x21U) && ((bKeyNoCardChange & 0x3FU) != 0x22U) && ((bKeyNoCardChange & 0x3FU) != 0x23U))
        {
            /* Invalid application key specified */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFEV2);
        }
    }
    if (bKeySetNo > 0x0FU)
    {
        /* Invalid KeySetNo specified */
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFEV2);
    }

    if (pDataParams->bAuthMode == PHAL_MFDFEV2_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDFEV2);
    }
#endif  /*RDR_LIB_PARAM_CHECK*/


    if ((wOption != PHAL_MFDFEV2_NO_DIVERSIFICATION))
    {
        smStatus = Se05x_API_DFDiversifyKey(&(pSession->s_ctx),
            dwSe050OldKeyUID,
            dwSe050NewKeyUID,
            pDivInput,
            bDivLen);
        if (smStatus != SM_OK) {
           LOG_E(" Diversify key failed");
           return PH_ERR_INTERNAL_ERROR;
        }
    }

    if (bKeyNoCardAuth == bKeyNoCardChange) {
        dwSe050OldKeyUID = SE05x_KeyID_MFDF_NONE;
    }

    (void)memset(bWorkBuffer, 0x00, 16);
    (void)memset(bCmdBuff, 0x00, 51);
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFEV2_CMD_CHANGE_KEY_EV2;
    bCmdBuff[wCmdLen++] = bKeySetNo;
    bCmdBuff[wCmdLen++] = bKeyNoCardChange;


       bKeySetNo = 0;
        size_t szCmdLen = 46;

        smStatus = Se05x_API_DFChangeKeyPart1(
            &(pSession->s_ctx),
            dwSe050OldKeyUID,
            dwSe050NewKeyUID,
            bKeySetNo,
            bKeyNoCardChange,
            (uint8_t)wNewKeyVer,
            &bCmdBuff[3],
            &szCmdLen);

        if (smStatus != SM_OK) {
           LOG_E(" Change key part 1 failed");
            return PH_ERR_INTERNAL_ERROR;
        }

        wCmdLen = (uint16_t)szCmdLen + 3;

         /* Send the command */
        statusTmp = phalMfdfEv2_ExchangeCmd(
            pDataParams,
            pDataParams->pPalMifareDataParams,
            pDataParams->bWrappedMode,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
        );
        if (statusTmp != PH_ERR_SUCCESS)
        {
            if (pDataParams->bAuthMode != PHAL_MFDFEV2_AUTHENTICATE)
            {
                phalMfdfEv2_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return statusTmp;
        }

        /*Changing of Auth Key, Se05x_API_DFChangeKeyPart2 not required*/
        if (wRxlen == 0U)
        {
            /* Increment the command counter. */
            pDataParams->wCmdCtr++;

            return PH_ERR_SUCCESS;
        }
        /* Max 8 byte CMAC is expected nothing more. */
        else if (wRxlen != 8U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
        }
        /* Verification of MAC also required for AuthEV2 */
        else
        {
            uint8_t result;

            /* Increment the command counter. */
            pDataParams->wCmdCtr++;

            smStatus = Se05x_API_DFChangeKeyPart2(&(pSession->s_ctx), pRecv /*bWorkBuffer*/, wRxlen, &result);
            if (smStatus != SM_OK) {

                phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);

            }
            if (result != kSE05x_Result_SUCCESS)
            {
                return PH_ERR_AUTH_ERROR;
            }

        }

        return PH_ERR_SUCCESS;
}





static phStatus_t loadSessionKey(sss_se05x_session_t * pSession,
    phalMfdfEv2_Sw_DataParams_t * pDataParams, uint8_t bKeyNoCard)
{
    smStatus_t smStatus;
    phStatus_t rdlibStatus = PH_ERR_SUCCESS;

    uint8_t bDumpKKeyRsp[38];
    size_t szDumpKeyRsp = 38;
    //Get the Session Keys
    smStatus = Se05x_API_DFDumpSessionKeys(
        &(pSession->s_ctx),
        bDumpKKeyRsp,
        &szDumpKeyRsp);
    if (smStatus != SM_OK) {
        phalMfdfEv2_Sw_ResetAuthentication(pDataParams);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFEV2);
    }

    /*Store the Dumped Session Key and related parameters to EV2 Structure*/
    (void)memcpy(&(pDataParams->bSesAuthENCKey[0]), &bDumpKKeyRsp[0], 16);
    (void)memcpy(&(pDataParams->bSesAuthMACKey[0]), &bDumpKKeyRsp[16], 16);

    /*4 Bytes of Transaction Identifier*/
    (void)memcpy(&(pDataParams->bTi[0]), &bDumpKKeyRsp[32], 4);
    /*Unsigned short comand counter*/
    pDataParams->wCmdCtr = (bDumpKKeyRsp[37] << 8) + bDumpKKeyRsp[36];

    LOG_MAU8_I("Dumped Session Key is", pDataParams->bSesAuthENCKey, 16);
    LOG_MAU8_I("Dumped Session Mac is", pDataParams->bSesAuthMACKey, 16);
    LOG_MAU8_I("Dumped TI is", pDataParams->bTi, 4);
    LOG_U16_I(pDataParams->wCmdCtr);


    pDataParams->bAuthMode = PHAL_MFDFEV2_AUTHENTICATEEV2;
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_AES128;
    pDataParams->bKeyNo = bKeyNoCard;

    /* Load the ENC session key */
    PH_CHECK_SUCCESS_FCT(rdlibStatus, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSesAuthENCKey,
        pDataParams->bCryptoMethod
    ));

    /*In Sw Implementation The IV is same as Mac. So the same is being done here*/
    /* Load IV */
    PH_CHECK_SUCCESS_FCT(rdlibStatus, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        &bDumpKKeyRsp[16], 16));


    /* Update the authentication state if VCA PC feature is required by the application. */
    if (pDataParams->pVCADataParams != NULL)
    {
        /* Set the Session key and IV for Virtual Card which is valid for this authentication */
        PH_CHECK_SUCCESS_FCT(rdlibStatus, phalVca_SetSessionKeyUtility(
            (phalVca_Sw_DataParams_t *)pDataParams->pVCADataParams,
            pDataParams->bSesAuthMACKey,
            pDataParams->bAuthMode
        ));
    }

    /* Load the MAC session key */
    PH_CHECK_SUCCESS_FCT(rdlibStatus, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsMac,
        pDataParams->bSesAuthMACKey,
        pDataParams->bCryptoMethod
    ));

    /* Set the keep Iv ON */
    PH_CHECK_SUCCESS_FCT(rdlibStatus, phCryptoSym_SetConfig(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        PH_CRYPTOSYM_VALUE_KEEP_IV_ON
    ));
    return rdlibStatus;
}

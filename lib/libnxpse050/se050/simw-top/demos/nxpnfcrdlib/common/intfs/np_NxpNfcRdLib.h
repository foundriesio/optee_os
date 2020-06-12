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
 * Over all management of RdLib Stack and Stack initializaiton from PC
 *
 * $Date$
 * $Author$
 * $Revision$
 */

#ifndef INC_NP_NXPNFCRDLIB_H_
#define INC_NP_NXPNFCRDLIB_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#ifdef __cplusplus
extern "C" {
#endif

#include <ph_TypeDefs.h>
#include <phNfcLib.h>
#include <phbalReg.h>

#ifdef NXPBUILD__PH_PLATFORM_DELEGATE
#include <phPlatform_Delegate.h>
#endif
#include <phhalHw.h>
#include "np_Delegate_Bal.h"
#include "np_Delegate_Platform.h"
#include "phTMIUtils.h"
#include "phalMfdfEv2.h"
#include "phalVca.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
/* NFCLIB Header */

#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define NP_HAL_TX_BUF_SIZE 256
#define NP_HAL_RX_BUF_SIZE 256

#define LOG_BUFFER(TEXT, BUF, LEN)   \
    {                                \
        int i;                       \
        printf("%s", TEXT);          \
        for (i = 0; i < LEN; i++) {  \
            printf(" %02X", BUF[i]); \
        }                            \
        printf("\n");                \
    }

#undef PH_CHECK_SUCCESS_FCT

#define PH_CHECK_SUCCESS_FCT(status, fct)                      \
    {                                                          \
        (status) = (fct);                                      \
        {                                                      \
            if ((status) != PH_ERR_SUCCESS) {                  \
                LOG_E(                                        \
                    "\n!Error: \"%s\" failed.\n\tstatus = "    \
                    "0x%04X\n\tFunction:%s. Line:%d. File %s", \
                    #fct,                                      \
                    status,                                    \
                    __FUNCTION__,                              \
                    __LINE__,                                  \
                    __FILE__);                                 \
                return (status);                               \
            }                                                  \
        }                                                      \
    }

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
enum NPSelectedHal
{
    NPSelectedHal_Unknown,
    NPSelectedHal_RC663,
    NPSelectedHal_LAST,
};

typedef struct
{
#ifdef NXPBUILD__PHBAL_REG_DELEGATE
    NPDelegateBalCtx_t sBalCtx;
#endif
    void *pBal;
#ifdef NXPBUILD__PH_PLATFORM_DELEGATE
    NPPlatformCtx_t sPlatformCtx;
#endif

    uint8_t gTxBuffer[NP_HAL_TX_BUF_SIZE];
    uint8_t gRxBuffer[NP_HAL_RX_BUF_SIZE];

    phhalHw_Rc663_DataParams_t sHal_Rc663;

    void *pHal;
    enum NPSelectedHal selectedHal;
    //uint8_t pLoadRegConfig[1];

    phpalI14443p3a_Sw_DataParams_t sType3APal;
    phpalI14443p4_Sw_DataParams_t sType4Pal;
    phpalI14443p4a_Sw_DataParams_t sType4APal;

    phpalMifare_Sw_DataParams_t sPalMifare;
    phKeyStore_Sw_DataParams_t sKeyStore;

    phCryptoSym_Sw_DataParams_t sCryptoEnc;
    phCryptoSym_Sw_DataParams_t sCryptoSymRnd;
    phCryptoSym_Sw_DataParams_t sCryptoMac;
    phCryptoRng_Sw_DataParams_t sCryptoRng; /*  Crypto Rng data-params */

    phalMfdfEv2_Sw_DataParams_t salMfdfEv2; /*   AL MFDFEV2 data-params */
    phTMIUtils_t sTMIDataParams;
    phalVca_Sw_DataParams_t salVca; /**< VCA Software parameter structure */

} NPNxpNfcRdLibCtx_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/**
 * Number of command line parameters are OK?
 * @param argc Number of commandline arguments.
 * @return #PH_ERR_SUCCESS if everything is fine.
 */
phStatus_t np_NxpNfcRdLib_CheckUsage(int argc, const char *exe_name);

#ifdef NXPBUILD__PHBAL_REG_DELEGATE

/**
 * Is the COM Port selection valid
 * @param [IN]szComPort String name of com port
 * @param [OUT] pBalCtx The COMPort name is set in this context
 * @return #PH_ERR_SUCCESS if everything is fine
 */
phStatus_t np_NxpNfcRdLib_CheckComPortName(
    const char *szComPort, NPDelegateBalCtx_t *pBalCtx);
#endif

/**
 * Is the selection of IC FrontEnd OK
 * @param [IN] szFeName Fronted Name. e.g. PN5180, 5180, RC663, CLRRC663, etc.
 * @param [OUT] pRdCtx The Front type is set in this context
 * @return #PH_ERR_SUCCESS if everything is fine
 */
phStatus_t np_NxpNfcRdLib_CheckFrontEndName(
    const char *szFeName, NPNxpNfcRdLibCtx_t *pRdCtx);

/** Initialisze the rdlib stack */
phStatus_t np_NxpNfcRdLib_Init(NPNxpNfcRdLibCtx_t *pRdCtx);

/*De-Init*/
phStatus_t np_NxpNfcRdLib_DeInit(NPNxpNfcRdLibCtx_t *pRdCtx);

#endif /* INC_NP_NXPNFCRDLIB_H_ */

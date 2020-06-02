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
 * MFDF Functionality for the example
 *
 * $Date$
 * $Author$
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <stdio.h>
#include "np_NxpNfcRdLib.h"
#include "phApp_Init.h"
#include "phCryptoRng.h"
#include "phExMfDfCrypto.h"
#include "ph_Status.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "nxLog_App.h"

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

phalVca_Sw_IidTableEntry_t aIidTableStorage
    [IID_KEY_COUNT]; /**< Pointer to the Iid Table storage for the layer. */
phalVca_Sw_CardTableEntry_t aCardTableStorage
    [IID_KEY_COUNT]; /**< Pointer to the Card Table storage for the layer. */

uint16_t wNumIidTableStorageEntries =
    IID_KEY_COUNT; /**< Number of possible Iid table entries in the storage. */
uint16_t wNumCardTableStorageEntries =
    IID_KEY_COUNT; /**< Number of possible Card table entries in the storage. */

uint8_t bTMIBuffer[TMI_BUFFER_SIZE] = {0};

uint8_t pSeed[8] = {0};
extern phStatus_t StoreKeysInKeyStore(phKeyStore_Sw_DataParams_t *pKeyStore);
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t np_mfdf_Crypto_Init(NPNxpNfcRdLibCtx_t *pRdCtx);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t np_mfdf_Init(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;
    PH_CHECK_SUCCESS_FCT(status,
        phpalI14443p3a_Sw_Init(
            &pRdCtx->sType3APal, sizeof(pRdCtx->sType3APal), pRdCtx->pHal));
    PH_CHECK_SUCCESS_FCT(status,
        phpalI14443p4a_Sw_Init(
            &pRdCtx->sType4APal, sizeof(pRdCtx->sType4APal), pRdCtx->pHal));
    PH_CHECK_SUCCESS_FCT(status,
        phpalI14443p4_Sw_Init(
            &pRdCtx->sType4Pal, sizeof(pRdCtx->sType4Pal), pRdCtx->pHal));
    PH_CHECK_SUCCESS_FCT(status,
        phpalMifare_Sw_Init(&pRdCtx->sPalMifare,
            sizeof(pRdCtx->sPalMifare),
            pRdCtx->pHal,
            &pRdCtx->sType4Pal));
    PH_CHECK_SUCCESS_FCT(status, np_mfdf_Crypto_Init(pRdCtx));
    return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t np_mfdf_Crypto_Init(NPNxpNfcRdLibCtx_t *pRdCtx)
{
    phStatus_t status;
    uint16_t wNoOfKeyEntries = PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES;
    uint16_t wNoOfKeyVersionPairs =
        PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYVERSIONPAIRS;
    uint16_t wNoOfKUCEntries = PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KUCENTRIES;

    /** MIFARE key store init */
    status = phKeyStore_Sw_Init(&pRdCtx->sKeyStore,
        sizeof(phKeyStore_Sw_DataParams_t),
        pKeyEntries,
        wNoOfKeyEntries,
        pKeyVersionPairs,
        wNoOfKeyVersionPairs,
        pKUCEntries,
        wNoOfKUCEntries);
    if (status != PH_ERR_SUCCESS) {
        LOG_E("Keystore init not successful\n");
        return status;
    }

    /** CryptoSym init */
    status = phCryptoSym_Sw_Init(&pRdCtx->sCryptoEnc,
        sizeof(phCryptoSym_Sw_DataParams_t),
        (phKeyStore_Sw_DataParams_t *)(&pRdCtx->sKeyStore));
    if (status != PH_ERR_SUCCESS) {
        LOG_E("pCryptoEnc init not successful\n");
        return status;
    }

    status = phCryptoSym_Sw_Init(&pRdCtx->sCryptoMac,
        sizeof(phCryptoSym_Sw_DataParams_t),
        (phKeyStore_Sw_DataParams_t *)(&pRdCtx->sKeyStore));
    if (status != PH_ERR_SUCCESS) {
        LOG_E("cryptoMac init not successful\n");
        return status;
    }
    status = phCryptoSym_Sw_Init(&pRdCtx->sCryptoSymRnd,
        sizeof(phCryptoSym_Sw_DataParams_t),
        (phKeyStore_Sw_DataParams_t *)(&pRdCtx->sKeyStore));
    if (status != PH_ERR_SUCCESS) {
        LOG_E("CryptoSymRnd init not successful\n");
        return status;
    }

    /** CryptoRng init */
    status = phCryptoRng_Sw_Init(&pRdCtx->sCryptoRng,
        sizeof(phCryptoRng_Sw_DataParams_t),
        (phCryptoSym_Sw_DataParams_t *)(&pRdCtx->sCryptoSymRnd));
    if (status != PH_ERR_SUCCESS) {
        LOG_E("CryptoRng init not successful\n");
       return status;
    }

    PH_CHECK_SUCCESS_FCT(
        status, phCryptoRng_Seed(&pRdCtx->sCryptoRng, pSeed, 8));
    //    CHECK_SUCCESS(status);

    PH_CHECK_SUCCESS_FCT(status,
        phTMIUtils_Init(
            &pRdCtx->sTMIDataParams, &bTMIBuffer[0], TMI_BUFFER_SIZE));
    //    CHECK_SUCCESS(status);

    /* Initialize the VCA component */
    status = phalVca_Sw_Init(&pRdCtx->salVca,
        sizeof(
            pRdCtx
                ->salVca), /**< [In] Specifies the size of the data parameter structure */
        &pRdCtx
             ->sPalMifare, /**< [In] Pointer to a spalMifare component context. */
        &pRdCtx
             ->sKeyStore, /**< [In] Pointer to the parameter structure of the KeyStore component. */
        &pRdCtx
             ->sCryptoEnc, /**< [In] Pointer to a Crypto component context for encryption. */
        &pRdCtx
             ->sCryptoRng, /**< [In] Pointer to a CryptoRng component context. */
        aIidTableStorage, /**< [In] Pointer to the Iid Table storage for the layer. */
        wNumIidTableStorageEntries, /**< [In] Number of possible Iid table entries in the storage. */
        aCardTableStorage, /**< [In] Pointer to the Card Table storage for the layer. */
        wNumCardTableStorageEntries /**< [In] Number of possible Card table entries in the storage. */
    );
    CHECK_SUCCESS(status);

    /* Initialize the MF DesFire EV2 component */
    status = phalMfdfEv2_Sw_Init(
        &pRdCtx
             ->salMfdfEv2, /* [In] Pointer to this layer's parameter structure. */
        sizeof(
            phalMfdfEv2_Sw_DataParams_t), /* [In] Specifies the size of the data parameter structure */
        &pRdCtx
             ->sPalMifare, /* [In] Pointer to a spalMifare component context. */
        &pRdCtx->sKeyStore, /* [In] Pointer to Key Store data parameters. */
        &pRdCtx
             ->sCryptoEnc, /* [In] Pointer to a Crypto component context for encryption. */
        &pRdCtx->sCryptoMac, /* [In] Pointer to a CryptoMAC context */
        &pRdCtx
             ->sCryptoRng, /* [In] Pointer to a CryptoRng component context. */
        &pRdCtx->sTMIDataParams, /* [In] Pointer to TMIUtils dataparams */
        &pRdCtx->salVca,         /* [In] Pointer to Virtual Card dataparams */
        pRdCtx->pHal); /* [In] Pointer to the HAL parameters structure. */
    CHECK_SUCCESS(status);

    status = phalVca_SetApplicationType(&pRdCtx->salVca, &pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);

    /* Set all the keys in the S/W key store */
    status = StoreKeysInKeyStore(&pRdCtx->sKeyStore);
    CHECK_SUCCESS(status);

    return PH_ERR_SUCCESS;
}

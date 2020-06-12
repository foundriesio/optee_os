/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Delegate BAL Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_DELEGATE

#include "phbalReg_Delegate.h"

phStatus_t phbalReg_Delegate_Init(
    phbalReg_Delegate_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
    void *                          pCtx,
    fpphbalReg_Delegate_OpenPort    fpOpenPort,
    fpphbalReg_Delegate_ClosePort   fpClosePort,
    fpphbalReg_Delegate_Exchange    fpExchange,
    fpphbalReg_Delegate_SetConfig   fpSetConfig,
    fpphbalReg_Delegate_GetConfig   fpGetConfig
)
{
    if (sizeof(phbalReg_Delegate_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }
    PH_ASSERT_NULL (fpExchange);
    pDataParams->wId            = PH_COMP_BAL | PHBAL_REG_DELEGATE_ID;
    pDataParams->pCtx = pCtx;
    pDataParams->fpOpenPort = fpOpenPort;
    pDataParams->fpClosePort = fpClosePort;
    pDataParams->fpExchange = fpExchange;
    pDataParams->fpSetConfig = fpSetConfig;
    pDataParams->fpGetConfig = fpGetConfig;
    return PH_ERR_SUCCESS;
}
phStatus_t phbalReg_Delegate_OpenPort(
    phbalReg_Delegate_DataParams_t * pDataParams
) {
    PH_ASSERT_NULL (pDataParams->fpOpenPort);
    return pDataParams->fpOpenPort(pDataParams->pCtx);
}

phStatus_t phbalReg_Delegate_ClosePort(
    phbalReg_Delegate_DataParams_t * pDataParams
) {
    PH_ASSERT_NULL (pDataParams->fpClosePort);
    return pDataParams->fpClosePort(pDataParams->pCtx);
}

phStatus_t phbalReg_Delegate_Exchange(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t * pRxBuffer,
    uint16_t * pRxLength
) {
    uint16_t rxLength;
    phStatus_t status;
    PH_ASSERT_NULL (pDataParams->fpExchange);
    status = pDataParams->fpExchange(pDataParams->pCtx,
        wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, &rxLength
    );
    if ( NULL != pRxLength)
        *pRxLength = rxLength;
    return status;
}

phStatus_t phbalReg_Delegate_SetConfig(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
) {
    PH_ASSERT_NULL (pDataParams->fpSetConfig);

    if ( wConfig == 0x03) // Bal Connection Type Selection
    {
        pDataParams->bBalType = (byte)wValue;
    }

    return pDataParams->fpSetConfig(pDataParams->pCtx,
        wConfig, wValue
    );
}

phStatus_t phbalReg_Delegate_GetConfig(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
) {
    PH_ASSERT_NULL (pDataParams->fpGetConfig);
    return pDataParams->fpGetConfig(pDataParams->pCtx,
        wConfig, pValue
    );
}

#endif /* NXPBUILD__PHBAL_REG_DELEGATE */

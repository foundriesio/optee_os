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

#ifndef PHBALREG_DELEGATE_H
#define PHBALREG_DELEGATE_H

#include <ph_Status.h>
#include <phbalReg.h>

#ifdef NXPBUILD__PHBAL_REG_DELEGATE

phStatus_t phbalReg_Delegate_OpenPort(
    phbalReg_Delegate_DataParams_t * pDataParams
);

phStatus_t phbalReg_Delegate_ClosePort(
    phbalReg_Delegate_DataParams_t * pDataParams
);

phStatus_t phbalReg_Delegate_Exchange(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t * pRxBuffer,
    uint16_t * pRxLength
);

phStatus_t phbalReg_Delegate_SetConfig(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
);

phStatus_t phbalReg_Delegate_GetConfig(
    phbalReg_Delegate_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
);

#endif /* NXPBUILD__PHBAL_REG_DELEGATE */
#endif /* PHBALREG_DELEGATE_H */

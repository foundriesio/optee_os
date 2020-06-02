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
 * Delegates to handle GPIOs between PC and Micro Controller
 *
 * $Date$
 * $Author$
 * $Revision$
 */

#ifndef INC_NP_DELEGATE_PLATFORM_H_
#define INC_NP_DELEGATE_PLATFORM_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#ifdef __cplusplus
extern "C" {
#endif

#include <phbalReg.h>

#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

#ifdef NXPBUILD__PH_PLATFORM_DELEGATE
typedef struct
{
#ifdef NXPBUILD__PHBAL_REG_SERIALWIN
    phbalReg_SerialWin_DataParams_t *pSerialWin;
#endif
} NPPlatformCtx_t;


/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

int np_Delegate_Platform_Init(NPPlatformCtx_t *pPlatformCtx);
#endif

#endif /* INC_NP_NXPNFCRDLIB_BAL_H_ */

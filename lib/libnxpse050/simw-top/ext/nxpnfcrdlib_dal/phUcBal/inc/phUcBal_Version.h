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

/** @file
 *
 * phUcBal_Version.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 24-Jan-2017 $
 * $Author: ing05193 $
 * $Revision$
 */
/*
 *  _   ___   _______    _   _ ______ _____    _____           _          _ _
 * | \ | \ \ / /  __ \  | \ | |  ____/ ____|  / ____|         | |        (_) |
 * |  \| |\ V /| |__) | |  \| | |__ | |      | |     ___   ___| | ___ __  _| |_
 * | . ` | > < |  ___/  | . ` |  __|| |      | |    / _ \ / __| |/ / '_ \| | __|
 * | |\  |/ . \| |      | |\  | |   | |____  | |___| (_) | (__|   <| |_) | | |_
 * |_| \_/_/ \_\_|      |_| \_|_|    \_____|  \_____\___/ \___|_|\_\ .__/|_|\__|
 *                                                                 | |
 *                                                                 |_|
 * $Revision$
 */

#ifndef PHUCBAL_VERSION_H
#define PHUCBAL_VERSION_H

/** @ingroup CommandGroupVersion
 * @{
 */

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include "phUcBal_Protocol.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/** Instructions for GPIO */
enum phUcBal_Version_INS {
    /** [\b 0x01] Get the Major version of NxpNfcRdLib */
    Version_INS_RD_Major      = 0x01,
    /** [\b 0x02] Get the Minor version  of NxpNfcRdLib*/
    Version_INS_RD_Minor      = 0x02,
    /** [\b 0x03] Get the Development version  of NxpNfcRdLib*/
    Version_INS_RD_Dev        = 0x03,
    /** [\b 0x04] Get the Version String of NxpNfcRdLib*/
    Version_INS_RD_String     = 0x04,

    /** [\b 0x11] Get the Major version of NNC uc VCOM Fw */
    Version_INS_uC_Major      = 0x11,
    /** [\b 0x12] Get the Minor version of NNC uc VCOM Fw */
    Version_INS_uC_Minor      = 0x12,
    /** [\b 0x13] Get the Development version of NNC uc VCOM Fw */
    Version_INS_uC_Dev        = 0x13,
    /** [\b 0x14] Get the Version String of the firmware of NNC uc VCOM Fw */
    Version_INS_uC_String     = 0x14,
    /** [\b 0x15] Get the date time of compilation the firmware of NNC uc VCOM Fw */
    Version_INS_uC_DateTime   = 0x15,

    /** [\b 0x20] Get the Frontend for which we have compiled the code */
    Version_INS_FrontEnd      = 0x20,
};

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupVersion */
phStatus_t phUcBal_Version(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** @} */

#endif /* PHUCBAL_VERSION_H */

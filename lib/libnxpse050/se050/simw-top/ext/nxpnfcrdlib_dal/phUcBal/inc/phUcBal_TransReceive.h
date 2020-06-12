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
 *
 * phUcBal_TransReceive.h : APIs for TransReceive between the Micro-Controller and frontend
 *                          in phUcBal
 *
 * Project:  NXP NFC Cockpit
 *
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

#ifndef PHUCBAL_TRANSRECEIVE_H
#define PHUCBAL_TRANSRECEIVE_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include "phUcBal_Protocol.h"

/**
 * @ingroup CommandGroupTransReceive
 * @{
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */


/** Instructions for @ref CommandGroupTransReceive */

enum phUcBal_TransReceive_INS {
    /** [\b 0x05]: Only send. '5'end */
    TransReceive_INS_Tx = 0x05,
    /** [\b 0x0E]: Only Receive. R'E'ceive */
    TransReceive_INS_Rx = 0x0E,
    /** [\b 0xFD]: Full-Duplex Tx and Rx. 'F'ull 'D'uplex */
    TransReceive_INS_TRx = 0xFD
};

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupTransReceive */
phStatus_t phUcBal_TransReceive(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** @} */
#endif /* PHUCBAL_TRANSRECEIVE_H */

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

/* @file
 *
 * phUcBal_LoopBack.h:  APIs for Loop Back/echo in phUcBal
 *
 * Project:  NXP NFC Cockpit
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

#ifndef PHUCBAL_LOOPBACK_H
#define PHUCBAL_LOOPBACK_H


/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal_Protocol.h"

/** @ingroup CommandGroupLoopBack
 * @{
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** Instructions for GPIO */
enum phUcBal_LoopBack_INS {
    /** [\b 0x0A] Return an array in ascending order. '0'ut 'A'scending. */
    LoopBack_INS_Out_Ascending = 0x0A,
    /** [\b 0x0D] Return an array in descending order. '0'ut 'D'escending. */
    LoopBack_INS_Out_Descending = 0x0D,
    /** [\b 0x1A] Return the length of received packet,
     * expecting ascending order of input data. 'i'N. 'A'scending */
    LoopBack_INS_In_Ascending = 0x1A,
    /** [\b 0xE0] Send same data back. 'E'ch'0' */
    LoopBack_INS_ECHO = 0xE0
};

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupSECFw */
phStatus_t phUcBal_LoopBack(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** @} */
#endif /* PHUCBAL_LOOPBACK_H */

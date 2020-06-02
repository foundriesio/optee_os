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
 * phUcBal_SECFw.h:  APIs for Secondary Firmware Upgrade in phUcBal
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

#ifndef PHUCBAL_SECFW_H
#define PHUCBAL_SECFW_H


/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal_Protocol.h"
#include "phOsal.h"

/** @ingroup CommandGroupSECFw
 * @{
 *
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * Start a task entry for Secondary Firmware.
 */
#define PHUCBAL_SECFW_TASKLIST_START() \
		const phUcBal_SECFw_Tasks_t gkphUcBal_SECFw_TaskList[] = {

/**
 * Add individual item for Task.
 *
 * @param FP_START Entry point of the Secondary Firmware Task
 * @param FP_END   Function to be called when Secondary Firmware Task is requested to be stopped/killed
 * @param STACK_SIZE RTOS Stack size needed for this task
 * @param NAME String/name of the task
 */
#define PHUCBAL_SECFW_TASKLIST_ITEM(FP_START, FP_END, STACK_SIZE, NAME) {(FP_START), (FP_END), (STACK_SIZE), (NAME)}

/** End the entry for the task list */
#define PHUCBAL_SECFW_TASKLIST_END() \
		{NULL,NULL, 0, NULL}	\
	};


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** Instruction for @ref CommandGroupSECFw */
enum phUcBal_SECFw_INS {
    /** [\b 0xC0] The number of total Secondary application tasks. 'C''0'unt */
    SECFw_INS_GetTaskCount = 0xC0,      //!< SECFw_INS_GetTaskCount
    /** [\b 0xEA] Get the name of given task.  G'E't n'A'me */
    SECFw_INS_GetTaskName = 0xEA,       //!< SECFw_INS_GetTaskName
    /** [\b 0x5A] Start a given task. '5'tart 'A'pp. */
    SECFw_INS_StartAppTask = 0x5A,      //!< SECFw_INS_StartAppTask
    /** [\b 0x0A] Stop a given task. Sto'0'p 'A'pp*/
    SECFw_INS_StopAppTask =  0x0A,      //!< SECFw_INS_StopAppTask
    /** [\b 0xB1] Enter FW upgrade mode.  'B'oot '1'oader (This will reboot the @ref uchost). */
    SECFw_INS_EnterFWUpgradeMode = 0xB1,//!< SECFw_INS_EnterFWUpgradeMode
    /** [\b 0xB1] Can Enter FW upgrade mode.  'C'an upgr'A'de?
     * The @ref pchost can know whether this feature is implemented in this firmware. */
    SECFw_INS_CanUpgrade = 0xCA,//!< SECFw_INS_EnterFWUpgradeMode
};

typedef void (*pphUcBal_SECFw_End_t)(void);


/** Task Pointer Object for Secondary FW */
typedef struct {
    /** RTOS Task Function Pointer.  This API would be used to Start / Stop the RTOS task. */
    pphOsal_StartFunc_t fnTask;

    /** If clean up is needed, call this task. */
    pphUcBal_SECFw_End_t fnEnd;

    /** Stack Size for the RTOS Task */
    uint16_t wStackSize;

    /** RTOS Task Name.   This name would be displayed in the GUI. */
    const char * szTaskName;
} phUcBal_SECFw_Tasks_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/** Array of Task Pointer Objects */
extern const phUcBal_SECFw_Tasks_t gkphUcBal_SECFw_TaskList[];

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupSECFw */
phStatus_t phUcBal_SECFw(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Stop the previous running RTOS Task */
phStatus_t phUcBal_SECFw_StopAppTask(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Is it possible to upgrade this app through secondary FW Upgrade mechanisam.
 *
 * If this feature is avaialble, the GUI can download a new Binary can be uploaded to
 * the controller without use of debugger/programmer.
 *
 * This feature is only available for PN5180 + LPC1769 and RC663 + PN5180 reference
 * boards.
 */
phStatus_t phUcBal_SECFw_CanUpgrade(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** @} */
#endif /* PHUCBAL_SECFW_H */

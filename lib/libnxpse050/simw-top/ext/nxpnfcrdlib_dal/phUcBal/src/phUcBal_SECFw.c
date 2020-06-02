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
 * phUcBal_SECFw.c:  Implementation for Secondary Firmware Management
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

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUcBal_SECFw.h"
#include "phOsal.h"
#include "phDriver.h"
#include "phUcBal_MainTask.h"
#include "phUcBal_Config.h"

/* *****************************************************************************************************************
 * Secondary Secondary Firmware - Query / Start / Stop
 * ***************************************************************************************************************** */

/** @ingroup CommandGroupSECFw
 * @{
 */

/** Maximum tasks supported for Secondary Application.
 *
 * Used so that when we are traversing @ref gkphUcBal_SECFw_Tasks via @ref GetTaskCount,
 * we still limit outselves to a limited range.
 */
#define MAX_SUPPORTED_TASKS         10

/** RTOS Thread/Task object for Secondary Task. */
static phOsal_ThreadObj_t secondaryTaskObject;

/** Handle for the previous/current task.
 * As a guideline, only one user task can be executed at a time.
 *
 * Eventually, @ref phUcBal_SECFw_StopAppTask would use to stop the running task. */
static phOsal_Thread_t * previousTaskThreadHandle = NULL;

static int32_t gLastTaskNumber = -1;

static phStatus_t phUcBal_SECFw_GetTaskCount(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_SECFw_GetTaskName(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_SECFw_StartAppTask(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static uint32_t GetTaskCount(void);

phStatus_t phUcBal_SECFw_EnterFWUpgradeMode(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phUcBal_SECFw(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch (Cmd->header.INS)
    {
    PH_UCBAL_CASE(SECFw, GetTaskCount);
    PH_UCBAL_CASE(SECFw, GetTaskName);
    PH_UCBAL_CASE(SECFw, StartAppTask);
    PH_UCBAL_CASE(SECFw, StopAppTask);
    PH_UCBAL_CASE(SECFw, EnterFWUpgradeMode);
    PH_UCBAL_CASE(SECFw, CanUpgrade);
    }

    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/** Give the number of tasks implemented by the Secondary Application.
 *
 * This API depends on the structure @ref gkphUcBal_SECFw_Tasks that is to be filled
 * in statically by the Secondary Application.
 */
static phStatus_t phUcBal_SECFw_GetTaskCount(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint32_t totalTaskCount = GetTaskCount();
    Rsp->header.Lr += 1;
    Rsp->header.Response[0] = totalTaskCount;

    return PH_ERR_SUCCESS;
}

/** Give the task name  of tasks implemented by the Secondary Application.
 *
 * Before calling this API, ensure @ref phUcBal_SECFw_GetTaskCount is invoked.
 *
 * Input: **P1** holds the task number.
 *
 * This API depends on the structure @ref gkphUcBal_SECFw_Tasks that is to be filled
 * in statically by the Secondary Application.
 *
 */
static phStatus_t phUcBal_SECFw_GetTaskName(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint32_t taskNumber = Cmd->header.P1;
    uint32_t totalTaskCount = GetTaskCount();
    if ( taskNumber > totalTaskCount)
        return PH_ERR_BUFFER_OVERFLOW;
    Rsp->header.Lr = strlen(gkphUcBal_SECFw_TaskList[taskNumber].szTaskName) + 1;
    memcpy(Rsp->header.Response, gkphUcBal_SECFw_TaskList[taskNumber].szTaskName, Rsp->header.Lr + 1);
    return PH_ERR_SUCCESS;
}

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t gphUcBal_SECFw_SecondaryFWStack[1600];
#endif

/** Start the secondary Application.
 *
 * Before calling this API, ensure @ref phUcBal_SECFw_GetTaskCount  and
 * @ref phUcBal_SECFw_GetTaskName is invoked.
 *
 * Input: **P1** holds the task number.
 *
 * This API depends on the structure @ref gkphUcBal_SECFw_Tasks that is to be filled
 * in statically by the Secondary Application.
 */

static phStatus_t phUcBal_SECFw_StartAppTask(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    phStatus_t status;

    uint32_t taskNumber = Cmd->header.P1;
    if ( taskNumber >= GetTaskCount())
        return PH_ERR_USE_CONDITION;

    if (previousTaskThreadHandle != NULL)
    {
        return PH_ERR_RESOURCE_ERROR;
    }

    phUcBal_Config_StoreIRQIsrHandling(IRQIsrHandling_Share);

    secondaryTaskObject.pTaskName = (uint8_t *) "Secondary";
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
    secondaryTaskObject.pStackBuffer = gphUcBal_SECFw_SecondaryFWStack;
#else
    secondaryTaskObject.pStackBuffer = NULL;
#endif
    secondaryTaskObject.priority = 4;
    secondaryTaskObject.stackSizeInNum = gkphUcBal_SECFw_TaskList[taskNumber].wStackSize;

    LED_RED(1);
    status = phOsal_ThreadCreate(&secondaryTaskObject.ThreadHandle,
        &secondaryTaskObject, gkphUcBal_SECFw_TaskList[taskNumber].fnTask, NULL);

    previousTaskThreadHandle = &secondaryTaskObject.ThreadHandle;
    gLastTaskNumber = taskNumber;
    return status;
}

phStatus_t phUcBal_SECFw_StopAppTask(
    const phUcBal_Cmd_t * UnUsedCmd, phUcBal_Rsp_t * UnUsedRsp)
{
    LED_RED(0);
    if (previousTaskThreadHandle == NULL)
    {
        return PH_ERR_USE_CONDITION;
    }
    else
    {
        if (-1 != gLastTaskNumber ) {
            if ( NULL != gkphUcBal_SECFw_TaskList[gLastTaskNumber].fnEnd)
            {
                gkphUcBal_SECFw_TaskList[gLastTaskNumber].fnEnd();
            }
        }
        phOsal_ThreadDelete(previousTaskThreadHandle);
        previousTaskThreadHandle = NULL;
        gLastTaskNumber = -1;
    }

    phUcBal_Config_StoreIRQIsrHandling(IRQIsrHandling_Consume);

    return PH_ERR_SUCCESS;
}

/** Iterate over @ref gkphUcBal_SECFw_Tasks so that we know
 * how many tasks are built by this application. */

static uint32_t GetTaskCount() {
    int32_t i;
    for ( i= 0; i < (MAX_SUPPORTED_TASKS); i++)
        if (gkphUcBal_SECFw_TaskList[i].fnTask == NULL
                || gkphUcBal_SECFw_TaskList[i].szTaskName == NULL
                || gkphUcBal_SECFw_TaskList[i].wStackSize == 0 )
            break;
    return i;
}

/** @} */

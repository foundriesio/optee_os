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
 * phUcBal_Config.c:  Configurations and Settings for the phUcBal
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
#include "phUcBal.h"
#include "phUcBal_Config.h"
#include "phUcBal_GPIO.h"
#include "phUcBal_Protocol.h"
#ifdef NXPBUILD__PHHAL_HW_PN5180
#   include <../comps/phhalHw/src/Pn5180/phhalHw_Pn5180.h>
#   include "phhalHw_Pn5180_Instr.h"
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_RC663
#	include "BoardSelection.h"
#endif /* NXPBUILD__PHHAL_HW_RC663 */

/** @ingroup CommandGroupConfig
 * @{
 */

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

/** Data structure holding configurable settings for @ref CommandGroupConfig.
 *
 * This is marked static for this file so that @ref gpkphUcBal_Config can be used access it
 * outside this file.
 */
static phUcBal_Config_t gphphUcBal_Config;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
static phStatus_t phUcBal_Config_RxTimeoutMs(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_GPIOWaitTimeoutMs(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
#endif

/** What to do before fetching a byte/frame from @ref dut by @ref uchost. */
static phStatus_t phUcBal_Config_WaitBeforeRX_Strategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** What to do before sending a byte/frame from @ref uchost to @ref dut. */
static phStatus_t phUcBal_Config_WaitBeforeTX_Strategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

#ifdef NXPBUILD__PHHAL_HW_PN5180
static phStatus_t phUcBal_Config_ConfigIRQPollStrategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_WaitIRQDelayWithTestBus(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
#endif

#ifdef NXPBUILD__PHHAL_HW_RC663
static phStatus_t phUcBal_Config_GetIrqType(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_SelectCommand(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_SelectCommandLength(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_NumValidBitsinLastByte(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_BeginRoundCommand(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_Config_TSprocessing(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
#endif
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

void phUcBal_Config_Init()
{
#if defined(NXPBUILD__PHHAL_HW_PN5180)
    gphphUcBal_Config.WaitBeforeTX_Strategy = WaitBefore_WaitForBusyLow;
    gphphUcBal_Config.WaitBeforeRX_Strategy = WaitBefore_WaitForBusyLow;
    gphphUcBal_Config.IRQPollStrategy = IRQHandling_UseInterrupts;
#elif defined(NXPBUILD__PHHAL_HW_RC663)
    gphphUcBal_Config.WaitBeforeTX_Strategy = WaitBefore_Immediate;
    gphphUcBal_Config.WaitBeforeRX_Strategy = WaitBefore_Immediate;
#else
    gphphUcBal_Config.WaitBeforeTX_Strategy = WaitBefore_Immediate;
    gphphUcBal_Config.WaitBeforeRX_Strategy = WaitBefore_Immediate;
#endif

    gphphUcBal_Config.u32IrqPinValue = 0;

#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
    gphphUcBal_Config.RxTimeoutMs = 500;
    gphphUcBal_Config.GPIOWaitTimeoutMs = 100;
#endif

    gphphUcBal_Config.IRQIsrHandling = IRQIsrHandling_Share;

#ifdef NXPBUILD__PHHAL_HW_RC663
    memset(gphphUcBal_Config.SelectCommand, 0, sizeof(gphphUcBal_Config.SelectCommand));
    gphphUcBal_Config.SelectCommandLength = 0;
    gphphUcBal_Config.NumValidBitsinLastByte = 0;
    memset(gphphUcBal_Config.BeginRoundCommand, 0, sizeof(gphphUcBal_Config.BeginRoundCommand));
    gphphUcBal_Config.TSProcessingStrategy = TSProcessing_One_Ts_Only;
#endif

    gpkphUcBal_Config = &gphphUcBal_Config;
}

phStatus_t phUcBal_Config(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    switch ( Cmd->header.INS )
    {
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
    PH_UCBAL_CASE(Config, RxTimeoutMs);
    PH_UCBAL_CASE(Config, GPIOWaitTimeoutMs);
#endif
    PH_UCBAL_CASE(Config, WaitBeforeTX_Strategy);
    PH_UCBAL_CASE(Config, WaitBeforeRX_Strategy);
#ifdef NXPBUILD__PHHAL_HW_PN5180
    PH_UCBAL_CASE(Config, ConfigIRQPollStrategy);
    PH_UCBAL_CASE(Config, WaitIRQDelayWithTestBus);
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_RC663
    PH_UCBAL_CASE(Config, GetIrqType);
    PH_UCBAL_CASE(Config, SelectCommand);
    PH_UCBAL_CASE(Config, SelectCommandLength);
    PH_UCBAL_CASE(Config, NumValidBitsinLastByte);
    PH_UCBAL_CASE(Config, BeginRoundCommand);
    PH_UCBAL_CASE(Config, TSprocessing);
#endif /* NXPBUILD__PHHAL_HW_RC663 */

    }
    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

void phUcBal_Config_StoreIRQPinValue(uint32_t newPinValue)
{
    gphphUcBal_Config.u32IrqPinValue = newPinValue;
}

void phUcBal_Config_StoreIRQIsrHandling(enum phUcBal_Config_IRQIsrHandling irq_handling) {
    gphphUcBal_Config.IRQIsrHandling = irq_handling;
}

void phUcBal_Config_StoreICInitFailed(uint32_t v) {
    gphphUcBal_Config.ICInitFailed = v;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
static phStatus_t phUcBal_Config_RxTimeoutMs(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    return PH_ERR_INTERNAL_ERROR;
}
static phStatus_t phUcBal_Config_GPIOWaitTimeoutMs(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    return PH_ERR_INTERNAL_ERROR;
}
#endif

/**
 * Strategy to Wait before TX.
 *
 * @sa phUcBal_Config_WaitBefore
 */
static phStatus_t phUcBal_Config_WaitBeforeTX_Strategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    enum phUcBal_Config_WaitBefore wb = (enum phUcBal_Config_WaitBefore) Cmd->header.P1;
    switch ( wb )
    {
    case WaitBefore_Immediate:
    case WaitBefore_WaitForBusyLow:
    case WaitBefore_WaitForIRQHigh:
        gphphUcBal_Config.WaitBeforeTX_Strategy = wb;
        return PH_ERR_SUCCESS;
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
    case WaitBefore_WaitForTimeoutMs:
#endif
    default:
        break;
    }
    return PH_ERR_INVALID_PARAMETER;
}

/**
 * Strategy to Wait before RX.
 *
 * @sa phUcBal_Config_WaitBefore
 */

static phStatus_t phUcBal_Config_WaitBeforeRX_Strategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    enum phUcBal_Config_WaitBefore wb = (enum phUcBal_Config_WaitBefore) Cmd->header.P1;
    switch ( wb )
    {
    case WaitBefore_Immediate:
    case WaitBefore_WaitForBusyLow:
    case WaitBefore_WaitForIRQHigh:
        gphphUcBal_Config.WaitBeforeRX_Strategy = wb;
        return PH_ERR_SUCCESS;
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
    case WaitBefore_WaitForTimeoutMs:
#endif
        default:
        break;
    }
    return PH_ERR_INVALID_PARAMETER;
}

#ifdef NXPBUILD__PHHAL_HW_PN5180
static phStatus_t phUcBal_Config_ConfigIRQPollStrategy(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    phStatus_t status;
    enum phUcBal_Config_IRQPollStrategy newPollStrategy = (enum phUcBal_Config_IRQPollStrategy)Cmd->header.P1;
    switch (newPollStrategy) {
        case IRQHandling_UseInterrupts:
        case IRQHandling_CheckTestBus:
        case IRQHandling_ReadRegsiter:
            gphphUcBal_Config.IRQPollStrategy = newPollStrategy;
            status = PH_ERR_SUCCESS;
            break;
        default:
            status = PH_ERR_INVALID_PARAMETER;
            break;
    }
    if (gphphUcBal_Config.IRQPollStrategy == IRQHandling_CheckTestBus && NULL != gpphUcBal_PtrHal)
    {
        phhalHw_Pn5180_DataParams_t * pDataParams = gpphUcBal_PtrHal;
        gphphUcBal_Config.IRQPollStrategy = (enum phUcBal_Config_IRQPollStrategy)Cmd->header.P1;

        status = phhalHw_Pn5180_Instr_ReadE2Prom(
            pDataParams,
            PHHAL_HW_PN5180_TESTBUS_ENABLE_ADDR, &pDataParams->bIsTestBusEnabled, 1);
        if ( 0 == (0x80 & pDataParams->bIsTestBusEnabled )) {
            /* Test Bus is disabled */
            gphphUcBal_Config.IRQPollStrategy = IRQHandling_UseInterrupts;
        }
        else
        {
            gphphUcBal_Config.IRQPollStrategy = IRQHandling_ReadRegsiter;
        }
    }

    if ( gphphUcBal_Config.IRQPollStrategy == IRQHandling_UseInterrupts && NULL != gpphUcBal_PtrHal) {
        phUcBal_GPIO_EnableIRQInterrupts();
    }
    else {
        phUcBal_PN5180Irq_DisableIRQ();
    }
    return status;
}
static phStatus_t phUcBal_Config_WaitIRQDelayWithTestBus(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint32_t * value = (uint32_t *) (&(Cmd->header.Payload));
    if ( NULL != gpphUcBal_PtrHal)
        gpphUcBal_PtrHal->wWaitIRQDelayWithTestBus = *(value);
    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_RC663

/**
 *  Get '1'RQ Typ'E' configuration
 *
 * @sa phUcBal_Config_GetIrqType
 */
static phStatus_t phUcBal_Config_GetIrqType(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    Rsp->header.Lr=1;
    Rsp->header.Response[0] = PIN_IRQ_TRIGGER_TYPE;
	return PH_ERR_SUCCESS;
}

/**
 * Configure '5'elect 'C'ommand
 *
 * @sa phUcBal_Config_SelectCommand
 */
static phStatus_t phUcBal_Config_SelectCommand(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint8_t * value = (uint8_t *) (&(Cmd->header.Payload));
    if ( Cmd->header.Lc <= sizeof(gphphUcBal_Config.SelectCommand))
    {
        memcpy(gphphUcBal_Config.SelectCommand, value, sizeof(gphphUcBal_Config.SelectCommand));
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ERR_INVALID_PARAMETER;
    }
}

/**
 * Configure '5'elect Command '1'ength
 *
 * @sa phUcBal_Config_SelectCommandLength
 */
static phStatus_t phUcBal_Config_SelectCommandLength(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint16_t * value = (uint16_t *) (&(Cmd->header.Payload));
    if ( *(value) <=  sizeof(gphphUcBal_Config.SelectCommand))
    {
        gphphUcBal_Config.SelectCommandLength = *(value);
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ERR_INVALID_PARAMETER;
    }
}

/**
 * Configure Number of Valid 'B'its in '1'ast byte of SelCmd
 *
 * @sa phUcBal_Config_NumValidBitsinLastByte
 */
static phStatus_t phUcBal_Config_NumValidBitsinLastByte(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint8_t * value = (uint8_t *) (&(Cmd->header.P1));
    gphphUcBal_Config.NumValidBitsinLastByte = *(value);
    return PH_ERR_SUCCESS;
}

/**
 * Configure 'B'eginRound 'C'ommand
 *
 * @sa phUcBal_Config_BeginRoundCommand
 */
static phStatus_t phUcBal_Config_BeginRoundCommand(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint8_t * value = (uint8_t *) (&(Cmd->header.Payload));
    if ( Cmd->header.Lc <= sizeof(gphphUcBal_Config.BeginRoundCommand))
    {
        memcpy(gphphUcBal_Config.BeginRoundCommand, value, sizeof(gphphUcBal_Config.BeginRoundCommand));
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ERR_INVALID_PARAMETER;
    }
}

/**
 * Configure Time '5'lot processing 'B'ehavior
 *
 * @sa phUcBal_Config_TSprocessing
 */
static phStatus_t phUcBal_Config_TSprocessing(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    enum phUcBal_Config_TSProcessing wb = (enum phUcBal_Config_TSProcessing) Cmd->header.P1;
    switch ( wb )
    {
    case TSProcessing_Get_Max_Resps:
    case TSProcessing_One_Ts_Only:
    case TSProcessing_Get_Tag_Handle:
        gphphUcBal_Config.TSProcessingStrategy = wb;
        return PH_ERR_SUCCESS;
    default:
        break;
    }
    return PH_ERR_INVALID_PARAMETER;
}
#endif /* NXPBUILD__PHHAL_HW_RC663 */
/** @} */

const phUcBal_Config_t * gpkphUcBal_Config = NULL;

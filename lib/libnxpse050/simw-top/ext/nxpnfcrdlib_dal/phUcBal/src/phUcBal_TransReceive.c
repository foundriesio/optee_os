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
 * phUcBal_TransReceive.c:  Trans/Receive to the IC Frontend
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
#include "phUcBal_TransReceive.h"
#include "phUcBal_MainTask.h"
#include "phUcBal_Config.h"
#include "phUcBal_GPIO.h"
#include "BoardSelection.h"
#include "phDriver.h"
#include "ph_NxpBuild.h"

/**
 * @ingroup CommandGroupTransReceive
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
#if (PHUCBAL_PORT_SPI_TXRX_ALWAYS_FULL_DUPLEX == 1)
static uint8_t gbaTxRxBuffer[270];
#endif

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

static phStatus_t phUcBal_TransReceive_Tx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_TransReceive_Rx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_TransReceive_TRx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

static void phUcBal_WaitBeforeTxRxHandling(enum phUcBal_Config_WaitBefore WaitBefore);
static void phUcBal_WaitBeforeTxHandling(void);
static void phUcBal_WaitBeforeRxHandling(void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phUcBal_TransReceive(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch(Cmd->header.INS)
    {
    PH_UCBAL_CASE(TransReceive, Tx);
    PH_UCBAL_CASE(TransReceive, Rx);
    PH_UCBAL_CASE(TransReceive, TRx);
    }
    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 * Send one frame to DUT.  The format of the command frame is:
 *
 *   | CLA                    | INS                       | P1 | P2 | Lc     | (Command) Payload   |
 *   |------------------------|---------------------------|----|----|--------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_Tx  | 0  | 0  | Len    | Payload for DUT     |
 *
 * The format of the response frame is:
 *
 *   | CLA                    | INS                       | S1 | S2 | Lr   | (Response) Payload  |
 *   |------------------------|---------------------------|----|----|------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_Tx  | S1 | S2 | 0    | Empty               |
 *
 * - If Transmit was successful, S1=0, S2=0
 */


static phStatus_t phUcBal_TransReceive_Tx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint16_t rxBufSize;
    uint8_t * pRxBuf;
    uint16_t dummyRxLen;

#if PHUCBAL_PORT_SPI_TXRX_ALWAYS_FULL_DUPLEX
    rxBufSize = Cmd->header.Lc; // same as tx.
    pRxBuf = gbaTxRxBuffer;
#else
    rxBufSize = 0;
    pRxBuf = NULL;
#endif
    phUcBal_WaitBeforeTxHandling();
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_LOW);


    phStatus_t status = phbalReg_Exchange(gpphUcBal_PtrBal,
            PH_EXCHANGE_DEFAULT,
            (uint8_t *)Cmd->header.Payload, //For phbalReg_Exchange, this is not constant
            Cmd->header.Lc,
			rxBufSize,
			pRxBuf,
            &dummyRxLen);
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_HIGH);
    return status;
}


/**
 * Send one frame to DUT.  The format of the command frame is:
 *
 *   | CLA                    | INS                       | P1       | P2       | Lc     | (Command) Payload   |
 *   |------------------------|---------------------------|----------|----------|--------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_Rx  | Len(LSB) | Len(MSB) | 0      | Empty               |
 *
 * The format of the response frame is:
 *
 *   | CLA                    | INS                       | S1 | S2 | Lr   | (Response) Payload  |
 *   |------------------------|---------------------------|----|----|------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_Rx  | S1 | S2 | Len  | Response from DUT   |
 *
 * - If Transmit was successful, S1=0, S2=0
 */

static phStatus_t phUcBal_TransReceive_Rx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    uint16_t u16MaxRxLen = Cmd->header.P2;

    uint16_t txBufSize;
    uint8_t * pTxBuf;

    u16MaxRxLen <<= 8;
    u16MaxRxLen |= Cmd->header.P1;

#if PHUCBAL_PORT_SPI_TXRX_ALWAYS_FULL_DUPLEX
    txBufSize = u16MaxRxLen; // same as tx.
    pTxBuf = gbaTxRxBuffer;
    memset(gbaTxRxBuffer, 0xFF, u16MaxRxLen);
#else
    txBufSize = 0;
    pTxBuf = NULL;
#endif

    phUcBal_WaitBeforeRxHandling();
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_LOW);
    phStatus_t status = phbalReg_Exchange(gpphUcBal_PtrBal,
            PH_EXCHANGE_DEFAULT,
			pTxBuf,
            txBufSize,
            u16MaxRxLen,
            Rsp->header.Response,
            &Rsp->header.Lr);
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_HIGH);
    return status;
}


/**
 * Full Duplex Send/Receive between Host Microcontroller and DUT.
 *
 * The format of the command frame is:
 *
 *   | CLA                    | INS                       | P1       | P2        | Lc     | (Command) Payload   |
 *   |------------------------|---------------------------|----------|-----------|--------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_TRx | Len(LSB) | Len(MSB)  | Len    | Payload for DUT     |
 *
 * The format of the response frame is:
 *
 *
 *   | CLA                    | INS                       | S1 | S2 | Lr   | (Response) Payload  |
 *   |------------------------|---------------------------|----|----|------|---------------------|
 *   | \ref CLA_TransReceive  | \ref TransReceive_INS_TRx | S1 | S2 | Len  | Response from DUT   |
 *
 * - If Transmit was successful, S1=0, S2=0
 */

static phStatus_t phUcBal_TransReceive_TRx(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
    phStatus_t status;
    uint16_t u16MaxRxLen = Cmd->header.P2;
    uint16_t txLen = Cmd->header.Lc;
    u16MaxRxLen <<= 8;
    u16MaxRxLen |= Cmd->header.P1;
    phUcBal_WaitBeforeTxHandling();
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_LOW);
#ifdef NXPBUILD__PHHAL_HW_PN5180
    if ( gpkphUcBal_Config->IRQPollStrategy == IRQHandling_ReadRegsiter) {
    	while(!phDriver_PinRead(PHDRIVER_PIN_BUSY, PH_DRIVER_PINFUNC_INPUT));
    }
#endif

#if PHUCBAL_PORT_SPI_TXRX_ALWAYS_FULL_DUPLEX
    if (u16MaxRxLen > txLen)
    	txLen = u16MaxRxLen;
    else if (txLen > u16MaxRxLen)
    	u16MaxRxLen = txLen;
#endif

    status = phbalReg_Exchange(gpphUcBal_PtrBal,
            PH_EXCHANGE_DEFAULT,
            (uint8_t *)Cmd->header.Payload, //For phbalReg_Exchange, this is not constant
			txLen,
            u16MaxRxLen,
            Rsp->header.Response,
            &Rsp->header.Lr);
    // FIXME: to be handled for K8x
    if ( Rsp->header.Lr == Cmd->header.Lc && Cmd->header.Lc  > u16MaxRxLen)
    {
        Rsp->header.Lr = u16MaxRxLen;
    }
    phDriver_PinWrite(PHDRIVER_PIN_SSEL, PH_DRIVER_SET_HIGH);
    return status;
}

/**
 * Special handling before Tx / Rx, if any.
 *
 * This is important for PN5180 where we explicitly have to wait for Busy Pin to be low.
 * While doing Secure FW Upgrade of PN5180, this handling changes.
 *
 */

static void phUcBal_WaitBeforeTxRxHandling(enum phUcBal_Config_WaitBefore WaitBefore) {
    switch ( WaitBefore )
    {
    case WaitBefore_WaitForBusyLow:
    {
#ifdef NXPBUILD__PHHAL_HW_PN5180
        /* While busy pin does not become low */
    	while(phDriver_PinRead(PHDRIVER_PIN_BUSY, PH_DRIVER_PINFUNC_INPUT));
#endif
        break;
    }
    case WaitBefore_WaitForIRQHigh:
    {
        /** While IRQ pin does not become high */
        while(PH_ON != phUcBal_GPIO_GetPinValue(PHUCBAL_GPIO_NAME_IRQ))
        {
            __NOP(); /* Wait */
        }
        break;
    }
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION
    case WaitBefore_WaitForTimeoutMs:
        break;
#endif
    case WaitBefore_Immediate:
        /* Fall through */
    default:
        break;
    }
}

/** If and whether we have to wait before Tx. \see phUcBal_Config_t */
static void phUcBal_WaitBeforeTxHandling() {
    phUcBal_WaitBeforeTxRxHandling(gpkphUcBal_Config->WaitBeforeTX_Strategy);
}

/** If and whether we have to wait before Rx. \see phUcBal_Config_t */
static void phUcBal_WaitBeforeRxHandling() {
    phUcBal_WaitBeforeTxRxHandling(gpkphUcBal_Config->WaitBeforeRX_Strategy);
}


#ifdef NXPBUILD__PHHAL_HW_PN5180

void phBal_PN5180_PostTxCB() {
    if ( gpkphUcBal_Config->WaitBeforeTX_Strategy == WaitBefore_WaitForBusyLow )
    {
        /* After Tx, wait for Busy to go High */
        while(!phUcBal_GPIO_GetPinValue(PHUCBAL_GPIO_NAME_BUSY));
    }
}

#endif /* NXPBUILD__PHHAL_HW_PN5180 */

/** @} */

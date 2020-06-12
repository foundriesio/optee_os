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
 * phUcBal_LoopBack.c:  Implementation for Loopback that can be used for testing
 *                      the VCOM Interface
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
#include "phUcBal_LoopBack.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/** @ingroup CommandGroupLoopBack
 * @{
 */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

static phStatus_t phUcBal_LoopBack_Out_Ascending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_LoopBack_Out_Descending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_LoopBack_In_Ascending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);
static phStatus_t phUcBal_LoopBack_ECHO(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phUcBal_LoopBack(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch(Cmd->header.INS)
    {
    PH_UCBAL_CASE(LoopBack, Out_Ascending);
    PH_UCBAL_CASE(LoopBack, Out_Descending);
    PH_UCBAL_CASE(LoopBack, In_Ascending);
    PH_UCBAL_CASE(LoopBack, ECHO);
    }

    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 * Pump out a big buffer, with values in ascending order.
 *
 *   | CLA                | INS                              | P1           | P2             | Lc[L,H] | (Command) Payload   |
 *   |--------------------|----------------------------------|--------------|----------------|---------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_Out_Ascending  | RspLen       | StartNumber    | 0       | Empty               |
 *
 * Reponse would be of Length RspLen.  Response would start from StartNumber and increment for every subsequent byte
 *
 * The format of the response frame is:
 *
 *   | CLA                | INS                              | S1           | S2   | Lr[L,H]  | (Response) Payload  |
 *   |--------------------|----------------------------------|--------------|------|----------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_Out_Ascending  | 0            | 0    | RspLen   | Empty               |
 *
 */

static phStatus_t phUcBal_LoopBack_Out_Ascending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint8_t RspLen = Cmd->header.P1;
    uint8_t StartNumber = Cmd->header.P2;
    uint8_t * pBuf =  &Rsp->header.Response[0];
    Rsp->header.Lr = RspLen;
    while(RspLen >0 )
    {
        *pBuf = StartNumber;
        StartNumber++;
        pBuf++;
        RspLen--;
    }
    return PH_ERR_SUCCESS;
}

/**
 * Pump out a big buffer, with values in descending order.
 *
 * Sane as \ref phUcBal_LoopBack_Out_Ascending, INS would be \ref LoopBack_INS_Out_Descending
 *
 */

static phStatus_t phUcBal_LoopBack_Out_Descending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint8_t RspLen = Cmd->header.P1;
    uint8_t StartNumber = Cmd->header.P2;
    uint8_t * pBuf = &Rsp->header.Response[0];
    Rsp->header.Lr = RspLen;
    while(RspLen > 0)
    {
        *pBuf = StartNumber;
        StartNumber--;
        pBuf++;
        RspLen--;
    }

    return PH_ERR_SUCCESS;
}


/**
 * Receive in a big buffer, with values in ascending order, and cofirm back whether there was any error in received frames.
 *
 *   | CLA                | INS                              | P1   | P2   | Lc[L,H] | (Command) Payload   |
 *   |--------------------|----------------------------------|------|------|---------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_In_Ascending   | 0    | 0    | BufLen  | AscendingValues     |
 *
 * This function would validate that values are in AscendingValues buffer are in ascending order.
 *
 * The format of the response frame is:
 *
 *   | CLA                | INS                              | S1 | S2 | Lr[L,H]  | (Response) Payload  |
 *   |--------------------|----------------------------------|----|----|----------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_In_Ascending   | *  | *  | 4        | Comparison Status   |
 *
 * - If Success, S1=0, S2=0
 *      Comparison Status:
 *          -# Byte 0   : The starting number
 *          -# Byte 1   : 0
 *          -# Byte 2,3 : Length of In/Received Buffer
 *
 * - If Failure, S1=PH_ERR_FRAMING_ERROR, S2=0
 *      Comparison Status:
 *          -# Byte 0   : The starting number
 *          -# Byte 1   : Unexpected Value
 *          -# Byte 2,3 : Position where mismatch seen.
 *
 */
static phStatus_t phUcBal_LoopBack_In_Ascending(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    uint16_t BufLen = Cmd->header.Lc;
    uint8_t start_number = Cmd->header.Payload[0];
    int32_t i;
    Rsp->header.Lr = 4;
    Rsp->header.Response[0] = Cmd->header.Payload[0];
    for (i = 0; i < BufLen; i++)
    {
        if ( Cmd->header.Payload[i] != (uint8_t)(start_number + i))
        {
            Rsp->header.Response[1] = Cmd->header.Payload[i];
            Rsp->header.Response[2] = (uint8_t)i;
            Rsp->header.Response[3] = (uint8_t)(i >> 8);
            return PH_ERR_FRAMING_ERROR;
        }
    }

    Rsp->header.Response[1] = 0;
    Rsp->header.Response[2] = (uint8_t)BufLen;
    Rsp->header.Response[3] = (uint8_t)(BufLen >> 8);
    return PH_ERR_SUCCESS;
}


/**
 * Echo Back the same received buffer.
 *
 *   | CLA                | INS                    | P1 | P2 | Lc[L,H] | (Command) Payload   |
 *   |--------------------|------------------------|----|----|---------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_ECHO | 0  | 0  | BufLen  | Values              |
 *
 * The format of the response frame is:
 *
 *   | CLA                | INS                    | S1 | S2 | Lr[L,H]  | (Response) Payload  |
 *   |--------------------|------------------------|----|----|----------|---------------------|
 *   | \ref CLA_LoopBack  | \ref LoopBack_INS_ECHO | 0  | 0  | BufLen   | Values              |
 *
 * Same received values are sent back.
 *
 */
static phStatus_t phUcBal_LoopBack_ECHO(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    memcpy(Rsp->header.Response, Cmd->header.Payload, Cmd->header.Lc);
    Rsp->header.Lr = Cmd->header.Lc;
    return PH_ERR_SUCCESS;
}

/** @} */

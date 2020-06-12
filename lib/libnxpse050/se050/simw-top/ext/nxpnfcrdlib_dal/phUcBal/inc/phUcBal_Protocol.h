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
 * phUcBal_Protocol.h:  APIs for Protocol Format in phUcBal
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

#ifndef PHUCBAL_PROTOCOL_H
#define PHUCBAL_PROTOCOL_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Status.h"
#include "phUcBal.h"

/**
 *
 *  @page protocol-overview Protocol Overview
 *
 *  Overview of the Protocol/Frame Format between PC and Micro-Controller.
 *
 *  Since NxpNfcCockpit is designed for RF Tunning of NFC Controller, the
 *  semantics of the protocol between PC and the MicroController is inspired form
 *  ISO-7816 APDU Structure with a forward looking idea that the the users
 *  who are interested in understanding this implementation would be more
 *  or less aware of the ISO-7816 APDU Structure.
 *
 *  @note This implementation is inspired from ISO-7816 APDU Structure but
 *  only uses it's semantics for simplicity. It's neither full nor even partially
 *  compatible to ISO-7816's APDU implementation.
 *
 * @section protocolcmd Command Frame
 *
 * All the commands follow the the following structure.
 *
 * | CLA  | INS  | P1   | P2   | Lc   | (Command) Payload   |
 * |------|------|------|------|------|---------------------|
 * |1 byte|1 byte|1 byte|1 byte|2 byte| Lc bytes            |
 *
 *
 * @section protocolrsp Response Frame
 *
 * The response follows the following structure.
 *
 * | CLA  | INS  | S1   | S2   | Lr   | (Response) Payload  |
 * |------|------|------|------|------|---------------------|
 * |1 byte|1 byte|1 byte|1 byte|2 byte| Lr bytes            |
 *
 * @note If you compare ISO 7816 and the above frames, you can see that Le, SW1 SW2 are missing.
 *
 * @section protocolfields Description of fields in Command Frame and Response Frame
 *
 * The below table provides description of each field.
 *
 * | Field     | Description
 * |-----------|-----------------------------------------
 * | CLA       | The main group of command.  e.g. @ref CommandGroupTransReceive , @ref CommandGroupGPIO, etc.
 * | INS       | The instruction for that group
 * | P1        | Parameter 1
 * | P2        | Parameter 2
 * | Lc        | Length of Command Payload, 2 Bytes, LSB First
 * | Lr        | Length of Response Payload, 2 Bytes, LSB First
 * | S1        | Status 1 : API Status
 * | S2        | Status 2 : Component Code
 * | S1S2      | For some of the Commands Status 1 and Status 2 may be merged,
 * |           | and represent vales from phStatus_t of NxpRdLib APIs.
 *
 * Within the framework of this protocol, different @ref CommandGroups are sent and
 * received between the PC and the Micro-Controller.
 *
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * @ingroup USBStackConstants
 * @{
 */
/* Ensuring 2^n buffer size for future application, cyclic buffer management.  */
#define PH_UCBAL_PROTOCOL_IN_BUFFER_SIZE     1024
#define PH_UCBAL_PROTOCOL_HEADER_SIZE           6
#define PH_UCBAL_PROTOCOL_CMD_PAYLOAD_SIZE    (PH_UCBAL_PROTOCOL_IN_BUFFER_SIZE - PH_UCBAL_PROTOCOL_HEADER_SIZE)
#define PH_UCBAL_PROTOCOL_RSP_PAYLOAD_SIZE    (512)
#define PH_UCBAL_PROTOCOL_OUT_BUFFER_SIZE     (PH_UCBAL_PROTOCOL_RSP_PAYLOAD_SIZE + PH_UCBAL_PROTOCOL_HEADER_SIZE)

/** @} */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/** @ingroup groupProtocol
 * @{
 */
/** Enum : Command Group/Class */
enum phUcBal_CLA
{
    /** \b 0x01 */CLA_TransReceive = 1, /**< TRans Receive. This is used/implemented as part of @ref CommandGroupTransReceive */
    /** \b 0x10 */CLA_GPIO = 0x10, /**< GPIO Control. This is used/implemented as part of @ref CommandGroupGPIO */
    /** \b 0x5E */CLA_SECFw = 0x5E, /**< 'S''E'condary Firmware Control. This is used/implemented as part of @ref CommandGroupSECFw */
    /** \b 0xC0 */CLA_Config = 0xC0, /**< Various 'C''o'nfiguration parameters. @ref CommandGroupConfig */
    /** \b 0x1B */CLA_LoopBack = 0x1B, /**< 'L'oop 'B'ack. This is used/implemented as part of @ref CommandGroupLoopBack */
    /** \b 0x0E */CLA_Version = 0x0E, /**< V'E'rsion Information. This is used/implemented as part of @ref CommandGroupVersion */
    /** \b 0xB1 */CLA_BLog = 0xB1, /**< 'B'uffered 'L'og @ref CommandBufferedLog */
    /** \b 0x18 */CLA_I18000p3m3 = 0x18, /**< I'1''8'000p3m3 Commands @ref CommandI18000p3m3 */
    CLA_Last
};

/** Command Frame as sent from PC to the Micro Controller.
 *
 * Also see @ref protocolcmd and @ref protocolfields */

struct phUcBal_Cmd_s
{
    /** Command Group : @see CLA_Enums */
    uint8_t CLA;
    /** Command specific instruction */
    uint8_t INS;
    /** Parameter 1 */
    uint8_t P1;
    /** Parameter 2 */
    uint8_t P2;
    /** Length of Command Payload */
    uint16_t Lc;
    /** Command Payload */
    uint8_t Payload[PH_UCBAL_PROTOCOL_CMD_PAYLOAD_SIZE];
};

/**
 * Command Frame Union to help use buf while receiving over USB,
 * and use header while processing
 */
typedef union
{
    /** Command Frame with separate header and payload */
    struct phUcBal_Cmd_s header;
    /** Full Buffer frame as received over USB */
    uint8_t buf[PH_UCBAL_PROTOCOL_IN_BUFFER_SIZE];
} phUcBal_Cmd_t;

/** Response Frame to be sent from the Micro Controller to PC.
 *
 * Also see @ref protocolrsp and @ref protocolfields */
struct phUcBal_Rsp_s
{
    /** Command Group : @see CLA_Enums */
    uint8_t CLA;
    /** Command specific instruction */
    uint8_t INS;
    /** Union to either access status as phStatus_t or individual bytes */
    union {
        /** Individual access to Status 1 and Status 2 in the @ref protocolrsp */
        struct {
            /** Status 1 : API Status */
            uint8_t S1;
            /** Status 2 : Component Code */
            uint8_t S2;
        } S1S2;
        /** Status as formulated from phStatus_t */
        phStatus_t status;
    } Status;
    /** Length of Response */
    uint16_t Lr;
    /** Actual Response */
    uint8_t Response[PH_UCBAL_PROTOCOL_RSP_PAYLOAD_SIZE];
};

/**
 * Response Frame Union to help use buf sending receiving over USB,
 * and use header while populating the buffer. */
typedef union
{
    /** Response Frame with separate header and payload */
    struct phUcBal_Rsp_s header;
    /** Full buffer to be transmitted back over USB */
    uint8_t buf[PH_UCBAL_PROTOCOL_OUT_BUFFER_SIZE];
} phUcBal_Rsp_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @} */

#endif /* PHUCBAL_PROTOCOL_H */

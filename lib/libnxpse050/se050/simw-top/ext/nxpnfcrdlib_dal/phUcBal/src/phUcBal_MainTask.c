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
 * phUcBal_MainTask.c:  Main running task
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
#include "BoardSelection.h"

#include "phUcBal.h"
#include "phUcBal_MainTask.h"
#include "phUcBal_Protocol.h"
#include "phUcBal_TransReceive.h"
#include "phUcBal_SECFw.h"
#include "phUcBal_GPIO.h"
#include "phUcBal_Config.h"
#include "phUcBal_MainTask.h"
#include "phUcBal_LoopBack.h"
#include "phUcBal_Version.h"
#include "phUcBal_I18000p3m3.h"
#include "phUcBal_VCOM.h"
#include "phDriver.h"

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

/** @ingroup groupProtocol
 * @{
 */

/** The command received from the Host-PC is filled and used from this structure.
 * See @ref protocolcmd
 */
static
#ifndef __DOXYGEN__
__attribute__ ((aligned(4)))
#endif
phUcBal_Cmd_t gphUcBal_Cmd;


/** The response to be sent the Host-PC is taken from this structure.
 * See @ref protocolrsp
 */
static
#ifndef __DOXYGEN__
__attribute__ ((aligned(4)))
#endif
phUcBal_Rsp_t gphUcBal_Rsp;

/** @} */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#if PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP
static void phUcBal_RfOnOff(void);
#endif

static uint32_t ReadFullBuffer(uint8_t *pRxBuffer, uint32_t dwBufferSize);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/** @ingroup ExecFlow
 * @{ */


void phUcBal_MainTask(void * param)
{
    phUcBal_Cmd_t * const Cmd = &gphUcBal_Cmd;
    phUcBal_Rsp_t * const Rsp = &gphUcBal_Rsp;
    phStatus_t status;

    if ( NULL == gpkphUcBal_Config ) {
        /* Nothing to do actually. By this time,
         * gpkphUcBal_Config should already be pointing
         * to actual structure. */
        __DISABLE_IRQ();
        __WFI();
    }


    /** 1) Initialize USB CDC Library so that we can expose ourselves as a USB Serial Device.
     *     This should be done as soon as possible on boot up. */
    phUcBal_VCOM_Init();
    LED_INIT();

#if PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP
    /** 2) If @ref PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP is enabled,
     *     perform RfOnOff so that there can be a quick feedback of the boot up of the
     *     FW and a working connection between the @ref uchost and @ref dut. */
    if ( gpkphUcBal_Config->ICInitFailed ) {
        LED_RED(1);
    }
    else
    {
        phUcBal_RfOnOff();
    }
#endif

    phUcBal_Config_StoreIRQIsrHandling(IRQIsrHandling_Consume);

    /** 3) Now we enter an Infinite loop.  Wait for packet from @ref pchost and process it. */
    while(1)
    {
        /** 4) If USB is connected then we have something to process, else WFI(). */
        if (phUcBal_VCOM_IsConnected() )
        {
            /** 5) Receive a command buffer from the PC. */
            uint32_t inLength = ReadFullBuffer(Cmd->buf, sizeof(Cmd->buf));

            /** 6) We need at least a packet as big as the header as shown in @ref protocolcmd */
            if ( inLength < PH_UCBAL_PROTOCOL_HEADER_SIZE )
            {
                /* Not handled. Incomplete incoming buffer. But nothing to do. */
                phOsal_ThreadDelay(1);
            }
            else
            {
                LED_ORANGE(1);
                LED_BLUE(0);
                LED_GREEN(1);
                Rsp->header.CLA = Cmd->header.CLA;
                Rsp->header.INS = Cmd->header.INS;
                Rsp->header.Lr = 0;
                status = (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
                /** 7) For a valid frame, dispatch @ref protocolcmd to respective @ref CommandGroups */
                switch(Cmd->header.CLA)
                {
                case CLA_TransReceive:
                    status = phUcBal_TransReceive(Cmd,Rsp);
                    break;
                case CLA_GPIO:
                    status = phUcBal_GPIO(Cmd,Rsp);
                    break;
                case CLA_SECFw:
                    status = phUcBal_SECFw(Cmd,Rsp);
                    break;
                case CLA_Config:
                    status = phUcBal_Config(Cmd,Rsp);
                    break;
                case CLA_LoopBack:
                    status = phUcBal_LoopBack(Cmd,Rsp);
                    break;
                case CLA_Version:
                    status = phUcBal_Version(Cmd,Rsp);
                    break;

#ifdef NXPBUILD__PHHAL_HW_RC663
                case CLA_I18000p3m3:
                    /* ApplyProtocolSettings/Inventory requires CLIF ISR Handling to be enabled on microcontroller */
                    phUcBal_Config_StoreIRQIsrHandling(IRQIsrHandling_Share);

                    status = phUcBal_I18000p3m3(Cmd,Rsp);

                    /* Disabling CLIF ISR Handling after ApplyProtocolSettings */
                    phUcBal_Config_StoreIRQIsrHandling(IRQIsrHandling_Consume);
                    break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

                default:
                    break;
                }
                Rsp->header.Status.status = status;
                /** 8) Give data back to the PC */
                if ( status == PH_ERR_SUCCESS || status == PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL))
                {
                    phUcBal_VCOM_Write(Rsp->buf, Rsp->header.Lr + PH_UCBAL_PROTOCOL_HEADER_SIZE);
                }
                else
                {
                    Rsp->header.Lr = 0;
                    phUcBal_VCOM_Write(Rsp->buf, PH_UCBAL_PROTOCOL_HEADER_SIZE);
                }
                LED_ORANGE(0);
                /** 9) Wait for the next command */
            }
        }
        else
        {
            LED_BLUE(1);
            LED_GREEN(0);
            phOsal_ThreadDelay(10);
        }
    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#if PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP

static void phUcBal_RfOnOff() {
    int32_t count = 5;

    phhalHw_ApplyProtocolSettings(gpphUcBal_PtrHal, PHHAL_HW_CARDTYPE_ISO14443A);

    while (count--) {
        phhalHw_FieldOn(gpphUcBal_PtrHal);
        phOsal_ThreadDelay(70);
        phhalHw_FieldOff(gpphUcBal_PtrHal);
        phOsal_ThreadDelay(70);
    }
}
#endif /* PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP */

/** Read full command buffer from @ref pchost
 *
 * Since the @ref protocolcmd is fixed and pre-determined, ensure that we have received
 * the full frame.
 */
static uint32_t ReadFullBuffer(uint8_t *pRxBuffer, uint32_t dwBufferSize)
{
    uint32_t readIndex = 0;
    int32_t reminingBytes = 0;
    uint32_t readLength = phUcBal_VCOM_Read(pRxBuffer, dwBufferSize);
    if ( readLength == 0)
        return 0;
    if ( readLength >= 6 )
    {
        uint32_t Lc = pRxBuffer[5];
        Lc <<= 8;
        Lc |= pRxBuffer[4];
        reminingBytes = PH_UCBAL_PROTOCOL_HEADER_SIZE + Lc - readLength;
        readIndex = readLength;
    }
    while (reminingBytes > 0 )
    {
        uint32_t readNext = phUcBal_VCOM_Read(&pRxBuffer[readIndex], dwBufferSize - readIndex);
        readIndex += readNext;
        reminingBytes -= readNext;
        readLength += readNext;
    }
    return readLength;
}


/** @} */


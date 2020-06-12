/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Command functions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. March 2010
*
*/
#include <phhalHw.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHHAL_HW_RC663

#include "phhalHw_Rc663.h"
#include "phhalHw_Rc663_Int.h"
#include "phhalHw_Rc663_Config.h"
#include <phhalHw_Rc663_Reg.h>
#include <phhalHw_Rc663_Cmd.h>

/* LPCD NNC implementation depicts the same implementation
    as normal LPCD except for manipulation of I/Q Min/Max thresholds */
phStatus_t phhalHw_Rc663_Cmd_LpcdNNC(
                                  phhalHw_Rc663_DataParams_t * pDataParams
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd;
    uint16_t    PH_MEMLOC_REM wOption;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    //uint32_t    PH_MEMLOC_REM dwReload;
    uint8_t     PH_MEMLOC_REM bRxAnaBackup;
    //uint8_t     PH_MEMLOC_REM bQMin;
    //uint8_t     PH_MEMLOC_REM bQMax;
    //uint8_t     PH_MEMLOC_REM bIMin;
    //uint8_t     PH_MEMLOC_REM bIMax;
    //uint16_t    PH_MEMLOC_REM wDerivative;
    //uint8_t     PH_MEMLOC_REM bVersion;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Configure T4 for AutoLPCD and Autowakeup and to use 16KHz LFO. */
    bRegister = PHHAL_HW_RC663_BIT_T4AUTOLPCD | PHHAL_HW_RC663_BIT_T4AUTOWAKEUP |
        PHHAL_HW_RC663_VALUE_TCLK_LFO_16_KHZ;

    /* Prepare command and option */
    aCmd = PHHAL_HW_RC663_CMD_LPCD;
    wOption = PH_EXCHANGE_DEFAULT;

    /* Prepare wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = PHHAL_HW_RC663_BIT_LPCDIRQ;

    /* Parameter check */
    switch (pDataParams->bMode & (uint8_t)~(uint8_t)PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_MASK)
    {
    case PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT:

        bIrq1WaitFor |= PHHAL_HW_RC663_BIT_TIMER3IRQ;
        break;

    case PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN_GUARDED:

        /* start and wait for timer */
        /* This option is used as fall through mechanism to exit LPCD loop after certain LPCD cycles. */
        wOption |= PHHAL_HW_RC663_RXTX_TIMER_START;
        bIrq1WaitFor |= PHHAL_HW_RC663_BIT_TIMER1IRQ;

    case PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN:

        /* Configure T4 additionally with AutoRestart to start T4 after every RFON during LPCD
         * to continue standby + rfon cycle continuously to detect antenna de-tuning. */
        bRegister |= PHHAL_HW_RC663_BIT_T4AUTORESTARTED;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set standby bit */
    aCmd |= PHHAL_HW_RC663_BIT_STANDBY;

    /* Set AutoTrimm bit if requested */
    if (pDataParams->bMode & PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_TRIMM_LPO)
    {
        bRegister |= PHHAL_HW_RC663_BIT_T4AUTOTRIMM;
    }

    /* Write T4Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, bRegister));

    /* Set LPCD_IRQ_Clr to zero to get correct LPCD measurements period and to avoid not detecting card in first LPCD cycle
     * when Filter is ON. This is also a workaround for HW Artifacts : artf207360, artf207359 and artf207361 of Rc66303 Si. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, 0));

    /* Set Mix2Adc bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, &bRegister));
    bRegister |= PHHAL_HW_RC663_BIT_RX_SHORT_MIX2ADC;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, bRegister));

    /* Backup current RxAna setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, &bRxAnaBackup));
    /* Raise receiver gain to maximum */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, PHHAL_HW_RC663_LPCD_RECEIVER_GAIN));

    /* Perform command */
    status = phhalHw_Rc663_Command_Int(
        pDataParams,
        aCmd,
        wOption,
        bIrq0WaitFor,
        bIrq1WaitFor,
        NULL,
        0,
        0,
        NULL,
        NULL);

    /* Restore RxAna register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, bRxAnaBackup));

    /* Clear Mix2Adc bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, &bRegister));
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_RX_SHORT_MIX2ADC;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, bRegister));

    /* Stop Timer4 if started */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, &bRegister));
    bRegister |= PHHAL_HW_RC663_BIT_T4STARTSTOPNOW;
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_T4RUNNING;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, bRegister));

    /* Stop the LPCD command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_COMMAND,
        PHHAL_HW_RC663_CMD_IDLE));

    /* Command error check (ignore timeout since interface could have been too slow) */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Check if LPCD-IRQ is set */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, &bRegister));

    /* Clear LPCD interrupt source to avoid any spurious LPCD_IRQ to be triggered. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, PHHAL_HW_RC663_BIT_LPCDIRQ_CLR));

    if (!(bRegister & PHHAL_HW_RC663_BIT_LPCDIRQ))
    {
        return PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/* LPCD GetConfig NNC implementation depicts the same implementation
    as normal Lpcd GetConfig except for manipulation of I/Q Min/Max thresholds */
phStatus_t phhalHw_Rc663_Cmd_Lpcd_GetConfigNNC(
                                            phhalHw_Rc663_DataParams_t * pDataParams,
                                            uint8_t * pI,
                                            uint8_t * pQ
                                            )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bDerivative;
    uint8_t     PH_MEMLOC_REM bVersion;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Set Qmin register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMIN, PHHAL_HW_RC663_LPCD_CALIBRATE_QMIN_REG));

    /* Set Qmax register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMAX, PHHAL_HW_RC663_LPCD_CALIBRATE_QMAX_REG));

    /* Set Imin register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_IMIN, PHHAL_HW_RC663_LPCD_CALIBRATE_IMIN_REG));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

    if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
    {
        /* Configure Charge Pump and Filter set by the Application. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_OPTIONS,
            (pDataParams->bLpcdOption & (PHHAL_HW_RC663_BIT_LPCD_CHARGEPUMP | PHHAL_HW_RC663_BIT_LPCD_FILTER))));
    }

    /* Use default mode, trimm lpo and do not permit the function to set I and Q values */
    pDataParams->bMode |= PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_TRIMM_LPO | PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_IGNORE_IQ;

    /* Execute CMD LPCD to get I and Q values to perform LPCD procedure */
    status = phhalHw_Rc663_Cmd_LpcdNNC(pDataParams);

    /* If Derivative is RC663 Plus read I/Q unstable bit. */
    if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_OPTIONS, &bRegister));
        pDataParams->bLpcdOption |= bRegister & (PHHAL_HW_RC663_BIT_LPCD_Q_UNSTABLE | PHHAL_HW_RC663_BIT_LPCD_I_UNSTABLE);
    }

    /* Ignore timeout error */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Retrieve I and Q results */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_I, pI));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, pQ));

    /* Mask out irrelevant bits */
    *pI &= 0x3F;
    *pQ &= 0x3F;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif //NXPBUILD__PHHAL_HW_RC663
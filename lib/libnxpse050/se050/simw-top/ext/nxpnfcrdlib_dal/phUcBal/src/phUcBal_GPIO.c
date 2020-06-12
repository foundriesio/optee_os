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
 * phUcBal_GPIO.c:  GPIO Control for phNxpN
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
#include "phUcBal_GPIO.h"
#include "phUcBal_MainTask.h"
#include "phDriver.h"
#include "BoardSelection.h"
#include "phUcBal_Config.h"
#ifdef NXPBUILD__PHHAL_HW_PN5180
#   include "phhalHw_Pn5180_Reg.h"
#   include "phhalHw_Pn5180_Instr.h"
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

/** @ingroup CommandGroupGPIO
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

#ifdef NXPBUILD__PHHAL_HW_RC663
/** BUSY Pin. Forcefully define it since it is not available in RC663. */
#	define PHDRIVER_PIN_BUSY 0
/** DWL Pin.  Forcefully define it since it is not available in RC663.  */
#	define PHDRIVER_PIN_DWL 0
#endif

/** Translate Enumerated @ref phUcBal_GPIO_Name_t to value expected by DAL(phDriver) */
static const uint32_t gphUcBal_GPIO_Name2phDriver[] = {
    0,  //Dummy value to meet indexing.
    PHDRIVER_PIN_RESET, //PHUCBAL_GPIO_NAME_RESET  0x1U
    PHDRIVER_PIN_IRQ, //PHUCBAL_GPIO_NAME_IRQ    0x2U
    0, //PHUCBAL_GPIO_NAME_SPI    0x3U
    0, //PHUCBAL_GPIO_NAME_I2C    0x4U
    PHDRIVER_PIN_BUSY, //PHUCBAL_GPIO_NAME_BUSY   0x5U
    PHDRIVER_PIN_DWL, //PHUCBAL_GPIO_NAME_DWL    0x6U
    0, //PHUCBAL_GPIO_NAME_IFSEL0 0x7U
    0, //PHUCBAL_GPIO_NAME_IFSEL1 0x8U
    0, //PHUCBAL_GPIO_NAME_AD0    0x9U
    0, //PHUCBAL_GPIO_NAME_AD1    0xAU
    0, //PHUCBAL_GPIO_NAME_AD2    0xBU
};

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/** Set value of a GPIO */
static phStatus_t phUcBal_GPIO_SetV(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Get value of a GPIO */
static phStatus_t phUcBal_GPIO_GetV(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Wait for a particular GPIO to get high */
static phStatus_t phUcBal_GPIO_WaitForHigh(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Wait for a particular GPIO to get high */
static phStatus_t phUcBal_GPIO_WaitForLow(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/**
 * Check whether this is a known GPIO
 *
 * @param gpIO the_gpio
 * @return status of known or unknown GPIO
 */
static bool IsKnownGPIO(phUcBal_GPIO_Name_t gpIO)
{
    switch(gpIO)
    {
    case PHUCBAL_GPIO_NAME_RESET:
    case PHUCBAL_GPIO_NAME_IRQ:
    case PHUCBAL_GPIO_NAME_BUSY:
    case PHUCBAL_GPIO_NAME_DWL:
        return true;
    default:
        return false;
    }
}
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phUcBal_GPIO(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch(Cmd->header.INS)
    {
    PH_UCBAL_CASE(GPIO, SetV);
    PH_UCBAL_CASE(GPIO, GetV);
    PH_UCBAL_CASE(GPIO, WaitForHigh);
    PH_UCBAL_CASE(GPIO, WaitForLow);
    }

    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 * Set value of the GPIO.  The format of the command frame is:
 *
 *   | CLA            | INS                 | P1           | P2       | Lc   | (Command) Payload   |
 *   |----------------|---------------------|--------------|----------|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_SetV  | TheGPIO      | Value    | 0    | Empty               |
 *
 * For enumeration of TheGPIO, phUcBal_GPIO_Name_t
 * Value should be one of PH_PLATFORM_SET_HIGH or PH_PLATFORM_SET_LOW
 *
 * The format of the response frame is:
 *
 *   | CLA            | INS                 | S1           | S2   | Lr   | (Response) Payload  |
 *   |----------------|---------------------|--------------|------|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_SetV  | S1           | S2   | 0    | Empty               |
 *
 * - If GPIO is known, S1=0, S2=0
 * - If GPIO is un-known, S1=PH_ERR_INVALID_PARAMETER, S2=PH_COMP_DRIVER
 */
static phStatus_t phUcBal_GPIO_SetV(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	phUcBal_GPIO_Name_t the_pin = (phUcBal_GPIO_Name_t)Cmd->header.P1;
    uint8_t pinValue = (uint8_t)Cmd->header.P2;
    if (IsKnownGPIO(the_pin))
        if ( pinValue == PH_ON || pinValue == PH_OFF ) {
            phUcBal_GPIO_SetPinValue(the_pin,pinValue);
            return PH_ERR_SUCCESS;
        }
    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}


/**
 * Get value of the GPIO.  The format of the command frame is:
 *
 *   | CLA            | INS                 | P1           | P2   | Lc   | (Command) Payload   |
 *   |----------------|---------------------|--------------|------|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_GetV  |TheGPIO       | 0    | 0    | Empty               |
 *
 * For enumeration of TheGPIO, see phUcBal_GPIO_Name_t
 *
 * The format of the response frame is:
 *
 *   | CLA            | INS                 | S1   | S2   | Lr   | (Response) Payload  |
 *   |----------------|---------------------|------|------|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_GetV  | 0    | 0    | 1    | Value               |
 *
 * - If GPIO is known, S1=0, S2=0
 * - If GPIO is un-known, S1=PH_ERR_INVALID_PARAMETER, S2=PH_COMP_DRIVER
 */
static phStatus_t phUcBal_GPIO_GetV(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	phUcBal_GPIO_Name_t the_pin = (phUcBal_GPIO_Name_t)Cmd->header.P1;
    bool pinValue;
    if (IsKnownGPIO(the_pin))
    {
        if ( PHUCBAL_GPIO_NAME_IRQ == the_pin)
        {
            Rsp->header.Lr = 1;
#ifdef NXPBUILD__PHHAL_HW_PN5180
            if ( gpkphUcBal_Config->IRQPollStrategy == IRQHandling_ReadRegsiter )
            {
                uint32_t irq_status = 0;
                phhalHw_Pn5180_Instr_ReadRegister(gpphUcBal_PtrHal, IRQ_STATUS, &irq_status);
                if ( irq_status == 0xFFFFFFFFU || irq_status == 0)
                    Rsp->header.Response[0] = 0;
                else
                    Rsp->header.Response[0] = 1;
            }
            else
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
            {
                Rsp->header.Response[0] = gpkphUcBal_Config->u32IrqPinValue;
                phUcBal_Config_StoreIRQPinValue(0);
            }
            return PH_ERR_SUCCESS;
        }
        else
        {
            pinValue = phUcBal_GPIO_GetPinValue(the_pin);
            Rsp->header.Lr=1;
            Rsp->header.Response[0] = pinValue;
            return PH_ERR_SUCCESS;
        }
    }
    else
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}


/**
 * Wait for a GPIO to turn low.  The format of the command frame is:
 *
 *   | CLA            | INS                        | P1     | P2   | Lc   | (Command) Payload   |
 *   |----------------|----------------------------|--------|------|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_WaitForHigh  |TheGPIO | 0    | 0    | Empty               |
 *
 * For enumeration of TheGPIO, see phUcBal_GPIO_Name_t
 *
 * The format of the response frame is:
 *
 *   | CLA            | INS                         | S1 | S2 | Lr   | (Response) Payload  |
 *   |----------------|-----------------------------|----|----|------|---------------------|
 *   | \ref CLA_GPIO  | \ref GPIO_INS_WaitForHigh   | *  | *  | 0    | Empty               |
 *
 * - If GPIO is un-known, S1=PH_COMP_BAL, S2=PH_ERR_INVALID_PARAMETER
 *
 * @warning this function does not handle Timer/Timeouts as of now.
 *
 */
static phStatus_t phUcBal_GPIO_WaitForHigh(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    phUcBal_GPIO_Name_t the_pin = (phUcBal_GPIO_Name_t)Cmd->header.P1;
    bool pinValue = false;
    if (IsKnownGPIO(the_pin)) {
        while ( pinValue == false )
        {
            pinValue = phUcBal_GPIO_GetPinValue(the_pin);
        }
        /* FIXME - Handle Timeout */
        return PH_ERR_SUCCESS;
    }
    else
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

/**
 * Same as @ref phUcBal_GPIO_WaitForHigh. But instead wait for that GPIO to turn low
 *
 * @warning this function does not handle Timer/Timeouts as of now.
 */
static phStatus_t phUcBal_GPIO_WaitForLow(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    phUcBal_GPIO_Name_t the_pin = (phUcBal_GPIO_Name_t)Cmd->header.P1;
    bool pinValue = true;
    if (IsKnownGPIO(the_pin)) {
        while ( pinValue == true )
        {
            pinValue = phUcBal_GPIO_GetPinValue(the_pin);
        }
        /* FIXME - Handle Timeout */
        return PH_ERR_SUCCESS;
    }
    else
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

/** @} */


phStatus_t phUcBal_GPIO_EnableIRQInterrupts() {
    phDriver_Pin_Config_t pinCfg;
	pinCfg.bOutputLogic = PH_DRIVER_SET_LOW;
	pinCfg.bPullSelect = PHDRIVER_PIN_IRQ_PULL_CFG;

	pinCfg.eInterruptConfig = PIN_IRQ_TRIGGER_TYPE;
	phDriver_PinConfig(PHDRIVER_PIN_IRQ, PH_DRIVER_PINFUNC_INTERRUPT, &pinCfg);

	NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(EINT_IRQn);
	NVIC_EnableIRQ(EINT_IRQn);

	return PH_ERR_SUCCESS;
}

void phUcBal_GPIO_SetPinValue(phUcBal_GPIO_Name_t bGPIO, uint8_t bVal) {
	phDriver_PinWrite(gphUcBal_GPIO_Name2phDriver[bGPIO],
			bVal);
}

bool phUcBal_GPIO_GetPinValue(phUcBal_GPIO_Name_t bGPIO) {
	return phDriver_PinRead(gphUcBal_GPIO_Name2phDriver[bGPIO],
			PH_DRIVER_PINFUNC_INPUT);
}

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
 * phUcBal_GPIO.h:  APIs for GPIO access in phUcBal
 *
 * Project:  NXP NFC Cockpit
 *
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

#ifndef PHUCBAL_GPIO_H
#define PHUCBAL_GPIO_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include "phUcBal_Protocol.h"

/** @ingroup CommandGroupGPIO
 * @{
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */



/** Instructions for @ref CommandGroupGPIO */

enum phUcBal_GPIO_INS {
    /** [\b 0x05]: Set Value of a GPIO. '5'et */
    GPIO_INS_SetV = 0x05,
    /** [\b 0x0E]: Get Value of a GPIO. g'E't */
    GPIO_INS_GetV = 0x0E,
    /** [\b 0xA1]: Wait till given GPIO Turns High.  w'A'it for '1' */
    GPIO_INS_WaitForHigh = 0xA1,
    /** [\b 0xA0]: Wait till given GPIO Turns Low.  w'A'it for '0' */
    GPIO_INS_WaitForLow = 0xA0
};

/**
 * Enumerated Abstraction of various PINs between Host and Frontend.
 */
typedef enum phUcBal_GPIO_Name
{
    PHUCBAL_GPIO_NAME_RESET              = 0x1U,    /**< Reset the Frontend  */
    PHUCBAL_GPIO_NAME_IRQ                = 0x2U,    /**< Interrupt from the Frontend to Host */
    //PHUCBAL_GPIO_NAME_SPI                = 0x3U,    /**< SPI Pins of the Host */
    //PHUCBAL_GPIO_NAME_I2C                = 0x4U,    /**< I2C Pins of the Host */
    PHUCBAL_GPIO_NAME_BUSY               = 0x5U,    /**< Frontend's Busy Status, if Applicable */
    PHUCBAL_GPIO_NAME_DWL                = 0x6U,    /**< Assert Download mode of Frontend, if Applicable */
    //PHUCBAL_GPIO_NAME_IFSEL0             = 0x7U,    /**< IFSEL0 of the Frontend, if Applicable */
    //PHUCBAL_GPIO_NAME_IFSEL1             = 0x8U,    /**< IFSEL1 of the Frontend, if Applicable */
    //PHUCBAL_GPIO_NAME_AD0                = 0x9U,    /**< Address Select Pin of Frontend, if Applicable */
    //PHUCBAL_GPIO_NAME_AD1                = 0xAU,    /**< Address Select Pin of Frontend, if Applicable */
    //PHUCBAL_GPIO_NAME_AD2                = 0xBU     /**< Address Select Pin of Frontend, if Applicable */
} phUcBal_GPIO_Name_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupGPIO */
phStatus_t phUcBal_GPIO(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Enable IRQ Interrupts on the GPIOs */
phStatus_t phUcBal_GPIO_EnableIRQInterrupts(void);

/**
 * Set a particular GPIO high or low
 *
 * @param bGpio the GPIO
 * @param bVal 0 for low. 1 for high
 */
void phUcBal_GPIO_SetPinValue(phUcBal_GPIO_Name_t bGpio, uint8_t bVal);

/**
 * Get the value of a particular GPIO
 *
 * @param bGpio the GPIO
 * @return The status of the GPIO
 *
 * @retval true If GPIO is high
 * @retval false If GPIO is low
 */
bool phUcBal_GPIO_GetPinValue(phUcBal_GPIO_Name_t bGpio);

#endif /* PHUCBAL_GPIO_H */
/** @} */

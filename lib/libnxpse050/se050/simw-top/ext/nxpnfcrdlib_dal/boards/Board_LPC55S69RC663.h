/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/

#ifndef BOARD_LPC55S69RC663_H
#define BOARD_LPC55S69RC663_H

#include <board.h>
#include "fsl_ctimer.h"
#include "fsl_pint.h"
#include "fsl_gpio.h"
#include "fsl_flexcomm.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_iocon.h"
//#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#define PORT0               0          /**< Port0. */
#define PORT1               1          /**< Port1. */
#define PORT2               2          /**< Port2. */
#define PORT3               3          /**< Port3. */
#define PORT4               4          /**< Port4. */

/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
//#define PHDRIVER_PIN_RESET         ((PORT0<<8) | 1)    /**< Reset pin, Port 2, pin5. */
#define PHDRIVER_PIN_RESET         ((PORT0<<8) | 15)
#define PHDRIVER_PIN_IRQ           ((PORT1<<8) | 10)   /**< Interrupt pin from Frontend to Host, Port2, pin12 */
/*#define PHDRIVER_PIN_BUSY          ((PORT2<<8) | 11)   *< Frontend's Busy Status, Port2, pin11
#define PHDRIVER_PIN_DWL           ((PORT0<<8) | 21)   *< Download mode pin of Frontend, Port0, pin21

#define PHDRIVER_PIN_RLED          ((PORT2<<8) | 0)    *< RED LED, Port2, pin0, Pin function 0
#define PHDRIVER_PIN_GLED          ((PORT3<<8) | 25)   *< GREEN LED, Port3, pin25, Pin function 0 */


//#define PHDRIVER_PIN_IF0	((PORT0 << 8) | 18)	/**< IFSEL0 pin, Pin9, GPIO1  */
//#define PHDRIVER_PIN_IF1	((PORT0 << 8) | 19) 	/**< IFSEL1 pin, Pin10, GPIO1  */
/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_DOWN
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_UP
/*
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP
*/
#define PHDRIVER_PIN_NSS_PULL_CFG      PH_DRIVER_PULL_UP


/******************************************************************
 * IRQ PIN NVIC settings
 ******************************************************************/
#define PIN_IRQ_TRIGGER_TYPE    PH_DRIVER_INTERRUPT_FALLINGEDGE  /**< IRQ pin Falling edge interrupt */
#define EINT_PRIORITY           6                /**< Interrupt priority. */
#define EINT_IRQn               PIN_INT0_IRQn       /**< NVIC IRQ */

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL  PH_DRIVER_SET_HIGH
#define RESET_POWERUP_LEVEL    PH_DRIVER_SET_LOW


/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#define LPC_SSP             SPI7   /**< SPI Module */
#define SSP_CLOCKRATE      CLOCK_GetFlexCommClkFreq(7)   /**< SPI clock rate. Allowed clock rate: 6 or 4 or 3 or 2.4MHz etc. */
/*****************************************************************
 * I2C Configuration
 ****************************************************************/
#define LPC_I2C             I2C4   /**< SPI Module */
#define I2c_CLOCKRATE      CLOCK_GetFreq(kCLOCK_Flexcomm4)   /**< SPI clock rate. Allowed clock rate: 6 or 4 or 3 or 2.4MHz etc. */
#define	I2C_MASTER_CLOCK_FREQUENCY	(12000000)
#define CLRC663_SLAVE_ADDRESS	 0x28



//#define PHDRIVER_PIN_SCK    ((IOCON_PIO_FUNC5<<16) | (PORT1<<8) | 21)  /*< I2C_SCL pin, Port1, pin20, Pin function 5*/
//#define PHDRIVER_PIN_SDA    ((IOCON_PIO_FUNC5<<16) | (PORT1<<8) | 20)  //*< I2C_SDA, Port1, pin21, Pin function 5
 /******************************************************************/
/* Pin configuration format : Its a 32 bit format where 1st 3 bytes represents each field as shown below.
 * | Byte3 | Byte2 | Byte1 | Byte0 |
 * | ---   | FUNC  | PORT  | PIN   |
 * */
#define PHDRIVER_PIN_MOSI     ((IOCON_PIO_FUNC7<<16) | (PORT0<<8) | 20)  // MOSI pin, Port0, pin18, Pin function 2
#define PHDRIVER_PIN_MISO     ((IOCON_PIO_FUNC7<<16) | (PORT0<<8) | 19)  // MISO pin, Port0, pin17, Pin function 2
#define PHDRIVER_PIN_SCK      ((IOCON_PIO_FUNC7<<16) | (PORT0<<8) | 21)  // Clock pin, Port0, pin15, Pin function 2
#define PHDRIVER_PIN_SSEL     ((IOCON_PIO_FUNC1<<16) | (PORT1<<8) | 11)  // Slave select, Port0, pin16, Pin function 0


/*****************************************************************
 * Timer Configuration
 ****************************************************************/
#define PH_DRIVER_LPC_TIMER                    CTIMER0           				/**< Use LPC timer0 */
#define PH_DRIVER_LPC_TIMER_FREQ               CLOCK_GetFreq(kCLOCK_CTmier0)  /**< Timer 0 clock FREQ */
#define PH_DRIVER_LPC_TIMER_MATCH_REGISTER     0x00 						 /* use match register 1 always. */
#define PH_DRIVER_LPC_TIMER_IRQ                CTIMER0_IRQn         		 /**< NVIC Timer0 Irq */
void PH_DRIVER_LPC_TIMER_IRQ_HANDLER(void);          						 /**< Timer0 Irq Handler */
#define PH_DRIVER_LPC_TIMER_IRQ_PRIORITY       5                   			 /**< NVIC Timer0 Irq priority */

#endif /* BOARD_LPC1769PN5180_H */

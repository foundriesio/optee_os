/*
 * Board_IMXRT1050Rc663.h
 *
 *  Created on: Aug 22, 2018
 *      Author: nxf48223
 */


/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
*
*/

#ifndef DAL_BOARDS_BOARD_IMXRT1050RC663_H_
#define DAL_BOARDS_BOARD_IMXRT1050RC663_H_



#define GPIO_PORT_A         1    /* Same macro for GPIOA/PORTA */
#define GPIO_PORT_B         2   /* Same macro for GPIOB/PORTB */
#define GPIO_PORT_C         3    /* Same macro for GPIOC/PORTC */
#define GPIO_PORT_D         4   /* Same macro for GPIOD/PORTD */
#define GPIO_PORT_E         5    /* Same macro for GPIOE/PORTE */

/******************************************************************
 * LPSPI clock configuration
 ******************************************************************/

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI_MASTER_CLOCK_FREQ LPSPI_CLOCK_FREQ


/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
/* Pin configuration format : Its a 32 bit format where every byte represents a field as shown below.
 * | Byte3 | Byte2 | Byte1      | Byte0 |
 * |  --   |  --   | GPIO/PORT  | PIN   |
 * */
#define PHDRIVER_PIN_RESET  ((GPIO_PORT_A << 8) | 10)  /**< Reset pin, Pin11, GPIO1  */
#define PHDRIVER_PIN_IRQ    ((GPIO_PORT_A << 8) | 9)   /**< IRQ pin, Pin18, GPIO1   */
/*

#define PHDRIVER_PIN_IF0	((GPIO_PORT_A << 8) | 9)	*< IFSEL0 pin, Pin9, GPIO1
#define PHDRIVER_PIN_IF1	((GPIO_PORT_A << 8) | 10) 	*< IFSEL1 pin, Pin10, GPIO1
*/

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_DOWN
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_NSS_PULL_CFG      PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ PIN NVIC settings
 ******************************************************************/
#define EINT_IRQn                 GPIO1_Combined_0_15_IRQn
#define EINT_PRIORITY             8
#define CLIF_IRQHandler           GPIO1_Combined_0_15_IRQHandler
#define PIN_IRQ_TRIGGER_TYPE      PH_DRIVER_INTERRUPT_FALLINGEDGE

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL         PH_DRIVER_SET_HIGH
#define RESET_POWERUP_LEVEL           PH_DRIVER_SET_LOW


/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#define PHDRIVER_IMX_SPI_POLLING          /* Enable to perform SPI transfer using polling method. */
#define PHDRIVER_IMX_SPI_MASTER           LPSPI1
#define PHDRIVER_IMX_SPI_DATA_RATE        5000000U
#define PHDRIVER_IMX_SPI_CLK_SRC          LPSPI_MASTER_CLOCK_FREQ
#define PHDRIVER_IMX_SPI_IRQ              LPSPI1_IRQn

#define LPSPI_IRQ_PRIORITY     7



#define PHDRIVER_PIN_SSEL     ((GPIO_PORT_C  << 8) | 13 )  /**< Reset pin, */ //Chip select pin 13 gpio3

/*****************************************************************
 * Timer Configuration
 ****************************************************************/
#define PH_DRIVER_IMX_PIT_TIMER          PIT
#define PH_DRIVER_IMX_PIT_CLK            kCLOCK_OscClk
#define PH_DRIVER_IMX_TIMER_CHANNEL      kPIT_Chnl_0    /**< PIT channel number 0 */
#define PH_DRIVER_IMX_TIMER_NVIC         PIT_IRQn
#define PH_DRIVER_IMX_TIMER_PRIORITY     8






#endif /* DAL_BOARDS_BOARD_IMXRT1050RC663_H_ */

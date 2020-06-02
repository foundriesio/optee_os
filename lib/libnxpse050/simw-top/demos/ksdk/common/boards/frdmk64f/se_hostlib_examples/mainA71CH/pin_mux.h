/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTA12 (number 42), U13[12]/RMII0_RXD_1
  @{ */
#define BOARD_RMII0_RXD1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD1_PIN 12U    /*!<@brief PORTA pin index: 12 */
                                    /* @} */

/*! @name PORTA13 (number 43), U13[13]/RMII0_RXD_0
  @{ */
#define BOARD_RMII0_RXD0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD0_PIN 13U    /*!<@brief PORTA pin index: 13 */
                                    /* @} */

/*! @name PORTA14 (number 44), U13[15]/RMII0_CRS_DV
  @{ */
#define BOARD_RMII0_CRS_DV_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_CRS_DV_PIN 14U    /*!<@brief PORTA pin index: 14 */
                                      /* @} */

/*! @name PORTA15 (number 45), U13[19]/RMII0_TXEN
  @{ */
#define BOARD_RMII0_TXEN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXEN_PIN 15U    /*!<@brief PORTA pin index: 15 */
                                    /* @} */

/*! @name PORTA16 (number 46), U13[20]/RMII0_TXD0
  @{ */
#define BOARD_RMII0_TXD0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD0_PIN 16U    /*!<@brief PORTA pin index: 16 */
                                    /* @} */

/*! @name PORTA17 (number 47), U13[21]/RMII0_TXD1
  @{ */
#define BOARD_RMII0_TXD1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD1_PIN 17U    /*!<@brief PORTA pin index: 17 */
                                    /* @} */

/*! @name PORTA5 (number 39), U13[17]/RMII0_RXER
  @{ */
#define BOARD_RMII0_RXER_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXER_PIN 5U     /*!<@brief PORTA pin index: 5 */
                                    /* @} */

/*! @name PORTB0 (number 53), U13[10]/RMII0_MDIO
  @{ */
#define BOARD_RMII0_MDIO_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_RMII0_MDIO_PIN 0U     /*!<@brief PORTB pin index: 0 */
                                    /* @} */

/*! @name PORTB1 (number 54), U13[11]/RMII0_MDC
  @{ */
#define BOARD_RMII0_MDC_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_RMII0_MDC_PIN 1U     /*!<@brief PORTB pin index: 1 */
                                   /* @} */

/*! @name PORTB16 (number 62), U7[4]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                       /* @} */

/*! @name PORTB17 (number 63), U10[1]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                       /* @} */

/*! @name PORTC16 (number 90), J1[2]
  @{ */
#define BOARD_TMR_1588_0_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_TMR_1588_0_PIN 16U    /*!<@brief PORTC pin index: 16 */
                                    /* @} */

/*! @name PORTC17 (number 91), J1[4]
  @{ */
#define BOARD_TMR_1588_1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_TMR_1588_1_PIN 17U    /*!<@brief PORTC pin index: 17 */
                                    /* @} */

/*! @name PORTE24 (number 31), J2[20]/U8[4]/I2C0_SCL
  @{ */
#define BOARD_ACCEL_SCL_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_SCL_PIN 24U    /*!<@brief PORTE pin index: 24 */
                                   /* @} */

/*! @name PORTE25 (number 32), J2[18]/U8[6]/I2C0_SDA
  @{ */
#define BOARD_ACCEL_SDA_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_SDA_PIN 25U    /*!<@brief PORTE pin index: 25 */
                                   /* @} */

/*! @name PORTB21 (number 67), D12[3]/LEDRGB_BLUE
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_BLUE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_BLUE_PIN 21U    /*!<@brief PORTB pin index: 21 */
                                  /* @} */

/*! @name PORTB22 (number 68), D12[1]/LEDRGB_RED
  @{ */
#define BOARD_LED_RED_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 22U    /*!<@brief PORTB pin index: 22 */
                                 /* @} */

/*! @name PORTE26 (number 33), J2[1]/D12[4]/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_GREEN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 26U    /*!<@brief PORTE pin index: 26 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

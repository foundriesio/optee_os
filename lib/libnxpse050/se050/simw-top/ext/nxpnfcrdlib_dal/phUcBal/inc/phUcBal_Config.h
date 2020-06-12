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
 * phUcBal_Config.h:  Configurations and settings for the phUcBal
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

#ifndef PHUCBAL_CONFIG_H
#define PHUCBAL_CONFIG_H


/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include "phUcBal_Protocol.h"



/** Macro to Disable/don't expose features for future extension, not available in this build.
 *
 * This enables for future extension.
 */
#define PH_UCBAL_CONFIG_FUTURE_EXTENSION  (0)


/** @ingroup CommandGroupConfig
 * @{
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 *
 * Instructions for Configuration
 *
 */
enum phUcBal_Config_INS
{
    /** [\b 0xB5] Strategy to wait before TX.
     *
     * (from @ref uchost to @ref dut.) 'B'efore '5'end to DUT */
    Config_INS_WaitBeforeTX_Strategy = 0xB5,
    /** [\b 0xBE] Strategy to wait before RX.
     *
     *  (By @ref uchost from @ref dut.) 'B'efore R'E'ceive from DUT */
    Config_INS_WaitBeforeRX_Strategy = 0xBE,

#ifdef NXPBUILD__PHHAL_HW_PN5180
    /* 1'R'q Poll Strategy
     *
     * In case of PN5180, if Test Bus is enabled, read IRQ_STATUS
     * register.  Else, wait for IRQ.
     */
    Config_INS_ConfigIRQPollStrategy = 0x14,

    Config_INS_WaitIRQDelayWithTestBus = 0x15,
#endif

#if PH_UCBAL_CONFIG_FUTURE_EXTENSION == 1
    /** [\b 0x01] Timeout for Rx from DUT */
    Config_INS_RxTimeoutMs           = 0x01,
    /** [\b 0x02] When using @ref GPIO_INS_WaitForHigh, how much time to wait */
    Config_INS_GPIOWaitTimeoutMs     = 0x02,
#endif /* PH_UCBAL_CONFIG_FUTURE_EXTENSION */

#ifdef NXPBUILD__PHHAL_HW_RC663
    /* Get '1'rq Typ'E' */
    Config_INS_GetIrqType = 0x1E,

	/* Configure '5'elect 'C'ommand */
    Config_INS_SelectCommand = 0x5C,

	/* Configure '5'elect Command '1'ength */
    Config_INS_SelectCommandLength = 0x51,

	/* Configure Number of Valid 'B'its in '1'ast byte of SelCmd */
    Config_INS_NumValidBitsinLastByte = 0xB1,

    /* Configure 'B'eginRound 'C'ommand */
    Config_INS_BeginRoundCommand = 0xBC,

	/* Configure Time '5'lot processing 'B'ehavior */
    Config_INS_TSprocessing = 0x5B,
#endif

};

/**
 * @enum phUcBal_Config_WaitBefore
 *
 * Strategies to wait before communication between @ref uchost and @ref dut.
 *
 * Possible values for @ref phUcBal_Config_WaitBeforeRX_Strategy and @ref phUcBal_Config_WaitBeforeTX_Strategy.
 *
 * @var phUcBal_Config_WaitBefore::WaitBefore_Immediate
 * @var phUcBal_Config_WaitBefore::WaitBefore_WaitForIRQHigh
 * @var phUcBal_Config_WaitBefore::WaitBefore_WaitForBusyLow
 *
 */
enum phUcBal_Config_WaitBefore
{
    /** [\b 0x01] Just go and do RX.
     *
     * '1'mmediate. */
    WaitBefore_Immediate = 0x01,
    /** [\b 0x11] Wait for the IRQ Pin to go High.
     *
     * 'I'RQ '1' High. */
    WaitBefore_WaitForIRQHigh = 0x11, /**<  */
    /** [\b 0x50] Wait for the Busy Pin to go Low. */
    WaitBefore_WaitForBusyLow = 0x50, /**< Bu'5'y l'0'w  */
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION == 1
    /** [\b 0xA1] Wait for timeout as set by @ref phUcBal_Config_RxTimeoutMs. W'A'it for T'i'meout */
    WaitBefore_WaitForTimeoutMs = 0xA1,
#endif /* PH_UCBAL_CONFIG_FUTURE_EXTENSION */
};

/**
 * What do for CLIF IRQ IRQ.
 *
 * Only when @ref CommandGroupSECFw is running, HAL on @ref uchost  would need this information.
 * Else, this IRQ is not of any purpose to hal on @ref uchost.
 *
 * By default only share this with HAL Running on @ref pchost  */

enum phUcBal_Config_IRQIsrHandling
{
    /** Consume the IRQ Status and do not share with HAL Running on the @ref uchost.
     *
     * If the HAL Running on @ref pchost needs it, it will query for this status. */
    IRQIsrHandling_Consume = 0xC0,
    IRQIsrHandling_Share = 0x5A, /**< Share the IRQ Status with HAL on @ref uchost. 'S'hare with h'A'L */
};

#ifdef NXPBUILD__PHHAL_HW_PN5180

/**
 * What do for IRQ Pin for PN5180.
 *
 * If IRQ Pin is used for Test Bus, we can not use interrupts.
 * So, we invoke Read Register of IRQ_STATUS Register.
 *
 * @sa Config_INS_ConfigIRQPollStrategy
 *
 */

enum phUcBal_Config_IRQPollStrategy
{
    /** Use IRQ Interrupts */
    IRQHandling_UseInterrupts = 0x01,
    /** Check test bus, and set to IRQHandling_UseInterrupts or IRQHandling_ReadRegsiter */
    IRQHandling_CheckTestBus = 0x0B,
    IRQHandling_ReadRegsiter = 0x44,
};

void phUcBal_PN5180Irq_DisableIRQ(void);

#endif /* NXPBUILD__PHHAL_HW_PN5180 */

/**
 * @enum phUcBal_Config_TSProcessing
 *
 * Strategies to process the TimeSlot behavior.
 *
 * Possible values for @ref phUcBal_Config_TSProcessing.
 *
 * @var phUcBal_Config_TSProcessing::TSProcessing_Get_Max_Resps
 * @var phUcBal_Config_TSProcessing::TSProcessing_One_Ts_Only
 * @var phUcBal_Config_TSProcessing::TSProcessing_Get_Tag_Handle
 *
 */

#ifdef NXPBUILD__PHHAL_HW_RC663
enum phUcBal_Config_TSProcessing
{
	/* Gets responses from more than one slots limited by the RX buffer size and number of slots. */
	TSProcessing_Get_Max_Resps,
	/* Gets response for only one time slot */
	TSProcessing_One_Ts_Only,
	/* Gets response for only one time slot. Also sends a ReqRN to get the tag handle for this slot. */
	TSProcessing_Get_Tag_Handle,
};
#endif

/** Runtime changeable configuration values */
typedef struct  {
#if PH_UCBAL_CONFIG_FUTURE_EXTENSION == 1
    uint32_t RxTimeoutMs;
    uint16_t GPIOWaitTimeoutMs;
#endif /* PH_UCBAL_CONFIG_FUTURE_EXTENSION */
    enum phUcBal_Config_WaitBefore WaitBeforeTX_Strategy; /**< Wait before TX. @sa phUcBal_Config_WaitBefore */
    enum phUcBal_Config_WaitBefore WaitBeforeRX_Strategy; /**< Wait before RX. @sa phUcBal_Config_WaitBefore */
    enum phUcBal_Config_IRQIsrHandling IRQIsrHandling;
#ifdef NXPBUILD__PHHAL_HW_PN5180
    enum phUcBal_Config_IRQPollStrategy IRQPollStrategy;
#endif
    /** Get IRQ Value based on IRQ Call Back */
    volatile uint32_t u32IrqPinValue;

#ifdef NXPBUILD__PHHAL_HW_RC663
    uint8_t GetIrqType; /**< GetIrqType Command*/
    uint8_t SelectCommand[39]; /**< Select Command*/
    uint16_t SelectCommandLength; /**< Select Command Length*/
    uint8_t NumValidBitsinLastByte; /**< Number of ValidBits in Last Byte of Select Command*/
    uint8_t BeginRoundCommand[3]; /**< Begin Round Command*/
    enum phUcBal_Config_TSProcessing TSProcessingStrategy; /**< Time Slot processing behavior*/
#endif

    /** For cases when we are not able to talk to IC, e.g. previous interrupted FW Download */
    uint32_t ICInitFailed;
} phUcBal_Config_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/**
 * Globally set / configured values.
 *
 * Constant, so that compiler would warn if anybody is accidently modifying this.
 * Pointer to @ref gphphUcBal_Config.
 */
extern const phUcBal_Config_t * gpkphUcBal_Config;

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupConfig */
phStatus_t phUcBal_Config(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Initialize values for gkpphBal_Config.
 *
 * Rather than initializing it as part of ZI/BBS/Data section,
 * it makes sense to initialize it with a specific function.
 */
void phUcBal_Config_Init(void);

/** NxpNfcRdLib and phPlatform layer stores information of IRQ Pin uniquely.
 *
 * By desgin, IRQ Pin value denotes whether an IRQ is yet to be processed and
 * not whether the IRQ Pin is high or low.  Since @ref dut HAL is instantiated both in @ref pchost
 * and @ref uchost, we need to store it in case the @ref pchost asks about that
 * information
 *
 * @param newIRQValue The new value of IRQ. Whether set or not.
 */
void phUcBal_Config_StoreIRQPinValue(uint32_t newIRQValue);

/** See @ref phUcBal_Config_IRQIsrHandling */
void phUcBal_Config_StoreIRQIsrHandling(enum phUcBal_Config_IRQIsrHandling irq_handling);

/** Share state that the IC Init has failed
 *
 * For example in the case of previous interrupted Secure FW Upgrade of
 * PN5180 IC.
 */
void phUcBal_Config_StoreICInitFailed(uint32_t v);

/** @} */

#endif /* PHUCBAL_CONFIG_H */

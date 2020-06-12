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



#include <board.h>

#include "fsl_inputmux_connections.h"

#include "BoardSelection.h"
#include "fsl_i2c.h"
#include "fsl_inputmux.h"
#include "fsl_ctimer.h"
#include "phDriver.h"

#define LPC_TIMER_MAX_32BIT            0xFFFFFFFFU
#define LPC_TIMER_MAX_32BIT_W_PRE      0xFFFFFFFE00000000U /* max 32bit x 32bit. */

#define PINT_PIN_INT0_IRQ kINPUTMUX_GpioPort1Pin14ToPintsel

/* Array of function pointers for callback for each channel */
ctimer_callback_t ctimer_callback_table[] = { (ctimer_callback_t)PH_DRIVER_LPC_TIMER_IRQ_HANDLER, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

static pphDriver_TimerCallBck_t pTimerIsrCallBack;
static volatile uint32_t dwTimerExp;

#ifdef PH_PLATFORM_HAS_ICFRONTEND
extern void CLIF_IRQHandler(void);
#endif

static void phDriver_TimerIsrCallBack(void);

phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack)
{
	 ctimer_match_config_t matchConfig0;
	 ctimer_config_t config;
    uint64_t qwTimerCnt;
    uint32_t dwPrescale;
    uint32_t dwTimerFreq;


    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);


    dwTimerFreq = CLOCK_GetCTimerClkFreq(0);

    /* Timer count = (delay * freq)/Units. */
    qwTimerCnt = dwTimerFreq;
    qwTimerCnt = (qwTimerCnt / eTimerUnit);
    qwTimerCnt = (dwTimePeriod * qwTimerCnt);

    if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_32BIT_W_PRE)
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    if(pTimerCallBack == NULL)
    {    /* Timer Start is blocking call. */
        dwTimerExp = 0;
        pTimerIsrCallBack = phDriver_TimerIsrCallBack;
    }else
    {   /* Call the Timer callback. */
        pTimerIsrCallBack = pTimerCallBack;
    }



    CTIMER_GetDefaultConfig(&config);
     /* 32-bit timers, check prescale is required. */
    if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_32BIT)
    {
        /* prescale is required. */
        for(dwPrescale=2;(dwPrescale<=LPC_TIMER_MAX_32BIT)&&((qwTimerCnt/dwPrescale)>LPC_TIMER_MAX_32BIT);
                dwPrescale++);

        qwTimerCnt /= dwPrescale;

        /* Setup 16-bit prescale value to extend range */
        config.prescale=dwPrescale;
        //Chip_TIMER_PrescaleSet(PH_DRIVER_LPC_TIMER, dwPrescale);
    }
    /* Initialize 16-bit timer 0 clock */


    CTIMER_Init(PH_DRIVER_LPC_TIMER, &config);

    /* Resets the timer terminal and prescale counts to 0 */
       CTIMER_Reset(PH_DRIVER_LPC_TIMER);



        /* Configuration  */
            matchConfig0.enableCounterReset = false;
            matchConfig0.enableCounterStop = true;
            matchConfig0.matchValue = qwTimerCnt;
            matchConfig0.outControl = kCTIMER_Output_Toggle;
            matchConfig0.outPinInitState = false;
            matchConfig0.enableInterrupt = true;


        CTIMER_RegisterCallBack(PH_DRIVER_LPC_TIMER, &ctimer_callback_table[0], kCTIMER_SingleCallback);
        CTIMER_SetupMatch(PH_DRIVER_LPC_TIMER, PH_DRIVER_LPC_TIMER_MATCH_REGISTER, &matchConfig0);


    /* Start timer */

    CTIMER_StartTimer(PH_DRIVER_LPC_TIMER);

    if(pTimerCallBack == NULL)
    {
        /* Block until timer expires. */
        while(!dwTimerExp);
    }

    phDriver_TimerStop();
    return PH_DRIVER_SUCCESS;
}

phStatus_t phDriver_TimerStop(void)
{



    CTIMER_DisableInterrupts(PH_DRIVER_LPC_TIMER, kCTIMER_Match1InterruptEnable);
    CTIMER_Deinit(PH_DRIVER_LPC_TIMER);
    /* Disable timer interrupt */
    NVIC_DisableIRQ(PH_DRIVER_LPC_TIMER_IRQ);

    return PH_DRIVER_SUCCESS;
}


static phStatus_t phDriver_PinConfigInterrupt(uint8_t bPortNum, uint8_t bPinNum, phDriver_Pin_Config_t *pPinConfig)
{



    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, kINPUTMUX_GpioPort1Pin10ToPintsel);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt1, kINPUTMUX_GpioPort1Pin18ToPintsel);
   /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

        PINT_Init(PINT);

    switch(pPinConfig->eInterruptConfig)
    {
    case PH_DRIVER_INTERRUPT_RISINGEDGE:

    	PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge,(pint_cb_t)CLIF_IRQHandler);
        break;

    case PH_DRIVER_INTERRUPT_FALLINGEDGE:

    	PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge,(pint_cb_t)CLIF_IRQHandler);
        break;

    case PH_DRIVER_INTERRUPT_EITHEREDGE:

        PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableBothEdges,(pint_cb_t)CLIF_IRQHandler);
        break;
    default:
        /* Do Nothing. */
        break;
    }

    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);
    return PH_DRIVER_SUCCESS;
}

static phStatus_t phDriver_PinConfigGpio(uint8_t bPortNum, uint8_t bPinNum, phDriver_Pin_Func_t ePinFunc,
        phDriver_Pin_Config_t *pPinConfig)
{

	 /* enable clock for GPIO*/
	CLOCK_EnableClock(kCLOCK_Gpio0);
	CLOCK_EnableClock(kCLOCK_Gpio1);
	phStatus_t wStatus = PH_DRIVER_SUCCESS;

	gpio_pin_config_t gpio_config = {
	    		kGPIO_DigitalOutput,  pPinConfig->bOutputLogic,
	    	};


    GPIO_PortInit(GPIO, bPortNum);
    switch(ePinFunc)
    {
    case PH_DRIVER_PINFUNC_INPUT:
    	 gpio_config.pinDirection=kGPIO_DigitalInput;
    	 GPIO_PinInit(GPIO, bPortNum, bPinNum,&gpio_config);
        break;

    case PH_DRIVER_PINFUNC_OUTPUT:

        GPIO_PinInit(GPIO, bPortNum, bPinNum,&gpio_config);
        break;

    default:
        wStatus = PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    return wStatus;
}

phStatus_t phDriver_PinConfig(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc, phDriver_Pin_Config_t *pPinConfig)
{
    uint8_t bPortNum;
    uint8_t bPinNum;
    phStatus_t wStatus;

    bPortNum = (uint8_t)((dwPinNumber & 0x0000FF00) >> 8);
    bPinNum = (uint8_t)(dwPinNumber & 0x000000FF);

    do{
        if(pPinConfig == NULL)
        {
            wStatus = PH_DRIVER_ERROR | PH_COMP_DRIVER;
            break;
        }

        if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
        {
            /* Level triggered interrupts are NOT supported in LPC1769. */
            if((pPinConfig->eInterruptConfig == PH_DRIVER_INTERRUPT_LEVELZERO) ||
                    (pPinConfig->eInterruptConfig == PH_DRIVER_INTERRUPT_LEVELONE))
            {
                wStatus = PH_DRIVER_ERROR | PH_COMP_DRIVER;
            }
            else
            {
                wStatus = phDriver_PinConfigInterrupt(bPortNum, bPinNum, pPinConfig);
            }
        }
        else
        {
            wStatus = phDriver_PinConfigGpio(bPortNum, bPinNum, ePinFunc, pPinConfig);
        }
    }while(0);

    return wStatus;
}

uint8_t phDriver_PinRead(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc)
{

    uint8_t bPinStatus = 0;
    uint8_t bPortNum;
    uint8_t bPinNum;

    bPortNum = (uint8_t)((dwPinNumber & 0x0000FF00) >> 8);
    bPinNum = (uint8_t)(dwPinNumber & 0x000000FF);

    if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
    {

    	 bPinStatus=PINT_PinInterruptGetStatus(PINT, kPINT_PinInt0);

    }
    else
    {
        bPinStatus = GPIO_PinRead(GPIO, bPortNum, bPinNum);
    }

    return bPinStatus;
}

void phDriver_PinWrite(uint32_t dwPinNumber, uint8_t bValue)
{
    uint8_t bPortNum;
    uint8_t bPinNum;

    bPortNum = (uint8_t)((dwPinNumber & 0x0000FF00) >> 8);
    bPinNum = (uint8_t)(dwPinNumber & 0x000000FF);
    GPIO_PinWrite(GPIO,bPortNum, bPinNum, bValue);
    //Chip_GPIO_SetPinState(LPC_GPIO, bPortNum, bPinNum, bValue);
}

void phDriver_PinClearIntStatus(uint32_t dwPinNumber)
{
    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt0);
}

void PH_DRIVER_LPC_TIMER_IRQ_HANDLER(void)
{

    CTIMER_ClearStatusFlags(PH_DRIVER_LPC_TIMER, kCTIMER_Match0Flag);
    pTimerIsrCallBack();
    CTIMER_DisableInterrupts(PH_DRIVER_LPC_TIMER, kCTIMER_Match1InterruptEnable);
    CTIMER_Deinit(PH_DRIVER_LPC_TIMER);

}

static void phDriver_TimerIsrCallBack(void)
{
    dwTimerExp = 1;
}




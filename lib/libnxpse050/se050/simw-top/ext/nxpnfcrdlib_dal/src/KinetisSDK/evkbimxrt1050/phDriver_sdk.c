/*
 * phDriver_Imxrtsdk.c
 *
 *  Created on: Aug 22, 2018
 *      Author: nxf48223
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phDriver.h"
#include "BoardSelection.h"
#include "fsl_device_registers.h"
#include <fsl_gpio.h>
#include <fsl_pit.h>

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define IMX_TIMER_MAX_32BIT      0xFFFFFFFFU
#define PIT_LED_HANDLER PIT_IRQHandler
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
volatile bool pitIsrFlag = false;
/* *****************************************************************************************************************
 * Global and Static Variables
 *
 *
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/* Array initializer of PORT peripheral base pointers */
//static const PORT_Type *pPortsBaseAddr[] = PORT_BASE_PTRS;
/* Array initializer of GPIO peripheral base pointers */
static const GPIO_Type *pGpiosBaseAddr[] = GPIO_BASE_PTRS;
/* Clock ip name array for PORT. */
//static const clock_ip_name_t pPortsClock[] = PORT_CLOCKS;
/* Used to map phDriver Interrupt triggers to Kinetis */
/*static const port_interrupt_t aInterruptTypes[] = {kGPIO_NoIntmode,  Unused.
		kGPIO_IntLowLevel,
		kGPIO_IntHighLevel,
		kGPIO_IntRisingEdge,
		kGPIO_IntFallingEdge,
		kGPIO_IntRisingOrFallingEdge,
};*/

static pphDriver_TimerCallBck_t pPitTimerCallBack;
static volatile uint8_t dwTimerExp;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phDriver_PitTimerIsrCallBack(void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/*******************************************************************************
 * Code
 ******************************************************************************/


phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack)
{
    uint64_t          qwTimerCnt;
    uint32_t          dwTimerFreq;

    dwTimerFreq = CLOCK_GetFreq(PH_DRIVER_IMX_PIT_CLK);


    /* Timer count = (delay * freq)/Units. */
    qwTimerCnt = dwTimerFreq;
    qwTimerCnt = (qwTimerCnt / eTimerUnit);
    qwTimerCnt = (dwTimePeriod * qwTimerCnt);

    /* 32-bit timers. */
    if(qwTimerCnt > (uint64_t)IMX_TIMER_MAX_32BIT)
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    if(pTimerCallBack == NULL)
    {    /* Timer Start is blocking call. */
        dwTimerExp = 0;
        pPitTimerCallBack = phDriver_PitTimerIsrCallBack;
    }
    else
    {   /* Call the Timer callback. */
        pPitTimerCallBack = pTimerCallBack;
    }

    /* Set PERCLK_CLK source to OSC_CLK*/
      CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
      /* Set PERCLK_CLK divider to 1 */
      CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);


      /* Structure of initialize PIT */
          pit_config_t pitConfig;

         PIT_GetDefaultConfig(&pitConfig);

         /* Init pit module */
         PIT_Init(PIT, &pitConfig);

         /* Set timer period for channel 0 */
         PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t)qwTimerCnt);

         /* Enable timer interrupts for channel 0 */
         PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

         /* Enable at the NVIC */
         EnableIRQ(PIT_IRQn);

         /* Start channel 0 */
        /* PRINTF("\r\nStarting channel No.0 ...");*/
         PIT_StartTimer(PIT, kPIT_Chnl_0);

         while (true)
         {
             /* Check whether occur interupt and toggle LED */
             if (true == pitIsrFlag)
             {
                /* PRINTF("\r\n Channel No.0 interrupt is occured ! (PIT TIMER USED FOR DELAY)");*/
                 pitIsrFlag = false;
                 break;

             }
         }

    return PH_DRIVER_SUCCESS;
}


phStatus_t phDriver_TimerStop(void)
{
    PIT_StopTimer(PH_DRIVER_IMX_PIT_TIMER, PH_DRIVER_IMX_TIMER_CHANNEL);
    PIT_DisableInterrupts(PH_DRIVER_IMX_PIT_TIMER, PH_DRIVER_IMX_TIMER_CHANNEL, kPIT_TimerInterruptEnable);

    /* Disable at the NVIC */
    DisableIRQ(PH_DRIVER_IMX_TIMER_NVIC);

    return PH_DRIVER_SUCCESS;
}


phStatus_t phDriver_PinConfig(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc, phDriver_Pin_Config_t *pPinConfig)
{
    gpio_pin_config_t sGpioConfig;
    uint8_t bPinNum;
    uint8_t bPortGpio;


    if((ePinFunc == PH_DRIVER_PINFUNC_BIDIR) || (pPinConfig == NULL))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    /* Extract the Pin, Gpio, Port details from dwPinNumber */
    bPinNum = (uint8_t)(dwPinNumber & 0xFF);
    bPortGpio = (uint8_t)((dwPinNumber & 0xFF00)>>8);

    CLOCK_EnableClock(kCLOCK_Iomuxc);
    sGpioConfig.direction = (ePinFunc == PH_DRIVER_PINFUNC_OUTPUT)?kGPIO_DigitalOutput:kGPIO_DigitalInput;
    sGpioConfig.outputLogic  =  pPinConfig->bOutputLogic;

    if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
    {
    	sGpioConfig.interruptMode = pPinConfig->eInterruptConfig;

    	 GPIO_PinInit((GPIO_Type *)pGpiosBaseAddr[bPortGpio],bPinNum,&sGpioConfig);
    	 GPIO_PortEnableInterrupts((GPIO_Type *)pGpiosBaseAddr[bPortGpio],1U << bPinNum);

    }

    else
    {
    	sGpioConfig.interruptMode = kGPIO_NoIntmode;
    	 GPIO_PinInit((GPIO_Type *)pGpiosBaseAddr[bPortGpio],bPinNum,&sGpioConfig);
    }




    return PH_DRIVER_SUCCESS;
}


uint8_t phDriver_PinRead(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc)
{
    uint8_t bValue;
    uint8_t bGpioNum;
    uint8_t bPinNum;

    /* Extract the Pin, Gpio details from dwPinNumber */
    bPinNum = (uint8_t)(dwPinNumber & 0xFF);
    bGpioNum = (uint8_t)((dwPinNumber & 0xFF00)>>8);

    if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
    {
        bValue = (uint8_t)((GPIO_GetPinsInterruptFlags((GPIO_Type *)pGpiosBaseAddr[bGpioNum]) >> bPinNum) & 0x01);
    }
    else
    {
        bValue = (uint8_t)GPIO_ReadPinInput((GPIO_Type *)pGpiosBaseAddr[bGpioNum], bPinNum);
    }

    return bValue;
}


void phDriver_PinWrite(uint32_t dwPinNumber, uint8_t bValue)
{
    uint8_t bGpioNum;
    uint8_t bPinNum;

    /* Extract the Pin, Gpio details from dwPinNumber */
    bPinNum = (uint8_t)(dwPinNumber & 0xFF);
    bGpioNum = (uint8_t)((dwPinNumber & 0xFF00)>>8);

    GPIO_WritePinOutput((GPIO_Type *)pGpiosBaseAddr[bGpioNum], bPinNum, bValue);
}


void phDriver_PinClearIntStatus(uint32_t dwPinNumber)
{
    uint8_t bGpioNum;
    uint8_t bPinNum;

    /* Extract the Pin, Gpio details from dwPinNumber */
    bPinNum = (uint8_t)(dwPinNumber & 0xFF);
    bGpioNum = (uint8_t)((dwPinNumber & 0xFF00)>>8);

    GPIO_ClearPinsInterruptFlags((GPIO_Type *)pGpiosBaseAddr[bGpioNum], (1U << bPinNum));
}


void PIT_IRQHandler(void)
{
		/* Clear interrupt flag.*/
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		/* Single shot timer. Stop it. */
		PIT_StopTimer(PIT, kPIT_Chnl_0);
		PIT_DisableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

		pitIsrFlag = true;

}


static void phDriver_PitTimerIsrCallBack(void)
{
    dwTimerExp = 1;
}



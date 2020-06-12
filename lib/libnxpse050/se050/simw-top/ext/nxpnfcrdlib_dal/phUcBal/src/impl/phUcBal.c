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

#include "phUcBal.h"
#include "phUcBal_MainTask.h"
#include "phUcBal_Config.h"
#include "cmsis.h"
#include "BoardSelection.h"
#include "phUcBal_GPIO.h" //FIXME
#include "ph_NxpBuild.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/** Buffer used by NxpNfcRdLib HAL while transmitting packet to IC/Frontend */
static uint8_t gphUcBal_BufferHALTx[PH_UCBAL_HAL_BUFSIZE];
static uint8_t gphUcBal_BufferHALRx[PH_UCBAL_HAL_BUFSIZE];

/* *****************************************************************************************************************
 * Global and Static Variables
 * ***************************************************************************************************************** */

#ifdef NXPBUILD__PHHAL_HW_PN5180
phhalHw_Pn5180_DataParams_t * gpphUcBal_PtrHal = NULL;
static phhalHw_Pn5180_DataParams_t sHalParams;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_RC663
phhalHw_Rc663_DataParams_t * gpphUcBal_PtrHal = NULL;
static phhalHw_Rc663_DataParams_t sHalParams;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

phbalReg_Type_t  *  gpphUcBal_PtrBal = NULL;
static phbalReg_Type_t sBalParams;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * @ingroup ExecFlow
 *
 * @{
 *
 * main() : Entry point of the Example.
 *
 */


int main()
{
    /** Before starting @ref phUcBal_MainTask, main() ensures the system is ready for required functionalities. */
    do
    {
        phStatus_t status = PH_ERR_INTERNAL_ERROR;
        phOsal_ThreadObj_t ThreadObject;

        gpphUcBal_PtrHal = &sHalParams;
        gpphUcBal_PtrBal = &sBalParams;

        /** 1) Perform OSAL Init first before everything. */
        phOsal_Init();

        phUcBal_Config_Init();

        status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));

        /** 2) Perform Hardware Init. Depending on the hardware used in ph_NxpBuild.h,
         * corresponding Front-End Hardware is initialized */


#ifdef NXPBUILD__PHHAL_HW_RC663
            /* Initialize the RC663 HAL component */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_Init(
                gpphUcBal_PtrHal,
                sizeof(*gpphUcBal_PtrHal),
                &sBalParams,
                (uint8_t *)gkphhalHw_Rc663_LoadConfig,
                gphUcBal_BufferHALTx,
                sizeof(gphUcBal_BufferHALTx),
                gphUcBal_BufferHALRx,
                sizeof(gphUcBal_BufferHALRx)
                ));

            phUcBal_GPIO_EnableIRQInterrupts();
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef NXPBUILD__PHHAL_HW_PN5180

            phDriver_PinWrite(PHDRIVER_PIN_DWL, PH_DRIVER_SET_LOW);

            /* Initialize the Pn5180 HAL component */
            status = phhalHw_Pn5180_Init(
                gpphUcBal_PtrHal,
                sizeof(*gpphUcBal_PtrHal),
                &sBalParams,
                NULL,
                gphUcBal_BufferHALTx,
                sizeof(gphUcBal_BufferHALTx),
                gphUcBal_BufferHALRx,
                sizeof(gphUcBal_BufferHALRx)
                );

            if (PH_ERR_SUCCESS == status) {
                if ( sHalParams.bIsTestBusEnabled != PH_ON) {
                    phUcBal_GPIO_EnableIRQInterrupts();
                }
            }
            else
            {
                phUcBal_Config_StoreICInitFailed(1);
                status = PH_ERR_SUCCESS;
            }
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

        ASSERT_SUCCESS(status);

        if (status != PH_ERR_SUCCESS)
            break;

#ifdef NXPBUILD__PHHAL_HW_RC663
        /*
         * Set irq0en (08h) register bit 8 to 1 to configure interrupt as active low
         * User may change this to active high based on the platform.
         * */
        status = phhalHw_Rc663_WriteRegister(gpphUcBal_PtrHal, PHHAL_HW_RC663_REG_IRQ0EN, 0x80);
        ASSERT_SUCCESS(status);
        if(status != PH_ERR_SUCCESS)
            break;
#endif

        /** 3) Instantiate RTOS Thread object.  Defaults are taken from @ref RTOSDefaults */
        ThreadObject.pTaskName = (uint8_t *) "phUcBal";
        ThreadObject.pStackBuffer = NULL;
        ThreadObject.priority = PH_UCBAL_MAINTASK_PRIO;
        ThreadObject.stackSizeInNum = PH_UCBAL_MAINTASK_STACK;
        status = phOsal_ThreadCreate(&ThreadObject.ThreadHandle,
            &ThreadObject, &phUcBal_MainTask, NULL);
        ASSERT_SUCCESS(status);

        /** 4) Finally trigger the RTOS Task Scheduler so that @ref phUcBal_MainTask and be triggered */
        phOsal_StartScheduler();

        while(1)
        {
            phOsal_ThreadDelay(1);
        }
    } while (0);
    return 0;
}

/** @} */


/** @ingroup CommandGroupConfig
 * @{
 */

#ifdef DEBUG
volatile uint32_t gphUcBal_RF_EventCallbackCount = 0;
#endif

void CLIF_IRQHandler(void)
{
    /* Read the interrupt status of external interrupt attached to the reader IC IRQ pin */
    if (phDriver_PinRead(PHDRIVER_PIN_IRQ, PH_DRIVER_PINFUNC_INTERRUPT))
    {
        #ifdef DEBUG
        gphUcBal_RF_EventCallbackCount++;
        #endif

        phUcBal_Config_StoreIRQPinValue(true);

        if ( IRQIsrHandling_Consume != gpkphUcBal_Config->IRQIsrHandling) {
            /* Call application registered callback. */
            if (gpphUcBal_PtrHal != NULL && gpphUcBal_PtrHal->pRFISRCallback != NULL)
            {
                gpphUcBal_PtrHal->pRFISRCallback(gpphUcBal_PtrHal);
            }
        }

    }
    phDriver_PinClearIntStatus(PHDRIVER_PIN_IRQ);
}

/** @} */
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

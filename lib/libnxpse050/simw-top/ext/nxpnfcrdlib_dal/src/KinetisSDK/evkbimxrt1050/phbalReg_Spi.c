/*
 * phbalReg_ImxrtSpi.c
 *
 *  Created on: Aug 22, 2018
 *      Author: nxf48223
 */




#include "phDriver.h"
#include "BoardSelection.h"

#include "fsl_pit.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_lpspi.h"

#ifndef PHDRIVER_IMX_SPI_POLLING
#include <fsl_dspi_freertos.h>
#endif

#define PHBAL_REG_KINETIS_SPI_ID               0x0FU       /**< ID for Kinetis/IMX SPI BAL component */


#ifndef PHDRIVER_IMX_SPI_POLLING
dspi_rtos_handle_t g_masterHandle;
#endif

static void phbalReg_SpiInit(void);


phStatus_t phbalReg_Init(
                         void * pDataParams,
                         uint16_t wSizeOfDataParams
                         )
{


    if ( (pDataParams == NULL) || (sizeof(phbalReg_Type_t) != wSizeOfDataParams))
    {
        return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
    }

       ((phbalReg_Type_t *)pDataParams)->wId      = PH_COMP_DRIVER | PHBAL_REG_KINETIS_SPI_ID;
       ((phbalReg_Type_t *)pDataParams)->bBalType = PHBAL_REG_TYPE_SPI;

       /*Set clock source for LPSPI*/
        CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);


        lpspi_master_config_t g_masterConfig;
        memset(&g_masterConfig, 0, sizeof(lpspi_master_config_t));


    /*Master config*/
        g_masterConfig.baudRate = PHDRIVER_IMX_SPI_DATA_RATE;
        g_masterConfig.bitsPerFrame = 8U;
        g_masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
        g_masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
        g_masterConfig.direction = kLPSPI_MsbFirst;

        g_masterConfig.pcsToSckDelayInNanoSec = 1000000000 / g_masterConfig.baudRate;
        g_masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / g_masterConfig.baudRate;
        g_masterConfig.betweenTransferDelayInNanoSec = 1000000000 / g_masterConfig.baudRate;

        g_masterConfig.whichPcs = kLPSPI_Pcs0;
        g_masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

        g_masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
        g_masterConfig.dataOutConfig = kLpspiDataOutRetained;

        /* Setting IFO AND IF1 host interface select pins
         * These pins are connected to IF0 and IF1 pins on the CLEV6630B board
         * IF0 = 0 and IF1 = 1 to select SPI as host interface communication
         */
/*

        phDriver_Pin_Config_t IF_config;

        IF_config.bOutputLogic = PH_DRIVER_SET_HIGH;
        phDriver_PinConfig(PHDRIVER_PIN_IF1,PH_DRIVER_PINFUNC_OUTPUT ,&IF_config);
        IF_config.bOutputLogic = PH_DRIVER_SET_LOW ;
        phDriver_PinConfig(PHDRIVER_PIN_IF0,PH_DRIVER_PINFUNC_OUTPUT ,&IF_config);

*/

    phbalReg_SpiInit();

#ifdef PHDRIVER_IMX_SPI_POLLING
    /* Initialize the LPSPI peripheral */
    LPSPI_MasterInit(PHDRIVER_IMX_SPI_MASTER, &g_masterConfig, PHDRIVER_IMX_SPI_CLK_SRC);
#else
   /* DSPI_RTOS_Init(&g_masterHandle, PHDRIVER_IMX_SPI_MASTER, &g_masterConfig, CLOCK_GetFreq(PHDRIVER_IMX_SPI_CLK_SRC));*/
#endif

    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_Exchange(
                             void * pDataParams,
                             uint16_t wOption,
                             uint8_t * pTxBuffer,
                             uint16_t wTxLength,
                             uint16_t wRxBufSize,
                             uint8_t * pRxBuffer,
                             uint16_t * pRxLength
                             )
{
    phStatus_t status = PH_DRIVER_SUCCESS;
    uint8_t * pRxBuf;
    status_t lpspiStatus;
    lpspi_transfer_t g_masterXfer;
    uint8_t g_dummyBuffer[260];

    memset(&g_masterXfer, 0, sizeof(lpspi_transfer_t));

    if(pRxBuffer == NULL)
    {
        pRxBuf = g_dummyBuffer;
    }
    else
    {
        pRxBuf = pRxBuffer;
    }

    /* Set up the transfer */
    g_masterXfer.txData = pTxBuffer;
    g_masterXfer.rxData = pRxBuf;
    g_masterXfer.dataSize = wTxLength;
    g_masterXfer.configFlags =  kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous ;

    /* Start transfer */
#ifdef PHDRIVER_IMX_SPI_POLLING
    lpspiStatus = LPSPI_MasterTransferBlocking(PHDRIVER_IMX_SPI_MASTER, &g_masterXfer);
#else
    lpspiStatus = LPSPI_RTOS_Transfer(&g_masterHandle, &g_masterXfer);
#endif
    if (lpspiStatus != kStatus_Success)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if (pRxLength != NULL)
    {
        *pRxLength = wTxLength;
    }

    return status;
}

phStatus_t phbalReg_SetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t wValue
                              )
{
    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_GetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t * pValue
                              )
{
    return PH_DRIVER_SUCCESS;
}


static void phbalReg_SpiInit(void)
{

    /* SPI Configuration */
    NVIC_SetPriority(PHDRIVER_IMX_SPI_IRQ, LPSPI_IRQ_PRIORITY);

}

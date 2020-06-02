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
* Generic phDriver(DAL) Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  PGh: Fixed case sensitivity for linux build
*  RS:  Generated 24. Jan 2017
*
*/



#include <board.h>
#include "BoardSelection.h"
#include "fsl_spi.h"
#include "phDriver.h"

#define PHBAL_REG_LPCOPEN_SPI_ID               0x0DU       /**< ID for LPC Open SPI BAL component */


/**
* \brief Initialize the LPC Open SPI BAL layer.
*
* \return Status code
* \retval #PH_DRIVER_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_Init(
                                      void * pDataParams,
                                      uint16_t wSizeOfDataParams
                                      )
{

	spi_master_config_t masterConfig = {0};
	if((pDataParams == NULL) || (sizeof(phbalReg_Type_t) != wSizeOfDataParams))
    {
        return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
    }

    ((phbalReg_Type_t *)pDataParams)->wId      = PH_COMP_DRIVER | PHBAL_REG_LPCOPEN_SPI_ID;
    ((phbalReg_Type_t *)pDataParams)->bBalType = PHBAL_REG_TYPE_SPI;


    /* attach 12 MHz clock to SPI7 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);

    /* reset FLEXCOMM for SPI */
    RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);


    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.sselNum = (spi_ssel_t)kSPI_Ssel0;
    masterConfig.sselPol = (spi_spol_t)kSPI_SpolActiveAllLow;
    masterConfig.baudRate_Bps=5000000U;
    SPI_MasterInit(LPC_SSP, &masterConfig,  SSP_CLOCKRATE);
    NVIC_SetPriority(FLEXCOMM7_IRQn,  7U);



    /*phDriver_Pin_Config_t IF_config;

            IF_config.bOutputLogic = PH_DRIVER_SET_HIGH;
            phDriver_PinConfig(PHDRIVER_PIN_IF1,PH_DRIVER_PINFUNC_OUTPUT ,&IF_config);
            IF_config.bOutputLogic = PH_DRIVER_SET_LOW ;
            phDriver_PinConfig(PHDRIVER_PIN_IF0,PH_DRIVER_PINFUNC_OUTPUT ,&IF_config);
*/





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


	spi_transfer_t g_masterXfer;
	/*phStatus_t status = PH_DRIVER_SUCCESS;*/
	uint8_t * pRxBuf;
	status_t spiStatus;
	uint8_t g_dummyBuffer[260];

	memset(&g_masterXfer, 0, sizeof(spi_transfer_t));

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

	  /* Start transfer */

	    spiStatus =  SPI_MasterTransferBlocking(LPC_SSP,&g_masterXfer);


	    if (spiStatus != kStatus_Success)
	    {
	        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
	    }

	    if (pRxLength != NULL)
	    {
	        *pRxLength = wTxLength;
	    }



    return PH_DRIVER_SUCCESS;
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

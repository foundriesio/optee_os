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
* Freertos OSAL Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  PC: Generated 23. Aug 2012
*
*/
#include <ph_Status.h>
#include <phOsal.h>

#ifdef NXPBUILD__PH_OSAL_ABEND

/* THIS IS JUST A STUB - IMPLEMENTATION TO BE DONE IN PROJECT WHERE IT IS USED, E.G. I&V PROJECT */

phStatus_t phOsal_Event_Init(void)
{

   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Event_GetEventHandle(void **pHandle)
{


    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Event_Post(phOsal_EventType_t eEvtType, phOsal_EventSource_t eSrc, phOsal_EventDestination_t eDest)
{

   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

void phOsal_Event_PostwithDetail(
                                 phOsal_EventType_t eEvtType,
                                 phOsal_EventSource_t eSrc,
                                 phOsal_EventDestination_t eDest,
                                 phOsal_EventDetail_t eDetail
                                 )
{
    phOsal_Event_Post(eEvtType, eSrc, eDest);
    phOsal_Event_SetDetail(eDetail);
}

phStatus_t phOsal_Event_Consume(phOsal_EventType_t eEvtType, phOsal_EventSource_t eSrc)
{

   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Event_WaitAny(phOsal_EventType_t eEvtType, uint32_t dwTimeoutCount, phOsal_EventType_t *pRcvdEvt)
{
   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Event_WaitAll(
                                phOsal_EventType_t eEvtType,
                                uint32_t dwTimeoutCount,
                                phOsal_EventType_t *pRcvdEvt
                                )
{
   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

void phOsal_Event_SetDetail(phOsal_EventDetail_t eEvtDetail)
{

}


void phOsal_Event_GetDetail(phOsal_EventDetail_t * eEvent_Detail)
{

}

phStatus_t phOsal_Event_Deinit(void)
{

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phOsal_SemaphoreHandle_t phOsal_Semaphore_Create(uint32_t dwMaxCount,
                                                uint32_t dwInitialCount
                                                )
{
   return NULL;

}


phStatus_t phOsal_Semaphore_Give(phOsal_SemaphoreHandle_t semHandle)
{
   return PH_ERR_SUCCESS;

}



phStatus_t phOsal_Semaphore_Take(phOsal_SemaphoreHandle_t semHandle,
                                uint32_t dwBlockTime)
{
   return PH_ERR_SUCCESS;
}


phStatus_t phOsal_Semaphore_Delete(phOsal_SemaphoreHandle_t semHandle)
{

    return PH_ERR_SUCCESS;
}

phOsal_MutexHandle_t phOsal_Mutex_Create(void)
{
   return NULL;

}

phStatus_t phOsal_Mutex_Take(phOsal_MutexHandle_t mutexHandle, uint32_t dwBlockTime)
{
   return PH_ERR_SUCCESS;
}

phStatus_t phOsal_Mutex_Give(phOsal_MutexHandle_t mutexHandle)
{
   return PH_ERR_SUCCESS;
}

phStatus_t phOsal_Mutex_Delete(phOsal_MutexHandle_t mutexHandle)
{

    return PH_ERR_SUCCESS;
}


phOsal_TimerHandle_t phOsal_Timer_Create(uint32_t dwTimerID, pphOsal_TimerCallback_t fpCallbackFunction)
{

   return NULL;

}

phStatus_t phOsal_Timer_Start(phOsal_TimerHandle_t timerHandle, uint32_t dwTimeMs)
{


   return PH_ERR_SUCCESS;
}

phStatus_t phOsal_Timer_Stop(phOsal_TimerHandle_t timerHandle)
{

   return PH_ERR_SUCCESS;
}

phStatus_t phOsal_Timer_Reset(phOsal_TimerHandle_t timerHandle)
{


   return PH_ERR_SUCCESS;
}

phStatus_t phOsal_Timer_Delete(phOsal_TimerHandle_t timerHandle)
{

   return PH_ERR_SUCCESS;
}

#endif  /* NXPBUILD__PH_OSAL_ABEND */

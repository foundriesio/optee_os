
#ifndef _PH_DRIVER_H_PC
#define _PH_DRIVER_H_PC

#include <phbalReg.h>

/**
* \brief Timer units.
*/
typedef enum{
    PH_DRIVER_TIMER_SECS       = 1,          /**< Seconds timer. */
    PH_DRIVER_TIMER_MILLI_SECS = 1000,          /**< Milliseconds timer. */
    PH_DRIVER_TIMER_MICRO_SECS = 1000000        /**< Microseconds timer. */
} phDriver_Timer_Unit_t;

/**
*
* \brief Timer callback interface which will be called when timer expires.
* \retval  None
*/
typedef void (*pphDriver_TimerCallBck_t)(void);

phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack);


/**
 * \brief Stops the running Timer.
 *
 * @return Status of the API
 * @retval #PH_DRIVER_SUCCESS Operation successful.
 */
phStatus_t phDriver_TimerStop(void);

//TODO
typedef enum{
    PH_DRIVER_INTERRUPT_LEVELZERO = 0x01,   /**< Interrupt when level zero. */
    PH_DRIVER_INTERRUPT_LEVELONE,           /**< Interrupt when level one. */
    PH_DRIVER_INTERRUPT_RISINGEDGE,         /**< Interrupt on rising edge. */
    PH_DRIVER_INTERRUPT_FALLINGEDGE,        /**< Interrupt on falling edge. */
    PH_DRIVER_INTERRUPT_EITHEREDGE,         /**< Interrupt on either edge. */
} phDriver_Interrupt_Config_t;

//#define PH_DRIVER_INTERRUPT_RISINGEDGE 1
//#define PH_DRIVER_INTERRUPT_FALLINGEDGE 2

#define PHDRIVER_PIN_RESET   1
//#define PH_NXPNFCRDLIB_CONFIG_RC663_IRQ_TYPE 2

#define PHDRIVER_HAS_PIN 0


#endif /* _PH_DRIVER_H_PC */

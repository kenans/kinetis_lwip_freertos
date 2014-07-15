/* ###################################################################
**     Filename    : Events.h
**     Project     : PiloteV5_v0.1
**     Processor   : MK60FN1M0VLQ12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-06-05, 09:39, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "FRTOS.h"
#include "UTIL.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "USB1.h"
#include "USB0.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "CS1.h"
#include "EE241.h"
#include "WAIT1.h"
#include "GI2C1.h"
#include "CI2C1.h"
#include "PWM1.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "TI1.h"
#include "TU2.h"
#include "ETH1.h"
#include "Cmp1.h"
#include "ACompLdd1.h"
/**
 *  Event method includes
 */
#include "LPTMR_PDD.h"
#include "eth.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MK60FN1M0LQ15]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


void FRTOS_vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  FRTOS_vApplicationStackOverflowHook (module Events)
**
**     Component   :  FRTOS [FreeRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  FRTOS_vApplicationTickHook (module Events)
**
**     Component   :  FRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  FRTOS_vApplicationIdleHook (module Events)
**
**     Component   :  FRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  FRTOS_vApplicationMallocFailedHook (module Events)
**
**     Component   :  FRTOS [FreeRTOS]
**     Description :
**         If enabled, the RTOS will call this hook in case memory
**         allocation failed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  AS1_OnError (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnError(void);

/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnRxChar(void);

/*
** ===================================================================
**     Event       :  AS1_OnTxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnTxChar(void);

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TI1_OnInterrupt(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  ETH1_OnFrameTransmitted (module Events)
**
**     Component   :  ETH1 [Ethernet_LDD]
*/
/*!
**     @brief
**         This event is called after the whole frame on the head of
**         the internal frame queue has been sent and removed from the
**         queue. The event is available only if IEEE 1588 is disabled.
**     @param
**         UserDataPtr     - Pointer to user data
**                           structure pointer.
*/
/* ===================================================================*/
void ETH1_OnFrameTransmitted(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  ETH1_OnFrameReceived (module Events)
**
**     Component   :  ETH1 [Ethernet_LDD]
*/
/*!
**     @brief
**         This event is called after the whole frame on the head of
**         the internal frame queue has been received and removed from
**         the queue. The event is available only if IEEE 1588 is
**         disabled.
**     @param
**         UserDataPtr     - Pointer to user data
**                           structure pointer.
**     @param
**         FragCount       - Received frame fragment count
**                           (the number of buffers used to store the
**                           frame data).
**     @param
**         Length          - Received frame length.
*/
/* ===================================================================*/
void ETH1_OnFrameReceived(LDD_TUserData *UserDataPtr, uint16_t FragCount, uint16_t Length);

/*
** ===================================================================
**     Event       :  ETH1_OnFatalError (module Events)
**
**     Component   :  ETH1 [Ethernet_LDD]
*/
/*!
**     @brief
**         This event is called when a fatal error has occurred (i.e.
**         ethernet bus error). The device should be reinitialized
**         during this event (Deinit and Init methods should be called).
**     @param
**         UserDataPtr     - Pointer to user data
**                           structure pointer.
*/
/* ===================================================================*/
void ETH1_OnFatalError(LDD_TUserData *UserDataPtr);

void FRTOS_vOnPreSleepProcessing(portTickType expectedIdleTicks);
/*
** ===================================================================
**     Event       :  FRTOS_vOnPreSleepProcessing (module Events)
**
**     Component   :  FRTOS [FreeRTOS]
**     Description :
**         Used in tickless idle mode only, but required in this mode.
**         Hook for the application to enter low power mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         expectedIdleTicks - expected idle
**                           time, in ticks
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  Cpu_OnLLSWakeUpINT (module Events)
**
**     Component   :  Cpu [MK60FN1M0LQ15]
*/
/*!
**     @brief
**         This event is called when Low Leakage WakeUp interrupt
**         occurs. LLWU flags indicating source of the wakeup can be
**         obtained by calling the [GetLLSWakeUpFlags] method. Flags
**         indicating the external pin wakeup source are automatically
**         cleared after this event is executed. It is responsibility
**         of user to clear flags corresponding to internal modules.
**         This event is automatically enabled when [LLWU interrupt
**         request] is enabled.
*/
/* ===================================================================*/
void Cpu_OnLLSWakeUpINT(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

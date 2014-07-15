/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

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
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

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
void FRTOS_vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName)
{
  /* This will get called if a stack overflow is detected during the context
     switch.  Set configCHECK_FOR_STACK_OVERFLOWS to 2 to also check for stack
     problems within nested interrupts, but only do this for debug purposes as
     it will increase the context switch time. */
  (void)pxTask;
  (void)pcTaskName;
  taskDISABLE_INTERRUPTS();
  /* Write your code here ... */
  for(;;) {}
}

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
void FRTOS_vApplicationTickHook(void)
{
  /* Called for every RTOS tick. */
  /* Write your code here ... */
}

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
void FRTOS_vApplicationIdleHook(void)
{
  /* Called whenever the RTOS is idle (from the IDLE task).
     Here would be a good place to put the CPU into low power mode. */
  /* Write your code here ... */
}

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
void FRTOS_vApplicationMallocFailedHook(void)
{
  /* Called if a call to pvPortMalloc() fails because there is insufficient
     free memory available in the FreeRTOS heap.  pvPortMalloc() is called
     internally by FreeRTOS API functions that create tasks, queues, software
     timers, and semaphores.  The size of the FreeRTOS heap is set by the
     configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
  taskDISABLE_INTERRUPTS();
  /* Write your code here ... */
  for(;;) {}
}

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
void AS1_OnError(void)
{
  /* Write your code here ... */
}

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
void AS1_OnRxChar(void)
{
  /* Write your code here ... */
}

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
void AS1_OnTxChar(void)
{
  /* Write your code here ... */
}

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
void TI1_OnInterrupt(LDD_TUserData *UserDataPtr)
{
    /* Write your code here ... */

    ++*(uint32_t*)(UserDataPtr);
}

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
void ETH1_OnFrameTransmitted(LDD_TUserData *UserDataPtr)
{
    /* Write your code here ... */
    EthFrameData *eth_data = (EthFrameData*)UserDataPtr;
    TX_ReleaseFrame(eth_data);
}

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
void ETH1_OnFrameReceived(LDD_TUserData *UserDataPtr, uint16_t FragCount, uint16_t Length)
{
    /* Write your code here ... */
    EthFrameData *eth_data = (EthFrameData*)UserDataPtr;
    uint8_t *ptr;
    uint16_t temp;
    LDD_QUEUE_INSERT(eth_data->rx_frag_counts, FragCount, temp);
    LDD_QUEUE_INSERT(eth_data->rx_lengths, Length, temp);
    while (FragCount--) {
        LDD_QUEUE_REMOVE(eth_data->empty_rx_buffers, ptr, temp);
        LDD_QUEUE_INSERT(eth_data->filled_rx_buffers, ptr, temp);
    }
}
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
void ETH1_OnFatalError(LDD_TUserData *UserDataPtr)
{
    /* Write your code here ... */
    EthFrameData *eth_data = (EthFrameData*)UserDataPtr;
    uint8_t *buffers[RX_BUFFER_COUNT];

    ETH1_Deinit(eth_data->eth_device_data);
    eth_data->eth_device_data = ETH1_Init(eth_data);
    /* ... reinitialize transmit and receive buffer queues ... */
    /* Pass the receive buffers to be filled with received data */
    ETH1_ReceiveFrame(eth_data->eth_device_data, buffers, RX_BUFFER_COUNT);

}

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
void FRTOS_vOnPreSleepProcessing(portTickType expectedIdleTicks)
{
#if 1
  /* example for Kinetis (enable SetOperationMode() in CPU component): */
  Cpu_SetOperationMode(DOM_STOP, NULL, NULL);
  /* or to wait for interrupt:
    __asm volatile("dsb");
    __asm volatile("wfi");
    __asm volatile("isb");
  */
#elif 0
  /* example for S08/S12/ColdFire V1 (enable SetWaitMode() in CPU): */
  Cpu_SetWaitMode();
#elif 0
  /* example for ColdFire V2: */
   __asm("stop #0x2000"); */
#else
  #error "you *must* enter low power mode (wait for interrupt) here!"
#endif
  /* Write your code here ... */
}

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
void Cpu_OnLLSWakeUpINT(void)
{
    /* Write your code here ... */
    if (Cpu_GetLLSWakeUpFlags()&LLWU_INT_MODULE0) {
        // Clear LPTMR interrupt flag
        LPTMR_PDD_ClearInterruptFlag(LPTMR0_BASE_PTR);
    }
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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

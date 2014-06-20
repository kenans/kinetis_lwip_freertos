/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CDC1.c
**     Project     : PiloteV5_0.1
**     Processor   : MK60FN1M0VLQ12
**     Component   : FSL_USB_CDC_Device
**     Version     : Component 01.033, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-06-16, 17:55, # CodeGen: 12
**     Abstract    :
**
**     Settings    :
**          Component name                                 : CDC1
**          CPU                                            : Kinetis K60
**          CDC Settings                                   : Enabled
**            .inf ClassGuid                               : 4D36E978-E325-11CE-BFC1-08002BE10318
**            .inf VID                                     : 2504
**            .inf PID                                     : 0300
**            .inf PRVDR                                   : Freescale
**            .inf MFGNAME                                 : My Company
**            .inf DESCRIPTION                             : Freescale CDC Device
**            .inf SERVICE                                 : Virtual Com Driver
**            Bus reported device                          : FSL CDC DEVICE
**            Bus reported vendor                          : FREESCALE INC.
**          Send Buffer                                    : RingBuffer
**          Receive Buffer                                 : RingBuffer
**          Use Timeout                                    : Disabled
**     Contents    :
**         ClearRxBuffer    - void CDC1_ClearRxBuffer(void);
**         ClearTxBuffer    - void CDC1_ClearTxBuffer(void);
**         GetFreeInTxBuf   - word CDC1_GetFreeInTxBuf(void);
**         GetCharsInTxBuf  - word CDC1_GetCharsInTxBuf(void);
**         GetCharsInRxBuf  - word CDC1_GetCharsInRxBuf(void);
**         GetChar          - byte CDC1_GetChar(CDC1_TComData *Chr);
**         RecvChar         - byte CDC1_RecvChar(CDC1_TComData *Chr);
**         SendChar         - byte CDC1_SendChar(CDC1_TComData Chr);
**         SendString       - byte CDC1_SendString(CDC1_TComData *Chr);
**         SendBlock        - byte CDC1_SendBlock(byte *data, word dataSize);
**         PutBufferChecked - byte CDC1_PutBufferChecked(byte *buf, size_t bufSize);
**         App_Callback     - void CDC1_App_Callback(byte controller_ID, byte event_type, void *val);
**         Notify_Callback  - void CDC1_Notify_Callback(byte controller_ID, byte event_type, void *val);
**         App_Task         - byte CDC1_App_Task(byte *txBuf, size_t txBufSize);
**         Init             - byte CDC1_Init(void);
**
**     (c) Copyright Freescale, all rights reserved, 2013
**     Ported as Processor Expert component: Erich Styger
**     http: www.mcuoneclipse.com
** ###################################################################*/
/*!
** @file CDC1.c
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup CDC1_module CDC1 module documentation
**  @{
*/         

/* MODULE CDC1. */

#include "CDC1.h"
#include "hidef.h"          /* for EnableInterrupts macro */
#include "derivative.h"     /* include peripheral declarations */
#include "types.h"          /* Contains User Defined Data Types */
#include "usb_cdc.h"        /* USB CDC Class Header File */
#include <stdio.h>
/* skip the inclusion in dependency state */
#ifndef __NO_SETJMP
  #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#define  CONTROLLER_ID      (0)   /* ID to identify USB CONTROLLER */

#if HIGH_SPEED_DEVICE
static uint_32 g_cdcBuffer[DIC_BULK_OUT_ENDP_PACKET_SIZE>>1];
#endif

/* Virtual COM Application start Init Flag */
static volatile boolean start_app = FALSE;

/* Virtual COM Application Carrier Activate Flag */
static volatile boolean start_transactions = FALSE;

static volatile boolean transactionOngoing = FALSE;
/*
** ===================================================================
**     Method      :  CDC1_GetFreeInTxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of free character in the send buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of free character in the receive
**                           buffer.
** ===================================================================
*/
/*
word CDC1_GetFreeInTxBuf(void)
{
  *** Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  CDC1_GetCharsInTxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of character in the send buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of character in the receive buffer.
** ===================================================================
*/
/*
word CDC1_GetCharsInTxBuf(void)
{
  *** implemented as macro in the header file
  return (word)Tx1_NofElements();
}
*/

/*
** ===================================================================
**     Method      :  CDC1_GetCharsInRxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of character in the receive buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of character in the receive buffer.
** ===================================================================
*/
word CDC1_GetCharsInRxBuf(void)
{
  static uint8_t txBuf[CDC1_DATA_BUFF_SIZE];

  if (CDC1_App_Task(txBuf, sizeof(txBuf))!=ERR_OK) { /* call USB handler: if active, then this will empty the buffer */
  }
  return (word)Rx1_NofElements();
}

/*
** ===================================================================
**     Method      :  CDC1_GetChar (component FSL_USB_CDC_Device)
**     Description :
**         Receives a character from the USB interface. Function is not
**         blocking if there is no character in the input buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to where to store the character
**                           received
**     Returns     :
**         ---             - Error code, ERR_OK for success,
**                           ERR_RXEMPTY if nothing is in RX buffer.
** ===================================================================
*/
/**
byte CDC1_GetChar(CDC1_TComData *Chr)
{
  *** implemented as macro in the header file
  return Rx1_Get(Chr);
}
*/
/*
** ===================================================================
**     Method      :  CDC1_RecvChar (component FSL_USB_CDC_Device)
**     Description :
**         Receives a character from the USB interface. Function is
**         blocking if there is no character in the input buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to where to store the character
**                           received
**     Returns     :
**         ---             - Error code, ERR_OK for success,
**                           ERR_RXEMPTY if nothing is in RX buffer.
** ===================================================================
*/
byte CDC1_RecvChar(CDC1_TComData *Chr)
{
  while(Rx1_Get(Chr)!=ERR_OK) {
    /* retry receiving until success */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  CDC1_SendChar (component FSL_USB_CDC_Device)
**     Description :
**         Method to send a character to the USB interface. Method is
**         non-blocking: If the output buffer is full, it tries to send
**         it over USB. If this fails or buffer is still full, the
**         character will be lost. If OnError() event is enabled, the
**         error event will be called in case of error.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code. ERR_OK for success and
**                           ERR_FAILED otherwise.
** ===================================================================
*/
byte CDC1_SendChar(CDC1_TComData Chr)
{
  static uint8_t txBuf[CDC1_DATA_BUFF_SIZE];

  if (Tx1_Put(Chr)==ERR_TXFULL) { /* retry once, otherwise throw it away  */
    if (CDC1_App_Task(txBuf, sizeof(txBuf))!=ERR_OK) { /* call USB handler: if active, then this will empty the buffer */
      return ERR_TXFULL;
    } else { /* retry, as USB App_Task() should have sent the buffer */
      return Tx1_Put(Chr); /* retry. If buffer is still full, we will lose the character */
    }
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  CDC1_SendBlock (component FSL_USB_CDC_Device)
**     Description :
**         Method to send a data block to the USB interface. Method is
**         non-blocking: if data cannot be sent, it will be lost. If
**         OnError() event is enabled, the error event will be called
**         in case of error.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Pointer to data to send.
**         dataSize        - Size of data in bytes
**     Returns     :
**         ---             - Error code. ERR_OK for success and
**                           ERR_FAILED otherwise.
** ===================================================================
*/
byte CDC1_SendBlock(byte *data, word dataSize)
{
  byte res = ERR_OK;

  while(dataSize > 0) {
    if (CDC1_SendChar(*data)!=ERR_OK) {
      res = ERR_TXFULL;
    }
    dataSize--; data++;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  CDC1_SendString (component FSL_USB_CDC_Device)
**     Description :
**         Method to send a string to the USB interface. Method is
**         non-blocking: if string cannot be sent, it will be lost. If
**         OnError() event is enabled, the error event will be called
**         in case of error.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to string to send.
**     Returns     :
**         ---             - Error code. ERR_OK for success and
**                           ERR_FAILED otherwise.
** ===================================================================
*/
byte CDC1_SendString(CDC1_TComData *Chr)
{
  byte res = ERR_OK;

  while(*Chr != '\0') {
    if (CDC1_SendChar(*Chr)!=ERR_OK) {
      res = ERR_TXFULL;
    }
    Chr++;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  CDC1_App_Callback (component FSL_USB_CDC_Device)
**     Description :
**         Method to handle class callbacks from USB
**     Parameters  :
**         NAME            - DESCRIPTION
**         controller_ID   - controller ID
**         event_type      - value of the event
**         val             - gives the configuration value
**     Returns     : Nothing
** ===================================================================
*/
void CDC1_App_Callback(byte controller_ID, byte event_type, void *val)
{
  UNUSED(controller_ID);
  UNUSED(val);
  if (event_type == USB_APP_BUS_RESET) {
    start_app = FALSE;
  } else if (event_type == USB_APP_ENUM_COMPLETE) {
#if HIGH_SPEED_DEVICE
    /* prepare for the next receive event */
    USB_Class_CDC_Interface_DIC_Recv_Data(&controller_ID,
                                           (uint_8_ptr)g_cdcBuffer,
                                           DIC_BULK_OUT_ENDP_PACKET_SIZE);
#endif
    start_app = TRUE;
  } else if ((event_type == USB_APP_DATA_RECEIVED)&&(start_transactions==TRUE)) {
    /* Copy Received Data buffer to Application Buffer */
    USB_PACKET_SIZE BytesToBeCopied;
    APP_DATA_STRUCT *dp_rcv = (APP_DATA_STRUCT*)val;
    uint_8 index;

    BytesToBeCopied = (USB_PACKET_SIZE)((dp_rcv->data_size > CDC1_DATA_BUFF_SIZE) ? CDC1_DATA_BUFF_SIZE:dp_rcv->data_size);
    for(index = 0; index<BytesToBeCopied ; index++) {
      if (Rx1_Put(dp_rcv->data_ptr[index])!=ERR_OK) {
        /* Failed to put byte into buffer. Is the buffer to small? Then increase the Rx buffer.
           Otherwise not much we could do here, so we are loosing byte here.
         */
        /* Enable OnError() event so this event will be called here */
      }
    }
    (void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, NULL, 0); /* see http://eprints.utar.edu.my/143/1/BI-2011-0708672-1.pdf, page 131 */
  } else if ((event_type == USB_APP_SEND_COMPLETE)&&(start_transactions==TRUE)) {
    transactionOngoing = FALSE;
    /* Previous Send is complete. Queue next receive */
#if HIGH_SPEED_DEVICE
    //(void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, g_cdcBuffer, 0);
#else
    (void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, NULL, 0);
#endif
  } else if (event_type == USB_APP_ERROR) { /* detach? */
    start_app = FALSE;
    start_transactions = FALSE;
  }
}

/*
** ===================================================================
**     Method      :  CDC1_Notify_Callback (component FSL_USB_CDC_Device)
**     Description :
**         Method to handle PSTN Sub Class callbacks
**     Parameters  :
**         NAME            - DESCRIPTION
**         controller_ID   - controller ID
**         event_type      - value of the event
**         val             - 
**     Returns     : Nothing
** ===================================================================
*/
void CDC1_Notify_Callback(byte controller_ID, byte event_type, void *val)
{
  UNUSED(controller_ID);
  UNUSED(val);
  if (start_app == TRUE) {
    if(event_type == USB_APP_CDC_CARRIER_ACTIVATED) {
      start_transactions = TRUE;
    } else if(event_type == USB_APP_CDC_CARRIER_DEACTIVATED) {
      start_transactions = FALSE;
    }
  }
  start_transactions = TRUE; /* ??? see http://forums.freescale.com/t5/Freescale-MQX-trade-USB-Host/Cant-get-CDC-virtual-com-demo-to-work-with-VB2005-on-xp-sp3/m-p/92713#M302 */
}

/*
** ===================================================================
**     Method      :  CDC1_RunUsbEngine (component FSL_USB_CDC_Device)
**
**     Description :
**         Runs the USB polling engine
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void CDC1_RunUsbEngine(void)
{
  /* not needed */
}

/*
** ===================================================================
**     Method      :  CDC1_SendDataBlock (component FSL_USB_CDC_Device)
**
**     Description :
**         Sends a USB data block
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
byte CDC1_SendDataBlock(byte *data, word dataSize)
{
  uint8_t res = ERR_OK;

  transactionOngoing = TRUE;
  if (USB_Class_CDC_Interface_DIC_Send_Data(CONTROLLER_ID, data, dataSize)!=USB_OK) {
    transactionOngoing = FALSE;
    return ERR_FAULT;
  }
  /* wait for transaction finish */
  while(transactionOngoing) { /* wait until transaction is finished */
    CDC1_RunUsbEngine();
  }
  return res;
}

/*
** ===================================================================
**     Method      :  CDC1_App_Task (component FSL_USB_CDC_Device)
**     Description :
**         Application task to be called periodically from the main
**         task.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * txBuf           - Pointer to temporary buffer used to
**                           transmit data over USB. Should be equal or
**                           greater than the endpoint buffer size. Data
**                           will be sent in an asynchronous way, so
**                           make sure the buffer is *not* on the stack.
**                           This buffer must be available until the
**                           next transmission.
**         txBufSize       - Size of the buffer in bytes
**     Returns     :
**         ---             - Error code, returns ERR_OK if USB
**                           enumeration has been finished, error code
**                           otherwise.
** ===================================================================
*/
byte CDC1_App_Task(byte *txBuf, size_t txBufSize)
{
  uint8_t i, res;

  /* device is Kinetis K60 */
  CDC1_RunUsbEngine();
  /* call the periodic task function */
  USB_Class_CDC_Periodic_Task();
  /* check whether enumeration is complete or not */
  if ((start_app==TRUE) && (start_transactions==TRUE)) {
    if (Tx1_NofElements()!=0) {
      i = 0;
      while(i<txBufSize && Tx1_Get(&txBuf[i])==ERR_OK) {
        i++;
      }
      res = CDC1_SendDataBlock(txBuf, i);
      if (res!=ERR_OK) {
        return res;
      }
#if 0 /* workaround for problem in USB stack v3.1.1: if last block is 8, 16, 32, 40, 48, ... bytes, it does not get out until the next transfer? */
      if ((i%8)==0) {
        /* workaround: sending a dummy block of zero bytes */
        res = CDC1_SendDataBlock(txBuf, 0);
        if (res!=ERR_OK) {
          return res;
        }
      }
#endif
    } /* if */
    return ERR_OK;
  } else {
    return ERR_BUSOFF; /* USB bus not available yet */
  }
}

/*
** ===================================================================
**     Method      :  CDC1_Init (component FSL_USB_CDC_Device)
**     Description :
**         Initializes the driver
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte CDC1_Init(void)
{
  uint_8 err;

  err = USB_Class_CDC_Init(CONTROLLER_ID, CDC1_App_Callback, NULL, CDC1_Notify_Callback, TRUE);
  if(err != USB_OK) {
    /* Error initializing USB-CDC Class */
    return ERR_FAILED;
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  CDC1_PutBufferChecked (component FSL_USB_CDC_Device)
**     Description :
**         Puts a data block into the output buffer, but does not send
**         it. If there is not enough size available, then ERR_TXFULL
**         is returned, otherwise ERR_OK. The application then needs to
**         call USB_App_Callback() to actually send the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to buffer to be sent
**         bufsize         - Buffer size in bytes
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte CDC1_PutBufferChecked(byte *buf, size_t bufSize)
{
  byte res;

  if(bufSize>CDC1_GetFreeInTxBuf()) { /* no room at the Inn... */
    res = ERR_TXFULL;
  } else {
    res = ERR_OK;
    while(bufSize>0 && res==ERR_OK) {
      res = Tx1_Put(*buf);
      bufSize--;
      buf++;
    }
  }
  return res;
}

/*
** ===================================================================
**     Method      :  CDC1_ClearRxBuffer (component FSL_USB_CDC_Device)
**     Description :
**         Clears the receiver buffer content
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
void CDC1_ClearRxBuffer(void)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  CDC1_ClearTxBuffer (component FSL_USB_CDC_Device)
**     Description :
**         Clears the transmit buffer content
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
void CDC1_ClearTxBuffer(void)
{
  Implemented as macro in the header file
}
*/

/* END CDC1. */

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

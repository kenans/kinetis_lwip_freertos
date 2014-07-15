/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : ACompLdd1.h
**     Project     : A058_Pilote_RF_IR_V5
**     Processor   : MK60FN1M0VLQ12
**     Component   : AnalogComp_LDD
**     Version     : Component 01.023, Driver 01.09, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-07-15, 18:01, # CodeGen: 1
**     Abstract    :
**         This component "AnalogComp_LDD" encapsulates Analog Comparator
**         peripheral supported on Freescale CPUs.
**         This component allows to select inputs to compare,
**         get the value of the comparator and specify when to generate
**         the compare event. If available for selected CPU, component
**         supports filtering and windowing functions.


**     Settings    :
**          Component name                                 : ACompLdd1
**          Analog comparator                              : CMP1
**          Interrupt service/event                        : Disabled
**          Positive inputs                                : 1
**            Positive input 0                             : 
**              Pin                                        : ADC0_SE16/CMP1_IN2/ADC0_SE21
**              Signal                                     : 
**              Source component                           : external
**          Negative inputs                                : 1
**            Negative input 0                             : 
**              Pin                                        : ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FBa_AD12/NFC_DATA9/I2S0_TX_FS
**              Signal                                     : 
**              Source component                           : external
**          Comparator output                              : Disabled
**          Output filter                                  : Disabled
**          Inverted output                                : no
**          Analog comp. mode                              : rising edge
**          Power mode                                     : Power saving
**          Hysteresis                                     : 0
**          DMA                                            : Disabled
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : yes
**            Positive input                               : Positive input 0
**            Negative input                               : Negative input 0
**            Event mask                                   : 
**              OnCompare                                  : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init           - LDD_TDeviceData* ACompLdd1_Init(LDD_TUserData *UserDataPtr);
**         Deinit         - void ACompLdd1_Deinit(LDD_TDeviceData *DeviceDataPtr);
**         GetOutputValue - LDD_AnalogComp_TOutputValue ACompLdd1_GetOutputValue(LDD_TDeviceData...
**         GetDriverState - LDD_TDriverState ACompLdd1_GetDriverState(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file ACompLdd1.h
** @version 01.09
** @brief
**         This component "AnalogComp_LDD" encapsulates Analog Comparator
**         peripheral supported on Freescale CPUs.
**         This component allows to select inputs to compare,
**         get the value of the comparator and specify when to generate
**         the compare event. If available for selected CPU, component
**         supports filtering and windowing functions.


*/         
/*!
**  @addtogroup ACompLdd1_module ACompLdd1 module documentation
**  @{
*/         

#ifndef __ACompLdd1_H
#define __ACompLdd1_H

/* MODULE ACompLdd1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "Cpu.h"
#include "CMP_PDD.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define ACompLdd1_PRPH_BASE_ADDRESS  0x40073008U
  
/*! Device data structure pointer used when auto initialization property is enabled. This constant can be passed as a first parameter to all component's methods. */
#define ACompLdd1_DeviceData  ((LDD_TDeviceData *)PE_LDD_GetDeviceStructure(PE_LDD_COMPONENT_ACompLdd1_ID))

/*  This constant contains mask of all events generated by Processor Expert */
#define ACompLdd1_AVAILABLE_EVENTS_MASK 0x00U
/* This constant contains number of available positive inputs */
#define ACompLdd1_AVAILABLE_POS_INPUTS_NUMBER 0x01U
/*  This constant contains mask of all available positive inputs */
#define ACompLdd1_AVAILABLE_POS_INPUTS_MASK (LDD_ANALOGCOMP_POSITIVE_INPUT_2_MASK)
/* This constant contains number of available negative inputs */
#define ACompLdd1_AVAILABLE_NEG_INPUTS_NUMBER 0x01U
/*  This constant contains mask of all available negative inputs */
#define ACompLdd1_AVAILABLE_NEG_INPUTS_MASK (LDD_ANALOGCOMP_NEGATIVE_INPUT_0_MASK)


typedef struct {
  LDD_TEventMask EventMask;            /* Mask of enabled events */
  LDD_TUserData *UserDataPtr;          /* RTOS device data structure */
} ACompLdd1_TDeviceData;               /* Device data structure type */

/*
** ===================================================================
**     Method      :  ACompLdd1_Init (component AnalogComp_LDD)
*/
/*!
**     @brief
**         Initializes the device according to design-time
**         configuration properties. Allocates memory for the device
**         data structure.
**         If the <Enable in init. code> is set to "yes" then the
**         device is also enabled and some time is required before
**         stabilizing of the comparator output. If the <Interrupt
**         service/event> property is enabled, it is recommended to
**         enable events after comparator output stabilizes to prevent
**         an unwanted event invocation before output stabilization.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an events or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* ACompLdd1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  ACompLdd1_Deinit (component AnalogComp_LDD)
*/
/*!
**     @brief
**         Disables the device and frees the device data structure
**         memory.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
*/
/* ===================================================================*/
void ACompLdd1_Deinit(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  ACompLdd1_GetOutputValue (component AnalogComp_LDD)
*/
/*!
**     @brief
**         This method stores current value of the (optionally filtered)
**         comparator output. Zero value is returned if comparator
**         output is low, otherwise non-zero value is returned.
**         If comparator is disabled then ERR_DISABLED is returned and
**         no data are stored.
**         If comparator has enabled filter and/or window sub-blocks
**         then stored value corresponds to filtered and/or windowed
**         comparator output.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Current comparator output value.
*/
/* ===================================================================*/
LDD_AnalogComp_TOutputValue ACompLdd1_GetOutputValue(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  ACompLdd1_GetDriverState (component AnalogComp_LDD)
*/
/*!
**     @brief
**         This method returns the current driver status.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - The current driver status mask.
**                           Following status masks defined in PE_Types.h
**                           can be used to check the current driver
**                           status.
**                           - PE_LDD_DRIVER_DISABLED_IN_CLOCK_CONFIGURAT
**                           ION - 1 - Driver is disabled in the current
**                           mode; 0 - Driver is enabled in the current
**                           mode.
**                           - PE_LDD_DRIVER_DISABLED_BY_USER - 1 -
**                           Driver is disabled by the user; 0 - Driver
**                           is enabled by the user.
*/
/* ===================================================================*/
LDD_TDriverState ACompLdd1_GetDriverState(LDD_TDeviceData *DeviceDataPtr);

/* END ACompLdd1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __ACompLdd1_H */
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

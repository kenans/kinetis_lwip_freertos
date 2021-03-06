/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Cmp1.h
**     Project     : A058_Pilote_RF_IR_V5
**     Processor   : MK60FN1M0VLQ12
**     Component   : FreescaleAnalogComp
**     Version     : Component 01.260, Driver 01.01, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-07-15, 18:01, # CodeGen: 1
**     Abstract    :
**         The component encapsulates Analog Comparator peripheral supported on Freescale HC08 derivatives
**     Settings    :
**          Component name                                 : Cmp1
**          Analog comparator                              : CMP1
**          Interrupt service/event                        : Disabled
**          Positive input                                 : 
**            Pin                                          : ADC0_SE16/CMP1_IN2/ADC0_SE21
**            Signal                                       : 
**          Additional positive inputs                     : 0
**          Negative input                                 : 
**            Pin                                          : ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FBa_AD12/NFC_DATA9/I2S0_TX_FS
**            Signal                                       : 
**          Additional negative inputs                     : 0
**          Comparator output                              : Disabled
**          Output filter                                  : Disabled
**          Inverted output                                : no
**          Analog comp. mode                              : rising edge
**          Power mode                                     : Power saving
**          Hysteresis                                     : smallest
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Event enabled in init.                       : yes
**          CPU clock/speed selection                      : 
**            High speed mode                              : This component enabled
**            Low speed mode                               : This component disabled
**            Slow speed mode                              : This component disabled
**     Contents    :
**         GetAnalogComparatorOutputValue - byte Cmp1_GetAnalogComparatorOutputValue(byte *pCmpOutputVal);
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
** @file Cmp1.h
** @version 01.01
** @brief
**         The component encapsulates Analog Comparator peripheral supported on Freescale HC08 derivatives
*/         
/*!
**  @addtogroup Cmp1_module Cmp1 module documentation
**  @{
*/         

#ifndef __Cmp1_H
#define __Cmp1_H

/* MODULE Cmp1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "ACompLdd1.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 



byte Cmp1_GetAnalogComparatorOutputValue(byte *pCmpOutputVal);
/*
** ===================================================================
**     Method      :  Cmp1_GetAnalogComparatorOutputValue (component FreescaleAnalogComp)
**     Description :
**         This method returns current value of the analog comparator
**         output.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * pCmpOutputVal   - Pointer to a variable
**                           to receive the current analog comparator
**                           output value. (Possible analog comparator
**                           output values are 0 or 1.)
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_SPEED - This device does not work in
**                           the active speed mode 
**                           - ERR_DISABLED - Device is disabled
** ===================================================================
*/



/* END Cmp1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __Cmp1_H */
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

/*
 * common.h
 *
 *  Created on: 11 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef COMMON_H_
#define COMMON_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Standard C includes */
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
/* Tool includes */
#include "crc.h"
/* lwIP/Ethernet driver includes */
#include "eth.h"
#include "lwip/tcpip.h"
#include "arch/ethernetif.h"


#endif /* COMMON_H_ */

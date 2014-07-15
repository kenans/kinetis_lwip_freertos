/*
 * low_power.h
 *
 *  Created on: 3 juil. 2014
 *      Author: OrpheoXlite
 */

#ifndef LOW_POWER_H_
#define LOW_POWER_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "common.h"
/**
 * -------------------------------------------------------------------------------------------
 *                                     Compile options
 * -------------------------------------------------------------------------------------------
 */
#define PILOTE_LOW_POWER_MODE  1
/**
 *  Returns pdTRUE if RTOS can enter tickless idle mode, pdFALSE otherwise
 */
extern BaseType_t xEnterTicklessIdle(void);

#endif /* LOW_POWER_H_ */

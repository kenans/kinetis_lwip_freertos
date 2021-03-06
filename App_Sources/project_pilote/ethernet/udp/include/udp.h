/*
 * udp.h
 *
 *  Created on: 22 juil. 2014
 *      Author: OrpheoXlite
 */

#ifndef UDP_H_
#define UDP_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
// System includes
#include "common.h"
// Project pilote includes
#include "pilote_config.h"
#include "pilote_mes.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  UDP_Task
 *      - An UDP task, called by Eth_Task()
 */
void UDP_Task(void *pvParameters);


#endif /* UDP_H_ */

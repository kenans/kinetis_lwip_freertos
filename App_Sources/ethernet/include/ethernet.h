/*
 * ethernet.h
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#ifndef ETHERNET_H_
#define ETHERNET_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "common.h"
#include "http_server.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  Ethernet system level task function. Called by RunTasks().
 */
void Eth_Task(void * pvParameters);

#endif /* ETHERNET_H_ */

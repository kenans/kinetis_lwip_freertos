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
#include "udp.h"
/**
 * -------------------------------------------------------------------------------------------
 *                                         Configurations
 * -------------------------------------------------------------------------------------------
 */
/**
 *  IP address
 */
#define ETH_IP_ADDR_0   192
#define ETH_IP_ADDR_1   168
#define ETH_IP_ADDR_2   1
#define ETH_IP_ADDR_3   59
/**
 *  Net Mask
 */
#define ETH_NET_MASK_0  255
#define ETH_NET_MASK_1  255
#define ETH_NET_MASK_2  255
#define ETH_NET_MASK_3  0
/**
 *  Gate Way
 */
#define ETH_GATE_WAY_0  192
#define ETH_GATE_WAY_1  168
#define ETH_GATE_WAY_2  1
#define ETH_GATE_WAY_3  1


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

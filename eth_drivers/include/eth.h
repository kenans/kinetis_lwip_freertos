/*
 * eth.h
 *
 *  Created on: 27 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef ETH_H_
#define ETH_H_

#include "eth_buf.h"
#include "eth_phy.h"

/**
 *  APIs
 */
// Is Ethernet link up
bool Eth_IsLinkUp(void);
// Read an Ethernet frame
bool Eth_ReadFrame(uint8_t *data, uint16_t *size);
// Send an Ethernet frame
bool Eth_SendFrame(uint8_t *data, uint16_t size);
// Initialization
bool Eth_Init(PhyConfig config, PhyLoopback loopback);

#endif /* ETH_H_ */

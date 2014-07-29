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
 *  Configurations
 */
#define ETH_MAC_ADDRESS_0 0x00U
#define ETH_MAC_ADDRESS_1 0x04U
#define ETH_MAC_ADDRESS_2 0x9FU
#define ETH_MAC_ADDRESS_3 0x02U
#define ETH_MAC_ADDRESS_4 0xF3U
#define ETH_MAC_ADDRESS_5 0xD3U

/**
 *  APIs
 */
// Is Ethernet link up
bool Eth_IsLinkUp(void);
// PHY enters power mode
bool Eth_EnterPowerMode(PhyPowerMode mode);
// Read an Ethernet frame
bool Eth_ReadFrame(uint8_t *data, uint16_t *size);
// Send an Ethernet frame
bool Eth_SendFrame(uint8_t *data, uint16_t size);
// Initialization
bool Eth_Init(PhyConfig config, PhyLoopback loopback);

#endif /* ETH_H_ */

/*
 * eth.c
 *
 *  Created on: 27 juin 2014
 *      Author: OrpheoXlite
 */

#include "eth.h"

/**
 *  Static declarations
 */
static EthFrameData _eth_data;

/**
 *  APIs
 */
// Initialization
// If success, returns ERR_OK
bool Eth_Init(PhyConfig config, PhyLoopback loopback)
{
    bool phy_init_success = FALSE;
    // MAC initialization
    _eth_data.eth_device_data = ETH1_Init(&_eth_data);
    // PHY initialization
    phy_init_success = PHY_Init(_eth_data.eth_device_data, PHY_ADDR, config, loopback);
    // Queues initialization
    if (phy_init_success) {
        TX_InitQueues(&_eth_data);
        RX_InitQueues(&_eth_data);
    } else {
        return 1;
    }
    return ERR_OK;
}
/**
 * Read an Ethernet frame
 *      If success, returns TRUE
 */
bool Eth_ReadFrame(uint8_t *data, uint16_t *size)
{
    return RX_ReceiveFrame(&_eth_data, data, size);
}

/**
 * Send an Ethernet frame
 *      If success, returns TRUE
 */
bool Eth_SendFrame(uint8_t *data, uint16_t size)
{
    return TX_SendFrame(&_eth_data, data, size);
}


/**
 * Is Eth link up
 *      if link up, returns TRUE
 */
bool Eth_IsLinkUp(void)
{
    return PHY_IsLinkUp(&_eth_data, PHY_ADDR);
}

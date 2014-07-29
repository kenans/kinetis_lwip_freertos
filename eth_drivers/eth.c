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
bool Eth_Init(PhyConfig config, PhyLoopback loopback)
{
    // MAC initialization
    _eth_data.eth_device_data = ETH1_Init(&_eth_data);
    // PHY initialization
    if (!PHY_Init(_eth_data.eth_device_data, PHY_ADDR, config, loopback)) {
        return FALSE;
    } else {
        // Queues initialization
        TX_InitQueues(&_eth_data);
        RX_InitQueues(&_eth_data);
    }
    return TRUE;
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
/**
 * Ethernet PHY enters the specific power mode
 *      If enters power mode successfully, returns TRUE
 */
bool Eth_EnterPowerMode(PhyPowerMode mode){
#if 0
    return PHY_EnterPowerMode(&_eth_data, PHY_ADDR, mode);
#else
    return TRUE;
#endif
}

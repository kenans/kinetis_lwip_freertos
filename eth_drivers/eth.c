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
/**
 *  Eth_MacInit
 *      This method initializes MAC controller and the Ethernet frame buffers
 *  If init ok, returns TRUE.
 */
bool Eth_MacInit(void)
{
    // MAC initialization
    _eth_data.eth_device_data = ETH1_Init(&_eth_data);
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    // Queues initialization
    TX_InitQueues(&_eth_data);
    RX_InitQueues(&_eth_data);
    return TRUE;
}
/**
 *  Eth_PhyInit
 *      This method initializes the Ethernet PHY, with a specific speed/duplex mode/loopback mode
 *  If init OK, returns TRUE;
 */
bool Eth_PhyInit(PhyConfig config, PhyLoopback loopback)
{
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    // PHY initialization
    if (!PHY_Init(_eth_data.eth_device_data, PHY_ADDR, config, loopback)) {
        return FALSE;
    }
    return TRUE;
}
/**
 * Read an Ethernet frame
 *      If success, returns TRUE
 */
bool Eth_ReadFrame(uint8_t *data, uint16_t *size)
{
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    return RX_ReceiveFrame(&_eth_data, data, size);
}

/**
 * Send an Ethernet frame
 *      If success, returns TRUE
 */
bool Eth_SendFrame(uint8_t *data, uint16_t size)
{
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    return TX_SendFrame(&_eth_data, data, size);
}


/**
 * Is Eth link up
 *      if link up, returns TRUE
 */
bool Eth_IsLinkUp(void)
{
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    return PHY_IsLinkUp(_eth_data.eth_device_data, PHY_ADDR);
}
/**
 * Ethernet PHY enters the specific power mode
 *      If enters power mode successfully, returns TRUE
 */
bool Eth_EnterPowerMode(PhyPowerMode mode)
{
#if 1  // Set 0 to disable PHY power mode
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    return PHY_EnterPowerMode(_eth_data.eth_device_data, PHY_ADDR, mode);
#else
    return TRUE;
#endif
}
/**
 * Reset Ethernet PHY
 *      If successfully reset, returns TRUE
 */
bool Eth_Reset(void)
{
    if (_eth_data.eth_device_data==NULL) {
        return FALSE;
    }
    return PHY_Reset(_eth_data.eth_device_data, PHY_ADDR);
}

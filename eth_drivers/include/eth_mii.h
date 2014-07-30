/*
 * eth_mii.h
 *
 *  Created on: 30 juil. 2014
 *      Author: OrpheoXlite
 */

#ifndef ETH_MII_H_
#define ETH_MII_H_
#include "ETH1.h"

// Constants
#define MII_TIMEOUT           100000UL

// APIs
bool MII_Read(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t *data);
bool MII_Write(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t data);

#endif /* ETH_MII_H_ */

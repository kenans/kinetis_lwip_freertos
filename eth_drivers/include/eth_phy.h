/*
 * eth_phy.h
 *
 *  Created on: Apr 10, 2014
 *      Author: OrpheoXlite
 */

#ifndef ETHERNETMANAGER_H_
#define ETHERNETMANAGER_H_

#include "ETH1.h"
#include "eth_mii.h"

// ------------------------- Common data structure declaration: ----------------------------------
typedef enum {
	PHY_AUTO_NEG,
	PHY_10_MBIT_HALF_DUPLEX,
	PHY_10_MBIT_FULL_DUPLEX,
	PHY_100_MBIT_HALF_DUPLEX,
	PHY_100_MBIT_FULL_DUPLEX
} PhyConfig;

typedef enum {
	PHY_NO_LOOPBACK,
	PHY_LOOPBACK,  /* External (PHY) loopback */
	PHY_ISOLATE    /* Internal (MAC) loopback */
} PhyLoopback;

typedef enum {
    PHY_POWER_ON,
    PHY_POWER_DOWN
} PhyPowerMode;

#define PHY_ADDR              1U        /* Depends on board configuration */

#define PHY_AUTO_NEG_TIMEOUT  10000UL


/* PHY register access defines */
#define MII_CONTROL_REG       0U
#define MII_STATUS_REG        1U
#define MII_RESET             0x8000U
#define MII_LOOP_BACK         0x4000U
#define MII_SPEED_100_MBIT    0x2000U
#define MII_AUTO_NEG_ENABLE   0x1000U
#define MII_POWER_DOWN        0x0800U
#define MII_ISOLATE           0x0400U
#define MII_FULL_DUPLEX       0x0100U
#define MII_AUTO_NEG_COMPLETE 0x0020U
#define MII_LINK_STATUS       0x0004U

/**
 *  PHY APIs
 */
bool PHY_Init(LDD_TDeviceData *eth_device_data, uint16_t phy_address, PhyConfig config, PhyLoopback loopback);
bool PHY_IsLinkUp(LDD_TDeviceData *eth_device_data, uint16_t phy_adress);
bool PHY_EnterPowerMode(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, PhyPowerMode mode);
bool PHY_Reset(LDD_TDeviceData *eth_device_data, uint16_t phy_adress);

#endif /* ETHERNETMANAGER_H_ */

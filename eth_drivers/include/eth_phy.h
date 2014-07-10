/*
 * eth_phy.h
 *
 *  Created on: Apr 10, 2014
 *      Author: OrpheoXlite
 */

#ifndef ETHERNETMANAGER_H_
#define ETHERNETMANAGER_H_

#include "ETH1.h"

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


#define PHY_ADDR              1        /* Depends on board configuration */

#define MII_TIMEOUT           100000

/* PHY register access defines */
#define MII_CONTROL_REG       0
#define MII_STATUS_REG        1
#define MII_LOOP_BACK         0x4000
#define MII_SPEED_100_MBIT    0x2000
#define MII_AUTO_NEG_ENABLE   0x1000
#define MII_ISOLATE           0x0400
#define MII_FULL_DUPLEX       0x0100
#define MII_AUTO_NEG_COMPLETE 0x0020
#define MII_LINK_STATUS       0x0004

/**
 *  PHY APIs
 */
bool PHY_Init(LDD_TDeviceData *eth_device_data, uint16_t phy_address, PhyConfig config, PhyLoopback loopback);
bool PHY_IsLinkUp(LDD_TDeviceData *eth_device_data, uint16_t phy_adress);

#endif /* ETHERNETMANAGER_H_ */

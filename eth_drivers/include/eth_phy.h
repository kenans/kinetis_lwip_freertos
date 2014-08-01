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

/**
 *  PHY Configurations
 */
#define PHY_ADDR              0x01U        /* Depends on board configuration */
#define PHY_AUTO_NEG_TIMEOUT  10000UL
/**
 *  PHY registers
 */
#define PHY_REG_BASIC_CONTROL               0x00U
#define PHY_REG_BASIC_STATUS                0x01U
#define PHY_REG_IDENTIFIER_1                0x02U
#define PHY_REG_IDENTIFIER_2                0x03U
#define PHY_REG_AUTO_NEG_ADVERTISE          0x04U
#define PHY_REG_AUTO_NEG_LINK_ABLILITY      0x05U
#define PHY_REG_AUTO_NEG_ECPANSION          0x06U
#define PHY_REG_AUTO_NEG_NEXT_PAGE          0x07U
#define PHY_REG_LINK_NEXT_PAGE_ABILITY      0x08U
#define PHY_REG_RXER_COUNTER                0x15U
#define PHY_REG_INTERRUPT_CONTROL_STATUS    0x1BU
#define PHY_REG_PHY_CONTROL_1               0x1EU
#define PHY_REG_PHY_CONTROL_2               0X1FU

/**
 *  Register Masks
 */
// Basic Control
#define PHY_RESET_MASK                  0x8000U
#define PHY_LOOP_BACK_MASK              0x4000U
#define PHY_SPEED_SELECT_MASK           0x2000U       // Set: 100Mbps; Clear: 10Mbps
#define PHY_AUTO_NEG_ENABLE_MASK        0x1000U
#define PHY_POWER_DOWN_MASK             0x0800U
#define PHY_ISOLATE_MASK                0x0400U
#define PHY_RESTART_AUTO_NEG_MASK       0x0200U
#define PHY_DUPLEX_MODE_MASK            0x0100U       // Set: Full-duplex; Clear: Half-duplex
#define PHY_COLLISION_TEST_MASK         0x0080U
#define PHY_DISABLE_TRANSMITTER_MASK    0x0001U
// Basic Status
#define PHY_100BASE_T4_MASK             0x8000U
#define PHY_100BASE_FULL_DUPLEX_MASK    0X4000U
#define PHY_100BASE_HALF_DUPLEX_MASK    0x2000U
#define PHY_10BASE_FULL_DUPLEX_MASK     0x1000U
#define PHY_10BASE_HALF_DUPLEX_MASK     0x0800U
#define PHY_NO_PREAMBLE_MASK            0x0040U
#define PHY_AUTO_NEG_COMPLETE_MASK      0x0020U
#define PHY_REMOTE_FAULT_MASK           0x0010U
#define PHY_AUTO_NEG_ABILITY_MASK       0x0008U
#define PHY_LINK_STATUS_MASK            0x0004U
#define PHY_JABBER_DETECT_MASK          0x0002U
#define PHY_EXTENDED_CAPABILITY_MASK    0x0001U

/**
 *  PHY APIs
 */
bool PHY_Init(LDD_TDeviceData *eth_device_data, uint16_t phy_address, PhyConfig config, PhyLoopback loopback);
bool PHY_IsLinkUp(LDD_TDeviceData *eth_device_data, uint16_t phy_adress);
bool PHY_EnterPowerMode(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, PhyPowerMode mode);
bool PHY_Reset(LDD_TDeviceData *eth_device_data, uint16_t phy_adress);

#endif /* ETHERNETMANAGER_H_ */

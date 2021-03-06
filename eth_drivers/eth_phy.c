/*
 * eth_phy.c
 *
 *  Created on: Apr 10, 2014
 *      Author: OrpheoXlite
 */

#include "eth_phy.h"

// Static functions declarations

// ------------------------- PHY APIs ----------------------------------
/**
 *  Initializes PHY. Returns FALSE if failed.
 *      This is an external [API], called to initialize a PHY.
 *  @param
 *      *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      phy_adress : The PHY address
 *      config  : A enum type, describes the configuration of PHY. Defined in eth_phy.h
 *      loopback: A enum type, describes the loopback mode of PHY. Defined in eth_phy.h
 *  @return:
 *      If init with success, returns TRUE; In case of timeout or other hardware error, returns FALSE.
 */
bool PHY_Init(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, PhyConfig config, PhyLoopback loopback)
{
    uint32_t Timeout = 0;
    uint16_t Status = 0;
    uint16_t Control = 0;
    bool Configured = FALSE;

    MII_Read(eth_device_data, phy_adress, PHY_REG_BASIC_STATUS, &Status);
    /* Check reserved bits and capabilities bits */
    if (((Status & 0x0780U) != 0) && ((Status & 0x7800U) != 0x7800U)) {
        return FALSE;
    }
    if (config == PHY_AUTO_NEG) {
        /* Perform autonegotiation */
        MII_Read(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, &Control);
        Control = PHY_AUTO_NEG_ENABLE_MASK;
        ETH1_WriteMII(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, Control);
        /* Wait for cable connection and autonegotiation completion */
        do {
            MII_Read(eth_device_data, phy_adress, PHY_REG_BASIC_STATUS, &Status);
            Configured = Status & (PHY_LINK_STATUS_MASK|PHY_AUTO_NEG_COMPLETE_MASK);
        } while (!Configured && Timeout++ < PHY_AUTO_NEG_TIMEOUT);
    } else {
        switch (loopback) {
        case PHY_NO_LOOPBACK:
            Control = 0;
            break;
        case PHY_LOOPBACK:
            Control = PHY_LOOP_BACK_MASK;
            break;
        case PHY_ISOLATE:
            Control = PHY_ISOLATE;
            break;
        }
        switch (config) {
        case PHY_10_MBIT_HALF_DUPLEX:
            Control |= 0;
            break;
        case PHY_10_MBIT_FULL_DUPLEX:
            Control |= PHY_DUPLEX_MODE_MASK;
            break;
        case PHY_100_MBIT_HALF_DUPLEX:
            Control |= PHY_SPEED_SELECT_MASK;
            break;
        case PHY_100_MBIT_FULL_DUPLEX:
            Control |= PHY_SPEED_SELECT_MASK | PHY_DUPLEX_MODE_MASK;
            break;
        default:
            // To suppress the PHY_AUTO_NEG not handled warning
            break;
        }
        MII_Write(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, Control);
    }
    return Timeout < MII_TIMEOUT;
}

/**
 *  Method: PHY_IsLinkUp
 *      Reads the PHY status register to check if Ethernet link is up. If link is up,
 *      returns TRUE, if not, returns FALSE
 *  @param
 *      - *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      - phy_adress : The PHY address
 *  @return
 *      If link is up, returns TRUE
 */
bool PHY_IsLinkUp(LDD_TDeviceData *eth_device_data, uint16_t phy_adress)
{
    uint16_t status = 0;
    if (!MII_Read(eth_device_data, phy_adress, PHY_REG_BASIC_STATUS, &status)) {
        return FALSE;
    }
    if (!(status&PHY_LINK_STATUS_MASK)) {
        return FALSE;
    }
    return TRUE;
}
/**
 *  Method: PHY_EnterPowerMode
 *      Write the PHY control register bit 11 to set its power mode.
 *  @param:
 *      - *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      - phy_adress : The PHY address
 *      - mode: PHY power mode
 *  @return
 *      If set OK, returns TRUE
 */
bool PHY_EnterPowerMode(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, PhyPowerMode mode)
{
    uint16_t control_reg=0;
    MII_Read(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, &control_reg);
    switch (mode) {
        case PHY_POWER_ON:
            return MII_Write(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, control_reg&(~PHY_POWER_DOWN_MASK));
        case PHY_POWER_DOWN:
            return MII_Write(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, control_reg|(PHY_POWER_DOWN_MASK));
        default:
            break;
    }
    return FALSE;
}
/**
 *  Method: PHY_Reset
 *      Reset PHY, if success, returns TRUE
 *  @param
 *      - *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      - phy_adress : The PHY address
 *  @return
 *      If reset successfully, returns TRUE
 */
bool PHY_Reset(LDD_TDeviceData *eth_device_data, uint16_t phy_adress)
{
    if (eth_device_data==NULL) return FALSE;
    return MII_Write(eth_device_data, phy_adress, PHY_REG_BASIC_CONTROL, PHY_RESET_MASK);
}



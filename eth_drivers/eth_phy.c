/*
 * eth_phy.c
 *
 *  Created on: Apr 10, 2014
 *      Author: OrpheoXlite
 */

#include "eth_phy.h"

// Static functions declarations
static bool MII_Read(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t *data);
static bool MII_Write(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t data);

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
    uint16_t Status;
    uint16_t Control;
    bool Configured = FALSE;

    MII_Read(eth_device_data, phy_adress, MII_STATUS_REG, &Status);
    /* Check reserved bits and capabilities bits */
    if (((Status & 0x0780) != 0) && ((Status & 0x7800) != 0x7800)) {
        return FALSE;
    }
    if (config == PHY_AUTO_NEG) {
        /* Perform autonegotiation */
        MII_Read(eth_device_data, phy_adress, MII_CONTROL_REG, &Control);
        Control = MII_AUTO_NEG_ENABLE;
        ETH1_WriteMII(eth_device_data, phy_adress, MII_CONTROL_REG, Control);
        /* Wait for cable connection and autonegotiation completion */
        do {
            MII_Read(eth_device_data, phy_adress, MII_STATUS_REG, &Status);
            Configured = Status & (MII_LINK_STATUS | MII_AUTO_NEG_COMPLETE);
        } while (!Configured && Timeout++ < MII_TIMEOUT);
    } else {
        switch (loopback) {
        case PHY_NO_LOOPBACK:
            Control = 0;
            break;
        case PHY_LOOPBACK:
            Control = MII_LOOP_BACK;
            break;
        case PHY_ISOLATE:
            Control = MII_ISOLATE;
            break;
        }
        switch (config) {
        case PHY_10_MBIT_HALF_DUPLEX:
            Control |= 0;
            break;
        case PHY_10_MBIT_FULL_DUPLEX:
            Control |= MII_FULL_DUPLEX;
            break;
        case PHY_100_MBIT_HALF_DUPLEX:
            Control |= MII_SPEED_100_MBIT;
            break;
        case PHY_100_MBIT_FULL_DUPLEX:
            Control |= MII_SPEED_100_MBIT | MII_FULL_DUPLEX;
            break;
        default:
            // To suppress the PHY_AUTO_NEG not handled warning
            break;
        }
        MII_Write(eth_device_data, phy_adress, MII_CONTROL_REG, Control&(~MII_POWER_DOWN));
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
    bool link_up = TRUE;
    uint16_t status = 0;
    if (!MII_Read(eth_device_data, phy_adress, MII_STATUS_REG, &status)) {
        return FALSE;
    }
    if ((status&MII_LINK_STATUS) == 0U)
        link_up = FALSE;
    return link_up;
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
    uint16_t control_reg;
    switch (mode) {
        MII_Read(eth_device_data, phy_adress, MII_CONTROL_REG, &control_reg);
        case PHY_POWER_ON:
            return MII_Write(eth_device_data, phy_adress, MII_CONTROL_REG, control_reg&(~MII_POWER_DOWN));
        case PHY_POWER_DOWN:
            return MII_Write(eth_device_data, phy_adress, MII_CONTROL_REG, control_reg|(MII_POWER_DOWN));
        default:
            break;
    }
    return FALSE;
}
// ------------------------- PHY register access functions: ----------------------------------
/**
 * 	Reads a PHY register by MII(RMII). Returns FALSE if failed.
 * 		This is a block function with MII_TIMEOUT defined in eth_phy.h
 * 		This is a static function, called only in this file.
 * 	@para
 * 		*eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 * 		phy_adress : The PHY address
 *		reg_address : The register address of PHY
 *		*data	: Pointer to the data of register. Address should be preallocated
 *	@return:
 *		If read with success, returns TRUE; if timeout, returns FALSE.
 */
static bool MII_Read(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t *data)
{
	uint32_t Timeout = 0;

	ETH1_ReadMII(eth_device_data, phy_adress, reg_address);
	/* Wait for response */
	while (ETH1_GetMIIResult(eth_device_data, data) == ERR_BUSY && Timeout++ < MII_TIMEOUT) {}
	return Timeout < MII_TIMEOUT;
}

/**
 * 	Writes a PHY register. Returns FALSE if failed.
 * 		This is a block function with MII_TIMEOUT defined in eth_phy.h
 * 		This is a static function, called only in this file.
 * 	@para
 * 		*eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 * 		phy_adress : The PHY address
 *		reg_address : The register address of PHY
 *		data	: The data to write
 *	@return:
 *		If write with success, returns TRUE; if timeout, returns FALSE.
 */
static bool MII_Write(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t data)
{
	uint32_t Timeout = 0;

	ETH1_WriteMII(eth_device_data, phy_adress, reg_address, data);
	/* Wait for response */
	while (ETH1_GetMIIResult(eth_device_data, &data) == ERR_BUSY && Timeout++ < MII_TIMEOUT) {}
	return Timeout < MII_TIMEOUT;
}



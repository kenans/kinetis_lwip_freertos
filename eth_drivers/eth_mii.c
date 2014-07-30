/*
 * eth_mii.c
 *
 *  Created on: 30 juil. 2014
 *      Author: OrpheoXlite
 */


#include "eth_mii.h"
// ------------------------- PHY register access functions: ----------------------------------
/**
 *  Reads a PHY register by MII(RMII). Returns FALSE if failed.
 *      This is a block function with MII_TIMEOUT defined in eth_phy.h
 *      This is a static function, called only in this file.
 *  @para
 *      *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      phy_adress : The PHY address
 *      reg_address : The register address of PHY
 *      *data   : Pointer to the data of register. Address should be preallocated
 *  @return:
 *      If read with success, returns TRUE; if timeout, returns FALSE.
 */
bool MII_Read(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t *data)
{
    uint32_t Timeout = 0;

    ETH1_ReadMII(eth_device_data, phy_adress, reg_address);
    /* Wait for response */
    while (ETH1_GetMIIResult(eth_device_data, data) == ERR_BUSY && Timeout++ < MII_TIMEOUT) {}
    return Timeout < MII_TIMEOUT;
}

/**
 *  Writes a PHY register. Returns FALSE if failed.
 *      This is a block function with MII_TIMEOUT defined in eth_phy.h
 *      This is a static function, called only in this file.
 *  @para
 *      *eth_device_data : A pointer to the instance of the ETH_TDeviceData structure
 *      phy_adress : The PHY address
 *      reg_address : The register address of PHY
 *      data    : The data to write
 *  @return:
 *      If write with success, returns TRUE; if timeout, returns FALSE.
 */
bool MII_Write(LDD_TDeviceData *eth_device_data, uint16_t phy_adress, uint16_t reg_address, uint16_t data)
{
    uint32_t Timeout = 0;

    ETH1_WriteMII(eth_device_data, phy_adress, reg_address, data);
    /* Wait for response */
    while (ETH1_GetMIIResult(eth_device_data, &data) == ERR_BUSY && Timeout++ < MII_TIMEOUT) {}
    return Timeout < MII_TIMEOUT;
}

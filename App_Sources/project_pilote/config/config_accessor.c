/*
 * config_accessor.c
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */


#include "config_accessor.h"

/**
 *  Save configuration from RAM to EEPROM
 *  @param
 *      *pilote_config_ptr : A pointer to the PiloteConfigurations structure
 *  @return
 *      If no error, returns ERR_OK
 *      If error occurs, returns ERR_EEPROM
 */
err_t PiloteSaveConfig(PiloteConfigurations *pilote_config_ptr)
{
    /**
     * Save data into the EEPROM
     */
    err_t error = ERR_OK;
#if USE_EEPROM==1
#if USE_24AA1026==1
    // Write the whole structure to EEPROM
    if (EE241_WriteBlock(EEPROM_MEM_START_ADDR, (byte*)pilote_config_ptr, sizeof(PiloteConfigurations)) != ERR_OK) {
        // Error write eeprom
        error = ERR_EEPROM;
    }
#endif // USE_24AA1026==1
#endif // USE_EEPROM
    return error;
}

/**
 *  Load configuration from EEPROM to RAM
 *  @param
 *      *pilote_config_ptr : A pointer to the PiloteConfigurations structure
 *  @return
 *      If no error, returns ERR_OK
 *      If error occurs, returns ERR_EEPROM
 */
err_t PiloteLoadConfig(PiloteConfigurations *pilote_config_ptr)
{
    /**
     * Load data from EEPROM
     */
    err_t error = ERR_OK;
#if USE_EEPROM==1
#if USE_24AA1026==1
    // Read the whole structure from EEPROM
    if (EE241_ReadBlock(EEPROM_MEM_START_ADDR, (byte*)pilote_config_ptr, sizeof(PiloteConfigurations)) != ERR_OK) {
        // Error write eeprom
        error = ERR_EEPROM;
    }
#endif // USE_24AA1026==1
#endif // USE_EEPROM
    return error;
}

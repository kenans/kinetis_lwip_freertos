/*
 * config_accessor.h
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef CONFIG_ACCESSOR_H_
#define CONFIG_ACCESSOR_H_
/**
 * -------------------------------------------------------------------------------------------
 *                                          Attention
 * -------------------------------------------------------------------------------------------
 */
/*
 *  This file defines access to a non-volatile memory component. However using which kind of
 *  component is undefined.
 *  Some compile options can be set, if want to enable/disable some functions.
 *
 *  A 24AA1026 EEPROM is used now. This component uses a I2C bus. No timeout is used in both
 *  24AA1026 and I2C bus drivers. So if any error occurs, it might get stuck in the loop
 *  somewhere in the driver (eg. Waiting for a ACK or an interrupt flag).
 */

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_config.h"
#include "pilote_mes.h"
#include "pilote_error.h"
#include "pilote_types.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                     Compile options
 * -------------------------------------------------------------------------------------------
 */
/*
 *  If USE_EEPROM==1, enables access to EEPROM;
 *  If USE_EEPROM==0, disables access to EEPROM, so PiloteSaveConfig() and PiloteLoadConfig()
 *  will always return ERR_OK without doing anything.
 */
#define USE_EEPROM      1
#define USE_24AA1026    1

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  Save configuration
 */
err_t PiloteSaveConfig(PiloteConfigurations *);

/**
 *  Load configuration
 */
err_t PiloteLoadConfig(PiloteConfigurations *);

#endif /* CONFIG_ACCESSOR_H_ */

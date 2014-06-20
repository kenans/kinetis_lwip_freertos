/*
 * pilote_error.h
 *
 *  Created on: 6 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef PILOTE_ERROR_H_
#define PILOTE_ERROR_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "common.h"

/**
 * -----------------------------------------------------------------------------
 *                       Pilote Error type definition
 * -----------------------------------------------------------------------------
 */
// Compatibility with lwIP
#ifdef __LWIP_ERR_H__
// No use to define err_t
#else
typedef signed char err_t; // -128 ~ +127
#endif

/**
 * ------------------------------------------------------------------------------------------
 *                                  Error values
 * ------------------------------------------------------------------------------------------
 * Attention :
 *     These errors might already be defined in other files (err.h for lwIP; PE_Error.h for PEx).
 * Should always turn on "-Wall" option in order to check if macros are redefined.
 *     Anyway, ERR_OK should be set 0, meaning no error occurs.
 * ------------------------------------------------------------------------------------------
 */
#ifndef ERR_OK
#define ERR_OK          0    // No error
#endif

#ifndef ERR_COMMON
#define ERR_COMMON     1    // common error, type not specified
#endif

#ifndef ERR_MEM
#define ERR_MEM        -1    // Out of memory error
#endif

#ifndef ERR_BUF
#define ERR_BUF        -2    // Buffer error
#endif

#ifndef ERR_TIMEOUT
#define ERR_TIMEOUT    -3    // Timeout
#endif

#ifndef ERR_EEPROM
#define ERR_EEPROM     -4    // EEPROM read or write error
#endif

#ifndef ERR_IR
#define ERR_IR         -5    // InfraRed error
#endif

#endif /* PILOTE_ERROR_H_ */

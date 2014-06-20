/*
 * crc.h
 *
 *  Created on: 13 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef CRC_H_
#define CRC_H_


/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "stdint.h"


/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */


/**
 *  METHOD:
 *  @bref
 *      Calculate by Byte the CRC code, using a table of CRC code.
 *  @para
 *      *data
 *          - Adress of the first data frame
 *      len
 *          - Length of the data frame in *byte*
 *  @return
 *      crc
 *          - The calculated CRC code
 */
uint8_t GetCRC8(const uint8_t *data, uint32_t len);

#endif /* CRC_H_ */

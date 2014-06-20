/*
 * pilote_mes.h
 *
 *  Created on: 16 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef PILOTE_MES_H_
#define PILOTE_MES_H_


/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_error.h"
#include "pilote_types.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                      Structures
 * -------------------------------------------------------------------------------------------
 */
/**
 *  PiloteMessagePackage structure. A message USB/UDP/http/etc package, for IN or OUT
 */
typedef struct {
    PiloteMesDirection      direction;  // Recv: from PC to MCU; Send: from MCU to PC
    PiloteMesType           mes_type;
    PiloteMesOperation      operation;
} PiloteMessagePackage;

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */


#endif /* PILOTE_MES_H_ */

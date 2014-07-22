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
 *  PiloteMessagePackage structure. A message USB/UDP/http/etc package, used for configuring
 */
typedef struct {
    PiloteMesDirection          direction;              // Recv: from PC to MCU; Send: from MCU to PC
    PiloteMesType               mes_type;               // USB/UDP/HTTP
    PiloteMesOperation          operation;              // Start/End/Read/Modify/Reply
    PiloteMesTarget             target;                 //
    uint32_t                    data;                   // Message value
    //void                        *data_ptr;              // A pointer to the message value if is needed
} PiloteMessagePackage;

/**
 *
 */

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */
/**
 *  Initialize a receive message package.
 *  @param
 *      mes_pkg: A receive message package structure instance
 */
#define PiloteInitRecvMesPackage(mes_pkg) do{(mes_pkg).direction=PILOTE_MES_RECV;}while(0)

/**
 *  Initialize a send message package.
 *  @param
 *      mes_pkg: A send message package structure instance
 */
#define PiloteInitSendMesPackage(mes_pkg) do{(mes_pkg).direction=PILOTE_MES_SEND;}while(0)

#endif /* PILOTE_MES_H_ */

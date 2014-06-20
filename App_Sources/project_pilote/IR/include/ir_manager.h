/*
 * ir_manager.h
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef IR_MANAGER_H_
#define IR_MANAGER_H_


/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_config.h"
#include "pilote_error.h"
#include "pilote_types.h"
#include "ir_transmitter.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 * IR task function. Called by RunTasks().
 */
void IR_Thread(void *);


#endif /* IR_MANAGER_H_ */

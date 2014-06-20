/*
 * Applications.h
 *
 *  Created on: 11 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef APPLICATIONS_H_
#define APPLICATIONS_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
/**
 * Common includes
 */
#include "common.h"     // PEx includes
/**
 * Application includes
 */
#include "config_manager.h"
#include "ir_manager.h"
#include "usb_manager.h"


/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 * Create all system level tasks. Called in main().
 *      Attention : It will only *create* tasks. The scheduler starts only after
 *      calling this function.
 */
void RunTasks(void);


#endif /* APPLICATIONS_H_ */

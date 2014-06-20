/*
 * config_manager.h
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_


/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_config.h"
#include "pilote_mes.h"
#include "pilote_error.h"
#include "pilote_types.h"
#include "config_accessor.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  Configuration task function. Called by RunTasks().
 */
void ConfigThread(void *);

#endif /* CONFIG_MANAGER_H_ */

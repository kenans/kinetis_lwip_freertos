/*
 * usb_manager.h
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#ifndef USB_MANAGER_H_
#define USB_MANAGER_H_
/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "common.h"
#include "pilote_config.h"
#include "pilote_mes.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */
/**
 * USB task function. Called by RunTasks().
 */
void USB_Task(void *pvParameters);

#endif /* USB_MANAGER_H_ */

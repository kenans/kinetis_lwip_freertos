/*
 * usb_manager.h
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#ifndef USB_MANAGER_H_
#define USB_MANAGER_H_

#include "common.h"
#include "pilote_config.h"
#include "pilote_mes.h"

#define USB_IN_BUFFER_COUNT 32

void USB_Task(void *pvParameters);

#endif /* USB_MANAGER_H_ */

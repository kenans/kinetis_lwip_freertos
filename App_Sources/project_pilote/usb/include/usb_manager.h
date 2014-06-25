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
 *                                          Macros
 * -------------------------------------------------------------------------------------------
 */
#define _LOCAL_BUFFER_COUNT 32
#define LocalBuf_IsEmpty()          ((_start)==(_end))
#define LocalBuf_IsFull()           (((_end+1)%(_size))==(_start))
#define LocalBuf_NumOfElements()    (((_end)>=(_start))?((_end)-(_start)):((_end)+(_size)-(_start)-1))
#define LocalBuf_Put(item)          do{_local_buf[_end]=item; ++_end; _end=_end%_size;}while(0)
#define LocalBuf_Get()              (++_start,_start=_start%_size,\
        (_start==0)?(_local_buf[_size-1]):(_local_buf[_start-1]))
#define LocalBuf_Peek()             (_local_buf[_start])
#define LocalBuf_PeekIndex(index)   (_local_buf[(_start+index)%_size])
#define LocalBuf_Remove()           do{_start++;_start%=_size;}while(0)
#define LocalBuf_Init()             do{(_start)=0;(_end)=0;}while(0)
#define LocalBuf_Clear()            LocalBuf_Init()
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

/*
 * low_power.c
 *
 *  Created on: 3 juil. 2014
 *      Author: OrpheoXlite
 */

#include "low_power.h"

static bool _usb_connected;
/**
 *  Returns pdTRUE if RTOS can enter tickless idle mode, pdFALSE otherwise
 */
BaseType_t xEnterTicklessIdle(void)
{
#if PILOTE_LOW_POWER_MODE==1
    Cmp1_GetAnalogComparatorOutputValue(&_usb_connected);
    /**
     *  If USB connected, disables low power mode, returns pdFALSE; pdTREU otherwise
     */
    if (_usb_connected)       // USB connected
        return pdFALSE;
    else
        return pdTRUE;
#else
    return pdFALSE;
#endif
}

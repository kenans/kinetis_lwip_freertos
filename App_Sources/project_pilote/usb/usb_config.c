/*
 * usb_cdc.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#include "usb_config.h"

static uint8_t              _cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t              _in_buffer[USB1_DATA_BUFF_SIZE];
static PiloteMessagePackage _pilote_mes_package;
static bool                 _running = FALSE;
/**
 * USB_Task
 * 		- A USB CDC Device task
 */
void USB_Task(void *pvParameters)
{
    (void)pvParameters; /* not used */
    uint16_t i = 0;
    extern xQueueHandle mbox_pilote_recv;   // Global queue definition
    PiloteMessagePackage *pilote_mes_package_ptr = &_pilote_mes_package;   // Used for Mes Queue

    while (1) {
        while(CDC1_App_Task(_cdc_buffer, sizeof(_cdc_buffer)) == ERR_BUSOFF) {
            // USB not connected, so device not enumerated
            if (_running) {                                         // If it's the
                _running=FALSE;
                pilote_mes_package_ptr->direction = PILOTE_MES_RECV;
                pilote_mes_package_ptr->mes_type = PILOTE_MES_TYPE_USB;
                pilote_mes_package_ptr->operation = PILOTE_MES_OPERATION_STOP;
                xQueueSend(mbox_pilote_recv, &pilote_mes_package_ptr, 0);
            }
            vTaskDelay(500/portTICK_PERIOD_MS);
        }

        if (!_running) {
            _running = TRUE;
            pilote_mes_package_ptr->direction = PILOTE_MES_RECV;
            pilote_mes_package_ptr->mes_type = PILOTE_MES_TYPE_USB;
            pilote_mes_package_ptr->operation = PILOTE_MES_OPERATION_START;
            xQueueSend(mbox_pilote_recv, &pilote_mes_package_ptr, 0);
        }

        if (CDC1_GetCharsInRxBuf()!=0) {
            i = 0;
            while(i < sizeof(_in_buffer)-1 &&
                  CDC1_GetChar(&_in_buffer[i])==ERR_OK) {
                i++;
            }
            _in_buffer[i] = '\0';
            (void)CDC1_SendString((unsigned char*)"echo: ");
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

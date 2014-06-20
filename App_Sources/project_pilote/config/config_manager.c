/*
 * config_manager.c
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#include "config_manager.h"

/**
 * Global decelerations
 */

/*
 * Static decelerations
 * Only used in this file
 */
static PiloteConfigurations _pilote_config;                      // Create instance. It's the only instance of program

/**
 *  Configuration task function. Called by RunTasks().
 */
void ConfigThread(void *pvParameters)
{
    (void)pvParameters;                                         // Suppress unused variable warning
    extern xQueueHandle mbox_pilote_config;                     // Global message queue
    extern xQueueHandle mbox_pilote_recv;
    //extern xQueueHandle mbox_pilote_send;
    bool configuring = FALSE;
    PiloteConfigurations *pilote_config_ptr = &_pilote_config;  // Used in message queue
    PiloteMessagePackage *pilote_mes_package_ptr;               // To receive a message

    if (PiloteConfigInit(pilote_config_ptr) != ERR_OK) {        // Initialization
        // Error occurs
        while (1)
            ;
    }
    if (PiloteLoadConfig(pilote_config_ptr) != ERR_OK) {        // Load configurations from EEPROM
        // Error occurs
        while (1)
            ;
    }
    while (mbox_pilote_config == NULL ||
           mbox_pilote_recv   == NULL) {                        // Wait for mailbox being created
        vTaskDelay(FREE_RTOS_DELAY_50MS);
    }
    // Start IR by default
    if (xQueueSend(mbox_pilote_config, (void*)&(pilote_config_ptr), MBOX_ZERO_TIMEOUT) != pdTRUE) {
        // Start IR error
        while (1)
            ;
    }
    while (1) {
        /**
         * TODO
         * 1. Block on a message queue with a timeout;
         * 2. If get a message,
         *      2.1 If is a START, block IR; if is an END, unblock IR.
         *      2.2 If is a USB message:
         *          a.
         *      2.3 If is a UDP message:
         *      2.4 If is a HTTP message:
         * 3. If error occurs, do nothing; else call PiloteWriteConfig()
         * 4. Check data consistency
         * 5. If everything is OK, unlock IR; else roll back transaction
         */
        if (xQueueReceive(mbox_pilote_recv, &pilote_mes_package_ptr, MBOX_TIMEOUT_500MS) == pdTRUE) {
            // If got a message
            if (pilote_mes_package_ptr != NULL) {               // Make sure nothing wrong
                if (pilote_mes_package_ptr->direction == PILOTE_MES_RECV) {
                    switch (pilote_mes_package_ptr->operation) {
                    case PILOTE_MES_OPERATION_START:
                        if (!configuring) {
                            configuring = TRUE;
                            xQueueReset(mbox_pilote_config);    // Block IR here
                        }
                        break;
                    case PILOTE_MES_OPERATION_STOP:
                        if (configuring) {
                            configuring = FALSE;
                            xQueueSend(mbox_pilote_config,
                                      (void*)&(pilote_config_ptr),
                                      MBOX_ZERO_TIMEOUT);       // Unblock IR here
                        }
                        break;
                    case PILOTE_MES_OPERATION_MODIFY:
                        break;
                    default:
                        // Should never get here
                        break;
                    }

                    if (configuring) {
                        switch (pilote_mes_package_ptr->mes_type) {
                        case PILOTE_MES_TYPE_USB:
                            break;
                        case PILOTE_MES_TYPE_UDP:
                            break;
                        case PILOTE_MES_TYPE_HTTP:
                            break;
                        default:
                            // Should never get here
                            break;
                        }

                        // Test :
                        uint8_t i = 0;
                        for (i = 0; i < 5 ; i++) {
                            while (AS1_SendChar((AS1_TComData)"TEST\n"[i]) != ERR_OK)
                                ;
                        }

                        // If no problem, write something to EEPROM??
                        // TODO

                        // Check data consistency!

                        // If still no problem, OK, else try to roll back transaction
                    }
                }
            } else {
                // Error : got NULL message. Should never get here
                while (1)
                    ;
            }
        } /*else {
            // If timeout
            if (!configuring && uxQueueMessagesWaiting(mbox_pilote_config) == MBOX_ZERO_ITEM) {
                // Do unblock IR, only when we are not configuring and IR is blocked
                xQueueSend(mbox_pilote_config, (void*)&(pilote_config_ptr), MBOX_ZERO_TIMEOUT); // Without TIME_OUT
            }
        }*/
    }
}

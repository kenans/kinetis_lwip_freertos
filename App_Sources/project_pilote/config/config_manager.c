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
static PiloteMessagePackage _mes_pkg_recv;
static PiloteMessagePackage _mes_pkg_send;

/**
 *  Configuration task function. Called by RunTasks().
 */
void ConfigThread(void *pvParameters)
{
    (void)pvParameters;                                         // Suppress unused variable warning
    extern xQueueHandle mbox_pilote_config;                     // Global message queue
    extern xQueueHandle mbox_pilote_recv;
    extern xQueueHandle mbox_pilote_send;

    bool configuring = FALSE;
    PiloteConfigurations *pilote_config_ptr = &_pilote_config;  // Used in message queue

    if (PiloteConfigInit(pilote_config_ptr) != ERR_OK) {        // Initialization
        while (1) {
            // Initialization error
        }
    }
    if (PiloteLoadConfig(pilote_config_ptr) != ERR_OK) {        // Load configurations from EEPROM
        while (1) {
            // Load EEPROM error
        }
    }
    while (mbox_pilote_config == NULL ||
           mbox_pilote_recv   == NULL ||
           mbox_pilote_send   == NULL) {                        // Wait for mailboxes being created
        vTaskDelay(FREE_RTOS_DELAY_50MS);
    }
    // Start IR by default
    if (xQueueSend(mbox_pilote_config, (void*)&(pilote_config_ptr), MBOX_ZERO_TIMEOUT) != pdTRUE) {
        while (1) {
            // Start IR error
        }
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
        if (xQueueReceive(mbox_pilote_recv, &_mes_pkg_recv, MBOX_TIMEOUT_500MS) == pdTRUE) {
            if (_mes_pkg_recv.direction == PILOTE_MES_RECV) {
                // If got a message
                switch (_mes_pkg_recv.operation) {
                    case PILOTE_MES_OPERATION_START:            // USB connected
                        if (!configuring) {
                            configuring = TRUE;
                            xQueueReset(mbox_pilote_config);    // Block IR here
                        }
                        break;
                    case PILOTE_MES_OPERATION_STOP:             // USB disconnected
                        if (configuring) {
                            configuring = FALSE;
                        }
                        break;
                    default:                                    // Do nothing here
                        break;
                    }

                if (configuring) {
                    switch (_mes_pkg_recv.mes_type) {
                        case PILOTE_MES_TYPE_USB:
                            switch (_mes_pkg_recv.operation) {
                                case PILOTE_MES_OPERATION_ASK_FOR_CONFIG:
                                    // Load configuration require
                                    // Firstly create a message package
                                    _mes_pkg_send.direction = PILOTE_MES_SEND;
                                    _mes_pkg_send.mes_type  = PILOTE_MES_TYPE_USB;
                                    _mes_pkg_send.operation = PILOTE_MES_OPERATION_SENDBACK_CONFIG;
                                    _mes_pkg_send.mes_content_ptr = (void*)pilote_config_ptr;
                                    // Then send back the message package
                                    xQueueSend(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_50MS);
                                    break;
                                case PILOTE_MES_OPERATION_MODIFY:
                                    // Modify configuration data
                                    switch (_mes_pkg_recv.target) {
                                    // Firstly parse the target, then modify the data
                                    case PILOTE_MES_TARGET_ENABLE:
                                        pilote_config_ptr->enabled = (bool)_mes_pkg_recv.mes_content;
                                        break;
                                    case PILOTE_MES_TARGET_CODE:
                                        pilote_config_ptr->code = (uint16_t)_mes_pkg_recv.mes_content;
                                        break;
                                    case PILOTE_MES_TARGET_MODE:
                                        pilote_config_ptr->mode = _mes_pkg_recv.mes_content;
                                        break;
                                    case PILOTE_MES_TARGET_NUMS_OF_FRAMES:
                                        pilote_config_ptr->nums_of_frames = (uint8_t)_mes_pkg_recv.mes_content;
                                        break;
                                    case PILOTE_MES_TARGET_TIME_BT_FRAMES:
                                        pilote_config_ptr->time_between_frames = _mes_pkg_recv.mes_content;
                                        break;
                                    default:
                                        break;

                                    }
                                    break;
                                default:
                                    break;
                            }
                        break;
                        case PILOTE_MES_TYPE_UDP:
                            // TODO
                            break;
                        case PILOTE_MES_TYPE_HTTP:
                            // TODO
                            break;
                        default:
                            // Should never get here
                            break;
                    }
                }
            }
        } else {
            // If timeout
            if (!configuring && uxQueueMessagesWaiting(mbox_pilote_config) == MBOX_ZERO_ITEM) {
                // TODO
                // If no problem, write something to EEPROM
                if (PiloteSaveConfig(pilote_config_ptr) != ERR_OK) {        // Save configurations to EEPROM
                    while (1) {
                        // Save EEPROM error
                    }
                }
                // Check data consistency!

                // If still no problem, OK, else try to roll back transaction

                // Finally unblock IR
                xQueueSend(mbox_pilote_config,
                          (void*)&(pilote_config_ptr),
                          MBOX_ZERO_TIMEOUT);                               // Unblock IR here
            } else {

            }
        }
    }
}

/*
 * Applications.c
 *
 *  Created on: 11 juin 2014
 *      Author: OrpheoXlite
 */

#include "Applications.h"

/**
 * Global variables
 *  - Semaphore
 *  - Queue
 */
//  TODO
QueueHandle_t mbox_pilote_config;
QueueHandle_t mbox_pilote_send;
QueueHandle_t mbox_pilote_recv;

/**
 * Static declarations
 */

/**
 * Create all system level tasks. Called in main().
 *      Attention : It will only *create* tasks. The scheduler starts only after
 *      calling this function.
 */
void RunTasks(void)
{
    // --------------------------      Initialize queues       -----------------------------------
    /**
     * mbox_pilote_config
     * - Containing MBOX_CONFIG_COUNT item;
     * - Pointer to the PiloteConfigurations structure
     */
    mbox_pilote_config = xQueueCreate(MBOX_CONFIG_COUNT, sizeof(PiloteConfigurations *));
    if (mbox_pilote_config == NULL) {
        // Error create a queue
        while (1) {}
            ;
    }
    /**
     * mbox_pilote_send
     * - Containing MBOX_SEND_COUNT item;
     */
    mbox_pilote_send = xQueueCreate(MBOX_SEND_COUNT, sizeof(PiloteMessagePackage));
    if (mbox_pilote_send == NULL) {
        while (1) {
        // Error create a queue
        }
    }
    /**
     * mbox_pilote_recv
     * - Containing MBOX_RECV_COUNT item;
     */
    mbox_pilote_recv = xQueueCreate(MBOX_RECV_COUNT, sizeof(PiloteMessagePackage));
    if (mbox_pilote_recv == NULL) {
        while (1) {
        // Error create a queue
        }
    }

    // --------------------------        Create Tasks         --------------------------------------
    if (xTaskCreate(IR_Thread, "IR", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-2, NULL) != pdPASS) {
        while (1) {
        // Error occurs, often out of heap size. Should never get hear.
        }
    }
    if (xTaskCreate(ConfigThread, "CONFIG", configMINIMAL_STACK_SIZE+100, NULL, configMAX_PRIORITIES-3, NULL) != pdPASS) {
        while (1) {
        // Error occurs, often out of heap size. Should never get hear.
        }
    }
    if (xTaskCreate(USB_Task, "USB", configMINIMAL_STACK_SIZE+100, NULL, configMAX_PRIORITIES-4, NULL) != pdPASS) {
        while (1) {
        // Error occurs, often out of heap size. Should never get hear.
        }
    }
}

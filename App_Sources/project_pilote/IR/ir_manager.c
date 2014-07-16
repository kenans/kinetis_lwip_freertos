/*
 * ir_manager.c
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#include "ir_manager.h"

/**
 * Static declarations
 *      Only used in this file
 */
/**
 *  Methods
 */
static err_t IR_InitFrame(uint8_t *);
static err_t IR_GenerateFrame(uint8_t *, const PiloteConfigurations* );

/**
 *  Variables
 */
/**
 * IR task function. Called by RunTasks().
 */
void IR_Thread(void *pvParameters)
{
    (void)pvParameters;                                          // Suppress unused variable warning
    extern xQueueHandle mbox_pilote_config;                      // Global message queue

    bool                         transmit_started = FALSE;       // Flag of transmit state (volatile?)
    PiloteTimeMs                 last_time_bt_frames = 0;        // a copy of time_bt_frames before stopping IR
    uint8_t                      ir_frame[IR_FRAME_COUNT];       // IR frame array
    const PiloteConfigurations*  pilote_config_ptr = NULL;       // Pointer to the configuration structure

    if (IR_InitFrame(ir_frame) != ERR_OK) {
        while (1) {
            // Initialization error
        }
    }
    // Start IR_TransmitThread
    if (xTaskCreate(IR_TransmitThread, "ir_tranmit", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL) != pdPASS) {
        while(1) {
            // Error, often out of heap size. Should never get hear.
        }
    }
    while (mbox_pilote_config == NULL) {                        // Wait for mailbox being created
        vTaskDelay(FREE_RTOS_DELAY_50MS);
    }
    while (1) {                                                 // Main loop
        // Start or stop transmitting
        if (xQueueReceive(mbox_pilote_config, &pilote_config_ptr, MBOX_TIMEOUT_INFINIT) == pdTRUE) {
            if (pilote_config_ptr != NULL) {                   // If not NULL pointer, start IR
                if (!transmit_started &&                       // If transmit not started,
                    pilote_config_ptr->enabled &&              // device and IR are both enabled
                    (pilote_config_ptr->output_mode&PILOTE_OUTPUT_IR) != 0) {
                    // First store a copy of time_bt_frames
                    last_time_bt_frames = pilote_config_ptr->time_between_frames;
                    if (IR_GenerateFrame(ir_frame,
                            pilote_config_ptr) != ERR_OK) {     // Create a frame
                        while (1) {
                        // Create frame error
                        }
                    }
                    if (IR_StartTransmit(ir_frame, IR_FRAME_COUNT,
                            pilote_config_ptr)!= ERR_OK) {      // Start transmit
                        while (1) {
                        // Start transmit error
                        }
                    }
                    transmit_started = TRUE;                    // Set transmitting
                } else {
                    // If IR transmit already started, or device or IR are not enabled
                    // then do not restart IR
                }
            } else {                                           // If NULL pointer, stop IR
                if (transmit_started) {                        // Should stop transmitting if is running
                    if (IR_StopTransmit() != ERR_OK) {
                        while (1) {
                            // Stop transmit error
                        }
                    }
                    transmit_started = FALSE;                  // Clear transmit_started
                    // Wait at least time_bt_frames before restart IR
                    vTaskDelay((TickType_t)last_time_bt_frames/portTICK_PERIOD_MS);
                }
            }
        } // QueueReceive
    } // Main loop
}


/**
 *  IR_InitFrame:
 *      Initializes a IR frame. 1st byte IR_FRAME_START. Other bytes 0.
 *      If no error, returns ERR_OK
 *
 *      This is an internal function, used only in this file
 *  @return error
 *      ERR_OK: no error
 *      ERR_MEM: error occurs
 */
static err_t IR_InitFrame(uint8_t *ir_frame)
{
    uint8_t i = 0;
    err_t error = ERR_OK;

    if (ir_frame != NULL)
    {
        ir_frame[0] = IR_FRAME_START;
        for (i = 1; i < IR_FRAME_COUNT ; i++) {
            ir_frame[i] = 0U;
        }
    } else {
        // Error should never get here
        error = ERR_MEM;
    }
    return error;
}


/**
 *  IR_GenerateFrame:
 *      Initializes a IR frame. 1st byte IR_FRAME_START. Other bytes 0.
 *      If no error, returns ERR_OK
 *
 *      This is an internal function, used only in this file
 *  @return error
 *      ERR_OK: no error
 *      ERR_MEM: error occurs
 */
static err_t IR_GenerateFrame(uint8_t *ir_frame,
        const PiloteConfigurations* pilote_config_ptr)
{
    err_t error = ERR_OK;
    if (ir_frame!=NULL && pilote_config_ptr!=NULL) {
        // Start Byte
        ir_frame[0] = IR_FRAME_START;
        // Fill with frame except START and CKS
        /**
         * Attention:
         *      IR transmit protocol is Big-Endian mode, while the storage of Kinetis is Little-Endian mode.
         */
        switch (pilote_config_ptr->mode) {
            case PILOTE_TRIGGER:
                ir_frame[1] = pilote_config_ptr->command_param[0];                // CMD1
                ir_frame[2] = pilote_config_ptr->command_param[1];                // CMD2
                ir_frame[3] = IR_FRAME_CMD_DATA_1;
                ir_frame[4] = IR_FRAME_CMD_DATA_2;
                ir_frame[5] = (uint8_t)((pilote_config_ptr->code>>8U)&0x00FFU);   //
                ir_frame[6] = (uint8_t)(pilote_config_ptr->code&0x00FFU);
                ir_frame[7] = pilote_config_ptr->group;
                break;
            case PILOTE_SYNCHRO:
                ir_frame[1] = pilote_config_ptr->command_param[0];                // CMD1
                ir_frame[2] = (uint8_t)((pilote_config_ptr->code>>8U)&0x00FFU);
                ir_frame[3] = (uint8_t)(pilote_config_ptr->code&0x00FFU);
                // TODO
                ir_frame[7] = pilote_config_ptr->group;
                break;
            case PILOTE_COMMAND:
                ir_frame[1] = pilote_config_ptr->command_param[0];                // CMD1
                ir_frame[2] = pilote_config_ptr->command_param[1];                // CMD2
                ir_frame[3] = IR_FRAME_CMD_DATA_1;
                ir_frame[4] = IR_FRAME_CMD_DATA_2;
                ir_frame[5] = (uint8_t)((pilote_config_ptr->command_data>>8U)&0x00FFU);
                ir_frame[6] = (uint8_t)(pilote_config_ptr->command_data&0x00FFU);
                ir_frame[7] = pilote_config_ptr->group;
                break;
            default:
                // Should never get here.
                error = ERR_COMMON;
                break;
        }
        // Checksum for the first 8 bytes
        ir_frame[8] = GetCRC8(ir_frame, IR_FRAME_COUNT-1);
    } else {
        error = ERR_MEM;
    }
    return error;
}

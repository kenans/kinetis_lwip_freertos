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
static err_t IR_InitFrame(void);
static err_t IR_GenerateFrame(void);
/**
 *  Variables
 */
static const PiloteConfigurations*  _pilote_config_ptr = NULL;      // Pointer to the configuration structure
static uint8_t                      _ir_frame[IR_FRAME_COUNT];      // IR frame array
static bool                         _transmit_started = FALSE;      // Flag of transmit state (volatile?)

/**
 * IR task function. Called by RunTasks().
 */
void IR_Thread(void *pvParameters)
{
    (void)pvParameters;                                             // Suppress unused variable warning
    extern xQueueHandle mbox_pilote_config;                         // Global message queue

    if (IR_InitFrame() != ERR_OK) {
        // Initialization error
        while (1)
            ;
    }

    // Start IR_TransmitThread
    if (xTaskCreate(IR_TransmitThread, "ir_tranmit", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL) != pdPASS) {
        // Error, often out of heap size. Should never get hear.
        while(1)
            ;
    }

    while (mbox_pilote_config == NULL) {                            // Wait for mailbox being created
        vTaskDelay(FREE_RTOS_DELAY_50MS);
    }

    while (1) {                                                     // Main loop
        // Should we start or stop transmitting ?
        if (xQueuePeek(mbox_pilote_config, &_pilote_config_ptr, MBOX_TIMEOUT_500MS) == pdTRUE) {
            if (_pilote_config_ptr != NULL) {
                if (!_transmit_started &&                       // If transmit not started,
                    _pilote_config_ptr->enabled &&              // device and IR are both enabled
                    (_pilote_config_ptr->output_mode&PILOTE_OUTPUT_IR) != 0) {

                    if (IR_GenerateFrame() != ERR_OK) {         // Create a frame
                        // Create frame error
                        while (1)
                            ;
                    }
                    if (IR_StartTransmit(
                            _ir_frame,
                            IR_FRAME_COUNT,
                            _pilote_config_ptr)!= ERR_OK) {     // Start transmit
                        // Start transmit error
                        while (1)
                            ;
                    }
                    _transmit_started = TRUE;                   // Set transmitting
                } else {
                    // If IR transmit already started, or device or IR are not enabled
                    // then block for a while
                    vTaskDelay(FREE_RTOS_DELAY_500MS);
                }
            } else {
                // Error: got NULL message. Should never get here
                while (1)
                    ;
            }
        } else {
            // If timeout
            if (_transmit_started) {                                // Should stop transmitting if is running
                if (IR_StopTransmit() != ERR_OK) {
                    // Stop transmit error
                    while (1)
                        ;
                }
                _transmit_started = FALSE;                          // Clear _transmit_started
            }
        }
    }
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
static err_t IR_InitFrame(void)
{
    int i = 0;
    err_t error = ERR_OK;

    if (_ir_frame != NULL)
    {
        _ir_frame[0] = IR_FRAME_START;
        for (i = 1; i < IR_FRAME_COUNT ; i++) {
            _ir_frame[i] = 0U;
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
static err_t IR_GenerateFrame(void)
{
    err_t error = ERR_OK;

    if (_ir_frame!=NULL && _pilote_config_ptr!=NULL) {
        // Start Byte
        _ir_frame[0] = IR_FRAME_START;
        // Fill with frame except START and CKS
        switch (_pilote_config_ptr->mode) {
            case PILOTE_TRIGGER:
                _ir_frame[1] = _pilote_config_ptr->command_param[0];
                _ir_frame[2] = _pilote_config_ptr->command_param[1];
                _ir_frame[3] = IR_FRAME_CMD_DATA_1;
                _ir_frame[4] = IR_FRAME_CMD_DATA_2;
                _ir_frame[5] = (uint8_t)((_pilote_config_ptr->code>>8U)&0x00FFU);
                _ir_frame[6] = (uint8_t)(_pilote_config_ptr->code&0x00FFU);
                _ir_frame[7] = _pilote_config_ptr->group;
                break;
            case PILOTE_SYNCHRO:
                _ir_frame[1] = _pilote_config_ptr->command_param[0];
                _ir_frame[2] = (uint8_t)((_pilote_config_ptr->code>>8U)&0x00FFU);
                _ir_frame[3] = (uint8_t)(_pilote_config_ptr->code&0x00FFU);
                // TODO
                _ir_frame[7] = _pilote_config_ptr->group;
                break;
            case PILOTE_COMMAND:
                _ir_frame[1] = _pilote_config_ptr->command_param[0];
                _ir_frame[2] = _pilote_config_ptr->command_param[1];
                _ir_frame[3] = IR_FRAME_CMD_DATA_1;
                _ir_frame[4] = IR_FRAME_CMD_DATA_2;
                _ir_frame[5] = (uint8_t)((_pilote_config_ptr->command_data>>8U)&0x00FFU);
                _ir_frame[6] = (uint8_t)(_pilote_config_ptr->command_data&0x00FFU);
                _ir_frame[7] = _pilote_config_ptr->group;
                break;
            default:
                // Should never get here.
                error = ERR_COMMON;
                break;
        }
        // Checksum for the first 8 bytes
        _ir_frame[8] = GetCRC8(_ir_frame, IR_FRAME_COUNT-1);
    } else {
        error = ERR_MEM;
    }
    return error;
}

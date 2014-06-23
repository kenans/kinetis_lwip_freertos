/*
 * usb_manager.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#include "usb_manager.h"

static uint8_t              _cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t              _in_buffer[USB1_DATA_BUFF_SIZE];
static bool                 _running = FALSE;

static uint8_t              _temp[16];
/**
 * USB_Task
 * 		- A USB CDC Device task
 */
void USB_Task(void *pvParameters)
{
    (void)pvParameters;                                                     // Not used
    uint16_t i = 0;
    bool in_buf_not_empty = FALSE;
    extern xQueueHandle mbox_pilote_recv;                                   // Global queue definition
    extern xQueueHandle mbox_pilote_send;
    PiloteMessagePackage mes_pkg_recv;                                      // Mes package for recv mbox
    PiloteMessagePackage mes_pkg_send;                                      // Mes package for send mbox

    while (1) {
        while(CDC1_App_Task(_cdc_buffer, sizeof(_cdc_buffer)) == ERR_BUSOFF) {
            // USB not connected, so device not enumerated
            if (_running) {                                                 // First time disconnect USB
                _running=FALSE;                                             // Send config END message
                mes_pkg_recv.direction = PILOTE_MES_RECV;
                mes_pkg_recv.mes_type = PILOTE_MES_TYPE_USB;
                mes_pkg_recv.operation = PILOTE_MES_OPERATION_STOP;
                xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
            }
            vTaskDelay(500/portTICK_PERIOD_MS);
        }

        if (!_running) {                                                    // First time connect USB
            _running = TRUE;                                                // Send config START message
            mes_pkg_recv.direction = PILOTE_MES_RECV;
            mes_pkg_recv.mes_type = PILOTE_MES_TYPE_USB;
            mes_pkg_recv.operation = PILOTE_MES_OPERATION_START;
            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
        }

        if (CDC1_GetCharsInRxBuf() != 0) {                                  // If there is some data transfered
            for (i = 0; i < sizeof(_in_buffer); i++) {                      // Copy them to a local buffer
                if (CDC1_GetChar(&_in_buffer[i]) == ERR_OK) {
                    in_buf_not_empty = TRUE;
                } else {
                    break;
                }
            }

            if (in_buf_not_empty) {                                         // Does the local buffer have some data?
                // Is the received frame valid?
                for (i = 0; i < sizeof(_in_buffer)-9; i++) {
                    if (_in_buffer[i+0] == '$' &&
                        _in_buffer[i+2] == '$' &&
                        _in_buffer[i+4] == '$' &&
                        _in_buffer[i+9] == '$') {
                        if (_in_buffer[i+1] == '0') {
                            // Load configuration
                            // TODO
                            (void)CDC1_SendString((unsigned char*)"echo: 0\n");
                            // First prepare message package for requiring configuration
                            mes_pkg_recv.direction = PILOTE_MES_RECV;
                            mes_pkg_recv.mes_type  = PILOTE_MES_TYPE_USB;
                            mes_pkg_recv.operation = PILOTE_MES_OPERATION_ASK_FOR_CONFIG;
                            // Then send message package to mbox_recv
                            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_50MS);
                            // Wait for the response from mbox_send
                            xQueueReceive(mbox_pilote_send, &mes_pkg_send, MBOX_TIMEOUT_INFINIT);
                            // Verify the got message
                            if (mes_pkg_send.direction == PILOTE_MES_SEND &&
                                mes_pkg_send.mes_type  == PILOTE_MES_TYPE_USB &&
                                mes_pkg_send.operation == PILOTE_MES_OPERATION_SENDBACK_CONFIG) {
                            // Parse message package in mbox_send, then send back to PC
                                PiloteConfigurations *pilote_config =
                                        (PiloteConfigurations*)mes_pkg_send.mes_content_ptr;
                                _temp[15] = '\0';
                                (void)CDC1_SendString((unsigned char*)"Enable:");
                                UTIL_Num8uToStr(_temp, sizeof(_temp), pilote_config->enabled);
                                (void)CDC1_SendString((unsigned char*)_temp);
                                (void)CDC1_SendString((unsigned char*)"\n");

                                (void)CDC1_SendString((unsigned char*)"Code:");
                                UTIL_Num16uToStr(_temp, sizeof(_temp), pilote_config->code);
                                (void)CDC1_SendString((unsigned char*)_temp);
                                (void)CDC1_SendString((unsigned char*)"\n");

                                (void)CDC1_SendString((unsigned char*)"Time Bt Frames:");
                                UTIL_Num32sToStr(_temp, sizeof(_temp), pilote_config->time_between_frames);
                                (void)CDC1_SendString((unsigned char*)_temp);
                                (void)CDC1_SendString((unsigned char*)"\n");

                                (void)CDC1_SendString((unsigned char*)"Nums of Frames:");
                                UTIL_Num8uToStr(_temp, sizeof(_temp), pilote_config->nums_of_frames);
                                (void)CDC1_SendString((unsigned char*)_temp);
                                (void)CDC1_SendString((unsigned char*)"\n");
                            }
                        } else if (_in_buffer[i+1] == '1') {
                            // Modify configuration
                            // TODO
                            (void)CDC1_SendString((unsigned char*)"echo: 1\n");
                            // First prepare message package for modifying data
                            mes_pkg_recv.direction = PILOTE_MES_RECV;
                            mes_pkg_recv.mes_type  = PILOTE_MES_TYPE_USB;
                            mes_pkg_recv.operation = PILOTE_MES_OPERATION_MODIFY;
                            // Then parse target and data, and add it to message package
                            if (_in_buffer[i+3] == '0') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_ENABLE;
                                (_in_buffer[i+8]=='1')
                                      ? (mes_pkg_recv.mes_content = 1)
                                      : (mes_pkg_recv.mes_content = 0);
                            } else if (_in_buffer[i+3] == '1') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_CODE;
                                mes_pkg_recv.mes_content =
                                        (_in_buffer[i+7] - '0') * 10u +
                                        (_in_buffer[i+8] - '0');
                            } else if (_in_buffer[i+3] == '2') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_TIME_BT_FRAMES;
                                mes_pkg_recv.mes_content =
                                        (_in_buffer[i+5] - '0') * 1000u +
                                        (_in_buffer[i+6] - '0') * 100u +
                                        (_in_buffer[i+7] - '0') * 10u +
                                        (_in_buffer[i+8] - '0');
                            } else if (_in_buffer[i+3] == '3') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_NUMS_OF_FRAMES;
                                mes_pkg_recv.mes_content =
                                        (_in_buffer[i+7] - '0') * 10u +
                                        (_in_buffer[i+8] - '0');
                            }
                            // Send the message package to mbox_recv
                            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_50MS);
                        }
                        break;
                    }
                }
                // Clear input buffer
                for (i = 0; i < sizeof(_in_buffer); i++) {
                    _in_buffer[i] = 0;
                }
                in_buf_not_empty = FALSE;
            }
        }
        // If USB connected but no data transfered
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

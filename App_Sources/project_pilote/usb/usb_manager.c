/*
 * usb_manager.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#include "usb_manager.h"

static err_t UsbManagerParseFrame(uint8_t *frame, PiloteMessagePackage *mes_pkg_recv);
static err_t UsbManagerSendbackFrame(err_t error_frame, const PiloteMessagePackage *mes_pkg_send);

static uint8_t              _cdc_buffer[USB1_DATA_BUFF_SIZE];
static bool                 _running = FALSE;
static uint8_t              _in_buffer[USB_IN_BUFFER_COUNT];

#define USB_OLD_FRAME   0
#if USB_OLD_FRAME==1
static uint8_t              _temp[16];
#endif  // if USB_OLD_FRAME==1
/**
 * USB_Task
 * 		- A USB CDC Device task
 */
void USB_Task(void *pvParameters)
{
    (void)pvParameters;                                                     // Not used

    extern xQueueHandle mbox_pilote_recv;                                   // Global queue definition
    extern xQueueHandle mbox_pilote_send;
    PiloteMessagePackage mes_pkg_recv;                                      // Mes package for recv mbox
    PiloteMessagePackage mes_pkg_send;                                      // Mes package for send mbox

    /**
     * Attention:
     *      direction and mes_type are set here and should never be changed!
     *      Only change operation, target and data.
     */
    PiloteInitRecvMesPackage(mes_pkg_recv);                                 // Initialization mes_pkg_recv
    mes_pkg_recv.mes_type = PILOTE_MES_TYPE_USB;
    PiloteInitSendMesPackage(mes_pkg_send);                                 // Initialization mes_pkg_send
    mes_pkg_recv.mes_type = PILOTE_MES_TYPE_USB;

    while (1) {
        while(CDC1_App_Task(_cdc_buffer, sizeof(_cdc_buffer)) == ERR_BUSOFF) {
            // USB not connected, so device not enumerated
            if (_running) {                                                 // First time disconnect USB
                _running=FALSE;                                             // Send config END message
                mes_pkg_recv.operation = PILOTE_MES_OPERATION_STOP;
                xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
            }
            vTaskDelay(500/portTICK_PERIOD_MS);
        }

        if (!_running) {                                                    // First time connect USB
            _running = TRUE;                                                // Send config START message
            mes_pkg_recv.operation = PILOTE_MES_OPERATION_START;
            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
        }

#if USB_OLD_FRAME==0
        // Every 50ms, if Rx not empty, copy all the items from Rx to local USB buffer;
        if (CDC1_GetCharsInRxBuf() != 0) {
            err_t error = ERR_OK;
            uint8_t i = 0;
            bool in_buf_not_empty = FALSE;
            // Copy from CDC buffer to a local USB buffer: _in_buffer
            for (i = 0; i < sizeof(_in_buffer); i++) {                      // Copy them to a local buffer
                if (CDC1_GetChar(&_in_buffer[i]) == ERR_OK) {
                    in_buf_not_empty = TRUE;
                } else {
                    break;
                }
            }
            // If local buffer not empty, search for valid frame
            if (in_buf_not_empty) {
                for (i = 0; i < sizeof(_in_buffer)-PILOTE_USB_PROTOCOL_FRAME_COUNT+1 ; i++) {
                    // If find a valid frame
                    if (_in_buffer[i+0]  == '$' &&
                        _in_buffer[i+2]  == '#' &&
                        _in_buffer[i+4]  == '#' &&
                        _in_buffer[i+9]  == '#' &&
                        _in_buffer[i+11] == '$') {
                        // First parse the frame and generate the message package
                        error = UsbManagerParseFrame(&_in_buffer[i], &mes_pkg_recv);
                        // Then if no error, send message package; if error, reply MASTER
                        if (error == ERR_OK) {
                            // Send the message package to mbox_recv, if mbox full, block and wait
                            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_INFINIT);
                            // Receive message package in mbox_send, if mbox empty, block and wait
                            xQueueReceive(mbox_pilote_send, &mes_pkg_send, MBOX_TIMEOUT_INFINIT);
                            // Send back a response to MASTER
                            if (mes_pkg_send.mes_type == PILOTE_MES_TYPE_USB) {
                                if (UsbManagerSendbackFrame(error, &mes_pkg_send) != ERR_OK) {
                                    while (1) {
                                        // Send back frame error, should never get here.
                                    }
                                }
                            }
                        } else if (error == ERR_CKS || error == ERR_FATAL) {
                            // If any error, send back response error
                            // TODO
                            if (UsbManagerSendbackFrame(error, NULL) != ERR_OK) {
                                while (1) {
                                    // Send back frame error, should never get here.
                                }
                            }
                        }
                        i = i + PILOTE_USB_PROTOCOL_FRAME_COUNT - 1;
                    }
                }
                // Clear input buffer
                for (i = 0; i < sizeof(_in_buffer); i++) {
                    _in_buffer[i] = 0;
                }
                in_buf_not_empty = FALSE;
            }
        } else {
            // If USB connected but no data transfered
            vTaskDelay(50/portTICK_PERIOD_MS);
        }

#else   // if USB_OLD_FRAME == 0
        if (CDC1_GetCharsInRxBuf() != 0) {                                  // If there is some data transfered
            uint8_t i = 0;
            bool in_buf_not_empty = FALSE;
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
                            mes_pkg_recv.operation = PILOTE_MES_OPERATION_READ_CONFIG;
                            // Then send message package to mbox_recv
                            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_50MS);
                            // Wait for the response from mbox_send
                            xQueueReceive(mbox_pilote_send, &mes_pkg_send, MBOX_TIMEOUT_INFINIT);
                            // Verify the got message
                            if (mes_pkg_send.direction == PILOTE_MES_SEND &&
                                mes_pkg_send.mes_type  == PILOTE_MES_TYPE_USB &&
                                mes_pkg_send.operation == PILOTE_MES_OPERATION_REPLY_CONFIG) {
                            // Parse message package in mbox_send, then send back to PC
                                PiloteConfigurations *pilote_config =
                                        (PiloteConfigurations*)mes_pkg_send.data_ptr;
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
                                      ? (mes_pkg_recv.data = 1)
                                      : (mes_pkg_recv.data = 0);
                            } else if (_in_buffer[i+3] == '1') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_CODE;
                                mes_pkg_recv.data =
                                        (_in_buffer[i+7] - '0') * 10u +
                                        (_in_buffer[i+8] - '0');
                            } else if (_in_buffer[i+3] == '2') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_TIME_BT_FRAMES;
                                mes_pkg_recv.data =
                                        (_in_buffer[i+5] - '0') * 1000u +
                                        (_in_buffer[i+6] - '0') * 100u +
                                        (_in_buffer[i+7] - '0') * 10u +
                                        (_in_buffer[i+8] - '0');
                            } else if (_in_buffer[i+3] == '3') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_NUMS_OF_FRAMES;
                                mes_pkg_recv.data =
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
#endif  // if USB_OLD_FRAME==1
    }
}

/**
 *
 */
static err_t UsbManagerParseFrame(uint8_t *frame, PiloteMessagePackage *mes_pkg_recv)
{
    err_t error = ERR_OK;
    uint8_t frame_cks[6];

    // Is checksum correct?
    frame_cks[0] = frame[1];
    frame_cks[1] = frame[3];
    frame_cks[2] = frame[5];
    frame_cks[3] = frame[6];
    frame_cks[4] = frame[7];
    frame_cks[5] = frame[8];
    if (GetCRC8(frame_cks, 6) != frame[10]) {
        return ERR_CKS;                     // Checksum wrong
    }
    // Can frame be parsed?
    if (frame[3] >= PILOTE_MES_TARGET_ITEMS_COUNT) {
        return ERR_FATAL;                   // Target byte out of definition range
    }
    // Parse command
    switch (frame[1]) {
        case 0x80:                          // Byte1 = 0x80 (128) : Read a configuration
            mes_pkg_recv->operation = PILOTE_MES_OPERATION_READ_CONFIG;
           break;
        case 0x81:                          // Byte1 = 0x81 (129) : Modify a configuration
            mes_pkg_recv->operation = PILOTE_MES_OPERATION_MODIFY;
            break;
        default:
            return ERR_FATAL;               // Byte1 cannot be recognized
            break;
    }
    /**
     * Attention:
     *      Target enumeration in the protocol and in the software should be the same.
     */
    mes_pkg_recv->target = frame[3];                      // Byte3 : Target enumeration.
    mes_pkg_recv->data = (uint32_t)(((frame[5]&0xffU) +   // Byte5~8 : Data, transfered in little-endian mode.
                                    ((frame[6]<<8)&0xff00U) +
                                    ((frame[7]<<16)&0xff0000U)+
                                    ((frame[8]<<24))&0xff000000U));

    return error;
}

/**
 *
 */
static err_t UsbManagerSendbackFrame(err_t error_frame, const PiloteMessagePackage *mes_pkg_send)
{
    err_t error = ERR_OK;
    uint8_t cks = 0;
    uint8_t frame_cks[6];
    uint8_t frame_send[12];

    // Parse msg_pkg_send and create frame_cks for CRC
    if (mes_pkg_send == NULL) {
        if (error_frame == ERR_OK) {
            return ERR_MEM;
        } else if (error_frame == ERR_CKS) {
            frame_cks[0] = 0x02U;                       // ERR_CKS
        } else if (error_frame == ERR_FATAL) {
            frame_cks[0] = 0x03U;                       // ERR_FATAL
        } else {
            return ERR_COMMON;
        }
        frame_cks[1] = 0U;
        frame_cks[2] = 0U;
        frame_cks[3] = 0U;
        frame_cks[4] = 0U;
        frame_cks[5] = 0U;
    } else {
        if (error_frame != ERR_OK) {
            return ERR_COMMON;
        }
        if (mes_pkg_send->operation == PILOTE_MES_OPERATION_REPLY_CONFIG) {
            frame_cks[0] = 0x00U;
        } else if (mes_pkg_send->operation == PILOTE_MES_OPERATION_REPLY_MODIFY) {
            frame_cks[0] = 0x01U;
        } else {
            return ERR_COMMON;
        }
        frame_cks[1] = mes_pkg_send->operation;
        // Little endian mode
        frame_cks[2] = (uint8_t)(mes_pkg_send->data&0x000000FFU);
        frame_cks[3] = (uint8_t)((mes_pkg_send->data&0x0000FF00U)>>8);
        frame_cks[4] = (uint8_t)((mes_pkg_send->data&0x00FF0000U)>>16);
        frame_cks[5] = (uint8_t)((mes_pkg_send->data&0xFF000000U)>>24);
    }

    // Get CRC result
    cks = GetCRC8(frame_cks, 6);

    // Create a frame to send
    frame_send[0]  = '$';
    frame_send[1]  = frame_cks[0];
    frame_send[2]  = '#';
    frame_send[3]  = frame_cks[1];
    frame_send[4]  = '#';
    frame_send[5]  = frame_cks[2];
    frame_send[6]  = frame_cks[3];
    frame_send[7]  = frame_cks[4];
    frame_send[8]  = frame_cks[5];
    frame_send[9]  = '#';
    frame_send[10] = cks;
    frame_send[11] = '$';

    // Finally send back the frame
    if (CDC1_SendBlock(frame_send, 12) != ERR_OK)
        return ERR_COMMON;

    return error;
}

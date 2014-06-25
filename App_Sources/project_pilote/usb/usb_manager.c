/*
 * usb_manager.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */

#include "usb_manager.h"

/**
 * Static decelerations
 * Only used in this file
 */
// Methods
static err_t UsbManagerParseFrame(uint8_t *frame, PiloteMessagePackage *mes_pkg_recv);
static err_t UsbManagerSendbackFrame(err_t error_frame, const PiloteMessagePackage *mes_pkg_send);
// Variables
static uint8_t _cdc_buffer[USB1_DATA_BUFF_SIZE];    // Buffer needed by USB CDC
// --------------------------------- Local ring buffer -----------------------------------------------
/**
 *   Methods
 *      These methods are implemented as macro in the header file
*/
// static bool LocalBuf_IsEmpty(void);              // If the buffer is empty returns TRUE
// static bool LocalBuf_IsFull(void);               // If the buffer is full returns TRUE
// static uint8_t LocalBuf_NumOfElements(void);     // Returns number of elements in the buffer
// static void LocalBuf_Put(item);                  // Append item to the end of the buffer
// static uint8_t LocalBuf_Get();                   // Get and remove the item from the front of the buffer
// static uint8_t LocalBuf_Peek();                  // Get but without removing the item from the front of the buffer
// static uint8_t LocalBuf_PeekIndex(index);        // Get but without removing the item from the index place
// static void LocalBuf_Remove();                   // Remove an item from the front of the buffer
// static void LocalBuf_Init();                     // Initialize the buffer
// static void LocalBuf_Clear();                    // Clear all the items in the buffer
// Variables
static uint8_t _local_buf[_LOCAL_BUFFER_COUNT];     // Local ring buffer
const static uint8_t _size = _LOCAL_BUFFER_COUNT;   // ring buffer size
static uint8_t _start = 0;                          // start id
static uint8_t _end = 0;                            // end id
// ---------------------------------------------------------------------------------------------------
/**
 * Compile options
 *      0 : Use new frame but no ring buffer
 *      -1: Use new frame with ring buffer
 *      1 : Use old frame but no ring buffer
 */
#define USB_OLD_FRAME   -1
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

    bool usb_connected = FALSE;                                            // USB connection flag
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
            if (usb_connected) {                                                 // First time disconnect USB
                usb_connected=FALSE;                                             // Send config END message
                mes_pkg_recv.operation = PILOTE_MES_OPERATION_STOP;
                xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
            }
            vTaskDelay(500/portTICK_PERIOD_MS);
        }

        if (!usb_connected) {                                                    // First time connect USB
            usb_connected = TRUE;                                                // Send config START message
            mes_pkg_recv.operation = PILOTE_MES_OPERATION_START;
            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, 0);
        }

#if USB_OLD_FRAME==0
        // Every 50ms, if Rx not empty, copy all the items from Rx to local USB buffer;
        if (CDC1_GetCharsInRxBuf() != 0) {
            err_t error = ERR_OK;
            uint8_t i = 0;
            bool in_buf_not_empty = FALSE;
            // Copy from CDC buffer to a local USB buffer: _local_buf
            for (i = 0; i < sizeof(_local_buf); i++) {                      // Copy them to a local buffer
                if (CDC1_GetChar(&_local_buf[i]) == ERR_OK) {
                    in_buf_not_empty = TRUE;
                } else {
                    break;
                }
            }
            // If local buffer not empty, search for valid frame
            if (in_buf_not_empty) {
                for (i = 0; i < sizeof(_local_buf)-PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT+1 ; i++) {
                    // If find a valid frame
                    if (_local_buf[i+0]  == '$' &&
                        _local_buf[i+2]  == '#' &&
                        _local_buf[i+4]  == '#' &&
                        _local_buf[i+9]  == '#' &&
                        _local_buf[i+11] == '$') {
                        // First parse the frame and generate the message package
                        error = UsbManagerParseFrame(&_local_buf[i], &mes_pkg_recv);
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
                        i = i + PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT - 1;
                    }
                }
                // Clear input buffer
                for (i = 0; i < sizeof(_local_buf); i++) {
                    _local_buf[i] = 0;
                }
                in_buf_not_empty = FALSE;
            }
        } else {
            // If USB connected but no data transfered
            vTaskDelay(50/portTICK_PERIOD_MS);
        }
#elif USB_OLD_FRAME==-1
        // Every 50ms, if Rx not empty, copy all the items from Rx to local USB buffer;
        if (CDC1_GetCharsInRxBuf() != 0) {
            err_t error = ERR_OK;
            uint8_t i = 0;
            uint8_t temp = 0;
            uint8_t temp_frame[PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT];
            // Copy from CDC buffer to a local USB buffer: _local_buf
            while (!LocalBuf_IsFull()) {
                if (CDC1_GetChar(&temp) == ERR_OK) {
                    // Pass to the next free element
                    LocalBuf_Put(temp);
                } else {
                    break;
                }
            }
            // If elements in local buffer not less than length of frame, search for valid frame
            while (LocalBuf_NumOfElements() >= PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT) {
                // If find a valid frame
                if (LocalBuf_PeekIndex(0)  != '$' ||
                    LocalBuf_PeekIndex(2)  != '#' ||
                    LocalBuf_PeekIndex(4)  != '#' ||
                    LocalBuf_PeekIndex(9)  != '#' ||
                    LocalBuf_PeekIndex(11) != '$') {
                    // If not a valid frame, pass to the next element
                    LocalBuf_Remove();
                } else {
                    // If a valid frame, copy it out and remove it from the frame
                    for (i = 0; i < PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT; i++) {
                        temp_frame[i] = LocalBuf_Get();
                    }
                    // First parse the frame and generate the message package
                    error = UsbManagerParseFrame(temp_frame, &mes_pkg_recv);
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
                }
            }
        } else {
            // If USB connected but no data transfered
            vTaskDelay(50/portTICK_PERIOD_MS);
        }
#elif USB_OLD_FRAME==1
        if (CDC1_GetCharsInRxBuf() != 0) {                                  // If there is some data transfered
            uint8_t i = 0;
            bool in_buf_not_empty = FALSE;
            for (i = 0; i < sizeof(_local_buf); i++) {                      // Copy them to a local buffer
                if (CDC1_GetChar(&_local_buf[i]) == ERR_OK) {
                    in_buf_not_empty = TRUE;
                } else {
                    break;
                }
            }

            if (in_buf_not_empty) {                                         // Does the local buffer have some data?
                // Is the received frame valid?
                for (i = 0; i < sizeof(_local_buf)-9; i++) {
                    if (_local_buf[i+0] == '$' &&
                        _local_buf[i+2] == '$' &&
                        _local_buf[i+4] == '$' &&
                        _local_buf[i+9] == '$') {
                        if (_local_buf[i+1] == '0') {
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
                        } else if (_local_buf[i+1] == '1') {
                            // Modify configuration
                            // TODO
                            (void)CDC1_SendString((unsigned char*)"echo: 1\n");
                            // First prepare message package for modifying data
                            mes_pkg_recv.direction = PILOTE_MES_RECV;
                            mes_pkg_recv.mes_type  = PILOTE_MES_TYPE_USB;
                            mes_pkg_recv.operation = PILOTE_MES_OPERATION_MODIFY;
                            // Then parse target and data, and add it to message package
                            if (_local_buf[i+3] == '0') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_ENABLE;
                                (_local_buf[i+8]=='1')
                                      ? (mes_pkg_recv.data = 1)
                                      : (mes_pkg_recv.data = 0);
                            } else if (_local_buf[i+3] == '1') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_CODE;
                                mes_pkg_recv.data =
                                        (_local_buf[i+7] - '0') * 10u +
                                        (_local_buf[i+8] - '0');
                            } else if (_local_buf[i+3] == '2') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_TIME_BT_FRAMES;
                                mes_pkg_recv.data =
                                        (_local_buf[i+5] - '0') * 1000u +
                                        (_local_buf[i+6] - '0') * 100u +
                                        (_local_buf[i+7] - '0') * 10u +
                                        (_local_buf[i+8] - '0');
                            } else if (_local_buf[i+3] == '3') {
                                mes_pkg_recv.target = PILOTE_MES_TARGET_NUMS_OF_FRAMES;
                                mes_pkg_recv.data =
                                        (_local_buf[i+7] - '0') * 10u +
                                        (_local_buf[i+8] - '0');
                            }
                            // Send the message package to mbox_recv
                            xQueueSend(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_50MS);
                        }
                        break;
                    }
                }
                // Clear input buffer
                for (i = 0; i < sizeof(_local_buf); i++) {
                    _local_buf[i] = 0;
                }
                in_buf_not_empty = FALSE;
            }
        }
#endif  // if USB_OLD_FRAME==0
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
    mes_pkg_recv->data = (uint32_t)((frame[5]&0xffU) +   // Byte5~8 : Data, transfered in little-endian mode.
                                    ((frame[6]<<8)&0xff00U) +
                                    ((frame[7]<<16)&0xff0000U)+
                                    ((frame[8]<<24)&0xff000000U));

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

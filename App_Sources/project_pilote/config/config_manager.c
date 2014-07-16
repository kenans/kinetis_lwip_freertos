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
// Functions
static err_t ConfigManagerParseMes(PiloteMessagePackage *mes_pkg_recv,
                                      PiloteMessagePackage *mes_pkg_send);
/**
 *  Use xQueueOverwrite instead of xQueueSend.
 */
#define ConfigManager_StopIR() \
        (pilote_config_ptr=NULL,\
         xQueueOverwrite(mbox_pilote_config,\
                        (void*)&(pilote_config_ptr)))
#define ConfigManager_StartIR() \
        (pilote_config_ptr=&_pilote_config,\
         xQueueOverwrite(mbox_pilote_config,\
                        (void*)&(pilote_config_ptr)))
// Variables
static PiloteConfigurations _pilote_config;                      // Create instance. It's the only instance of program
/**
 *  Configuration task function. Called by RunTasks().
 */
void ConfigThread(void *pvParameters)
{
    (void)pvParameters;                                         // Suppress unused variable warning
    extern xQueueHandle mbox_pilote_config;                     // Global message queue
    extern xQueueHandle mbox_pilote_recv;
    extern xQueueHandle mbox_pilote_send;

    /**
     *  Counts start times:
     *      If got START packet, config_recursive++; if got END packet,
     *  config_recursive--. Only when config_recursive==0, that is to
     *  say, got the same times START and STOP, we start IR; otherwise
     *  keep IR stopped.
     *
     *  Attention: config_recursive should never be negative.
     */
    uint8_t config_recursive = 0;
    PiloteMessagePackage mes_pkg_recv;
    PiloteMessagePackage mes_pkg_send;
    PiloteConfigurations *pilote_config_ptr = &_pilote_config;  // Used in message queue
    if (PiloteConfigInit(&_pilote_config) != ERR_OK) {        // Initialization pilote_config
        while (1) {
            // Initialization error
        }
    }
    PiloteInitRecvMesPackage(mes_pkg_recv);                    // Initialization mes_pkg_recv
    PiloteInitSendMesPackage(mes_pkg_send);                    // Initialization mes_pkg_send

#define REWRITE_EEPROM 0                                      // If anything changed, should rewrite the EEPROM
#if REWRITE_EEPROM==1
    if (PiloteSaveConfig(&_pilote_config) != ERR_OK) {        // Save configurations to EEPROM
        while (1) {
            // Save EEPROM error
        }
    }
#endif  // if REWRITE_EEPROM==1

    if (PiloteLoadConfig(&_pilote_config) != ERR_OK) {        // Load configurations from EEPROM
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
    if (ConfigManager_StartIR() != pdTRUE) {
        while (1) {
            // Start IR error
        }
    }
    while (1) {
        /**
         * 1. Block on a message queue with a timeout;
         * 2. If get a message,
         *      2.1 If is a START, block IR; if is an END, unblock IR.
         *      2.2 If is a USB message:
         *      2.3 If is a UDP message:
         *      2.4 If is a HTTP message:
         * 3. If error occurs, do nothing; else call PiloteWriteConfig()
         * 4. Check data consistency
         * 5. If everything is OK, unlock IR; else roll back transaction
         */
        if (xQueueReceive(mbox_pilote_recv, &mes_pkg_recv, MBOX_TIMEOUT_INFINIT) == pdTRUE) {
            if (mes_pkg_recv.direction == PILOTE_MES_RECV) {
                // If got a message
                switch (mes_pkg_recv.operation) {
                    case PILOTE_MES_OPERATION_START:            // USB connected
                        if (config_recursive == 0) {
                            ConfigManager_StopIR();             // Block IR here
                        }                                       // Otherwise, do not re-stop IR
                        config_recursive++;
                        break;
                    case PILOTE_MES_OPERATION_STOP:             // USB disconnected
                        // If no problem, write something to EEPROM
                        if (PiloteSaveConfig(&_pilote_config) != ERR_OK) { // Save configurations to EEPROM
                            while (1) {
                                // Save EEPROM error
                            }
                        }
                        // Check data consistency!
                        // If still no problem, OK, else try to roll back transaction
                        // TODO
                        if (config_recursive > 0) {
                            if (--config_recursive==0)
                                ConfigManager_StartIR();        // Unblock IR here
                        }
                        // If receive STOP but config_recursive==0, do nothing
                        break;
                    default:
                        // Do nothing here
                        break;
                }
                if (config_recursive > 0&&
                    mes_pkg_recv.operation != PILOTE_MES_OPERATION_START &&
                    mes_pkg_recv.operation != PILOTE_MES_OPERATION_STOP) {
                    switch (mes_pkg_recv.mes_type) {
                        case PILOTE_MES_TYPE_HTTP:
                        case PILOTE_MES_TYPE_USB:
                            // Parse the received message then do something and generate send message
                            ConfigManagerParseMes(&mes_pkg_recv, &mes_pkg_send);
                            // Send back message to USB Manager
                            xQueueSend(mbox_pilote_send, &mes_pkg_send, MBOX_TIMEOUT_INFINIT);
                            break;
                        case PILOTE_MES_TYPE_UDP:
                            // TODO
                            break;
                        default:
                            // Should never get here
                            break;
                    }
                }
            } else {
                while (1) {
                    // Error: received a "send" message. Should never get here
                }
            }
        } // QueueReceive
    } // Main loop
}

/**
 *  ConfigManagerParseMes
 *      - Parses a receive message packet then creates a new send message packet.
 *      - If no error occurs, returns ERR_OK; otherwise returns corresponding error number.
 *  @param
 *      - mes_pkg_recv: pointer to the receive message package
 *      - mes_pkg_send: pointer to the send message package
 *  @return
 *      - If everything is OK, returns ERR_OK;
 *      - Otherwise returns error number
 */
static err_t ConfigManagerParseMes(PiloteMessagePackage *mes_pkg_recv,
                                   PiloteMessagePackage *mes_pkg_send)
{
    err_t error = ERR_OK;

    if (mes_pkg_recv->direction != PILOTE_MES_RECV) {                           // If mes_pkg_recv error
        return ERR_COMMON;
    }

    switch (mes_pkg_recv->mes_type) {
        case PILOTE_MES_TYPE_HTTP:
            mes_pkg_send->mes_type = PILOTE_MES_TYPE_HTTP;
            break;
        case PILOTE_MES_TYPE_USB:
            mes_pkg_send->mes_type = PILOTE_MES_TYPE_USB;
            break;
        case PILOTE_MES_TYPE_UDP:
            mes_pkg_send->mes_type = PILOTE_MES_TYPE_USB;
            break;
        default:
            return ERR_COMMON;
            break;
    }

    switch (mes_pkg_recv->operation) {
        case PILOTE_MES_OPERATION_READ_CONFIG:
            mes_pkg_send->operation = PILOTE_MES_OPERATION_REPLY_CONFIG;        // Set mes_pkg_send operation
            mes_pkg_send->target = mes_pkg_recv->target;                                // Set mes_pkg_send target
            break;
        case PILOTE_MES_OPERATION_MODIFY:
            mes_pkg_send->operation = PILOTE_MES_OPERATION_REPLY_MODIFY;        // Set mes_pkg_send operation
            mes_pkg_send->target = mes_pkg_recv->target;                                // Set mes_pkg_send target
            switch (mes_pkg_recv->target) {
                case PILOTE_MES_TARGET_ENABLE:
                    _pilote_config.enabled = (bool)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_MODE:
                    _pilote_config.mode = (PiloteMode)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_OUTPUT_MODE:
                    _pilote_config.output_mode = (PiloteOutputMode)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_SOURCE_MODE:
                    _pilote_config.source_mode = (PiloteSourceMode)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_VIDEO_MODE:
                    _pilote_config.video_mode = (PiloteVideoMode)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_TOKEN_RING:
                    _pilote_config.token_ring = (PiloteTokenRing)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_CODE:
                    _pilote_config.code = (uint16_t)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_GROUP:
                    _pilote_config.group = (uint8_t)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_NUMS_OF_FRAMES:
                    _pilote_config.nums_of_frames = (uint8_t)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_UDP_ID:
                    _pilote_config.udp_id[0] = (uint8_t)((mes_pkg_recv->data&0xff000000U)>>24);
                    _pilote_config.udp_id[1] = (uint8_t)((mes_pkg_recv->data&0x00ff0000U)>>16);
                    _pilote_config.udp_id[2] = (uint8_t)((mes_pkg_recv->data&0xff00ff00U)>>8);
                    _pilote_config.udp_id[3] = (uint8_t)((mes_pkg_recv->data&0xff000000U));
                    break;
                case PILOTE_MES_TARGET_UDP_DATA:
                    // TODO
                    break;
                case PILOTE_MES_TARGET_COMMAND_PARAM:
                    _pilote_config.command_param[0] = (uint8_t)((mes_pkg_recv->data&0xff00)>>8);
                    _pilote_config.command_param[1] = (uint8_t)(mes_pkg_recv->data);
                    break;
                case PILOTE_MES_TARGET_COMMAND_DATA:
                    _pilote_config.command_data = (uint16_t)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_TIME_BT_FRAMES:
                    _pilote_config.time_between_frames = (PiloteTimeMs)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_DELAY_SOURCE:
                    _pilote_config.delay_source = (PiloteTimeMs)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_DELAY_OUTPUT:
                    _pilote_config.delay_output = (PiloteTimeMs)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_START_TIME:
                    _pilote_config.start_time = (PiloteTimeMs)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_END_TIME:
                    _pilote_config.end_time = (PiloteTimeMs)mes_pkg_recv->data;
                    break;
                case PILOTE_MES_TARGET_WEEKDAY:
                    _pilote_config.weekday = (PiloteWeekday)mes_pkg_recv->data;
                    break;
                default:
                    return ERR_COMMON;
                    break;
            }
            break;
        default:
            return ERR_COMMON;
            break;
    }

    switch (mes_pkg_recv->target) {
        mes_pkg_send->data = 0;
        case PILOTE_MES_TARGET_ENABLE:
            mes_pkg_send->data = (uint32_t)_pilote_config.enabled;
            break;
        case PILOTE_MES_TARGET_MODE:
            mes_pkg_send->data = (uint32_t)_pilote_config.mode;
            break;
        case PILOTE_MES_TARGET_OUTPUT_MODE:
            mes_pkg_send->data = (uint32_t)_pilote_config.output_mode;
            break;
        case PILOTE_MES_TARGET_SOURCE_MODE:
            mes_pkg_send->data = (uint32_t)_pilote_config.source_mode;
            break;
        case PILOTE_MES_TARGET_VIDEO_MODE:
            mes_pkg_send->data = (uint32_t)_pilote_config.video_mode;
            break;
        case PILOTE_MES_TARGET_TOKEN_RING:
            mes_pkg_send->data = (uint32_t)_pilote_config.token_ring;
            break;
        case PILOTE_MES_TARGET_CODE:
            mes_pkg_send->data = (uint32_t)_pilote_config.code;
            break;
        case PILOTE_MES_TARGET_GROUP:
            mes_pkg_send->data = (uint32_t)_pilote_config.group;
            break;
        case PILOTE_MES_TARGET_NUMS_OF_FRAMES:
            mes_pkg_send->data = (uint32_t)_pilote_config.nums_of_frames;
            break;
        case PILOTE_MES_TARGET_UDP_ID:
            mes_pkg_send->data = (uint32_t)(((_pilote_config.udp_id[0]<<24)&0xff000000U)+
                                            ((_pilote_config.udp_id[1]<<16)&0x00ff0000U)+
                                            ((_pilote_config.udp_id[2]<<8)&0x0000ff00U)+
                                            ((_pilote_config.udp_id[3])&0x000000ffU));
            break;
        case PILOTE_MES_TARGET_UDP_DATA:
            // TODO
            break;
        case PILOTE_MES_TARGET_COMMAND_PARAM:
            mes_pkg_send->data = (uint32_t)(((_pilote_config.command_param[0]<<8)&0xff00U) +
                    (_pilote_config.command_param[1]));
            break;
        case PILOTE_MES_TARGET_COMMAND_DATA:
            mes_pkg_send->data = (uint32_t)_pilote_config.command_data;
            break;
        case PILOTE_MES_TARGET_TIME_BT_FRAMES:
            mes_pkg_send->data = (uint32_t)_pilote_config.time_between_frames;
            break;
        case PILOTE_MES_TARGET_DELAY_SOURCE:
            mes_pkg_send->data = (uint32_t)_pilote_config.delay_source;
            break;
        case PILOTE_MES_TARGET_DELAY_OUTPUT:
            mes_pkg_send->data = (uint32_t)_pilote_config.delay_output;
            break;
        case PILOTE_MES_TARGET_START_TIME:
            mes_pkg_send->data = (uint32_t)_pilote_config.start_time;
            break;
        case PILOTE_MES_TARGET_END_TIME:
            mes_pkg_send->data = (uint32_t)_pilote_config.end_time;
            break;
        case PILOTE_MES_TARGET_WEEKDAY:
            mes_pkg_send->data = (uint32_t)_pilote_config.weekday;
            break;
        default:
            return ERR_COMMON;
            break;
    }

    return error;
}

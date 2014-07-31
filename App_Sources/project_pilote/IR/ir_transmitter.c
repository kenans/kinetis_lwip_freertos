/*
 * ir_transmitter.c
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */
#include "ir_transmitter.h"

/**
 * Static declarations
 */
// Variables
static volatile bool                _transmitting = FALSE;  // volatile ?
static uint8_t                      _size = 0;
static uint8_t                      *_frame = NULL;
static const PiloteConfigurations   *_pilote_config_ptr = NULL;
static LDD_TDeviceData              *_interrupt_device_ptr = NULL;
static volatile uint32_t            _interrupt_flag = 0;    // Interrupt flag. It's changed in Events.c
// Methods
static void IR_SendByte(uint8_t data);
static err_t IR_SendFrame();
#define IR_TimeBtFramesIsLongEnough() \
    (_pilote_config_ptr->time_between_frames>=IR_PROTOCOL_TIME_BETWEEN_FRAME_MIN_MS &&\
     _pilote_config_ptr->time_between_frames>=(_pilote_config_ptr->nums_of_frames*IR_PROTOCOL_FRAME_MIN_LENGTH_MS))

/**
 *  Start the IR transmit session.
 */
err_t IR_StartTransmit(uint8_t *frame, uint8_t size, const PiloteConfigurations *pilote_config_ptr)
{
    err_t error = ERR_OK;
    if (_transmitting)
        return ERR_IR;
    _frame = frame;
    _size = size;
    _pilote_config_ptr = pilote_config_ptr;
    /**
     * Attention:
     *      _transmitting should be set at last to avoid NULL pointer
     *  in the thread function.
     *      A MUTEX is not recommended here because it may cause the
     *  priority inversion problem
     */
    _transmitting = TRUE;
    return error;
}

/**
 * Stop the IR transmit session.
 */
err_t IR_StopTransmit(void)
{
    err_t error = ERR_OK;
    if (!_transmitting)
        return ERR_IR;
    /**
     *  Similarly, _transmitting should be clear at last
     */
    _transmitting = FALSE;
    return error;
}

/**
 * IR transmit thread. Sends frames depending on the configuration.
 * It's created by IR_Thread() function.
 */
void IR_TransmitThread(void *pvParameters)
{
    (void)pvParameters;

    uint16_t i = 0;
    bool first_time_start = TRUE;
    TickType_t last_wake_time = 0;
    // Transmit allowed flag
    bool transmit_allowed = FALSE;
    // Own copy of configuration data
    uint8_t nums_of_frames = 0;
    uint8_t udp_id[4];
    PiloteTimeMs time_between_frames = 0;
    PiloteMode mode;
    PiloteSourceMode source_mode;
    PiloteVideoMode  video_mode;
    // UDP command mailbox and message packet
    extern QueueHandle_t mbox_pilote_udp_cmd;
    PiloteUdpCmdMes udp_cmd_mes;
    // Synchronization video variables
    uint32_t delta_time_code = 0;
    uint32_t time_code = 0;
    uint32_t count_1_minute = 0;
    uint32_t count_10_minutes = 0;
    // Initialize periodic interrupt timer
    _interrupt_device_ptr = TI1_Init((LDD_TUserData*)&_interrupt_flag);
    // Main loop
    while (1) {
        if (_transmitting &&                                    // If transmit is enabled
            _pilote_config_ptr != NULL &&                       // If get the configuration structure and
            _frame != NULL) {                                   // frame address
            if (first_time_start) {                             // If first time starts, get new configurations
                if (IR_TimeBtFramesIsLongEnough()) {
                    // Save a copy of some configurations of the pilote
                    nums_of_frames = _pilote_config_ptr->nums_of_frames;
                    time_between_frames = _pilote_config_ptr->time_between_frames;
                    source_mode = _pilote_config_ptr->source_mode;
                    mode = _pilote_config_ptr->mode;
                    video_mode = _pilote_config_ptr->video_mode;
                    first_time_start = FALSE;
                    transmit_allowed = FALSE;
                    /**
                     *  - If first time start an UDP source mode, should clean the mailbox mbox_pilote_udp_cmd
                     *    in order to remove the command received when not in udp mode.
                     *  - Should also copy udp_id
                     */
                    if (_pilote_config_ptr->source_mode==PILOTE_SOURCE_UDP) {
                        xQueueReset(mbox_pilote_udp_cmd);
                        for (i = 0; i < PILOTE_UDP_ID_COUNT; i++) {
                            udp_id[i] = _pilote_config_ptr->udp_id[i];
                        }
                    }
#if 0
                    /**
                     *  - If mode is Synchro, should set time_between_frames multiples of
                     *    PILOTE_VIDEO_FRAMES_GAP_MS
                     */
                    if (mode == PILOTE_SYNCHRO) {
                        time_between_frames +=
                                (PILOTE_VIDEO_FRAMES_GAP_MS -
                                 (time_between_frames%PILOTE_VIDEO_FRAMES_GAP_MS));
                        delta_time_code = time_between_frames/PILOTE_VIDEO_FRAMES_GAP_MS;
                    }
#else
                    /**
                     *  - If mode is Synchro, should set time_between_frames multiples of
                     *    PILOTE_VIDEO_CINEMA_MUTIPLE/ PILOTE_VIDEO_NTSC_MUTIPLE/
                     *    PILOTE_VIDEO_PAL_MUTIPLE.
                     */
                    if (mode == PILOTE_SYNCHRO) {
                        switch (video_mode) {
                        case PILOTE_VIDEO_CINEMA:
                            if (time_between_frames%PILOTE_VIDEO_CINEMA_MULTIPLE) {
                                time_between_frames +=
                                    (PILOTE_VIDEO_CINEMA_MULTIPLE -
                                     (time_between_frames%PILOTE_VIDEO_CINEMA_MULTIPLE));
                            }
                            delta_time_code=(PILOTE_VIDEO_CINEMA_FPS*time_between_frames)
                                /PILOTE_VIDEO_1000_MS;
                            break;
                        case PILOTE_VIDEO_NTSC:
                            if (time_between_frames%PILOTE_VIDEO_NTSC_MULTIPLE) {
                                time_between_frames +=
                                    (PILOTE_VIDEO_NTSC_MULTIPLE -
                                     (time_between_frames%PILOTE_VIDEO_NTSC_MULTIPLE));
                            }
                            delta_time_code=(PILOTE_VIDEO_NTSC_FPS*time_between_frames)
                                /PILOTE_VIDEO_1000_MS;
                            break;
                        case PILOTE_VIDEO_PAL_SECAM:
                            if (time_between_frames%PILOTE_VIDEO_PAL_MULTIPLE) {
                                time_between_frames +=
                                    (PILOTE_VIDEO_PAL_MULTIPLE -
                                     (time_between_frames%PILOTE_VIDEO_PAL_MULTIPLE));
                            }
                            delta_time_code=(PILOTE_VIDEO_PAL_FPS*time_between_frames)
                                /PILOTE_VIDEO_1000_MS;
                            break;
                        default:
                            break;
                        }
                    }
#endif
                } else {
                    while (1) {
                    // Configuration error
                    // Should avoid this while configuring
                    }
                }
            }
            switch (source_mode) {
                case PILOTE_SOURCE_OFF:
                    if (!transmit_allowed) {
                        transmit_allowed = TRUE;
                        last_wake_time = xTaskGetTickCount();       // Initialize last_wake_time with current time.
                    }
                    break;
                case PILOTE_SOURCE_CONTACT_ON:
                case PILOTE_SOURCE_TTL_ON:
                    if (ContactSec_GetVal()==0) {   // Low level, starts IR
                        if (!transmit_allowed) {
                            time_code = 0;          // Reset time_code here
                            count_10_minutes = 0;
                            count_1_minute = 0;
                            transmit_allowed = TRUE;
                            last_wake_time = xTaskGetTickCount();       // Initialize last_wake_time with current time.
                        }
                    } else {
                        if (transmit_allowed) {
                            transmit_allowed = FALSE;
                        }
                    }
                    break;
                case PILOTE_SOURCE_CONTACT_OFF:
                case PILOTE_SOURCE_TTL_OFF:
                    if (ContactSec_GetVal()==1) { // High level, starts IR
                        if (!transmit_allowed) {
                            time_code = 0;        // Reset time_code here
                            count_10_minutes = 0;
                            count_1_minute = 0;
                            transmit_allowed = TRUE;
                            last_wake_time = xTaskGetTickCount();       // Initialize last_wake_time with current time.
                        }
                    } else {
                        if (transmit_allowed) {
                            transmit_allowed = FALSE;
                        }
                    }
                    break;
                case PILOTE_SOURCE_UDP:
                    // First disable transmit
                    transmit_allowed = FALSE;
                    // Wait for an UDP command with a timeout
                    if (xQueueReceive(mbox_pilote_udp_cmd, &udp_cmd_mes, MBOX_TIMEOUT_500MS) == pdTRUE) {
                        // If received an UDP command, parse its type (play, stop, ...)
                        if (udp_id[0] == udp_cmd_mes.udp_id[0] &&
                            udp_id[1] == udp_cmd_mes.udp_id[1] &&
                            udp_id[2] == udp_cmd_mes.udp_id[2] &&
                            udp_id[3] == udp_cmd_mes.udp_id[3]) {
                            switch (udp_cmd_mes.cmd_type) {
                                case PILOTE_UDP_CMD_PLAY:
                                    // Modify the current frame
                                    // Allow transmit
                                    transmit_allowed = TRUE;
                                    break;
                                case PILOTE_UDP_CMD_PLAY_SYNCHRO:
                                    break;
                                case PILOTE_UDP_CMD_PAUSE:
                                    break;
                                case PILOTE_UDP_CMD_RESUME:
                                    break;
                                case PILOTE_UDP_CMD_STOP:
                                    break;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            // If Synchro and with source mode, calculate video time code
            if (mode == PILOTE_SYNCHRO && source_mode != PILOTE_SOURCE_OFF &&
                video_mode != PILOTE_VIDEO_OFF) {
                /**
                 *  - Increment time_code here.
                 *  - If video_mode==NTSC, should drop 00 and 01 frames every 1 minute except
                 *    multiples of 10 minutes.
                 */
                time_code+=delta_time_code;
                if (video_mode==PILOTE_VIDEO_NTSC) {
                    if ((time_code/PILOTE_VIDEO_NTSC_1_MINUTE_COUNT) != count_1_minute) {
                        time_code+=(2u*(time_code/PILOTE_VIDEO_NTSC_1_MINUTE_COUNT - count_1_minute));
                        count_1_minute = time_code/PILOTE_VIDEO_NTSC_1_MINUTE_COUNT;
                    }
                    if ((time_code/PILOTE_VIDEO_NTSC_10_MINUTES_COUNT)!=count_10_minutes) {
                        time_code-=(2u*(time_code/PILOTE_VIDEO_NTSC_10_MINUTES_COUNT - count_10_minutes));
                        count_10_minutes = time_code/PILOTE_VIDEO_NTSC_10_MINUTES_COUNT;
                    }
                }
                // 3 bytes in an IR frame for video code
                _frame[4] = (uint8_t)(time_code>>16u);
                _frame[5] = (uint8_t)(time_code>>8u);
                _frame[6] = (uint8_t)time_code;
            }
            // Send IR frames
            if (source_mode != PILOTE_SOURCE_UDP) {      // If source mode==UDP, do not delay
                /***
                 *  This is transmit for non-UDP source mode
                 */
                if (transmit_allowed) {
                    for (i = 0; i < nums_of_frames ; i++) {
                        if (IR_SendFrame() != ERR_OK) {
                            while (1) {
                                // IR error
                            }
                        }
                    }
#if 1
#include "AS1.h"
                    // Video code debug
                    while (AS1_SendChar((byte)_frame[4]) != ERR_OK);
                    while (AS1_SendChar((byte)_frame[5]) != ERR_OK);
                    while (AS1_SendChar((byte)_frame[6]) != ERR_OK);
                    while (AS1_SendChar((byte)'\n') != ERR_OK);
#endif
                    vTaskDelayUntil(&last_wake_time, (TickType_t)time_between_frames/portTICK_PERIOD_MS);
                } else {
                    vTaskDelay(FREE_RTOS_DELAY_500MS);      // If transmit not allowed, polling every 500ms
                }
            } else {
                /**
                 *  This is transmit for UDP source mode
                 */
                if (transmit_allowed) {
                    for (i = 0; i < nums_of_frames ; i++) {
                        if (IR_SendFrame() != ERR_OK) {
                            while (1) {
                                // IR error
                            }
                        }
                    }
                }
            }
        } else {
            // If not transmitting (configuring or disabled)
            if (!first_time_start)
                first_time_start = TRUE;
            if (time_between_frames > 500)
                vTaskDelay((TickType_t)time_between_frames/portTICK_PERIOD_MS);
            else
                vTaskDelay(FREE_RTOS_DELAY_500MS);
        }
    }
}

/**
 *  Send a IR frame.
 *  If any error occurs, returns ERR_IR; if no error, returns ERR_OK
 *
 *  This function is internal, used only in this file.
 */
static err_t IR_SendFrame()
{
    err_t error = ERR_OK;
    uint8_t i = 0;
    if (_frame==NULL)
        return ERR_MEM;
    if (TI1_Enable(_interrupt_device_ptr) != ERR_OK)
        return ERR_IR;
    for (i = 0; i < _size ; i++) {
        IR_SendByte(_frame[i]);
    }
    if (TI1_Disable(_interrupt_device_ptr) != ERR_OK)
        return ERR_IR;
    return error;
}

/**
 *  Send a byte of data.
 *
 *  This function is internal, used only in this file.
 */
static void IR_SendByte(uint8_t data)
{
    uint16_t i = 0;
    // Bit0 Start: UART 0  <->  IR 1
    while (_interrupt_flag == 0)                // Wait for the interrupt
        ;
    PWM1_SetRatio16(0x7FFFU);                   // Set value
    _interrupt_flag = 0u;                       // Clear interrupt flag
    // Bit 1~8 data: Should be inverse
    for (i = 0; i < 8 ; i++) {
        while (_interrupt_flag == 0)
            ;
        if (data & 0x01u) {
            PWM1_SetRatio16(0x0000U);
        } else {
            PWM1_SetRatio16(0x7FFFU);
        }
        _interrupt_flag = 0u;
        data >>= 1;
    }
    // Bit9 END: UART 1  <->  IR 0
    while (_interrupt_flag == 0)
        ;
    PWM1_SetRatio16(0x0000U);
    _interrupt_flag = 0u;
    // Reset to zero
    while (_interrupt_flag == 0)
        ;
    PWM1_SetRatio16(0x0000U);
}

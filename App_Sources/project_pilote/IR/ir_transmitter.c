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
static const uint8_t                *_frame = NULL;
static const PiloteConfigurations   *_pilote_config_ptr = NULL;
static LDD_TDeviceData              *_interrupt_device_ptr = NULL;
static volatile uint32_t            _interrupt_flag = 0;    // Interrupt flag. It's changed in Events.c
// Methods
static void IR_SendByte(uint8_t data);
static err_t IR_SendFrame();

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

    // Own copy of configuration data
    uint8_t nums_of_frames = 0;
    PiloteTimeMs time_between_frames = 0;
    PiloteSourceMode source_mode = PILOTE_SOURCE_OFF;

    // Initialize periodic interrupt timer
    _interrupt_device_ptr = TI1_Init((LDD_TUserData*)&_interrupt_flag);

    // Main loop
    while (1) {
        if (_transmitting &&                                    // If transmit is enabled
            _pilote_config_ptr != NULL &&                       // If get the configuration structure and
            _frame != NULL) {                                   // frame address
            if (first_time_start) {
                if (_pilote_config_ptr->time_between_frames >= IR_PROTOCOL_TIME_BETWEEN_FRAME_MIN_MS &&
                    _pilote_config_ptr->time_between_frames >=
                    (_pilote_config_ptr->nums_of_frames*IR_PROTOCOL_FRAME_MIN_LENGTH_MS)) {
                    // Save a copy of some configurations of the pilote
                    nums_of_frames = _pilote_config_ptr->nums_of_frames;
                    time_between_frames = _pilote_config_ptr->time_between_frames;
                    source_mode = _pilote_config_ptr->source_mode;
                    first_time_start = FALSE;
                } else {
                    while (1) {
                    // Configuration error
                    // Should avoid this while configuring
                    }
                }
            }
            switch (source_mode) {
                case PILOTE_SOURCE_OFF:
                    last_wake_time = xTaskGetTickCount();       // Initialize last_wake_time with current time.
                    for (i = 0; i < nums_of_frames ; i++) {
                        if (IR_SendFrame() != ERR_OK) {
                            while (1) {
                            // IR error
                            }
                        }
                    }
                    vTaskDelayUntil(&last_wake_time, (TickType_t)time_between_frames/portTICK_PERIOD_MS);
                    break;
                case PILOTE_SOURCE_CONTACT_ON:
                    break;
                    // TODO
                case PILOTE_SOURCE_CONTACT_OFF:
                    break;
                    // TODO
                case PILOTE_SOURCE_TTL_ON:
                    break;
                    // TODO
                case PILOTE_SOURCE_TTL_OFF:
                    break;
                    // TODO
                case PILOTE_SOURCE_ANALOG_1KHZ:
                    break;
                    // TODO
                default:
                    break;
            }
        } else {
            // If not transmitting
            if (!first_time_start)
                first_time_start = TRUE;
            if (time_between_frames > 100)
                vTaskDelay((TickType_t)time_between_frames/portTICK_PERIOD_MS);
            else
                vTaskDelay(FREE_RTOS_DELAY_100MS);
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

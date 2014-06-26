/*
 * pilote_types.h
 *
 *  Created on: 6 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef PILOTE_TYPES_H_
#define PILOTE_TYPES_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "common.h"

/**
 * -----------------------------------------------------------------------------
 *                           Platform Related
 * -----------------------------------------------------------------------------
 */
// Compatibility with PEx
#ifndef __PE_Types_H

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#ifndef FALSE
#define FALSE 0x00U
#endif
#ifndef TRUE
#define TRUE 0x01U
#endif
#ifndef NULL
#define NULL 0x00U
#endif

#endif

/**
 * -----------------------------------------------------------------------------
 *                   Structure PiloteConfigurations
 * -----------------------------------------------------------------------------
 */
#define PILOTE_UDP_ID_COUNT        4U
#define PILOTE_UDP_DATA_COUNT      50U
#define PILOTE_COMMAND_PARAM_COUNT 2U

#define PILOTE_MODE_ITEMS_COUNT    3U
typedef enum {
    PILOTE_TRIGGER,
    PILOTE_SYNCHRO,
    PILOTE_COMMAND
} PiloteMode;

#define PILOTE_OUTPUT_MODE_ITEMS_COUNT  6U
typedef unsigned char PiloteOutputMode;
#define PILOTE_OUTPUT_OFF          0x00U
#define PILOTE_OUTPUT_IR           0x01U
#define PILOTE_OUTPUT_RF           0x02U
#define PILOTE_OUTPUT_UDP          0x04U
#define PILOTE_OUTPUT_CONTACT_ON   0x08U
#define PILOTE_OUTPUT_TTL_ON       0x10U

#define PILOTE_SOUCE_MODE_ITEMS_COUNT   11U
typedef enum {
    PILOTE_SOURCE_OFF,
    PILOTE_SOURCE_CONTACT_ON,
    PILOTE_SOURCE_CONTACT_OFF,
    PILOTE_SOURCE_TTL_ON,
    PILOTE_SOURCE_TTL_OFF,
    PILOTE_SOURCE_ANALOG_1KHZ,
    PILOTE_SOURCE_UDP,
    PILOTE_SOURCE_RS232,
    PILOTE_SOURCE_EDGE,
    PILOTE_SOURCE_PIONEER,
    PILOTE_SOURCE_DISPLAY_MATE
} PiloteSourceMode;

#define PILOTE_TOKEN_RING_ITEMS_COUNT   4U
typedef enum {
    PILOTE_TOKEN_RING_OFF,
    PILOTE_TOKEN_RING_MASTER,
    PILOTE_TOKEN_RING_SLAVE,
    PILOTE_TOKEN_RING_ALL
} PiloteTokenRing;

#define PILOTE_VIDEO_MODE_ITEMS_COUNT   4U
typedef enum {
    PILOTE_VIDEO_OFF,
    PILOTE_VIDEO_PAL_SECAM,
    PILOTE_VIDEO_CINEMA,
    PILOTE_VIDEO_NTSC
} PiloteVideoMode;

typedef signed int PiloteTimeMs;

#define PILOTE_WEEKDAY_TEMS_COUNT       8U
typedef uint8_t PiloteWeekday;
#define PILOTE_WEEKDAY_OFF         0x00U
#define PILOTE_WEEKDAY_SUNDAY      0x01U
#define PILOTE_WEEKDAY_MONDAY      0x02U
#define PILOTE_WEEKDAY_TUESDAY     0x04U
#define PILOTE_WEEKDAY_WEDNESDAY   0x08U
#define PILOTE_WEEKDAY_THURSDAY    0x10U
#define PILOTE_WEEKDAY_FRIDAY      0x20U
#define PILOTE_WEEKDAY_SATUSDAY    0x40U
/**
 * -----------------------------------------------------------------------------
 *                           Structure PiloteMessage
 * -----------------------------------------------------------------------------
 */
typedef enum {
    PILOTE_MES_RECV,
    PILOTE_MES_SEND
} PiloteMesDirection;

typedef enum {
    PILOTE_MES_TYPE_USB,
    PILOTE_MES_TYPE_UDP,
    PILOTE_MES_TYPE_HTTP
} PiloteMesType;

typedef enum {
    PILOTE_MES_OPERATION_START,
    PILOTE_MES_OPERATION_STOP,
    PILOTE_MES_OPERATION_MODIFY,
    PILOTE_MES_OPERATION_REPLY_MODIFY,
    PILOTE_MES_OPERATION_READ_CONFIG,
    PILOTE_MES_OPERATION_REPLY_CONFIG
} PiloteMesOperation;

#define PILOTE_MES_TARGET_ITEMS_COUNT   19U
typedef enum {
    PILOTE_MES_TARGET_ENABLE,
    PILOTE_MES_TARGET_MODE,
    PILOTE_MES_TARGET_OUTPUT_MODE,
    PILOTE_MES_TARGET_SOURCE_MODE,
    PILOTE_MES_TARGET_VIDEO_MODE,
    PILOTE_MES_TARGET_TOKEN_RING,
    PILOTE_MES_TARGET_CODE,
    PILOTE_MES_TARGET_GROUP,
    PILOTE_MES_TARGET_NUMS_OF_FRAMES,
    PILOTE_MES_TARGET_UDP_ID,
    PILOTE_MES_TARGET_UDP_DATA,
    PILOTE_MES_TARGET_COMMAND_PARAM,
    PILOTE_MES_TARGET_COMMAND_DATA,
    PILOTE_MES_TARGET_TIME_BT_FRAMES,
    PILOTE_MES_TARGET_DELAY_SOURCE,
    PILOTE_MES_TARGET_DELAY_OUTPUT,
    PILOTE_MES_TARGET_START_TIME,
    PILOTE_MES_TARGET_END_TIME,
    PILOTE_MES_TARGET_WEEKDAY
} PiloteMesTarget;

/**
 * -------------------------------------------------------------------------------------
 *                                    USB Protocol
 * -------------------------------------------------------------------------------------
 */
#define PILOTE_USB_PROTOCOL_FRAME_BYTES_COUNT   12U

/**
 * -------------------------------------------------------------------------------------
 *                                    IR Protocol
 * -------------------------------------------------------------------------------------
 */
#define IR_PROTOCOL_FREQUENCY_PER_BIT           2400U
#define IR_PROTOCOL_BIT_COUNT_PER_BYTE          10U
#define IR_PROTOCOL_FRAME_MIN_LENGTH_MS     \
    (1+(1000*IR_FRAME_COUNT*IR_PROTOCOL_BIT_COUNT_PER_BYTE)/IR_PROTOCOL_FREQUENCY_PER_BIT)
#define IR_PROTOCOL_TIME_BETWEEN_FRAME_MIN_MS   100

/**
 * -------------------------------------------------------------------------------------
 *                                    IR Frame
 * -------------------------------------------------------------------------------------
 */
#define IR_FRAME_COUNT             9U
#define IR_FRAME_START             0xB7U
#define IR_FRAME_CMD_DATA_1        0x5AU
#define IR_FRAME_CMD_DATA_2        0x5BU
/**
 * -------------------------------------------------------------------------------------
 *                            FreeRTOS Delay common
 * -------------------------------------------------------------------------------------
 */
// Delay time 0ms, should equal to (0/portTICK_PERIOD_MS)
#define FREE_RTOS_NONE_DELAY      ((TickType_t)(0U/portTICK_PERIOD_MS))
// Delay time 1s, should equal to (1000/portTICK_PERIOD_MS)
#define FREE_RTOS_DELAY_1S        ((TickType_t)(1000U/portTICK_PERIOD_MS))
// Delay time 500ms, should equal to (500/portTICK_PERIOD_MS)
#define FREE_RTOS_DELAY_500MS     ((TickType_t)(500U/portTICK_PERIOD_MS))
// Delay time 100ms, should equal to (100/portTICK_PERIOD_MS)
#define FREE_RTOS_DELAY_100MS     ((TickType_t)(100U/portTICK_PERIOD_MS))
// Delay time 50ms, should equal to (50/portTICK_PERIOD_MS)
#define FREE_RTOS_DELAY_50MS      ((TickType_t)(50U/portTICK_PERIOD_MS))
/**
 * -------------------------------------------------------------------------------------
 *                              Mailbox common
 * -------------------------------------------------------------------------------------
 */
#define MBOX_ZERO_ITEM            0x00U
#define MBOX_ZERO_TIMEOUT         FREE_RTOS_NONE_DELAY
#define MBOX_TIMEOUT_INFINIT      portMAX_DELAY
#define MBOX_TIMEOUT_1S           FREE_RTOS_DELAY_1S
#define MBOX_TIMEOUT_500MS        FREE_RTOS_DELAY_500MS
#define MBOX_TIMEOUT_50MS         FREE_RTOS_DELAY_50MS
/**
 * -------------------------------------------------------------------------------------
 *              Pilote Config Mailbox (between cofig_manager and ir_manager)
 * -------------------------------------------------------------------------------------
 */
#define MBOX_CONFIG_COUNT         1U

/**
 * -------------------------------------------------------------------------------------
 *            Communication Mailbox (between USB/UDP/HTTP and config_manager)
 * -------------------------------------------------------------------------------------
 */
/**
 *  mbox_recv
 *      Receive from PC
 *  mbox_send
 *      Send to PC
 */
#define MBOX_RECV_COUNT           3U
#define MBOX_SEND_COUNT           3U

/**
 * -------------------------------------------------------------------------------------
 *                              EEPROM 24AA1026
 * -------------------------------------------------------------------------------------
 */
#define EEPROM_MEM_START_ADDR    0x10

/**
 * -------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------
 */
#endif /* PILOTE_TYPES_H_ */

/*
 * ir_transmitter.h
 *
 *  Created on: 10 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef IR_TRANSMITTER_H_
#define IR_TRANSMITTER_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_config.h"
#include "pilote_error.h"
#include "pilote_types.h"
#include "pilote_mes.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  Start the IR transmit session.
 */
err_t IR_StartTransmit(uint8_t *frame, uint8_t size, const PiloteConfigurations *pilote_config_ptr);
/**
 * Stop the IR transmit session.
 */
err_t IR_StopTransmit(void);
/**
 * IR transmit thread. Sends frames depending on the configuration.
 */
void IR_TransmitThread(void *pvParameters);

#endif /* IR_TRANSMITTER_H_ */

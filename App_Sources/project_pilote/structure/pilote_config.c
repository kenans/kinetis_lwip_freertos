/*
 * pilote_config.c
 *
 *  Created on: 6 juin 2014
 *      Author: OrpheoXlite
 */

#include "pilote_config.h"


/**
 *  @param *pilote_config
 *      a pointer to the instance of a PiloteConfigurations structure. Attention, memory
 *      should be preallocated before using.
 *  @return
 *      ERR_OK if no errors.
 */
err_t PiloteConfigInit(PiloteConfigurations *pilote_config)
{
    err_t error = ERR_OK;
    int i = 0; // Should be large enough

    if (pilote_config != NULL) {
        //pilote_config->enabled = FALSE;
        pilote_config->enabled = TRUE;
        pilote_config->mode = PILOTE_TRIGGER;
        pilote_config->code = 0;
        pilote_config->group = 0;
        pilote_config->nums_of_frames = 1;
        pilote_config->time_between_frames = 100;
        pilote_config->source_mode = PILOTE_SOURCE_OFF;
        pilote_config->token_ring = PILOTE_TOKEN_RING_OFF;
        pilote_config->video_mode = PILOTE_VIDEO_OFF;
        pilote_config->command_data = 0;
        // Time
        pilote_config->delay_source = 0;
        pilote_config->delay_output = 0;
        pilote_config->start_time = 0;
        pilote_config->end_time = 0;
        // Mask
        pilote_config->output_mode = PILOTE_OUTPUT_IR;
        pilote_config->weekday = PILOTE_WEEKDAY_OFF;
        // Array filled with zeros
        for (i = 0; i < PILOTE_UDP_ID_COUNT ; i++) {
            pilote_config->udp_id[i] = 0;
        }
        for (i = 0; i < PILOTE_UDP_DATA_COUNT ; i++) {
            pilote_config->udp_data[i] = 0;
        }
        for (i = 0; i < PILOTE_COMMAND_PARAM_COUNT ; i++) {
            pilote_config->command_param[i] = 0;
        }
    } else {
        error = ERR_MEM;
    }

    return error;
}

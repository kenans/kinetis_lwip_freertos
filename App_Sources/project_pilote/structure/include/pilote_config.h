/*
 * pilote_config.h
 *
 *  Created on: 6 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef PILOTE_CONFIG_H_
#define PILOTE_CONFIG_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
#include "pilote_error.h"
#include "pilote_types.h"

/**
 * -------------------------------------------------------------------------------------------
 *                                      Structures
 * -------------------------------------------------------------------------------------------
 */
/**
 *  Pilote configurations structure. Use to keep the configuration settings of a pilote.
 */
typedef struct {
    bool                enabled;
    PiloteMode          mode;
    PiloteOutputMode    output_mode;    // Sur quelle sortie envoie des signaux
    PiloteSourceMode    source_mode;    // Quand est ce qu'on envoie des signaux
    PiloteVideoMode     video_mode;
    PiloteTokenRing     token_ring;
    uint16_t            code;
    uint8_t             group;
    uint8_t             nums_of_frames;
    uint8_t             udp_id[PILOTE_UDP_ID_COUNT];
    uint8_t             udp_data[PILOTE_UDP_DATA_COUNT];
    uint8_t             command_param[PILOTE_COMMAND_PARAM_COUNT];  // CMD1,2
    uint16_t            command_data;   // Volume/ PARAM3,4  sauf PLAY
    PiloteTimeMs        time_between_frames;    // >100ms
    PiloteTimeMs        delay_source;       //
    PiloteTimeMs        delay_output;       //

    PiloteTimeMs        start_time;
    PiloteTimeMs        end_time;
    PiloteWeekday       week_day;
} PiloteConfigurations;

/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  Initializes a PiloteConfigurations structure. It should be called when a new instance is
 *  created. Returns error if the creation is failed.
 *  @param *pilote_config
 *      a pointer to the instance of a PiloteConfigurations structure. Attention, memory
 *      should be preallocated before using.
 *  @return
 *      ERR_OK if no errors.
 *
 *  Sample usage:
 *      PiloteConfigurations pilote_config;
 *      err_t err = PiloteConfigInit(&pilote_config);
 *      if (err!=ERR_OK) {
 *          // Deal with the error
 *      }
 *      // ...
 */
err_t PiloteConfigInit(PiloteConfigurations *pilote_config);

/**
 *
 */
#define PiloteConfigEnable(pilote_config) do{(pilote_config)->enabled=TRUE}while(0)
#define PiloteConfigDisable(pilote_config) do{(pilote_config)->enabled=FALSE}while(0)

#endif /* PILOTE_CONFIG_H_ */

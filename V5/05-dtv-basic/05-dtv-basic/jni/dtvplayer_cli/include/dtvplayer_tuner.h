#ifndef __DTV_PLAYER_TUNER_H__
#define __DTV_PLAYER_TUNER_H__

#include "dtvplayer_utils.h"

/**
 * @brief This function is used to initialize tuner module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Init();

/**
 * @brief This function is used to tune to desired frequency
 * 
 * @param frequency 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Tune(uint32_t frequency);

/**
 * @brief This function is used to untune from previously tuned frequency
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Untune();

/**
 * @brief This function is used to deinitialize tuner module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Deinit();

#endif // __DTV_PLAYER_TUNER_H__

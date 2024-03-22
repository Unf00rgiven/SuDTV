#ifndef __DTV_PLAYER_AV_H__
#define __DTV_PLAYER_AV_H__

#include "tdal_av.h"
#include "dtvplayer_parser_utils.h"

/**
 * @brief This function is used to initialize AV module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Init();

/**
 * @brief This function is used to start AV playback
 * 
 * @param codecVideo video codec enum value
 * @param codecAudio audio codec enum value
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Start(int codecVideo, int codecAudio);

/**
 * @brief his function is used to stop AV playback
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Stop();

/**
 * @brief Use this function to deinitialize AV module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Deinit();

#endif // __DTV_PLAYER_AV_H__
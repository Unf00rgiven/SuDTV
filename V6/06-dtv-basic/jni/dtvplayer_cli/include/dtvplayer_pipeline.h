#ifndef __DTV_PLAYER_PIPELINE_H__
#define __DTV_PLAYER_PIPELINE_H__

#include "dtvplayer_utils.h"


/**
 * @brief This function is used to initialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Init();

/**
 * @brief This function is used to connect input data path
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectInput();

/**
 * @brief This function is used to disconnect input data path
 * Input data path is path between tuner and demux
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_DisconnectInput();

/**
 * @brief This function is used to deinitialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Deinit();

/**
 * @brief This function is used to connect output data path
 * Output data path is path between demux and av decoders
 * 
 * @param channelIdVideo channel id opened for Video data filtering
 * @param channelIdAudio channel id opened for Audio data filtering
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectOutput(tTDAL_DMX_ChannelId channelIdVideo, tTDAL_DMX_ChannelId channelIdAudio);

/**
 * @brief This function is used to disconnect output data path
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_DisconnectOutput();

#endif // __DTV_PLAYER_PIPELINE_H__
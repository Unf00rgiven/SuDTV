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


#endif // __DTV_PLAYER_PIPELINE_H__
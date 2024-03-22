#include <jni.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_disp.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_ts.h"

#include <android/log.h>


#include "dtvplayer_parser_pat.h"
#include "dtvplayer_parser_pmt.h"
#include "dtvplayer_pipeline.h"


/**
 * @brief This function is used to initialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Init()
{

    // TODO Implement Exercise 3
    // NOTE: Initialize TDAL TSROUTE module

    // IMPLEMENTATION
    tTDAL_TSROUTE_Error error;
    error = TDAL_TSROUTE_Init();
    if(error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to connect input data path
 * 
 * @param handleTuner tunner handle
 * @param handleDemux demux handle
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectInput(tTDAL_TS_Object handleTuner ,tTDAL_TS_Object handleDemux)
{
    tTDAL_TSROUTE_Error tsr_error = eTDAL_TSROUTE_NO_ERROR;

    // TODO Implement Exercise 3
    // NOTE: Use TDAL TSROUTE module to connect input handles

    // IMPLEMENTATION
    tsr_error = TDAL_TSROUTE_Connect(handleTuner, handleDemux);
    if(tsr_error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Connect failed. Error: [%d]", __FUNCTION__, __LINE__, tsr_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to disconnect input data path
 * Input data path is path between tuner and demux
 * 
 * @param handleTuner tunner handle
 * @param handleDemux demux handle
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_DisconnectInput(tTDAL_TS_Object handleTuner ,tTDAL_TS_Object handleDemux)
{
    tTDAL_TSROUTE_Error tsr_error = eTDAL_TSROUTE_NO_ERROR;

    // TODO Implement Exercise 3
    // NOTE: Use TDAL TSROUTE module to disconnect input handles

    // IMPLEMENTATION
    tsr_error = TDAL_TSROUTE_Disconnect(handleTuner, handleDemux);
    if(tsr_error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Disconnect failed. Error: [%d]", __FUNCTION__, __LINE__, tsr_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to connect output data path
 * Output data path is path between demux and av decoders
 * 
 * @param channelIdVideo channel id opened for Video data filtering
 * @param channelIdAudio channel id opened for Audio data filtering
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectOutput(tTDAL_DMX_ChannelId channelIdVideo, tTDAL_DMX_ChannelId channelIdAudio)
{
    tTDAL_AV_Error av_error;
    // TODO Implement Exercise 6
    // NOTE: Use TDAL AV module to set demux channel data and connect with decoders

    // IMPLEMENTATION
    av_error = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1, channelIdVideo);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_InputStreamSet failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
    }

    av_error = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1, channelIdAudio);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_InputStreamSet failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
    }
    //////////////////////////////////////////////////
     return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to disconnect output data path
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_DisconnectOutput()
{
    // If DMX is stopped we dont need to disconnect anything additionally
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to deinitialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Deinit()
{
    return ePLAYER_ERROR_OK;
}

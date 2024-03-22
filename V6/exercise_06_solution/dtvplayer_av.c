#include <stdio.h>
#include <string.h> 
#include <unistd.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_disp.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_ts.h"

#include "dtvplayer_av.h"
#include "dtvplayer_utils.h"

/**
 * @brief This function is used to initialize AV module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Init()
{
    // TODO Implement Exercise 6
    // NOTE: Initialize TDAL AV module

    // IMPLEMENTATION
    tTDAL_AV_Error av_error;
    av_error = TDAL_AV_Init();
    if(av_error != eTDAL_AV_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_AV_Init failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start AV playback
 * 
 * @param codecVideo video codec enum value
 * @param codecAudio audio codec enum value
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Start(int codecVideo, int codecAudio)
{
    // TODO Implement Exercise 6
    // NOTE: Call start in TDAL AV module
    // NOTE: Use first audio and first video decoders

    // IMPLEMENTATION
    tTDAL_AV_Error av_error;
    tTDAL_AV_StreamType streamType;

    streamType.videoType = codecVideo;
    av_error = TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, streamType);
    if(av_error != eTDAL_AV_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_AV_Start VIDEO failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return ePLAYER_ERROR_GENERAL;
    }

    streamType.audioType = codecAudio;
    av_error = TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, streamType);
    if(av_error != eTDAL_AV_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_AV_Start AUDIO failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief his function is used to stop AV playback
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Stop()
{
    // TODO Implement Exercise 6
    // NOTE: Call stop in TDAL AV module
    // NOTE: Use same decoder indexes as used for starting

    // IMPLEMENTATION
    tTDAL_AV_Error av_error;
    av_error = TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
    if(av_error != eTDAL_AV_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_AV_Stop VIDEO failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return ePLAYER_ERROR_GENERAL;
    }

    av_error = TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
    if(av_error != eTDAL_AV_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_AV_Stop AUDIO failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief Use this function to deinitialize AV module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Deinit()
{
    return ePLAYER_ERROR_OK;
}
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
    tTDAL_AV_Error av_error;

    // DONE Call AV module initialization in SDK
    av_error = TDAL_AV_Init();
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_Init failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return -1;
    }
    // NOTE 1: Find TDAL_AV_Init function in API and provide appropriate params
    // NOTE 2: Check for errors

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
    tTDAL_AV_Error av_error;
    tTDAL_AV_StreamType streamType;
    
    // DONE Start AV decoders
    streamType.videoType = codecVideo;
    av_error = TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, streamType);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_Start failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return -1;
    }

    streamType.audioType = codecAudio;
    av_error = TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, streamType);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_Start failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return -1;
    }
    // NOTE 1: Find TDAL_AV_Start function in API and provide appropriate params
    // NOTE 1.1: Use eTDAL_AV_DECODER_VIDEO_1 and eTDAL_AV_DECODER_AUDIO_1 as decoder enumerators
    // NOTE 2: Check tTDAL_AV_StreamType in API!!!
    // NOTE 3: Check for errors

    return ePLAYER_ERROR_OK;
}

/**
 * @brief his function is used to stop AV playback
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Stop()
{
    tTDAL_AV_Error av_error;

    // DONE Stop AV decoders
    av_error = TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_Stop failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return -1;
    }

    av_error = TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_Stop failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
        return -1;
    }
    // NOTE 1: Find TDAL_AV_Stop function in API and provide appropriate params
    // NOTE 1.1: Use eTDAL_AV_DECODER_VIDEO_1 and eTDAL_AV_DECODER_AUDIO_1 as decoder enumerators
    // NOTE 2: Check for errors
    
    return ePLAYER_ERROR_OK;
}

/**
 * @brief Use this function to deinitialize AV module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_AV_Deinit()
{
    TDAL_AV_Terminate();
    return ePLAYER_ERROR_OK;
}

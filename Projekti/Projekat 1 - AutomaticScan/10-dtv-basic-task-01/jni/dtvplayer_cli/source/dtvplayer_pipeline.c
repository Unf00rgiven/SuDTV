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


#include "dtvplayer_pipeline.h"


/**
 * @brief This function is used to initialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Init()
{
    tTDAL_TSROUTE_Error error;

    error = TDAL_TSROUTE_Init();
    if(error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to connect input data path
 * 
 * @param handleTuner tunner handle
 * @param handleDemux demux handle
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectInput()
{
    tTDAL_TSROUTE_Error error = eTDAL_TSROUTE_NO_ERROR;

    tTDAL_TS_Object tuner = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_SOURCE, eTDAL_TS_TYPE_TUNER, eTDAL_DMD_FE_0);
    tTDAL_TS_Object demux = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_DESTINATION, eTDAL_TS_TYPE_DEMUX, eTDAL_DMX_DEMUX0);

    error = TDAL_TSROUTE_Connect(tuner, demux);
    if(error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Connect failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }
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
tErrorCode DtvPlayer_Pipeline_DisconnectInput()
{
    tTDAL_TSROUTE_Error error = eTDAL_TSROUTE_NO_ERROR;

    tTDAL_TS_Object tuner = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_SOURCE, eTDAL_TS_TYPE_TUNER, eTDAL_DMD_FE_0);
    tTDAL_TS_Object demux = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_DESTINATION, eTDAL_TS_TYPE_DEMUX, eTDAL_DMX_DEMUX0);

    error = TDAL_TSROUTE_Disconnect(tuner, demux);
    if(error != eTDAL_TSROUTE_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_TSROUTE_Disconnect failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to deinitialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Deinit()
{
    TDAL_TSROUTE_Term();
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

    av_error = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1, channelIdVideo);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_InputStreamSet failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
    }

    av_error = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1, channelIdAudio);
    if(av_error != eTDAL_AV_NO_ERROR) {
        LOGE("[%s:%d] TDAL_AV_InputStreamSet failed. Error: [%d]", __FUNCTION__, __LINE__, av_error);
    }
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

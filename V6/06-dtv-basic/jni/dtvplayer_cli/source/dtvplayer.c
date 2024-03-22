#include <stdio.h>

#include "tkel.h"
#include "dtvplayer.h"
#include "dtvplayer_comm.h"
#include "dtvplayer_tuner.h"
#include "dtvplayer_demux.h"
#include "dtvplayer_pipeline.h"
#include "dtvplayer_av.h"

#include "tdal_common.h"

// AV demux channels used for connecting data path/pipeline
static tTDAL_DMX_ChannelId gChannelIdVideo;
static tTDAL_DMX_ChannelId gChannelIdAudio;

/**
 * @brief Module entry point.
 * This function is used to initiate all modules that are being used
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Init()
{
    tErrorCode error;
    tTDAL_AV_Error av_error;
    TKEL_Init();
    TDAL_Init();

    // Initialize player here
    error = DtvPlayer_Tuner_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner init failed!\n");
        return -1;
    } else {
        printf("Tuner init success!\n");
    }

    error = DtvPlayer_Demux_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux init failed!\n");
        return -1;
    } else {
        printf("Demux init success!\n");
    }

    error = DtvPlayer_Pipeline_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline init failed!\n");
        return -1;
    } else {
        printf("Pipeline init success!\n");
    }

    error = DtvPlayer_Pipeline_ConnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline connect failed!\n");
        return -1;
    } else {
        printf("Pipeline connect success!\n");
    }

    // DONE: Initalize AV Module here
    error = DtvPlayer_AV_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("AV Init failed!\n");
        return -1;
    }
    printf("AV init success!\n");
    // NOTE 1: Check for errors

    return ePLAYER_ERROR_OK;
}

/**
 * @brief Module deinitialization.
 * This finction is used to deinitialize all used modules
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Deinit()
{
    tErrorCode error;
    tTDAL_AV_Error av_error;

    error = DtvPlayer_Pipeline_DisconnectInput();
    if(error != ePLAYER_ERROR_OK) {
    // NOTE 2: Check for errors
        printf("Pipeline disconnect failed!\n");
        return -1;
    } else {
        printf("Pipeline disconnect success!\n");
    }

    error = DtvPlayer_Tuner_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner deinit failed!\n");
        return -1;
    } else {
        printf("Tuner deinit success!\n");
    }

    error = DtvPlayer_Demux_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux deinit failed!\n");
        return -1;
    } else {
        printf("Demux deinit success!\n");
    }

    error = DtvPlayer_Pipeline_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline deinit failed!\n");
        return -1;
    } else {
        printf("Pipeline deinit success!\n");
    }

    // DONE: Deinitalize AV Module here
    error = DtvPlayer_AV_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("AV Deinit failed!\n");
        return -1;
    }
    printf("AV Deinit success!\n");
    // NOTE 1: Check for errors

    TDAL_Term();
    return ePLAYER_ERROR_OK;
}


/**
 * @brief Use this function to tune to desired frequency
 * This function passes tune request into module where tune is actually implemented.
 * After this function is called, tuner will lock to desired frequency.
 * After this step is completed, it is safe to connect Tuner with DMX.
 * 
 * @param frequency input frequency
 */
void DtvPlayer_Tune(int frequency)
{
    tErrorCode error;
    if(frequency > 0 && frequency < 1e3)
    {
        frequency *= 1e6;
    } else if(frequency >= 1e3 && frequency < 1e6)
    {
        frequency *= 1e3;
    }

    error = DtvPlayer_Tuner_Tune(frequency);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Tune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Tune success.", __FUNCTION__, __LINE__);
    }
}

/**
 * @brief Use this function to untune from previously tuned frequency
 * If data path between tuner and demux is connected, it is necessary to 
 * disconnect it first before untuning
 */
void DtvPlayer_Untune()
{
    tErrorCode error;

    error = DtvPlayer_Tuner_Untune();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
}


void DtvPlayer_Analyze()
{
    tErrorCode error;

    error = DtvPlayer_Demux_AnalyzePsi();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
}


/**
 * @brief Use this function to start channel playback.
 * To play a channel, it is important for following conditions to be met:
 *      Tune to a desired frequency.
 *      Data path must be formed between tuner and demux.
 *      Stream parameters must be found (video pid and codec, audio pid and codec)
 *      Demux has to filter AV packages
 *      Data path must be formed between demux and av
 *      Decoders to be started
 * 
 * @param pidVideo channel video PID
 * @param pidAudio channel audio PID
 * @param codecVideo channel video codec
 * @param codecAudio channel audio codec
 */
void DtvPlayer_Start(int pidVideo, int pidAudio, int codecVideo, int codecAudio)
{
    tErrorCode error;

    // DONE: Start AV data filtering and start AV decoders
    error = DtvPlayer_Demux_StartAVFiltering(pidVideo, pidAudio, &gChannelIdVideo, &gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK) {
        LOGE("[%s:%d] DtvPlayer_Demux_StartAVFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    }
    // NOTE 1: Call function DtvPlayer_Demux_StartAVFiltering and provide apropriate parameters
    error = DtvPlayer_AV_Start(codecVideo, codecAudio);
    if(error != ePLAYER_ERROR_OK) {
        LOGE("[%s:%d] DtvPlayer_AV_Start failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    }
    // NOTE 2: Call function DtvPlayer_AV_Start and provide apropriate parameters
    // NOTE 3: check for errors
}

/**
 * @brief Use this function to stop channel playback.
 * Channel playback has to be active for this function to work.
 * 
 */
void DtvPlayer_Stop()
{
    tErrorCode error;

    // DONE: Stop AV decoiders and AV data filtering
    error = DtvPlayer_AV_Stop();
    if(error != ePLAYER_ERROR_OK) {
        LOGE("[%s:%d] DtvPlayer_AV_Stop failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    }
    // NOTE 1: Call function DtvPlayer_AV_Stop
    error = DtvPlayer_Demux_StopAVFiltering(&gChannelIdVideo, &gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK) {
        LOGE("[%s:%d] DtvPlayer_Demux_StopAVFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    }
    // NOTE 2: Call function DtvPlayer_Demux_StopAVFiltering and provide apropriate parameters
    // NOTE 3: check for errors
}

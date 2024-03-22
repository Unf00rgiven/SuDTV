#include <stdio.h>

#include "tkel.h"
#include "dtvplayer.h"
#include "dtvplayer_comm.h"
#include "dtvplayer_tuner.h"
#include "dtvplayer_demux.h"
#include "dtvplayer_pipeline.h"
#include "dtvplayer_av.h"
#include "tdal_disp.h"

#include "tdal_common.h"

// AV demux channels used for connecting data path/pipeline
static tTDAL_DMX_ChannelId gChannelIdVideo;
static tTDAL_DMX_ChannelId gChannelIdAudio;

// Channel data callback fn declaration
static void DtvPlayer_ChannelCbFn(tChannel*);

// Channel list
static tChannelList gChannelList;

static uint8_t gIsAvStarted;

static uint32_t gCurrentFrequency;

/**
 * @brief Module entry point.
 * This function is used to initiate all modules that are being used
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Init()
{
    tErrorCode error;
    TKEL_Init();
    TDAL_Init();
    TDAL_DISP_Init();

    // Initialize player here
    error = DtvPlayer_Tuner_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner init failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Tuner init success!\n");
    }

    error = DtvPlayer_Demux_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux init failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Demux init success!\n");
    }
    DtvPlayer_Demux_RegisterPsiCallback(DtvPlayer_ChannelCbFn);

    error = DtvPlayer_Pipeline_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline init failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Pipeline init success!\n");
    }

    error = DtvPlayer_Pipeline_ConnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline connect failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Pipeline connect success!\n");
    }

    error = DtvPlayer_AV_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("AV init failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("AV init success!\n");
    }

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

    error = DtvPlayer_Pipeline_DisconnectInput();
    if(error != ePLAYER_ERROR_OK) {
    // NOTE 2: Check for errors
        printf("Pipeline disconnect failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Pipeline disconnect success!\n");
    }

    error = DtvPlayer_Tuner_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner deinit failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Tuner deinit success!\n");
    }

    error = DtvPlayer_Demux_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux deinit failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Demux deinit success!\n");
    }

    error = DtvPlayer_Pipeline_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline deinit failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("Pipeline deinit success!\n");
    }

    error = DtvPlayer_AV_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("AV deinit failed!\n");
        return ePLAYER_ERROR_NOT_STARTED;
    } else {
        printf("AV deinit success!\n");
    }

    // TDAL_Term();
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

    gCurrentFrequency = frequency;

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

    error = DtvPlayer_Pipeline_DisconnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline disconnect failed!\n");
        return;
    }

    error = DtvPlayer_Tuner_Untune();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
    error = DtvPlayer_Pipeline_ConnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline connect failed!\n");
        return;
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
    uint32_t pHandle;
    TDAL_DISP_LayerHandleGet(0, &pHandle);


    error = DtvPlayer_Demux_StartAVFiltering(pidVideo, pidAudio, &gChannelIdVideo, &gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Demux_StartAVFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Demux_StartAVFiltering success.", __FUNCTION__, __LINE__);
    }

    // TODO: Connect AV and DMX pipeline
    // NOTE 1: Call function DtvPlayer_Pipeline_ConnectOutput and provide apropriate parameters
    // NOTE 3: check for errors

    error = DtvPlayer_AV_Start(codecVideo, codecAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_AV_Start failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    } else
    {
        LOGI("[%s:%d] DtvPlayer_AV_Start success.", __FUNCTION__, __LINE__);
    }
    gIsAvStarted = 1;
}

/**
 * @brief Use this function to stop channel playback.
 * Channel playback has to be active for this function to work.
 * 
 */
void DtvPlayer_Stop()
{
    tErrorCode error;

    error = DtvPlayer_AV_Stop();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_AV_Stop failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_AV_Stop success.", __FUNCTION__, __LINE__);
    }

    error = DtvPlayer_Demux_StopAVFiltering(&gChannelIdVideo, &gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Demux_StopAVFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Demux_StopAVFiltering success.", __FUNCTION__, __LINE__);
    }
    gIsAvStarted = 0;
}


/**
 * @brief Use this function to print out channel list
 * 
 */
void DtvPlayer_List()
{
    if(gChannelList.channelCount == 0)
    {
        printf("No channels.\n");
        return;
    }

    // TODO: Add Frequency print
    // NOTE 1: Add it before video info 

}

/**
 * @brief Use this function to directly start channel from channel list
 * 
 * @param channelNumber channel index from channel list
 */
void DtvPlayer_StartChannel(int channelNumber)
{
    // TODO Implement zapping based on channel number and channel list
    // NOTE 1: Check if number is valid
    // NOTE 2: Check if channel list is valid

    if(gIsAvStarted)
    {
        DtvPlayer_Stop();
        usleep(1e6); // add 1s wait time for decoders to stop
    }
    // NOTE 3: Check frequency and tune if necessary
}

/**
 * @brief Use this function to receive channel data from demux module
 * 
 * @param event 
 * @param channel 
 */
static void DtvPlayer_ChannelCbFn(tChannel* channel)
{
    channel->freq = gCurrentFrequency;

    // TODO: Add channel to channel list
    // NOTE 1: Check if channel already exists
}

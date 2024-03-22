#include <stdio.h>
#include <android/native_window_jni.h>


#include "dtvplayer_native.h"
#include "dtvplayer_comm.h"
#include "dtvplayer_commands.h"
#include "dtvplayer_pipeline.h"
#include "dtvplayer_tuner.h"
#include "dtvplayer_demux.h"
#include "dtvplayer_av.h"

// Channel data callback fn declaration
static void DtvPlayer_ChannelCbFn(tChannelEventType, tChannelItem*);

// Channel list
static tChannelList gChannelList;

// AV demux channels used for connecting data path/pipeline
static tTDAL_DMX_ChannelId gChannelIdVideo;
static tTDAL_DMX_ChannelId gChannelIdAudio;

// Is input data path connected
static uint8_t gAreInputsConnected;

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
    // Initialize communication module
    DtvPlayer_Comm_Init();

    // Initalize Code DTV SDK modules
    TKEL_Init();
    TDAL_Init();
    TDAL_DISP_Init();

    // TODO Implement Exercise 2
    // NOTE: Initialize Tuner module
    
    // IMPLEMENTATION
    tErrorCode error;
    error = DtvPlayer_Tuner_Init();
    if(error != ePLAYER_ERROR_OK)
    {
        // If this step failes we will receive errors for all other Tuner calls
        // In this case we should gracefully stop application because functionality is broken and 
        // there is no sense to continue application execution, but it is not required for this course.
        LOGE("[%s:%d] DtvPlayer_Tuner_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Init success.", __FUNCTION__, __LINE__);
    }
    //////////////////////////////////////////////////

    // TODO Implement Exercise 3
    // NOTE: Initialize Demux module

    // IMPLEMENTATION
    error = DtvPlayer_Demux_Init();
    if(error != ePLAYER_ERROR_OK)
    {
        // If this step failes we will receive errors for all other Tuner calls
        // In this case we should gracefully stop application because functionality is broken and 
        // there is no sense to continue application execution, but it is not required for this course.
        LOGE("[%s:%d] DtvPlayer_Demux_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Demux_Init success.", __FUNCTION__, __LINE__);
    }
    DtvPlayer_Demux_RegisterPsiCallback(DtvPlayer_ChannelCbFn);

    error = DtvPlayer_Pipeline_Init();
    if(error != ePLAYER_ERROR_OK)
    {
        // If this step failes we will receive errors for all other Tuner calls
        // In this case we should gracefully stop application because functionality is broken and 
        // there is no sense to continue application execution, but it is not required for this course.
        LOGE("[%s:%d] DtvPlayer_Pipeline_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Pipeline_Init success.", __FUNCTION__, __LINE__);
    }
    //////////////////////////////////////////////////
    
    
    // TODO Implement Exercise 3
    // NOTE: Initialize data path/pipeline module

    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 6
    // NOTE: Initialize AV module
    error = DtvPlayer_AV_Init();
    if(error != ePLAYER_ERROR_OK)
    {
        // If this step failes we will receive errors for all other Tuner calls
        // In this case we should gracefully stop application because functionality is broken and 
        // there is no sense to continue application execution, but it is not required for this course.
        LOGE("[%s:%d] DtvPlayer_AV_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_AV_Init success.", __FUNCTION__, __LINE__);
    }


    FILE * outfile = fopen(CHANNEL_LIST_PATH,"rb");  // w for write, b for binary
    if(outfile) // file exist?
    {
        fread(&gChannelList, 1, sizeof(gChannelList), outfile);
        fclose(outfile);
    }
    // IMPLEMENTATION

    //////////////////////////////////////////////////
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
    // Deinitalize communication module
    DtvPlayer_Comm_Deinit();

    return ePLAYER_ERROR_OK;
}

/**
 * @brief Register surface for video rendering.
 * This function is used to set surface for video rendering.
 * Without this, decoders cannot be started.
 * 
 * @param window Surface pointer converted to ANativeWindow
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_SetSurface(void* window) {
 
    TDAL_DISP_LayerHandleSet(0, window);

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
    // TODO Implement Exercise 1
    // NOTE: Add print for function enter
    
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 2
    // NOTE: Convert input frequency into expected range. SDK expect frequency in Hz
    // NOTE: Call tune function realized in Tuner module
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
    gCurrentFrequency = frequency/1e6;
    // IMPLEMENTATION

    //////////////////////////////////////////////////
}

// TODO Implement Exercise 8
// NOTE: Add Function for URL tune


/**
 * @brief Use this function to scan PAT&PMT tables from tuned channel
 * This function needs to initiate PAT and PMT parsing in order to gain
 * information about available channels in tuned frequency.
 * In order to get tables in Demultiplexer module, first tuner and demux must be
 * connected. After connection is established data path is formed between tuner and demux.
 * Once data path/pipeline is connected this function should call demux module to start
 * parsing operations.
 * If channel information is needed, it is possible to register data callback on demux module
 * to receive parsed information.  
 * 
 */
void DtvPlayer_Scan()
{

    // TODO Implement Exercise 1
    // NOTE: Add print for function enter

    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 3
    // NOTE: Connect input if not already connected

    // IMPLEMENTATION
    tErrorCode error;
    if(gAreInputsConnected == 0)
    {
        tTDAL_TS_Object handleTuner, handleDemux;
        DtvPlayer_Tuner_GetHandle(&handleTuner);
        DtvPlayer_Demux_GetHandle(&handleDemux);
        
        error = DtvPlayer_Pipeline_ConnectInput(handleTuner, handleDemux);
        if(error != ePLAYER_ERROR_OK)
        {
            LOGE("[%s:%d] DtvPlayer_Pipeline_ConnectInput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
            return;
        } else
        {
            LOGI("[%s:%d] DtvPlayer_Pipeline_ConnectInput success.", __FUNCTION__, __LINE__);
        }
        gAreInputsConnected = 1;
    }
    error = DtvPlayer_Demux_ScanPsi();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Demux_ScanPsi failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Demux_ScanPsi success.", __FUNCTION__, __LINE__);
    }
    //////////////////////////////////////////////////

    // TODO Implement Exercise 4
    // NOTE: Call PAT&PMT parsing in demux module
    // NOTE: Register callback to receive data from demux module

    // IMPLEMENTATION

    //////////////////////////////////////////////////
}

/**
 * @brief Use this function to untune from previously tuned frequency
 * If data path between tuner and demux is connected, it is necessary to 
 * disconnect it first before untuning
 */
void DtvPlayer_Untune()
{
    // TODO Implement Exercise 1
    // NOTE: Add print for function enter
    
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 3
    // NOTE: Disconnect input if not already disconnected

    // IMPLEMENTATION
    tErrorCode error;
    if(gAreInputsConnected == 1)
    {
        tTDAL_TS_Object handleTuner, handleDemux;
        DtvPlayer_Tuner_GetHandle(&handleTuner);
        DtvPlayer_Demux_GetHandle(&handleDemux);
        
        error = DtvPlayer_Pipeline_DisconnectInput(handleTuner, handleDemux);
        if(error != ePLAYER_ERROR_OK)
        {
            LOGE("[%s:%d] DtvPlayer_Pipeline_DisconnectInput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
            return;
        } else
        {
            LOGI("[%s:%d] DtvPlayer_Pipeline_DisconnectInput success.", __FUNCTION__, __LINE__);
        }
        gAreInputsConnected = 0;
    }
    //////////////////////////////////////////////////

    // TODO Implement Exercise 2
    // NOTE: Call untune function realized in Tuner module
    error = DtvPlayer_Tuner_Untune();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
    // IMPLEMENTATION

    //////////////////////////////////////////////////
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
    // TODO Implement Exercise 1
    // NOTE: Add print for function enter
    
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 6
    // NOTE: Check if input data path is connected like in previous Exercise
    // NOTE: Start AV filtering
    // NOTE: Connect data path/pipeline between demux and av modules
    // NOTE: Call starting decoders realized in AV module

    // IMPLEMENTATION
    tErrorCode error;
    if(gAreInputsConnected == 0)
    {
        tTDAL_TS_Object handleTuner, handleDemux;
        DtvPlayer_Tuner_GetHandle(&handleTuner);
        DtvPlayer_Demux_GetHandle(&handleDemux);
        
        error = DtvPlayer_Pipeline_ConnectInput(handleTuner, handleDemux);
        if(error != ePLAYER_ERROR_OK)
        {
            LOGE("[%s:%d] DtvPlayer_Pipeline_ConnectInput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
            return;
        } else
        {
            LOGI("[%s:%d] DtvPlayer_Pipeline_ConnectInput success.", __FUNCTION__, __LINE__);
        }
        gAreInputsConnected = 1;
    }
    error = DtvPlayer_Demux_StartAVFiltering(pidVideo, pidAudio, &gChannelIdVideo, &gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Demux_StartAVFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Demux_StartAVFiltering success.", __FUNCTION__, __LINE__);
    }

    error = DtvPlayer_Pipeline_ConnectOutput(gChannelIdVideo, gChannelIdAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Pipeline_ConnectOutput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Pipeline_ConnectOutput success.", __FUNCTION__, __LINE__);
    }

    error = DtvPlayer_AV_Start(codecVideo, codecAudio);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_AV_Start failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return;
    } else
    {
        LOGI("[%s:%d] DtvPlayer_AV_Start success.", __FUNCTION__, __LINE__);
    }
    //////////////////////////////////////////////////
}

/**
 * @brief Use this function to directly start channel from channel list
 * 
 * @param channelNumber channel index from channel list
 */
void DtvPlayer_StartChannel(int channelNumber)
{
    // TODO Implement Exercise 1
    // NOTE: Add print for function enter
    
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 7
    // NOTE: implement channel list saving mechanism
    // NOTE: Use previously used functions to setup data path and achieve playback
    
    // IMPLEMENTATION
    if(gChannelList.channelCount == 0)
    {
        DtvPlayer_Comm_Print("Channel list is empty!");
        return;
    }
    if(gChannelList.channelCount <= channelNumber)
    {
        DtvPlayer_Comm_Print("Invalid channel number, Channel list has %d channels.", gChannelList.channelCount);
        return;
    }
    if(gChannelList.channel[channelNumber].isScrambled)
    {
        DtvPlayer_Comm_Print("Channel is scrambled, cannot start.", gChannelList.channelCount);
        return;
    }
    if(gIsAvStarted)
    {
        DtvPlayer_Stop();
        usleep(1e6); // add 1s wait time for decoders to stop
    }
    DtvPlayer_Tune(gChannelList.channel[channelNumber].freq);
    DtvPlayer_Start(gChannelList.channel[channelNumber].pidVideo, gChannelList.channel[channelNumber].pidAudio[0],
                    gChannelList.channel[channelNumber].codecVideo, gChannelList.channel[channelNumber].codecAudio[0]);
    // TODO Implement Exercise 9
    // NOTE: Add Checks for scrambled channel

    //////////////////////////////////////////////////
}

/**
 * @brief Use this function to stop channel playback.
 * Channel playback has to be active for this function to work.
 * 
 */
void DtvPlayer_Stop()
{
    // TODO Implement Exercise 1
    // NOTE: Add print for function enter
    
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 6
    // NOTE: Check if playback is started
    // NOTE: Call stop decoders realized in AV module
    // NOTE: Stop AV data filtering

    // IMPLEMENTATION
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
    //////////////////////////////////////////////////
}

/**
 * @brief Use this function to print out channel list
 * 
 */
void DtvPlayer_List()
{
    char output[1024];
    char tmp_out[1024];
    int i, j;
    memset(output, 0, 1024);
    memset(tmp_out, 0, 1024);

    if(gChannelList.channelCount == 0)
    {
        DtvPlayer_Comm_Print("No channels.");
        return;
    }

    for( i = 0 ; i < gChannelList.channelCount; i++)
    {
        strcpy(tmp_out, output);
        sprintf(output, "\n%s%d: VPID[%d] VCODEC[%d];",tmp_out, i, gChannelList.channel[i].pidVideo, gChannelList.channel[i].codecVideo);
        for(j = 0; j < gChannelList.channel[i].audioTrackCount; j++)
        {
            strcpy(tmp_out, output);
            sprintf(output, "%s APID[%d] ACODEC[%d];",tmp_out, gChannelList.channel[i].pidAudio[j], gChannelList.channel[i].codecAudio[j]);
        }
        strcpy(tmp_out, output);
        sprintf(output, "%s\n", tmp_out);
    }
    DtvPlayer_Comm_Print("%s", output);
}

/**
 * @brief Use this function to exit application
 * 
 */
void DtvPlayer_Exit()
{
    DtvPlayer_Comm_Deinit();
    exit(0);
}

/**
 * @brief Use this function to receive channel data from demux module
 * 
 * @param event 
 * @param channel 
 */
static void DtvPlayer_ChannelCbFn(tChannelEventType event, tChannelItem* channel)
{
    // IMPLEMENTATION

    //////////////////////////////////////////////////

    // TODO Implement Exercise 7
    // NOTE: Implement message handling
    // NOTE: Save channel received in gChannelList list.

    // IMPLEMENTATION
    if(event == eDTVPLAYER_CHANNEL_EVENT_NEW)
    {
        channel->freq = gCurrentFrequency;

        int i;
        for( i = 0 ; i < gChannelList.channelCount; i++)
        {
            if(memcmp(channel, &gChannelList.channel[i], sizeof(tChannelItem)) == 0 )
            {
                LOGI("[%s:%d] channel already included in list.", __FUNCTION__, __LINE__);
                return;
            }
        }
        memcpy(&gChannelList.channel[gChannelList.channelCount],channel, sizeof(tChannelItem));
        gChannelList.channelCount++;
    } else if(event == eDTVPLAYER_CHANNEL_EVENT_DONE){
        DtvPlayer_Comm_Print("Channels found: %d ", gChannelList.channelCount);

        FILE *outfile = fopen(CHANNEL_LIST_PATH,"wb");  // w for write, b for binary
        fwrite(&gChannelList, sizeof(gChannelList), 1, outfile);
        fclose(outfile);
    }
    //////////////////////////////////////////////////
}

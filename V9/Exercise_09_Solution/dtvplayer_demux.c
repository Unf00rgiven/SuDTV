#include <pthread.h>
#include <stdio.h>
#include <string.h> 
#include <unistd.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_disp.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_ts.h"

#include "dtvplayer_utils.h"
#include "dtvplayer_parser_pat.h"
#include "dtvplayer_parser_pmt.h"
#include "dtvplayer_demux.h"

// Filter depth for table parsing
#define TABLE_FILTER_DEPTH  4

// Maximal programs inside stream
#define MAX_PROGRAM_COUNT 10

// Demux PSI table data callback fn declaration
static void DtvPlayer_Demux_DataCallbackFct ( tTDAL_DMX_ChannelId ChannelId,    /* channel ID */
                                              tTDAL_DMX_FilterId  FilterId,     /* filter ID */
                                              uint32_t            Filter_track, /* track */
                                              uint32_t            Length,       /* length of the buffer */
                                              uint8_t             *Buffer);


// PAT table container
static tPATTable PATTable;
// PMT table containers
static tPMTTable PMTTable[MAX_PROGRAM_COUNT];
// PMT table acquisition status
static uint8_t PMTTableAcquire[MAX_PROGRAM_COUNT];

// PMT table requests containers
static tDmxRequest gPmtDmxRequest[MAX_PROGRAM_COUNT];
// PAT table request container
static tDmxRequest gPatDmxRequest;

// PMT tables with value < 100 should not be parsed. Use this flag to avoud requests
static uint8_t gStreamContainsPrivatePMT = 0;

// PSI table callback function
static DtvPlayer_Demux_PsiCallbackFn_t gChannelEventCallback;

// Channel fount count
static int gChannelCount = 0;

/**
 * @brief THis function is used to initialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Init()
{
    // Init TDAL DMX module
    // TODO Implement Exercise 3
    // NOTE: Call init in DMX module

    // IMPLEMENTATION
    tTDAL_DMX_Error error;
    error = TDAL_DMX_Init();
    if(error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to retrieve demux handle, usually needed for coonecting data paths
 * 
 * @param dmxHandle demux handle is returned trough this pointer
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_GetHandle(tTDAL_TS_Object* dmxHandle)
{
    tTDAL_TSROUTE_Error tsr_error = eTDAL_TSROUTE_NO_ERROR;
    *dmxHandle = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_DESTINATION, eTDAL_TS_TYPE_DEMUX, eTDAL_DMX_DEMUX0);

    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start AV elementary streams for playback
 * 
 * @param videoPid video pid for filtering
 * @param audioPid audio pid for filtering
 * @param videoChannelId opened audio channel in demux
 * @param audioChannelId opened video channel in demux
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_StartAVFiltering(uint16_t videoPid, uint16_t audioPid,
        tTDAL_DMX_ChannelId* videoChannelId, tTDAL_DMX_ChannelId* audioChannelId)
{
    tTDAL_DMX_Error dmx_error = kTDAL_DMX_NO_ERROR;

    // TODO Implement Exercise 6
    // NOTE: Allocate channel for section video/audio streams and decoder output
    // NOTE: set channel PID
    // NOTE: Enable channel

    // IMPLEMENTATION
    tTDAL_DMX_ChannelType_t videoChannelType;
    videoChannelType.eChannelStream = eTDAL_DMX_VIDEO_STREAM;
    videoChannelType.eChannelOutput = eTDAL_DMX_DECODER;
    videoChannelType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    videoChannelType.eChannelStreamType = eTDAL_DMX_STREAM_TYPE_UNKOWN;
    
    dmx_error = TDAL_DMX_Allocate_Channel(eTDAL_DMX_DEMUX0, &videoChannelType, videoChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Allocate_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Set_Channel_PID(*videoChannelId, videoPid);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Set_Channel_PID failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Channel(*videoChannelId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    tTDAL_DMX_ChannelType_t audioChannelType;
    audioChannelType.eChannelStream = eTDAL_DMX_AUDIO_STREAM;
    audioChannelType.eChannelOutput = eTDAL_DMX_DECODER;
    audioChannelType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    audioChannelType.eChannelStreamType = eTDAL_DMX_STREAM_TYPE_UNKOWN;

    dmx_error = TDAL_DMX_Allocate_Channel(eTDAL_DMX_DEMUX0, &audioChannelType, audioChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Allocate_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Set_Channel_PID(*audioChannelId, audioPid);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Set_Channel_PID failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Channel(*audioChannelId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to stop AV elemntary stream filtering
 * 
 * @param videoChannelId previously allocated and opened video channel on demux
 * @param audioChannelId previously allocated and opened audio channel on demux
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_StopAVFiltering(tTDAL_DMX_ChannelId* videoChannelId, tTDAL_DMX_ChannelId* audioChannelId)
{
    tTDAL_DMX_Error dmx_error = kTDAL_DMX_NO_ERROR;

    // TODO Implement Exercise 6
    // NOTE: disable allocated channels
    // NOTE: free allocated channels

    // IMPLEMENTATION
    dmx_error = TDAL_DMX_Control_Channel(*audioChannelId, eTDAL_DMX_CTRL_DISABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Channel(*videoChannelId, eTDAL_DMX_CTRL_DISABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Free_Channel(*audioChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Free_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Free_Channel(*videoChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Free_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief THis function is used to deinitialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Deinit()
{
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start filtering PSI tables
 * 
 * @param request request with table pid, table id, channel and filter values
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_StartPsiFiltering(tDmxRequest* request)
{
    tTDAL_DMX_Error       dmx_error = kTDAL_DMX_NO_ERROR;
    uint8_t               size = 1; // it was 0 here previously
    uint8_t               filter;
    uint8_t               mask;
    uint8_t               mode;
    uint32_t              filter_track;;
    // TODO Implement Exercise 3
    // NOTE: Allocate channel for section streams and memory output
    // NOTE: Allocate filter on opened channel
    // NOTE: register data callback
    // NOTE: set channel PID
    // NOTE: set filter with folowing masking mode: filter=TID, Mask = 0xFF, Mode=0xFF
    // NOTE: Enable channel and filter

    // IMPLEMENTATION
    tTDAL_DMX_ChannelType_t   stType;
    stType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    stType.eChannelOutput = eTDAL_DMX_MEMORY;
    stType.eChannelStream = eTDAL_DMX_SECTION_STREAM;

    dmx_error = TDAL_DMX_Allocate_Channel(eTDAL_DMX_DEMUX0, &stType, &request->channelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Allocate_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Register_Channel_Callback(request->channelId, DtvPlayer_Demux_DataCallbackFct);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Register_Channel_Callback failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Set_Channel_PID(request->channelId, request->pid);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Set_Channel_PID failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Allocate_Filter(request->channelId, &request->filterId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Allocate_Filter failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    filter_track = request->tableId;
    filter = filter_track;
    mask   = 0xFF;
    mode   = 0xFF;

    dmx_error = TDAL_DMX_Set_Filter(request->channelId,
                        request->filterId,
                        filter_track,
                        size,
                        &filter,
                        &mask,
                        &mode,
                        eTDAL_DMX_CRC_IGNORE);

    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Set_Filter failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Channel(request->channelId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Filter(request->filterId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Filter failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }
    LOGE("[%s:%d] ALL OK failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to stop and release resources from previously started PSI parsing.
 * 
 * @param request previously started request
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_StopPsiFiltering(tDmxRequest* request)
{
    tTDAL_DMX_Error dmx_error = kTDAL_DMX_NO_ERROR;

    // TODO Implement Exercise 3
    // NOTE: disable filter and channel
    // NOTE: free filter
    // NOTE: free channel

    // IMPLEMENTATION
    dmx_error = TDAL_DMX_Control_Filter(request->filterId, eTDAL_DMX_CTRL_DISABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Filter failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Control_Channel(request->channelId, eTDAL_DMX_CTRL_DISABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Free_Filter(request->filterId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Free_Filter failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    dmx_error = TDAL_DMX_Free_Channel(request->channelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Free_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return ePLAYER_ERROR_GENERAL;
    }

    memset(request, -1, sizeof(tDmxRequest));
    //////////////////////////////////////////////////
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start scanning for PSI tables.
 * Initially this function has to create PAT request to be able to recive PMT data.
 * Once PAT table is received it has to be parsed and request PMT tables
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_ScanPsi()
{
    memset(&gPatDmxRequest, -1, sizeof(tDmxRequest));

    gPatDmxRequest.pid = 0;
    gPatDmxRequest.tableId = TID_PAT;

    DtvPlayer_Demux_StartPsiFiltering(&gPatDmxRequest);
    return ePLAYER_ERROR_OK;
}

/**
 * @brief THis function is used to register callback function for channel parsing notifications
 * 
 * @param cb callback function pointer
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_RegisterPsiCallback(DtvPlayer_Demux_PsiCallbackFn_t cb)
{
    gChannelEventCallback = cb;
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This is demux callback function which is called by TDAL DMX module with table section data
 * 
 * @param ChannelId channel id for which request was created
 * @param FilterId filter id for which request was created
 * @param Filter_track table id for which request was created
 * @param Length buffer length
 * @param Buffer data
 */
static void DtvPlayer_Demux_DataCallbackFct ( tTDAL_DMX_ChannelId    ChannelId,
                                              tTDAL_DMX_FilterId      FilterId,
                                              uint32_t                Filter_track,
                                              uint32_t                Length,
                                              uint8_t                 *Buffer)
{
    // TODO Implement Exercise 3
    // Add print so we can know that we received DMX data callback
    //LOGD("[%s:%d] callback called.", __FUNCTION__, __LINE__);
    LOGD("[%s:%d] ChannelId [%d], Filter_track [%d], length [%d].",
                __FUNCTION__, __LINE__, ChannelId, Filter_track, Length);

    // TODO Implement Exercise 4
    // NOTE: Parse PAT table
    // NOTE: Create PMT requests
    // NOTE: Parse PMT requests
    // NOTE: Free data from Buffer
    // TODO Implement Exercise 9
    // NOTE: Implement scrambled channel detection

    // IMPLEMENTATION
    switch (Filter_track)
    {
        case TID_PAT: // PAT table found
        {
            int i;
            tParserStatus parser_error;
            tErrorCode player_error;
            tTDAL_DMX_Error dmx_error;

            // Parse PAT table
            parser_error = DtvPlayer_ParserPATParse(Buffer, &PATTable);

            // Free buffer allocated by DMX
            dmx_error = TDAL_DMX_Free_Buffer(Buffer);
            if(dmx_error != kTDAL_DMX_NO_ERROR)
            {
                LOGE("[%s:%d] TDAL_DMX_Free_Buffer failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
                break;
            }

            // Check parser status
            if(parser_error != PARSER_SUCCESS)
            {
                LOGE("[%s:%d] DtvPlayer_ParserPATParse failed. Error: [%d]", __FUNCTION__, __LINE__, parser_error);
                break;
            }

            // Stop filtering for PAT tables, we don't need them anymore
            player_error = DtvPlayer_Demux_StopPsiFiltering(&gPatDmxRequest);
            if(player_error != ePLAYER_ERROR_OK)
            {
                LOGE("[%s:%d] DtvPlayer_Demux_StopPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
                break;
            }

            // Print out PAT table to logcat
            DtvPlayer_ParserPATDump(&PATTable);

            // Create 
            int nit_found = 0;
            for(i = 0 ; i < PATTable.program_size; i++)
            {
                int pmtindex = i - nit_found;
                // Skip NIT Table
                if(PATTable.programs[i].program_map_pid == 16)
                {
                    nit_found = 1;
                    continue;
                }
                // memset(&gPmtDmxRequest[pmtindex], -1, sizeof(tDmxRequest)); clean requests, done in init

                // form PMT request with PMT pid
                gPmtDmxRequest[pmtindex].pid = PATTable.programs[i].program_map_pid;
                gPmtDmxRequest[pmtindex].tableId = TID_PMT;

                // Request filtering for new PMT table
                player_error = DtvPlayer_Demux_StartPsiFiltering(&gPmtDmxRequest[pmtindex]);
                if(player_error != ePLAYER_ERROR_OK)
                {
                    LOGE("[%s:%d] DtvPlayer_Demux_StartPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
                    break;
                } else
                {
                    LOGD("[%s:%d] Created request fo Program map pid: [%d]", __FUNCTION__, __LINE__, PATTable.programs[i].program_map_pid);
                }
                
            }
            break;
        }
        case TID_PMT: // PMT table found
        {
            LOGD("[%s:%d] PMT request arrived", __FUNCTION__, __LINE__);
            int patIndex;
            tErrorCode player_error;
            tTDAL_DMX_Error dmx_error;
            tParserStatus parser_error;

            // Find which (index of) gPmtDmxRequest was used for this PMT request
            for(patIndex = 0; patIndex < MAX_PROGRAM_COUNT; patIndex++){
                if(ChannelId == gPmtDmxRequest[patIndex].channelId && FilterId == gPmtDmxRequest[patIndex].filterId ){
                    break;
                }
            }
            ///////////////////////////////////////////////////////////////////////
            // TODO: Parse and dump PMT
            ///////////////////////////////////////////////////////////////////////
            parser_error = DtvPlayer_ParserPMTParse(Buffer, &PMTTable);

            // Free buffer allocated by DMX
            dmx_error = TDAL_DMX_Free_Buffer(Buffer);
            if(dmx_error != kTDAL_DMX_NO_ERROR)
            {
                LOGE("[%s:%d] TDAL_DMX_Free_Buffer failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
                break;
            }

            // Check if request was found. If not we cannot stop filtering with invalid value.
            if(patIndex >= MAX_PROGRAM_COUNT)
            {
                LOGE("[%s:%d] request not found, something went wrong!!!", __FUNCTION__, __LINE__);
                break;
            }

            // Stop filtering for this PMT table, we don't need to receive duplicates
            player_error = DtvPlayer_Demux_StopPsiFiltering(&gPmtDmxRequest[patIndex]);

            DtvPlayer_ParserPMTDump(&PMTTable);

            // cleanup request
            memset(&gPmtDmxRequest[patIndex], -1, sizeof(tDmxRequest));

            if(player_error != ePLAYER_ERROR_OK)
            {
                LOGE("[%s:%d] DtvPlayer_Demux_StopPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
                break;
            }

            // Get data needed for playback
            LOGD("PMT Found: 0x%x", PATTable.programs[patIndex].program_map_pid);
            tChannelItem channel;

            memset(&channel, 0, sizeof(tChannelItem));

            int z;
            channel.isScrambled = 0;
            for( z = 0; z < PMTTable[patIndex].pi_descriptor_number; z++ )
            {
                if(PMTTable[patIndex].pi_descriptors[z].tag == DESC_TAG_CA)
                {
                    if(PMTTable[patIndex].pi_descriptors[z].ca_descriptor.ca_system_id == 0xD /* Reserved field */)
                    {
                        continue;
                    }
                    channel.isScrambled = 1;
                    break;
                }
            }

            for(int j = 0; j < PMTTable[patIndex].streams_number; j++)
            {
                int streamType = PMTTable[patIndex].streams[j].stream_type;
                int pid = PMTTable[patIndex].streams[j].elementary_pid;

                switch (streamType)
                {
                case STREAM_TYPE_ITU_T_H262_ISO_IEC_13818_2_MPEG_2_VIDEO:
                    channel.codecVideo = eTDAL_AV_VIDEO_TYPE_MPEG2;
                    channel.pidVideo = pid;
                    break;
                case STREAM_TYPE_ISO_IEC_11172_MPEG_1_VIDEO:
                    channel.codecVideo = eTDAL_AV_VIDEO_TYPE_MPEG1;
                    channel.pidVideo = pid;
                    break;
                case STREAM_TYPE_ISO_IEC_14496_10_H264:
                    channel.codecVideo = eTDAL_AV_VIDEO_TYPE_H264;
                    channel.pidVideo = pid;
                    break;
                case STREAM_TYPE_ISO_IEC_11172_MPEG_1_AUDIO:
                    channel.codecAudio[channel.audioTrackCount] = eTDAL_AV_AUDIO_TYPE_MPEG;
                    channel.pidAudio[channel.audioTrackCount] = pid;
                    channel.audioTrackCount++;
                    break;
                case STREAM_TYPE_ISO_IEC_13818_7_AAC:
                    channel.codecAudio[channel.audioTrackCount] = eTDAL_AV_AUDIO_TYPE_MPEG_AAC;
                    channel.pidAudio[channel.audioTrackCount] = pid;
                    channel.audioTrackCount++;
                    break;
                default:
                    break;
                }
            }
            PMTTableAcquire[patIndex] = TRUE;
            gChannelCount ++;

            gChannelEventCallback(eDTVPLAYER_CHANNEL_EVENT_NEW, &channel);
            if(gChannelCount == PATTable.program_size)
            {
                gChannelEventCallback(eDTVPLAYER_CHANNEL_EVENT_DONE, NULL);
            }
            break;
        }
        default:
            break;
    }
    //////////////////////////////////////////////////

    // TODO Implement Exercise 7
    // NOTE: notify channel parsing callback

    // IMPLEMENTATION

    //////////////////////////////////////////////////
}

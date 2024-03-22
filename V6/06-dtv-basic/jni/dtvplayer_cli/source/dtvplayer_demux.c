#include <stdio.h>
#include <string.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_dmx.h"

#include "dtvplayer_utils.h"
#include "dtvplayer_demux.h"

#include "dtvplayer_parser_pat.h"
#include "dtvplayer_parser_pmt.h"


static const tTDAL_DMX_DemuxId DEMUX_ID = eTDAL_DMX_DEMUX0;

// Maximal programs inside stream
#define MAX_PROGRAM_COUNT 10

static tErrorCode DtvPlayer_Demux_StartPsiFiltering(tDmxRequest* request);
static tErrorCode DtvPlayer_Demux_StopPsiFiltering(tDmxRequest* request);

// Demux PSI table data callback fn declaration
static void DtvPlayer_Demux_DataCallbackFct ( tTDAL_DMX_ChannelId ChannelId,    /* channel ID */
                                              tTDAL_DMX_FilterId  FilterId,     /* filter ID */
                                              uint32_t            Filter_track, /* track */
                                              uint32_t            Length,       /* length of the buffer */
                                              uint8_t             *Buffer);


// PMT table requests containers
static tDmxRequest gPmtDmxRequest[MAX_PROGRAM_COUNT];
// PAT table request container
static tDmxRequest gPatDmxRequest;

/**
 * @brief THis function is used to initialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Init()
{
    tTDAL_DMX_Error error;

    error = TDAL_DMX_Init();
    if(error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }

    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start scanning for PSI tables.
 * Initially this function has to create PAT request to be able to recive PMT data.
 * Once PAT table is received it has to be parsed and request PMT tables
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_AnalyzePsi()
{
    tErrorCode error;
    // Initialize PAT request
    gPatDmxRequest.pid = 0;
    gPatDmxRequest.tableId = TID_PAT;

    error = DtvPlayer_Demux_StartPsiFiltering(&gPatDmxRequest);
    if(error != ePLAYER_ERROR_OK)
    {
        printf("Cannot start analyzing PSI stream");
        LOGE("[%s:%d] DtvPlayer_Demux_StartPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return ePLAYER_ERROR_GENERAL;
    }

    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to deinitialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Deinit()
{
    if(gPatDmxRequest.pid != -1) {
        DtvPlayer_Demux_StopPsiFiltering(&gPatDmxRequest);
    }
    TDAL_DMX_Terminate();
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to start filtering PSI tables
 * 
 * @param request request with table pid, table id, channel and filter values
 * @return tErrorCode 
 */
static tErrorCode DtvPlayer_Demux_StartPsiFiltering(tDmxRequest* request)
{
    tTDAL_DMX_Error       dmx_error = kTDAL_DMX_NO_ERROR;
 
    uint8_t                 size = 1;
    uint8_t                 filter = request->tableId;
    uint8_t                 mask = 0xFF;
    uint8_t                 mode = 0xFF;
    uint32_t                filter_track = request->tableId;
    tTDAL_DMX_FilterCRCMode crcMode = eTDAL_DMX_CRC_IGNORE;

    tTDAL_DMX_ChannelType_t stType;
    stType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    stType.eChannelOutput = eTDAL_DMX_MEMORY;
    stType.eChannelStream = eTDAL_DMX_SECTION_STREAM;

    dmx_error = TDAL_DMX_Allocate_Channel(DEMUX_ID, &stType, &request->channelId);
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

    dmx_error = TDAL_DMX_Set_Filter(request->channelId,
                        request->filterId,
                        filter_track,
                        size,
                        &filter,
                        &mask,
                        &mode,
                        crcMode);

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
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to stop and release resources from previously started PSI parsing.
 * 
 * @param request previously started request
 * @return tErrorCode 
 */
static tErrorCode DtvPlayer_Demux_StopPsiFiltering(tDmxRequest* request)
{
    tTDAL_DMX_Error dmx_error = kTDAL_DMX_NO_ERROR;

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
    LOGD("[%s:%d] callback called.", __FUNCTION__, __LINE__);
    LOGD("[%s:%d] ChannelId [%d], Filter_track [%d], length [%d].",
                __FUNCTION__, __LINE__, ChannelId, Filter_track, Length);
    
    if(Filter_track == TID_PAT)
    {
        tParserStatus parser_error;
        tErrorCode player_error;
        tTDAL_DMX_Error dmx_error;
        tPATTable table;
        printf("\nPAT Table found!\n");
        
        // Stop filtering for PAT tables, we don't need them anymore
        player_error = DtvPlayer_Demux_StopPsiFiltering(&gPatDmxRequest);
        if(player_error != ePLAYER_ERROR_OK)
        {
            LOGE("[%s:%d] DtvPlayer_Demux_StopPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
        }

        parser_error = DtvPlayer_ParserPATParse(Buffer, &table);
        // Check parser status
        if(parser_error != PARSER_SUCCESS) {
            LOGE("[%s:%d] DtvPlayer_ParserPATParse failed. Error: [%d]", __FUNCTION__, __LINE__, parser_error);
        } else {
            // DtvPlayer_ParserPATDump(&table);
        }

        dmx_error = TDAL_DMX_Free_Buffer(Buffer);
        if(dmx_error != kTDAL_DMX_NO_ERROR)
        {
            LOGE("[%s:%d] TDAL_DMX_Free_Buffer failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        }
        
        int nit_found = 0;
        int i;
        for(i = 0 ; i < table.program_size; i++)
        {
            int pmtindex = i - nit_found;
            // Skip NIT Table
            if(table.programs[i].program_map_pid == 16)
            {
                nit_found = 1;
                continue;
            }
            // memset(&gPmtDmxRequest[pmtindex], -1, sizeof(tDmxRequest)); clean requests, done in init

            // form PMT request with PMT pid
            gPmtDmxRequest[pmtindex].pid = table.programs[i].program_map_pid;
            gPmtDmxRequest[pmtindex].tableId = TID_PMT;

            // Request filtering for new PMT table
            player_error = DtvPlayer_Demux_StartPsiFiltering(&gPmtDmxRequest[pmtindex]);
            if(player_error != ePLAYER_ERROR_OK)
            {
                LOGE("[%s:%d] DtvPlayer_Demux_StartPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
                break;
            } else
            {
                LOGD("[%s:%d] Created request fo Program map pid: [%d]", __FUNCTION__, __LINE__, table.programs[i].program_map_pid);
            }  
        }
    } else if(Filter_track == TID_PMT) {
        printf("\nPMT Table found!\n");

        int i;
        tErrorCode player_error;
        tTDAL_DMX_Error dmx_error;
        tParserStatus parser_error;
        tPMTTable table;

        // Find which (index of) gPmtDmxRequest was used for this PMT request
        for(i = 0; i < MAX_PROGRAM_COUNT; i++){
            if(ChannelId == gPmtDmxRequest[i].channelId && FilterId == gPmtDmxRequest[i].filterId )
            {
                player_error = DtvPlayer_Demux_StopPsiFiltering(&gPmtDmxRequest[i]);
                if(player_error != ePLAYER_ERROR_OK)
                {
                    LOGE("[%s:%d] DtvPlayer_Demux_StopPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
                }
                // cleanup request
                memset(&gPmtDmxRequest[i], -1, sizeof(tDmxRequest));
                break;
            }
        }
        
        parser_error = DtvPlayer_ParserPMTParse(Buffer, &table);
        // Check parser status
        if(parser_error != PARSER_SUCCESS) {
            LOGE("[%s:%d] DtvPlayer_ParserPATParse failed. Error: [%d]", __FUNCTION__, __LINE__, parser_error);
        } else {
            // DtvPlayer_ParserPMTDump(&table);
        }

        // Free buffer allocated by DMX
        dmx_error = TDAL_DMX_Free_Buffer(Buffer);
        if(dmx_error != kTDAL_DMX_NO_ERROR)
        {
            LOGE("[%s:%d] TDAL_DMX_Free_Buffer failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        }

        printf("Channel found:\n");
        for(int j = 0; j < table.streams_number; j++)
        {
            int streamType = table.streams[j].stream_type;
            int pid = table.streams[j].elementary_pid;

            switch (streamType)
            {
            case STREAM_TYPE_ITU_T_H262_ISO_IEC_13818_2_MPEG_2_VIDEO:
                printf("VPID = %d Codec: MPEG2 \n", pid);
                break;
            case STREAM_TYPE_ISO_IEC_11172_MPEG_1_VIDEO:
                printf("VPID = %d Codec: MPEG1 \n", pid);
                break;
            case STREAM_TYPE_ISO_IEC_14496_10_H264:
                printf("VPID = %d Codec: H264  \n", pid);
                break;
            case STREAM_TYPE_ISO_IEC_11172_MPEG_1_AUDIO:
                printf("APID = %d Codec: MPEG \n", pid);
                break;
            case STREAM_TYPE_ISO_IEC_13818_7_AAC:
                printf("APID = %d Codec: AAC \n", pid);
                break;
            default:
                break;
            }
        }
    }
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
    tTDAL_DMX_ChannelType_t videoChannelType;
    tTDAL_DMX_ChannelType_t audioChannelType;

    videoChannelType.eChannelStream = eTDAL_DMX_VIDEO_STREAM;
    videoChannelType.eChannelOutput = eTDAL_DMX_DECODER;
    videoChannelType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    videoChannelType.eChannelStreamType = eTDAL_DMX_STREAM_TYPE_UNKOWN;

    audioChannelType.eChannelStream = eTDAL_DMX_AUDIO_STREAM;
    audioChannelType.eChannelOutput = eTDAL_DMX_DECODER;
    audioChannelType.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
    audioChannelType.eChannelStreamType = eTDAL_DMX_STREAM_TYPE_UNKOWN;

    // DONE Implement AV data filtering
    //video
    dmx_error = TDAL_DMX_Allocate_Channel(DEMUX_ID, &videoChannelType, videoChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR) {
        LOGE("[%s:%d] TDAL_DMX_Allocate_channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }
    
    dmx_error = TDAL_DMX_Set_Channel_PID(*videoChannelId, videoPid);
    if(dmx_error != kTDAL_DMX_NO_ERROR) {
        LOGE("[%s:%d] TDAL_DMX_Set_Channel_PID failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }

    dmx_error = TDAL_DMX_Control_Channel(*videoChannelId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }

    //audio
    dmx_error = TDAL_DMX_Allocate_Channel(DEMUX_ID, &audioChannelType, audioChannelId);
    if(dmx_error != kTDAL_DMX_NO_ERROR) {
        LOGE("[%s:%d] TDAL_DMX_Allocate_channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }
    
    dmx_error = TDAL_DMX_Set_Channel_PID(*audioChannelId, audioPid);
    if(dmx_error != kTDAL_DMX_NO_ERROR) {
        LOGE("[%s:%d] TDAL_DMX_Set_Channel_PID failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }

    dmx_error = TDAL_DMX_Control_Channel(*audioChannelId, eTDAL_DMX_CTRL_ENABLE);
    if(dmx_error != kTDAL_DMX_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMX_Control_Channel failed. Error: [%d]", __FUNCTION__, __LINE__, dmx_error);
        return -1;
    }
    
    // NOTE 1: Allocate channel for section video stream
    // NOTE 2: set channel PID for video
    //TDAL_DMX_Allocate_Filter()
    //TDAL_DMX_Set_Filter()
    // NOTE 3: Start Video Channel
    // NOTE 4: Allocate channel for section audio stream
    // NOTE 5: set channel PID for audio
    // NOTE 6: Start Audio Channel
    // NOTE 7: Do we need to create filter in this case?
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

    // TODO Implement mechanism to stop AV filtering
    // NOTE 1: Disable allocated channels (for video and audio)
    // NOTE 2: free allocated channels (for video and audio)

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
    return ePLAYER_ERROR_OK;
}

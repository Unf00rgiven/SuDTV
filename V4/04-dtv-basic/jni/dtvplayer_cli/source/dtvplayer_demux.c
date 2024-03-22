#include <stdio.h>
#include <string.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_dmx.h"

#include "dtvplayer_utils.h"
#include "dtvplayer_demux.h"

#include "dtvplayer_parser_pat.h"


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
        tTDAL_DMX_Error dmx_error;
        tErrorCode player_error;
        tPATTable table;
        tParserStatus parser_error;
        printf("\nPAT Table found!\n");
        // Stop filtering for PAT tables, we don't need them anymore
        player_error = DtvPlayer_Demux_StopPsiFiltering(&gPatDmxRequest);
        if(player_error != ePLAYER_ERROR_OK)
        {
            LOGE("[%s:%d] DtvPlayer_Demux_StopPsiFiltering failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
        }

        // TODO: Parse PAT table and print result
        parser_error = DtvPlayer_ParserPATParse(Buffer, &table);
        if(parser_error != PARSER_SUCCESS) {
            LOGE("[%s:%d] DtvPlayer_ParserPATParse failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
        }
        // NOTE 1: Find DtvPlayer_ParserPATParse function in API and provide appropriate params
        DtvPlayer_ParserPATDump(&table);
        // NOTE 2: Find DtvPlayer_ParserPATDump function in API and provide appropriate params
        dmx_error = TDAL_DMX_Free_Buffer(Buffer);
        if(dmx_error != kTDAL_DMX_NO_ERROR) {
            LOGE("[%s:%d] DtvPlayer_Demux_TDAL_DMX_Free_Buffer failed. Error: [%d]", __FUNCTION__, __LINE__, player_error);
            //return -1;
        }
        // NOTE 3: Find TDAL_DMX_Free_Buffer function in API and provide appropriate params
        
        // Use TABLE to get PMT request
    } else if(Filter_track == TID_PMT) {
        printf("\nPMT Table found!\n");
        // Will be implemented
    }
}

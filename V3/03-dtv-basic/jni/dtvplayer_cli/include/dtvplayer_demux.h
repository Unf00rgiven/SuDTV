#ifndef __DTV_PLAYER_DEMUX_H__
#define __DTV_PLAYER_DEMUX_H__

#include "tdal_dmx.h"

typedef enum {
    TID_PAT     = 0x00,
    TID_CAT     = 0x01,
    TID_PMT     = 0x02,
    TID_BAT     = 0x4a,
    TID_NIT_ACT = 0x40,
    TID_NIT_OTH = 0x41,
    TID_SDT_ACT = 0x42,
    TID_SDT_OTH = 0x46,
    TID_EIT_MIN = 0x4e,
    TID_EIT_MAX = 0x6f,
    TID_ECM     = 0x82
} tTableId;

typedef struct _DmxRequest {
    int pid;
    tTableId tableId;
    tTDAL_DMX_ChannelId channelId;
    tTDAL_DMX_FilterId filterId;
} tDmxRequest;

/**
 * @brief THis function is used to initialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Init();

/**
 * @brief THis function is used to deinitialize demux module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_Deinit();

/**
 * @brief This function is used to start scanning for PSI tables.
 * Initially this function has to create PAT request to be able to recive PMT data.
 * Once PAT table is received it has to be parsed and request PMT tables
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Demux_AnalyzePsi();

#endif // __DTV_PLAYER_DEMUX_H__

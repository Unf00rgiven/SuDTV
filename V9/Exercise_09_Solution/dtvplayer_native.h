#ifndef __DTV_PLAYER_NATIVE_H__
#define __DTV_PLAYER_NATIVE_H__

#include "tdal_av.h"
#include "dtvplayer_utils.h"

typedef enum _channelEvent{
    eDTVPLAYER_CHANNEL_EVENT_NEW,
    eDTVPLAYER_CHANNEL_EVENT_DONE
} tChannelEventType;

typedef struct _channelItem {
    uint32_t freq;
    int pidVideo;
    tTDAL_AV_VideoType codecVideo;
    int audioTrackCount;
    int pidAudio [10];
    tTDAL_AV_AudioType codecAudio[10];
    uint8_t isScrambled;
} tChannelItem;

typedef struct _channelInfo {
    int channelCount;
    tChannelItem channel[10];
} tChannelList;

tErrorCode DtvPlayer_Init();
tErrorCode DtvPlayer_Deinit();
tErrorCode DtvPlayer_SetSurface(void* window);

void DtvPlayer_Tune(int);
void DtvPlayer_Scan();
void DtvPlayer_Untune();
void DtvPlayer_Start(int, int, int, int);
void DtvPlayer_StartChannel(int);
void DtvPlayer_Stop();
void DtvPlayer_List();
void DtvPlayer_Exit();

#endif // __DTV_PLAYER_NATIVE_H__

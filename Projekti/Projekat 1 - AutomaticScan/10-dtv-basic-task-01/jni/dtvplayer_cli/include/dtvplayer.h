#ifndef __DTV_PLAYER_NATIVE_H__
#define __DTV_PLAYER_NATIVE_H__

#include "dtvplayer_utils.h"
#include "tdal_av.h"

typedef struct _channelItem {
    uint32_t freq;
    int pidVideo;
    tTDAL_AV_VideoType codecVideo;
    int audioTrackCount;
    int pidAudio [10];
    tTDAL_AV_AudioType codecAudio[10];
} tChannel;

typedef struct _channelInfo {
    int channelCount;
    tChannel channel[10];
} tChannelList;

tErrorCode DtvPlayer_Init();
tErrorCode DtvPlayer_Deinit();

void DtvPlayer_Tune(int frequency);
void DtvPlayer_Untune();

void DtvPlayer_Analyze();

void DtvPlayer_Start(int pidVideo, int pidAudio, int codecVideo, int codecAudio);

void DtvPlayer_Stop();

void DtvPlayer_List();

void DtvPlayer_StartChannel(int channelNumber);

void DtvPlayer_Scan();

#endif // __DTV_PLAYER_NATIVE_H__

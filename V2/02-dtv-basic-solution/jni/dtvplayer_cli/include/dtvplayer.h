#ifndef __DTV_PLAYER_NATIVE_H__
#define __DTV_PLAYER_NATIVE_H__

#include "dtvplayer_utils.h"

tErrorCode DtvPlayer_Init();
tErrorCode DtvPlayer_Deinit();

void DtvPlayer_Tune();
void DtvPlayer_Untune();

#endif // __DTV_PLAYER_NATIVE_H__

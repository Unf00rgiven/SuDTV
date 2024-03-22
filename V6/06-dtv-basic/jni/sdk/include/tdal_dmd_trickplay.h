
#ifndef __TDAL_DMD_TRICKPLAY_H__
#define __TDAL_DMD_TRICKPLAY_H__

#include "crules.h"
#include "tdal_dmd.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef enum
{
    eTDAL_DMD_MEDIA_STATUS_BEGIN_OF_FILE,
    eTDAL_DMD_MEDIA_STATUS_END_OF_FILE,
    eTDAL_DMD_MEDIA_STATUS_CURRENT_TIME,
    eTDAL_DMD_MEDIA_STATUS_SEEK_COMPLETE
} TDAL_DMD_MEDIA_Status;


typedef void (*tTDAL_DMD_Media_CallbackFct) (TDAL_DMD_MEDIA_Status event, void *data);

tTDAL_DMD_Error TDAL_DMD_Trickplay_SetSpeed (tTDAL_DMD_FE FEID, int32_t speed, uint64_t pts);
tTDAL_DMD_Error TDAL_DMD_Trickplay_Seek (tTDAL_DMD_FE FEID, long long unsigned int positionSec);
tTDAL_DMD_Error TDAL_DMD_Trickplay_SetPlayackStartPosition(tTDAL_DMD_FE FEID, double positionSec);
tTDAL_DMD_Error TDAL_DMD_Trickplay_SetInitialBitrate(tTDAL_DMD_FE FEID, uint64_t bitrate);

tTDAL_DMD_Error TDAL_DMD_MEDIA_SubscribeStatusCallback(tTDAL_DMD_Media_CallbackFct callbackFct);
tTDAL_DMD_Error TDAL_DMD_MEDIA_UnsubscribeStatusCallback(tTDAL_DMD_Media_CallbackFct callbackFct);
#ifdef __cplusplus
}
#endif


#endif

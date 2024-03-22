#ifndef _TDAL_MUXER_H_
#define _TDAL_MUXER_H_
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "tdal_av.h"
/*****************************************************************************
 *  Defines                                                                  *
 *****************************************************************************/
#define kTDAL_MUXER_EMPTY_AUDIO_COMPONENT 0xFFFF

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum
{
    eTDAL_MUXER_NO_ERROR,
    eTDAL_MUXER_ERROR,
    eTDAL_MUXER_BAD_PARAMETER_ERROR,
    eTDAL_MUXER_FINISHED,
    eTDAL_MUXER_LIMITATION_ERROR
} tTDAL_MUXER_Error;

typedef enum
{
    // 6-STD-B21v5_4-E1.pdf, page 111/309: transition_flag
    eTDAL_MUXER_SELECTION_ONLY_CHANGE,
    eTDAL_MUXER_ORIGINAL_SOURCE_CHANGE,
    eTDAL_MUXER_NB
} tTDAL_MUXER_Discontinuity;

typedef struct TDAL_MUXER_ChannelConfig_t {
    uint32_t numberOfPids;
    uint32_t* pu32ChannelPids;
} TDAL_MUXER_ChannelConfig;

typedef struct TDAL_MUXER_AudioComponentData_t {
    uint32_t componentType;
} TDAL_MUXER_AudioComponentData;

typedef enum
{
    eTDAL_MUXER_AUDIO_COMPONENT_0 = 0,
    eTDAL_MUXER_AUDIO_COMPONENT_1,
    eTDAL_MUXER_AUDIO_COMPONENT_2,
    eTDAL_MUXER_AUDIO_COMPONENT_3,
    eTDAL_MUXER_AUDIO_COMPONENT_4,
    eTDAL_MUXER_AUDIO_COMPONENT_5,
    eTDAL_MUXER_AUDIO_COMPONENT_6,
    eTDAL_MUXER_AUDIO_COMPONENT_7,
    eTDAL_MUXER_AUDIO_COMPONENT_NB
} tTDAL_MUXER_AudioComponentIndex;

typedef enum
{
    eTDAL_MUXER_EVENT_CIRCBUFFER_FULL = 0,
    eTDAL_MUXER_EVENT_CIRCBUFFER_EMPTY
} tTDAL_MUXER_Event;

typedef void (*tTDAL_MUXER_EventCallback) (tTDAL_AV_Decoder decoder, tTDAL_MUXER_Event muxerEvent);
/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
tTDAL_MUXER_Error TDAL_MUXER_Init();
tTDAL_MUXER_Error TDAL_MUXER_IsRunning(tTDAL_AV_Decoder decoderId, bool *bRunning);
tTDAL_MUXER_Error TDAL_MUXER_OpenChannel(tTDAL_AV_Decoder decoderId, TDAL_MUXER_ChannelConfig *channelConfig);
tTDAL_MUXER_Error TDAL_MUXER_SubscribeEventCallback(tTDAL_AV_Decoder decoder, tTDAL_MUXER_EventCallback muxerEventCallback);
tTDAL_MUXER_Error TDAL_MUXER_UnsubscribeEventCallback(tTDAL_AV_Decoder decoder);
tTDAL_MUXER_Error TDAL_MUXER_SetChannelPid(tTDAL_AV_Decoder decoderId, TDAL_MUXER_ChannelConfig *channelConfig);
tTDAL_MUXER_Error TDAL_MUXER_StartMuxer(tTDAL_AV_Decoder decoderId);
tTDAL_MUXER_Error TDAL_MUXER_SetAudioComponentData(tTDAL_AV_Decoder decoderId, tTDAL_MUXER_AudioComponentIndex audioComponentIdx, TDAL_MUXER_AudioComponentData *audioComponentData);
tTDAL_MUXER_Error TDAL_MUXER_SetDiscontinuityType(tTDAL_AV_Decoder decoderId, tTDAL_MUXER_Discontinuity transition_flag);
tTDAL_MUXER_Error TDAL_MUXER_SetEOS(tTDAL_AV_Decoder decoderId, bool eos_flag);
tTDAL_MUXER_Error TDAL_MUXER_Read(tTDAL_AV_Decoder decoderId, uint8_t *buffer, uint32_t *size);
tTDAL_MUXER_Error TDAL_MUXER_CloseChannel(tTDAL_AV_Decoder decoderId);
tTDAL_MUXER_Error TDAL_MUXER_Deinit();

#endif /* _TDAL_MUXER_H_ */

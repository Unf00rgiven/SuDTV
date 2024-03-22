/******************************************************************************
 *                          COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_AV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_av.h $
 *            $Rev: 15059 $
 *            $Date: 2014-10-30 16:50:38 +0100 (Thu, 30 Oct 2014) $
 *
 * Description              : Audio/Video library primitives header.
 *
 *****************************************************************************/

#ifndef _TDAL_AV_H_
#define _TDAL_AV_H_

#include <crules.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*         Public types definition      */
/*--------------------------------------*/

typedef enum
{
    eTDAL_AV_NO_ERROR,
    eTDAL_AV_ERROR,
    eTDAL_AV_BAD_PARAMETER_ERROR,
    eTDAL_AV_LIMITATION_ERROR
} tTDAL_AV_Error;

typedef enum
{
    eTDAL_AV_DECODER_VIDEO_1 = 0x00000001,
    eTDAL_AV_DECODER_VIDEO_2 = 0x00000002,
    eTDAL_AV_DECODER_VIDEO_3 = 0x00000004,
    eTDAL_AV_DECODER_VIDEO_4 = 0x00000008,
    eTDAL_AV_DECODER_VIDEO_5 = 0x00000010,
    eTDAL_AV_DECODER_VIDEO_6 = 0x00000020,
    eTDAL_AV_DECODER_VIDEO_7 = 0x00000040,
    eTDAL_AV_DECODER_VIDEO_8 = 0x00000080,
    eTDAL_AV_DECODER_AUDIO_1 = 0x00000100,
    eTDAL_AV_DECODER_AUDIO_2 = 0x00000200,
    eTDAL_AV_DECODER_AUDIO_3 = 0x00000400,
    eTDAL_AV_DECODER_AUDIO_4 = 0x00000800,
    eTDAL_AV_DECODER_AUDIO_5 = 0x00001000,
    eTDAL_AV_DECODER_AUDIO_6 = 0x00002000,
    eTDAL_AV_DECODER_AUDIO_7 = 0x00004000,
    eTDAL_AV_DECODER_AUDIO_8 = 0x00008000,
    eTDAL_AV_ENCODER_VIDEO_1 = 0x00010000,
    eTDAL_AV_ENCODER_VIDEO_2 = 0x00020000,
    eTDAL_AV_ENCODER_VIDEO_3 = 0x00040000,
    eTDAL_AV_ENCODER_VIDEO_4 = 0x00080000,
    eTDAL_AV_ENCODER_VIDEO_5 = 0x00100000,
    eTDAL_AV_ENCODER_VIDEO_6 = 0x00200000,
    eTDAL_AV_ENCODER_VIDEO_7 = 0x00400000,
    eTDAL_AV_ENCODER_VIDEO_8 = 0x00800000,
    eTDAL_AV_ENCODER_AUDIO_1 = 0x01000000,
    eTDAL_AV_ENCODER_AUDIO_2 = 0x02000000,
    eTDAL_AV_ENCODER_AUDIO_3 = 0x04000000,
    eTDAL_AV_ENCODER_AUDIO_4 = 0x08000000,
    eTDAL_AV_ENCODER_AUDIO_5 = 0x10000000,
    eTDAL_AV_ENCODER_AUDIO_6 = 0x20000000,
    eTDAL_AV_ENCODER_AUDIO_7 = 0x40000000,
    eTDAL_AV_ENCODER_AUDIO_8 = 0x80000000,
} tTDAL_AV_Decoder;


typedef enum
{
    eTDAL_AV_PICTURE_NONE    = 0x000000001,
    eTDAL_AV_PICTURE_MPEG    = 0x000000002,
    eTDAL_AV_PICTURE_YUV_420 = 0x000000004,
    eTDAL_AV_PICTURE_YUV_422 = 0x000000008,
    eTDAL_AV_PICTURE_YUV_444 = 0x000000010,
    eTDAL_AV_PICTURE_RGB     = 0x000000020
} tTDAL_AV_PictureType;

typedef enum
{
    eTDAL_AV_SAMPLE_NONE  = 0x000000001,
    eTDAL_AV_SAMPLE_WAVE  = 0x000000002,
    eTDAL_AV_SAMPLE_MPEG1 = 0x000000004,
    eTDAL_AV_SAMPLE_PCM   = 0x000000008,
    eTDAL_AV_SAMPLE_MPEG2 = 0x000000010,
    eTDAL_AV_SAMPLE_AC3   = 0x000000020,
    eTDAL_AV_SAMPLE_MP3   = 0x000000040
} tTDAL_AV_SampleType;

typedef enum
{
    eTDAL_AV_AUDIO_TYPE_UNKOWN    = 0x000000000,
    eTDAL_AV_AUDIO_TYPE_AC3       = 0x000000001, /* Dolby Digital (DD) */
    eTDAL_AV_AUDIO_TYPE_MPEG      = 0x000000002,
    eTDAL_AV_AUDIO_TYPE_MPEG_AAC  = 0x000000004, /* Advanced Audio Coding */
    eTDAL_AV_AUDIO_TYPE_HE_AAC    = 0x000000008, /* High Eficiency AAC */
    eTDAL_AV_AUDIO_TYPE_EAC3      = 0x000000010  /* Enhanced AC3 (or DD+) */
} tTDAL_AV_AudioType;

typedef enum
{
    eTDAL_AV_VIDEO_TYPE_UNKOWN = 0x00000000,
    eTDAL_AV_VIDEO_TYPE_H264  = 0x000000001, /* ISO/IEC 14496                     */
    eTDAL_AV_VIDEO_TYPE_MPEG2 = 0x000000002, /* ISO/IEC 13818                     */
    eTDAL_AV_VIDEO_TYPE_MPEG1 = 0x000000004, /* ISO/IEC 11172                     */
    eTDAL_AV_VIDEO_TYPE_VC1   = 0x000000008, /* SMPTE 421M                        */
    eTDAL_AV_VIDEO_TYPE_DIVX  = 0x000000010,  /* ISO/IEC 14496p2=MPEG4Part2 DivX */
	eTDAL_AV_VIDEO_TYPE_HEVC  = 0x000000020  /* HEVC 							  */
} tTDAL_AV_VideoType;

typedef enum
{
    eTDAL_AV_AUDIO_MODE_PREFERRED,
    eTDAL_AV_AUDIO_MODE_CURRENT
} tTDAL_AV_AudioChannelPreference;

typedef enum
{
    eTDAL_AV_AUDIO_STEREO_MODE_MONO = 0, /* Left and right input channels
                    are mixed and the result is
                    output on both front channels */
    eTDAL_AV_AUDIO_STEREO_MODE_STEREO, /* Stereo output on front channels
                    (left and right) */
    eTDAL_AV_AUDIO_STEREO_MODE_STEREO_DUAL_LEFT, /* Left channel of the
                        stream output both on
                        front channels */
    eTDAL_AV_AUDIO_STEREO_MODE_STEREO_DUAL_RIGHT /* Right channel of the
                        stream output both on
                        front channels */
}tTDAL_AV_AudioStereoMode;


typedef enum
{
    eTDAL_AV_SYNCHRO_MODE_NONE = 0,      /* No synchronisation */
    eTDAL_AV_SYNCHRO_MODE_LIVE_FEED,     /* AV feed from live source */
    eTDAL_AV_SYNCHRO_MODE_STORAGE_FEED,  /* AV feed from storage source (e.g. HDD) */
    eTDAL_AV_SYNCHRO_MODE_NETWORK_FEED   /*AV feed from network source (e.g. IP network ) */
} tTDAL_AV_SynchroMode;

typedef enum
{
    eTDAL_AV_EVENT_VIDEO_SEQUENCE_INFO = 0,
    eTDAL_AV_EVENT_VIDEO_NEW_PICTURE_DECODED,
    eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE,
    eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE,
    eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY,
    eTDAL_AV_EVENT_VIDEO_USER_DATA,
    eTDAL_AV_EVENT_VIDEO_FRAME_RATE_CHANGE,
    eTDAL_AV_EVENT_AUDIO_NEW_FREQUENCY,
    eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED,
    eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED,
    eTDAL_AV_EVENT_AUDIO_NEW_AUDIO_DECODED,
    eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED,
    eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED,
    eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE,
    eTDAL_AV_EVENT_END_OF_STREAM,
    eTDAL_AV_EVENT_VIDEO_PLAYBACK_STARTED,
    eTDAL_AV_EVENT_STALL_START,
    eTDAL_AV_EVENT_STALL_END,
    eTDAL_AV_EVENT_QUALITY_CHANGE,
    eTDAL_AV_EVENT_BASE_PTS,
    eTDAL_AV_EVENT_PTS,
    eTDAL_AV_EVENT_DECODE_STATUS,
    eTDAL_AV_EVENT_CHUNK_CHANGE,
    eTDAL_AV_EVENT_VIDEO_PID_CHANGE,
    eTDAL_AV_EVENT_AUDIO_PID_CHANGE,
    eTDAL_AV_EVENT_MAX_NUMBER
} tTDAL_AV_Event;

typedef enum
{
    eTDAL_AV_ASPECT_RATIO_MPEG_4_3,
    eTDAL_AV_ASPECT_RATIO_MPEG_16_9,
    eTDAL_AV_ASPECT_RATIO_MPEG_1_1,
    eTDAL_AV_ASPECT_RATIO_MPEG_221_1
} tTDAL_AV_VideoAspectRatio;

typedef enum
{
    eTDAL_AV_BRIGHTNESS,
    eTDAL_AV_CONTRAST,
    eTDAL_AV_SATURATION,
    eTDAL_AV_COLORTEMPERATURE,
    eTDAL_AV_HUE
} tTDAL_AV_VideoPictureComponent;

typedef enum
{
    eTDAL_AV_TEST_SEQUENCE_TYPE_OFF                  = 0x00,
    eTDAL_AV_TEST_SEQUENCE_TYPE_STATIC               = 0x01,
    eTDAL_AV_TEST_SEQUENCE_TYPE_MOVING               = 0x02,
    eTDAL_AV_TEST_SEQUENCE_TYPE_MOVING_FULL_BITRATE  = 0x03
} tTDAL_AV_TestSequenceType;

typedef uint32_t tTDAL_AV_VideoFrameRate;
typedef uint32_t tTDAL_AV_AudioFrequency;

typedef void (*tTDAL_AV_CallbackProc_t)
(
    tTDAL_AV_Decoder decoder,
    tTDAL_AV_Event event,
    const void *pEventData
);

typedef struct
{
    uint32_t quality;
}
tTDAL_AV_DecodeStatus;

typedef struct
{
    tTDAL_AV_Decoder decoderSupported;
    tTDAL_AV_Decoder encoderSupported;
} tTDAL_AV_Capability;

typedef struct
{
    tTDAL_AV_PictureType pictureTypeSupported;
    tTDAL_AV_VideoType videoTypeSupported;
    bool isFreezeSupported;
    bool isPictureCaptureSupported;
    bool isSpeedSupported;
    int32_t speedBackwardMax;
    int32_t speedForwardMax;
#ifdef USE_MULTI_PLATFORM_SUPPORT
    uint8_t platformId;
#endif
} tTDAL_AV_VideoCapability;

typedef struct
{
    tTDAL_AV_SampleType sampleTypeSupported;
    tTDAL_AV_AudioType audioTypeSupported;
    bool isBeepSupported;
    bool isSpeedSupported;
    int32_t speedBackwardMax;
    int32_t speedForwardMax;
#ifdef USE_MULTI_PLATFORM_SUPPORT
    uint8_t platformId;
#endif
} tTDAL_AV_AudioCapability;

typedef struct
{
    uint16_t width;
    uint16_t height;
} tTDAL_AV_VideoResolution;

typedef struct
{
    uint16_t notCompressWidth;		      /* Width of picture */
    uint16_t notCompressHeight;		      /* Height of picture */
	/* When partial picture data in buffer,
	 * these are x, y, width and height of these data
	 * in the picture itself
	 */
    uint16_t notCompressDataBufferPosX;   /* X position of picture in data buffer for partial displaying */
    uint16_t notCompressDataBufferPosY;   /* Y position of picture in data buffer for partial displaying */
    uint16_t notCompressDataBufferWidth;  /* Width of picture in data buffer for partial displaying */
    uint16_t notCompressDataBufferHeight; /* Height of picture in data buffer for partial displaying */
} tTDAL_AV_VideoPictureParams;

typedef struct
{
    uint8_t AFD;
} tTDAL_AV_VideoUserData;

typedef union
{
    tTDAL_AV_VideoType videoType;
    tTDAL_AV_AudioType audioType;
} tTDAL_AV_StreamType;

typedef struct
{
    uint32_t positionMs;
    //TODO
}
tTDAL_AV_VideoStatus;

typedef struct
{
    uint32_t positionMs;
    //TODO
}
tTDAL_AV_AudioStatus;

typedef union
{
    tTDAL_AV_VideoStatus videoStatus;
    tTDAL_AV_AudioStatus audioStatus;
}
tTDAL_AV_StreamStatus;

typedef void (* tTDAL_AV_NotifyAudioChannelFct)( uint32_t    audio_channel,
                                                            void*    cid);

typedef struct
{
    tTDAL_AV_NotifyAudioChannelFct    callback;
    void*                                  cid;
}
tTDAL_AV_Callback_Param;

typedef enum
{
    eTDAL_AV_CORRELATION_IDLE,
    eTDAL_AV_CORRELATION_SUCCESSFUL,
    eTDAL_AV_CORRELATION_FAILED
} tTDAL_AV_CorrelationResult;

typedef struct
{
    tTDAL_AV_CorrelationResult correlationResult;
    float correlationFactor;
    int32_t timeDelay;
    int32_t volumeDifference;
}
tTDAL_AV_CorrelationStatus;


/*------------------------------------------*/
/*       Public Functions Declarations      */
/*------------------------------------------*/

tTDAL_AV_Error TDAL_AV_Init(void);
tTDAL_AV_Error TDAL_AV_Terminate(void);
tTDAL_AV_Error TDAL_AV_Start(tTDAL_AV_Decoder decoder,tTDAL_AV_StreamType StreamType);
tTDAL_AV_Error TDAL_AV_Stop(tTDAL_AV_Decoder decoder);
tTDAL_AV_Error TDAL_AV_CapabilityGet(tTDAL_AV_Capability *pstCapability);
const char *   TDAL_AV_APIRevisionGet(void );
const char *   TDAL_AV_PlatformRevisionGet(void );
tTDAL_AV_Error TDAL_AV_EventSubscribe(tTDAL_AV_Decoder decoder,tTDAL_AV_Event event,tTDAL_AV_CallbackProc_t notifyFunction);
tTDAL_AV_Error TDAL_AV_EventUnsubscribe(tTDAL_AV_Decoder decoder,tTDAL_AV_Event event, tTDAL_AV_CallbackProc_t notifyFunction);
tTDAL_AV_Error TDAL_AV_SynchroModeSet(tTDAL_AV_Decoder videoDecoder,tTDAL_AV_Decoder audioDecoder,tTDAL_AV_SynchroMode mode);
tTDAL_AV_Error TDAL_AV_SpeedSet(tTDAL_AV_Decoder decoder, float speed);
tTDAL_AV_Error TDAL_AV_Reset(tTDAL_AV_Decoder decoder);
tTDAL_AV_Error TDAL_AV_Status(tTDAL_AV_Decoder decoder, tTDAL_AV_StreamStatus *pStreamStatus);
tTDAL_AV_Error TDAL_AV_GetDemuxId(tTDAL_AV_Decoder decoder, uint32_t* demuxId);
tTDAL_AV_Error TDAL_AV_InputStreamSet(tTDAL_AV_Decoder decoder,uint32_t streamHandle);
tTDAL_AV_Error TDAL_AV_VideoCapabilityGet(tTDAL_AV_Decoder decoder,tTDAL_AV_VideoCapability *pstCapability);
tTDAL_AV_Error TDAL_AV_VideoDestinationSet(tTDAL_AV_Decoder decoder,uint32_t layerHandle);
tTDAL_AV_Error TDAL_AV_VideoFreeze(tTDAL_AV_Decoder decoder);
tTDAL_AV_Error TDAL_AV_VideoPictureStart(tTDAL_AV_Decoder decoder,tTDAL_AV_PictureType pictureType,uint8_t* pData,uint32_t size, tTDAL_AV_VideoPictureParams *pstPictureParams);
tTDAL_AV_Error TDAL_AV_VideoPictureStop(tTDAL_AV_Decoder decoder);
tTDAL_AV_Error TDAL_AV_VideoPictureCaptureGet(tTDAL_AV_Decoder decoder,tTDAL_AV_PictureType pictureType,uint8_t** ppData,uint32_t *pSize);
tTDAL_AV_Error TDAL_AV_VideoPictureCaptureRelease(tTDAL_AV_Decoder decoder,tTDAL_AV_PictureType pictureType,uint8_t* pData,uint32_t size);
tTDAL_AV_Error TDAL_AV_VideoPictureParamsSet(tTDAL_AV_VideoPictureComponent eComponent, int16_t value);
tTDAL_AV_Error TDAL_AV_VideoPictureParamsGet(tTDAL_AV_VideoPictureComponent eComponent, int16_t *value);
tTDAL_AV_Error TDAL_AV_AudioCapabilityGet(tTDAL_AV_Decoder decoder,tTDAL_AV_AudioCapability *pstCapability);
tTDAL_AV_Error TDAL_AV_AudioSampleStart(tTDAL_AV_Decoder decoder,tTDAL_AV_SampleType sampleType,uint8_t* pData,uint32_t size,uint16_t nbTimes);
tTDAL_AV_Error TDAL_AV_AudioSampleStop(tTDAL_AV_Decoder decoder);
tTDAL_AV_Error TDAL_AV_AudioBeepPlay(tTDAL_AV_Decoder decoder,uint16_t frequency,uint16_t duration);
tTDAL_AV_Error TDAL_AV_AudioModeGet(tTDAL_AV_Decoder decoder, uint32_t *pstMode);
tTDAL_AV_Error TDAL_AV_AudioStereoModeGet(tTDAL_AV_Decoder decoder,tTDAL_AV_AudioStereoMode *pstMode);
tTDAL_AV_Error TDAL_AV_AudioStereoModeSet(tTDAL_AV_Decoder decoder,tTDAL_AV_AudioStereoMode mode);
tTDAL_AV_Error TDAL_AV_AudioDestinationSet(tTDAL_AV_Decoder eDecoder, uint32_t SpeakerHandle);
tTDAL_AV_Error TDAL_AV_GetPairedAudioDecoder(tTDAL_AV_Decoder videoDecoder, tTDAL_AV_Decoder *audioDecoder);
tTDAL_AV_Error TDAL_AV_AudioSetEnv(void* vm, void* cls, void* obj);
tTDAL_AV_Error TDAL_AV_DecoderSetEnv(void* vm, void* cls, void* obj);
tTDAL_AV_Error TDAL_AV_SetVideo4kCapability(tTDAL_AV_Decoder decoder, uint8_t isCapable);
tTDAL_AV_Error TDAL_AV_SetVideoDecoderMode(tTDAL_AV_Decoder decoder, uint8_t mode);
void           TDAL_AV_RegisterAudioChannelCallback(tTDAL_AV_Callback_Param *param);

tTDAL_AV_Error TDAL_AV_AudioDescriptionSet(tTDAL_AV_Decoder decoder, uint32_t streamHandle, tTDAL_AV_AudioType codec, uint8_t substreamId);
tTDAL_AV_Error TDAL_AV_Jump(tTDAL_AV_Decoder decoder, double posSec);

tTDAL_AV_Error TDAL_AV_RegisterExternalCallback(void* callback, void* data);
tTDAL_AV_Error TDAL_AV_UnregisterExternalCallback(void* callback, void* data);

#ifdef USE_PIP_SWAP
tTDAL_AV_Error TDAL_AV_SwapMainAndPipDecoders(tTDAL_AV_Decoder mainDecoder, tTDAL_AV_Decoder pipDecoder);
#endif

#ifdef PRODUCT_USE_TDAL_RPC
tTDAL_AV_Error TDAL_AV_RPC_SetOutputVideoMode (bool bIsEnable);
bool           TDAL_AV_RPC_GetOutputVideoMode (void);
tTDAL_AV_Error TDAL_AV_RPC_SetOutputAudioMode (bool bIsEnable);
bool           TDAL_AV_RPC_GetOutputAudioMode (void);
#endif

tTDAL_AV_Error TDAL_AVm_GetPts(tTDAL_AV_Decoder decoder, uint64_t* pts);

tTDAL_AV_Error TDAL_AVm_SetPassDolby(tTDAL_AV_Decoder decoder, bool bPassDolby);

tTDAL_AV_Error TDAL_AVm_GetPos(uint64_t* posUs);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_AV_H_*/

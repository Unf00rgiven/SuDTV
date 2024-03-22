/*
 * tdal_pvr.h
 *
 *  Created on: Mar 7, 2013
 *      Author: rtrk
 */

#ifndef TDAL_PVR_H_
#define TDAL_PVR_H_


#define kTDAL_PVR_MAX_STREAM_NUM 8
#define kTDAL_PVR_MAX_REC_PID_NUM         32
#define kTDAL_PVR_MAX_SUBTITLEINFO_NUM    12
#define kTDAL_PVR_MAX_AUDIOINFO_NUM       16
#define kTDAL_PVR_TS_PACKET_SIZE		  188
#define kTDAL_PVR_MAX_RECORDING_FILE_SIZE_IN_KB 3000000

/* ****************** *
 * PVR typedefs  *
 * ****************** */
/* error codes */
typedef enum TDAL_PVR_Error_e {
    eTDAL_PVR_NO_ERROR = 0,     /* no error                  */
    eTDAL_PVR_ERROR_NOT_INIT,   /* driver is not initialised */
    eTDAL_PVR_ERROR_BAD_ARG,    /* wrong argument value      */
    eTDAL_PVR_ERROR_IN_DRIVER,  /* error in the driver       */
    eTDAL_PVR_ERROR_NOT_DONE    /* command not done          */
} tTDAL_PVR_Error;

typedef enum TDAL_PVR_Encr_e {
    eTDAL_PVR_ENCR_NONE,
    eTDAL_PVR_ENCR_DEFAULT,
    eTDAL_PVR_ENCR_USER,
    eTDAL_PVR_ENCR_CIPLUS,
    eTDAL_PVR_ENCR_SMARTCARD
} tTDAL_PVR_Encr;

/* pvr events codes */
typedef enum tTDAL_PVR_Event_e {
    eTDAL_PVR_END_FILE = 0,     /* playback reached the end of file     */
    eTDAL_PVR_BEGIN_FILE,       /* playback reached the end of file     */
    eTDAL_PVR_OPEN_FILE_FAIL,   /* open file failed                     */
    eTDAL_PVR_READ_FILE_FAIL,   /* read file failed                     */
    eTDAL_PVR_WRITE_FILE_FAIL,  /* write file failed                    */
    eTDAL_PVR_NO_DISK_SPACE,    /* no disk space for recording          */
    eTDAL_PVR_RECORDING_ERROR,  /* something went wrong with recording  */
    eTDAL_PVR_PLAYBACK_ERROR,   /* something went wrong with playback   */
    eTDAL_PVR_FILE_THRESHOLD,    /* threshold for file size reached      */
    eTDAL_PVR_PLAY_ELAPSED_TIME, /* current playback elapsed time        */
    eTDAL_PVR_DISK_SLOW          /* disk speed is slower than rec speed  */
} tTDAL_PVR_Event;

/* pvr events of Audio/Video pid changes */
typedef enum tTDAL_PVR_PidChange_Event_e {
    eTDAL_PVR_PID_CHANGE_DURING_RECORDING,
    eTDAL_PVR_PID_CHANGE_DURING_PLAYBACK,
    eTDAL_PVR_PID_ADD_DURING_PLAYBACK,
    eTDAL_PVR_PID_REMOVE_DURING_PLAYBACK
} tTDAL_PVR_PidChange_Event;

typedef enum tTDAL_PVR_Speed_e {
    eTDAL_PVR_SPEED_64X_BACKWARD ,
    eTDAL_PVR_SPEED_32X_BACKWARD ,
    eTDAL_PVR_SPEED_16X_BACKWARD ,
    eTDAL_PVR_SPEED_8X_BACKWARD ,
    eTDAL_PVR_SPEED_4X_BACKWARD ,
    eTDAL_PVR_SPEED_2X_BACKWARD ,
    eTDAL_PVR_SPEED_1X_BACKWARD ,
    eTDAL_PVR_SPEED_0X5_BACKWARD ,
    eTDAL_PVR_SPEED_0X25_BACKWARD ,
    eTDAL_PVR_SPEED_0X ,
    eTDAL_PVR_SPEED_FREEZE ,
    eTDAL_PVR_SPEED_0X25_FORWARD ,
    eTDAL_PVR_SPEED_0X5_FORWARD ,
    eTDAL_PVR_SPEED_1X_FORWARD ,
    eTDAL_PVR_SPEED_2X_FORWARD ,
    eTDAL_PVR_SPEED_4X_FORWARD ,
    eTDAL_PVR_SPEED_8X_FORWARD ,
    eTDAL_PVR_SPEED_16X_FORWARD ,
    eTDAL_PVR_SPEED_32X_FORWARD ,
    eTDAL_PVR_SPEED_64X_FORWARD
} tTDAL_PVR_Speed;

typedef enum
{
    eTDAL_PVR_STREAM_TYPE_VIDEO_MPEG1,
    eTDAL_PVR_STREAM_TYPE_VIDEO_MPEG2,
    eTDAL_PVR_STREAM_TYPE_VIDEO_H264,
    eTDAL_PVR_STREAM_TYPE_VIDEO_HEVC,
    eTDAL_PVR_STREAM_TYPE_AUDIO_MPEG,
    eTDAL_PVR_STREAM_TYPE_AUDIO_AC3,
    eTDAL_PVR_STREAM_TYPE_AUDIO_EAC3,
    eTDAL_PVR_STREAM_TYPE_AUDIO_AAC,
    eTDAL_PVR_STREAM_TYPE_AUDIO_HEAAC,
    eTDAL_PVR_STREAM_TYPE_SUBTITLE,
    eTDAL_PVR_STREAM_TYPE_TELETEXT,
    eTDAL_PVR_STREAM_TYPE_PCR,
    eTDAL_PVR_STREAM_TYPE_PSI,
    eTDAL_PVR_STREAM_TYPE_MAX
}
tTDAL_PVR_StreamType;

typedef enum TDAL_PVR_type_e {
    eTDAL_PVR_TYPE_RECORDING,
    eTDAL_PVR_TYPE_PLAYBACK,
    eTDAL_PVR_TYPE_TIMESHIFT_RECORDING,
    eTDAL_PVR_TYPE_TIMESHIFT_PLAYBACK,
    eTDAL_PVR_TYPE_MAX
} tTDAL_PVR_Type;

typedef struct
{
    uint16_t             pid;
    tTDAL_PVR_StreamType stream_type;
}
tTDAL_PVR_StreamInfo;

typedef struct
{
    uint16_t             length;
    uint16_t             pid[kTDAL_PVR_MAX_REC_PID_NUM];
    tTDAL_PVR_StreamType stream_type[kTDAL_PVR_MAX_REC_PID_NUM];
}
tTDAL_PVR_StreamInfoArray;

typedef struct tTDAL_PVR_BasicProgInfo_t
{
    uint32_t             u32PmtPid;
    uint32_t             u32LCN;
    char*                fileName;
    uint32_t             fileThreshold;
    uint8_t              filesCount;
    tTDAL_PVR_Encr       eEncryptionType;
    uint8_t              bIsEncrypted;
    uint8_t              u8CryptKey[1024];
    uint8_t              recordStreamCount;
    tTDAL_PVR_StreamInfo recordStreams[kTDAL_PVR_MAX_STREAM_NUM];
    uint8_t              playbackStreamCount;
    tTDAL_PVR_StreamInfo playbackStreams[kTDAL_PVR_MAX_STREAM_NUM];
    uint32_t             demux_id_rec;
    uint32_t             demux_id_play;
    uint32_t             videoDecoder_id;
    uint32_t             audioDecoder_id;
    bool                 bAppend;
    uint32_t             playbackType;
    tTDAL_PVR_Type       eSettingsType;
} tTDAL_PVR_BasicProgInfo;

typedef struct tTDAL_PVR_ComplexProgInfo_t
{
    tTDAL_PVR_BasicProgInfo stPVRBasicProgInfo;
    uint32_t                u32duration;
    uint32_t                u32StartTime;
    uint64_t                u64FileLength;
    uint8_t                 u8Age;
    bool                    bLocked;
    uint32_t                u32LastPlayPositionInSec;
} tTDAL_PVR_ComplexProgInfo;

typedef uint32_t tTDAL_PVR_Handle;
#ifdef PRODUCT_USE_NAGRA_DVL
typedef uint32_t tTDAL_PVR_KeySlotHandle;
#endif

typedef struct tTDAL_PVR_PidChangeInfo_t
{
    uint8_t              nbPidsToAdd;
    uint16_t             pidsToAdd[kTDAL_PVR_MAX_REC_PID_NUM];
    tTDAL_PVR_StreamType streamType[kTDAL_PVR_MAX_REC_PID_NUM];
}tTDAL_PVR_PidChangeInfo;

typedef void (* tTDAL_PVR_NotifyAudioChannelFct)( uint32_t    audio_channel,
                                                            void*    cid);
typedef struct
{
    tTDAL_PVR_NotifyAudioChannelFct    callback;
    void*                                  cid;
}tTDAL_PVR_Callback_Param;

tTDAL_PVR_Error TDAL_PVR_Init(void);
tTDAL_PVR_Error TDAL_PVR_Term(void);
tTDAL_PVR_Error TDAL_PVR_Open(tTDAL_PVR_Handle *pvrHandle, bool bIsTimeshift, const char* pPath);
tTDAL_PVR_Error TDAL_PVR_Close(tTDAL_PVR_Handle pvrHandle);
#ifdef PRODUCT_USE_NAGRA_DVL
tTDAL_PVR_Error TDAL_PVR_Record_Start(tTDAL_PVR_Handle pvrHandle,tTDAL_PVR_KeySlotHandle keySlotHandle);
#else
tTDAL_PVR_Error TDAL_PVR_Record_Start(tTDAL_PVR_Handle pvrHandle);
#endif // PRODUCT_USE_NAGRA_DVL
tTDAL_PVR_Error TDAL_PVR_Record_Stop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Record_ChangePid(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_PidChangeInfo pidChangeInfo);
tTDAL_PVR_Error TDAL_PVR_Record_Pause(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Record_Resume(tTDAL_PVR_Handle pvrHandle);
#ifdef PRODUCT_USE_NAGRA_DVL
tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStart(tTDAL_PVR_Handle pvrHandle,tTDAL_PVR_KeySlotHandle keySlotHandle);
#else
tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStart(tTDAL_PVR_Handle pvrHandle);
#endif // PRODUCT_USE_NAGRA_DVL
tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_Stop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec);
tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackAddStream(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_StreamInfo *pStream);
tTDAL_PVR_Error TDAL_PVR_PlaybackRemoveStream(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_StreamInfo *pStream);
tTDAL_PVR_Error TDAL_PVR_PlaybackSetNewPids(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_StreamInfo *pStream);
tTDAL_PVR_Error TDAL_PVR_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec);
tTDAL_PVR_Error TDAL_PVR_PlaybackStop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackPause(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackResume(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackGetSpeed(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed* pSpeed);
tTDAL_PVR_Error TDAL_PVR_PlaybackFastForward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick);
tTDAL_PVR_Error TDAL_PVR_PlaybackFastBackward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpToTime(tTDAL_PVR_Handle pvrHandle, int32_t *u32Hour, uint32_t *u32Minute, uint32_t *u32Second);
tTDAL_PVR_Error TDAL_PVR_SetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_BasicProgInfo *pBasicProgInfo);
tTDAL_PVR_Error TDAL_PVR_GetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_ComplexProgInfo *pPVRComplexInfo);
tTDAL_PVR_Error TDAL_PVR_SetTimeshiftIntervalInSec(uint32_t interval);
tTDAL_PVR_Error TDAL_PVR_GetTimeshiftIntervalInSec(uint32_t *interval);
tTDAL_PVR_Error TDAL_PVR_ChangeTimeshiftIntervalInSec(tTDAL_PVR_Handle pvrHandle, uint32_t interval);
tTDAL_PVR_Error TDAL_PVR_SetIdx(uint32_t *idx);
tTDAL_PVR_Error TDAL_PVR_GetIdx(void);
tTDAL_PVR_Error TDAL_PVR_PlaybackStepIn(void);
tTDAL_PVR_Error TDAL_PVR_PlaybackWithNormalSpeed(void);
tTDAL_PVR_Error TDAL_PVR_CaptureThumbnail(tTDAL_PVR_Handle pvrHandle, char *filename);
tTDAL_PVR_Error TDAL_PVR_JumpToThumbnail(char *filename);
tTDAL_PVR_Error TDAL_PVR_HideThumbnail(void);
tTDAL_PVR_Error TDAL_PVR_PlaybackABLoop(uint32_t *u32ABBeginTime,uint32_t *u32ABEndTime);
tTDAL_PVR_Error TDAL_PVR_PlaybackResetABLoop(void);
tTDAL_PVR_Error TDAL_PVR_PlaybackAddSkipTime(uint32_t *u32BeginTimeInSec,uint32_t *u32EndTimeInSec);
tTDAL_PVR_Error TDAL_PVR_PlaybackRemoveSkipTime(uint32_t *u32BeginTimeInSec,uint32_t *u32EndTimeInSec);
tTDAL_PVR_Error TDAL_PVR_SetPlaybackRetentionLimit(tTDAL_PVR_Handle pvrHandle, const uint32_t *u32RetentionLimitTimeInSec);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpForward(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpBackward(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_ShowTime(const uint32_t *u32Type,const uint32_t *u32Set);
tTDAL_PVR_Error TDAL_PVR_RemoveFile(char *fileName, int filesCount);
tTDAL_PVR_Error TDAL_PVR_SetPlaybackType(uint32_t* pu32PlaybackType);
tTDAL_PVR_Error TDAL_PVR_SetRecordType(uint32_t* pu32RecordType);
tTDAL_PVR_Error TDAL_PVR_RenameFile(char *fileName,char *newFileName);
tTDAL_PVR_Error TDAL_PVR_GetTimeshiftRecStartTime(void);
tTDAL_PVR_Error TDAL_PVR_SetTimeshiftRecScreenFrozen(bool bFrozen);
tTDAL_PVR_Error TDAL_PVR_CheckUSBSpeed(void);
tTDAL_PVR_Error TDAL_PVR_EnableStillImageZapping(uint8_t *bEnable);
tTDAL_PVR_Error TDAL_PVR_Get(uint8_t *bEnable);
bool            TDAL_PVR_IsRecording(tTDAL_PVR_Handle pvrHandle);
void            TDAL_PVR_SetFreeSpaceThreshold(uint8_t percent);
uint8_t         TDAL_PVR_GetFreeSpaceThreshold(void);
tTDAL_PVR_Error TDAL_PVR_Record_GetDurationByPath(char* path, uint32_t* duration);
uint32_t        TDAL_PVR_Record_GetDuration(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackGetTime(tTDAL_PVR_Handle pvrHandle, uint32_t* uiPosition);
tTDAL_PVR_Error TDAL_PVR_PlaybackSetSpeed(tTDAL_PVR_Handle pvrHandle, int32_t speed);
typedef void (*tTDAL_PVR_Callback)(int, void*);
typedef void (*tTDAL_PVR_PIDCHANGE_Callback)(int, void*);

tTDAL_PVR_Error TDAL_PVR_RegisterCallback(tTDAL_PVR_Callback PVR_CB);
tTDAL_PVR_Error TDAL_PVR_PidChangeRegisterCallback(tTDAL_PVR_PIDCHANGE_Callback PVR_PIDCHANGE_CB);

#ifdef PRODUCT_USE_NAGRA_DVL
uint32_t 		  TDAL_PVR_GetPlaybackHandle (tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PreparePlaybackStart(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_PreparePlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec);
#endif // PRODUCT_USE_NAGRA_DVL

void TDAL_PVR_RegisterAudioChannelCallback(tTDAL_PVR_Callback_Param *param);
void TDAL_PVR_AudioModeGet (uint32_t demuxID, uint32_t *pstMode);
tTDAL_PVR_Error TDAL_PVR_IndexGetOffset(uint8_t* fileName, uint32_t u32TimeMs, uint64_t *pu64GlobalOffset);
tTDAL_PVR_Error TDAL_PVR_PlaybackSetPosition(tTDAL_PVR_Handle pvrHandle, bool bRelative, int32_t i32PositionMs);
#endif /* TDAL_PVR_H_ */

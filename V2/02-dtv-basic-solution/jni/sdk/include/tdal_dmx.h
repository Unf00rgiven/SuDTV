/******************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME:     TDAL_DMX
 *
 * FILE NAME: $Workfile:   $
 *            $Revision: 11314 $
 *            $Date: 2013-10-23 14:32:40 +0200 (Wed, 23 Oct 2013) $
 *
 * PUBLIC 
 *
 * DESCRIPTION:     Header of Demux filter utility
 *
 *****************************************************************************/

#ifndef _TDAL_DMX_H_
#define _TDAL_DMX_H_    

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*              Defines                                */
/*******************************************************/

#define kTDAL_DMX_ILLEGAL_DEMUX         -1      /* illegal demux value   */
#define kTDAL_DMX_ILLEGAL_CHANNEL       -1      /* illegal channel value */
#define kTDAL_DMX_ILLEGAL_FILTER        -1      /* illegal filter value  */
#define kTDAL_DMX_ILLEGAL_PID           -1      /* illegal pid value     */
#define kTDAL_DMX_ILLEGAL_DEMUX_CLIENT  -1
/*#define TDAL_DMX_DBG_TRC_PCK_CNT*/

/*******************************************************/
/*              Macros                                 */
/*******************************************************/



/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

typedef  signed char tTDAL_DMX_ChannelId;
typedef  signed char tTDAL_DMX_FilterId;

typedef enum
{
    kTDAL_DMX_NO_ERROR = 0,                     /* no error                         */
    kTDAL_DMX_ERROR_BAD_PARAMETERS,             /* bad parameter in function call   */ 
    kTDAL_DMX_ERROR_UNABLE_TO_GRANT,            
    kTDAL_DMX_ERROR_TOO_MANY_REQ,               /* too many request already active  */
    kTDAL_DMX_ERROR_INVALID_DEMUX_ID,           /* invalid demux id                 */  
    kTDAL_DMX_ERROR_INVALID_CHANNEL_ID,         /* invalid channel id               */
    kTDAL_DMX_ERROR_INVALID_FILTER_ID,          /* invalid filter id                */
    kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER,   /* free filter not possible         */
    kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL,  /* free channel not possible        */
    kTDAL_DMX_ERROR_PID_IN_USE,                 /* PID already in use               */
    kTDAL_DMX_ERROR_NOT_INITIALIZED,            /* TDAL_DMX not initialized         */
    kTDAL_DMX_ERROR_MUTEX                       /* OS error                         */
}tTDAL_DMX_Error;

typedef enum
{
    eTDAL_DMX_ILLEGAL_DEMUX = kTDAL_DMX_ILLEGAL_DEMUX,
    eTDAL_DMX_DEMUX0 = 0,
    eTDAL_DMX_DEMUX1,
    eTDAL_DMX_DEMUX2,
    eTDAL_DMX_DEMUX3,
    eTDAL_DMX_DEMUX4,
    eTDAL_DMX_DEMUX5,
    eTDAL_DMX_DEMUX6,
    eTDAL_DMX_DEMUX7,
    eTDAL_DMX_NB_DEMUX
}tTDAL_DMX_DemuxId;

typedef enum {
    eTDAL_DMX_TYPE_TS,
    eTDAL_DMX_TYPE_MMT
}tTDAL_DMX_Type;

typedef enum
{
    eTDAL_DMX_CTRL_RESET,                       /* Reset a channel/filter           */
    eTDAL_DMX_CTRL_ENABLE,                      /* Enable a channel/filter          */
    eTDAL_DMX_CTRL_DISABLE                      /* Disable a channel/filter         */
}tTDAL_DMX_Ctrl;

typedef enum
{
    eTDAL_DMX_VIDEO_STREAM,                     /* Video stream                     */
    eTDAL_DMX_AUDIO_STREAM,                     /* Audio stream                   	*/
	eTDAL_DMX_AUDIO_DESCRIPTION_STREAM,         /* Audio description stream         */
    eTDAL_DMX_PCR_STREAM,                       /* Pcr stream                       */
    eTDAL_DMX_TELETEXT_STREAM,                  /* Teletext stream                  */
    eTDAL_DMX_SUBTITLE_STREAM,                  /* Subtitle stream                  */
    eTDAL_DMX_SECTION_STREAM,                   /* Section stream                   */
	eTDAL_DMX_TP_STREAM, 		 				/* TP stream 						*/
    eTDAL_DMX_CLOSED_CAPTION_STREAM,            /* Closed caption stream            */
    eTDAL_DMX_SUPERIMPOSE_STREAM                /* Superimpose stream               */
}tTDAL_DMX_ChannelStream;

typedef enum
{
    eTDAL_DMX_STREAM_TYPE_UNKOWN,
    eTDAL_DMX_STREAM_TYPE_VIDEO_H264,
    eTDAL_DMX_STREAM_TYPE_VIDEO_HEVC,
    eTDAL_DMX_STREAM_TYPE_VIDEO_MPEG2,
    eTDAL_DMX_STREAM_TYPE_AUDIO_AAC,
    eTDAL_DMX_STREAM_TYPE_AUDIO_MPEG2,
}tTDAL_DMX_ChannelStreamType;

typedef enum
{
   eTDAL_DMX_DECODER,                           /* It references a decoder output   */
   eTDAL_DMX_MEMORY                             /* It references the memory (send to the application) */
} tTDAL_DMX_ChannelOutput;

typedef enum
{
    eTDAL_DMX_NO_MEMORY,                        /* no more memory needed for filtering     */
    eTDAL_DMX_LOW_MEMORY_SIZE,                  /* at least 4 kilo bytes                   */    
    eTDAL_DMX_MEDIUM_MEMORY_SIZE,               /* at least 4 kilo bytes                   */    
    eTDAL_DMX_HIGHT_MEMORY_SIZE                 /* at least 16 kilo bytes                  */    
}tTDAL_DMX_ChannelMemorySize;


typedef enum
{
    eTDAL_DMX_CRC_CHECK,                       /* Check the CRC of a section filter         */
    eTDAL_DMX_CRC_IGNORE                       /* Ignore the CRC of a section filter        */
}tTDAL_DMX_FilterCRCMode;

/*--------------------------------------------------------*/
/* Callback function type definition given by the client  */
/* to be notified when a section buffer has been filtered */    
/*--------------------------------------------------------*/
typedef  void (*tTDAL_DMX_ChannelCallbackFct) (tTDAL_DMX_ChannelId      ChannelId,      /* channel ID           */
                                               tTDAL_DMX_FilterId       FilterId,       /* filter ID            */
                                               uint32_t                 Filter_track,   /* track                */  
                                               uint32_t                 Length,         /* length of the buffer */
                                               uint8_t                  *Buffer);       /* buffer of data       */

/* Channel type */
typedef struct
{
    tTDAL_DMX_ChannelStream          eChannelStream;        /* type of the stream for the channel   */
    tTDAL_DMX_ChannelOutput          eChannelOutput;        /* specifiy where to send filtered data */ 
    tTDAL_DMX_ChannelMemorySize      eChannelMemorySize;    /* Memory need for the channel      */
    tTDAL_DMX_ChannelStreamType      eChannelStreamType;
}tTDAL_DMX_ChannelType_t;

/* TS error types */
typedef enum
{
    eTDAL_DMX_TS_CC_ERROR = 0x1,            /* transport stream continuity counter error */
    eTDAL_DMX_AV_CC_ERROR = 0x2             /* audio and video stream continuity counter error */
} tTDAL_DMX_TSErrorType;

/* TS error monitoring parameters */
typedef struct
{
    uint32_t errorType;                     /* continuity counter error type bit-mask */
    uint32_t CCErrorNumber;                 /* continuity counter (CC) error limit for specified time interval */
    uint32_t CCErrorTimeInterval;           /* time interval in milliseconds for CC error limit check */
}tTDAL_DMX_TSErrorMonitoringParameters;

typedef void (*tTDAL_DMX_TSErrorMonitoringCallback)(tTDAL_DMX_DemuxId demuxId,
        int8_t clientId, tTDAL_DMX_TSErrorType error);

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/


/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

/******************************************************************************
* Function Name : TDAL_DMX_Init
*
* Description   : this function initialises the demux filter module
*
* Side effects  : Return    kTDAL_DMX_NO_ERROR if initialised successfully
*                           kTDAL_DMX_ERROR_MUTEX if mutex can't be created
*                           kTDAL_DMX_ERROR_UNABLE_TO_GRANT otherwise
*
* Comment       : allocate global memory for module
*
*****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Init(void);
/******************************************************************************
* Function Name : TDAL_DMX_Terminate
*
* Description   : this function deinitialises the demux filter module
*
* Side effects  : Return    kTDAL_DMX_NO_ERROR if initialised successfully
*                           kTDAL_DMX_ERROR_MUTEX if mutex can't be deleted
*                           kTDAL_DMX_ERROR_UNABLE_TO_GRANT otherwise
*
* Comment       : free global memory for module
*
*****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Terminate(void);
tTDAL_DMX_Error TDAL_DMX_GetType(tTDAL_DMX_DemuxId dmxId, tTDAL_DMX_Type* type);

const char *     TDAL_DMX_APIRevisionGet(void );
const char *     TDAL_DMX_PlatformRevisionGet(void );

/******************************************************************************
 * Function Name : TDAL_DMX_Allocate_Channel
 *
 * Description   : This function allocates a channel. The output is the channel Id
 *                 if succedded. Else the output is kTDAL_DMX_ILLEGAL_CHANNEL.
 *
 * Side effects  :
 *                  allocation  of a section or PES filtering channel
 *                  return  kTDAL_DMX_NO_ERROR if all is correct.
 *                          kTDAL_DMX_ERROR_NOT_INITIALIZED if the module is not
 *                          initialized.
 *                          kTDAL_DMX_ERROR_TOO_MANY_REQ if there is no more
 *                          channel available
 *                          kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                          kTDAL_DMX_ERROR_UNABLE_TO_GRANT if stream allocate
 *                          failed
 *                          kTDAL_DMX_ERROR_BAD_PARAMETERS if a parameter is bad
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Allocate_Channel    (tTDAL_DMX_DemuxId         DemuxId,
                                                  tTDAL_DMX_ChannelType_t   *pType,
                                                  tTDAL_DMX_ChannelId       *pChannelId);
/*********************************************************************************
 * Function Name : TDAL_DMX_Allocate_Filter
 *
 * Description   : this function allocates a filter on a channel. The output
 *                 is the filter Id if succedded. Else the output is
 *                 kTDAL_DMX_ILLEGAL_FILTER.
 *
 * Side effects  :
 *                  return  kTDAL_DMX_NO_ERROR if all is correct
 *                          kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if bad Id
 *                          kTDAL_DMX_ERROR_TOO_MANY_REQ if there is no more
 *                          filter available
 *                          kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                          kTDAL_DMX_ERROR_UNABLE_TO_GRANT 
 *
 * Comment       :
 *
 ********************************************************************************/                                                
tTDAL_DMX_Error     TDAL_DMX_Allocate_Filter     (tTDAL_DMX_ChannelId   ChannelId,
                                                  tTDAL_DMX_FilterId    *pFilterId);
/******************************************************************************
 * Function Name : TDAL_DMX_Register_Channel_Callback
 *
 * Description   : this function register a callback on a channel
 *
 * Side effects  :
 *                  return  kTDAL_DMX_NO_ERROR if all is correct
 *                          kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if Id does not exist
 *                          kTDAL_DMX_ERROR_BAD_PARAMETERS if callback is null
 *                          kTDAL_DMX_ERROR_MUTEX for mutex issue
 *
 * Comment       : ! no need to stop filters on the channel.
 *
 *****************************************************************************/                                               
tTDAL_DMX_Error     TDAL_DMX_Register_Channel_Callback (tTDAL_DMX_ChannelId          ChannelId,
                                                        tTDAL_DMX_ChannelCallbackFct pCallback);
/******************************************************************************
 * Function Name : TDAL_DMX_Set_Channel_PID
 *
 * Description   : this function set a PID on a channel
 *
 * Side effects  :
 *                  return  kTDAL_DMX_NO_ERROR if all is correct
 *                          kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if bad Id
 *                          kTDAL_DMX_ERROR_MUTEX for mutex error
 *                          kTDAL_DMX_ERROR_UNABLE_TO_GRANT 
 *                          kTDAL_DMX_ERROR_BAD_PARAMETERS for bad PID
 *              not used -> kTDAL_DMX_ERROR_PID_IN_USE if PID is already used
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Set_Channel_PID        (tTDAL_DMX_ChannelId    ChannelId,
                                                     uint16_t               Pid);
/******************************************************************************
 * Function Name : TDAL_DMX_Set_Filter
 *
 * Description   : This function sets a hardware section filter on channel 
 *
 *              Return  kTDAL_DMX_NO_ERROR if all is correct
 *                      kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if channel is invalid
 *                      or channel not used or bad PID on the channel
 *                      kTDAL_DMX_ERROR_INVALID_FILTER_ID if filter is invalid
 *                      kTDAL_DMX_ERROR_BAD_PARAMETERS if size is bad
 *                      kTDAL_DMX_ERROR_MUTEX for mutex error
 *                      kTDAL_DMX_ERROR_UNABLE_TO_GRANT for error 
 *
 * Comment       : size limited to kTDAL_DMX_DEPTH_SIZE
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Set_Filter             (tTDAL_DMX_ChannelId    ChannelId,
                                                     tTDAL_DMX_FilterId     FilterId,
                                                     uint32_t               Filter_track,
                                                     uint8_t                Size,
                                                     uint8_t                *pFilter,
                                                     uint8_t                *pMask,
                                                     uint8_t                *pMode,
                                                     tTDAL_DMX_FilterCRCMode eCRCMode);
/******************************************************************************
 * Function Name : TDAL_DMX_Control_Channel
 *
 * Description   : this function is used to control a channel
 *                  ( enabling, stopping, resetting)
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kSFILTER_ERROR_INVALID_CHANNEL_ID if no channel is invalid
 *                  or channel not used or bad PID
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS for bad argument ctrl
 *                  kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                  kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL if channel is not used
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Control_Channel(tTDAL_DMX_ChannelId    ChannelId,
                                             tTDAL_DMX_Ctrl         Ctrl);

/******************************************************************************
 * Function Name : TDAL_DMX_StreamHandleGet
 *
 * Description   : this function is used to get The streamId of a channel channel
 *                  
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kSFILTER_ERROR_INVALID_CHANNEL_ID if no channel is invalid
 *                  or channel not used or bad PID
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS for bad argument ctrl
 *                  kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                  kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL if channel is not used
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error
TDAL_DMX_StreamHandleGet (tTDAL_DMX_ChannelId ChannelId,
                          uint32_t* pStreamHandle);
/******************************************************************************
 * Function Name : TDAL_DMX_Control_Filter
 *
 * Description   : this function is used to control a filter
 *                  (enabling, stopping, resetting)
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kSFILTER_ERROR_INVALID_FILTER_ID if no filter is invalid
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS if bad argument
 *                  kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                  kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER if filter is not used
 *                  kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to start or
 *                  to stop the filter
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Control_Filter(tTDAL_DMX_FilterId     FilterId,
                                            tTDAL_DMX_Ctrl         Ctrl);
/******************************************************************************
 * Function Name : TDAL_DMX_Free_Filter
 *
 * Description   : this function is used to free a filter from a channel
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kSFILTER_ERROR_INVALID_FILTER_ID if filter Id is invalid
 *                  kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                  kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER if filter is not used
 *                  kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to stop or to
 *                  deallocate the filter 
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Free_Filter(tTDAL_DMX_FilterId      FilterId);
/******************************************************************************
 * Function Name : TDAL_DMX_Free_Channel
 *
 * Description   : this function is used to free a channel
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kSFILTER_ERROR_INVALID_CHANNEL_ID if channel Id is invalid
 *                  kTDAL_DMX_ERROR_MUTEX for mutex issue
 *                  kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL if channel is not used
 *                  kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to stop a filter,
 *                  to deallocate a filter or the stream 
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Free_Channel(tTDAL_DMX_ChannelId     ChannelId);
/******************************************************************************
 * Function Name : TDAL_DMX_Free_Buffer
 *
 * Description   : this function is used to free a buffer allocated by the demux filter
 *                 module when a section buffer is received     
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS if buffer is null
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Free_Buffer(void * buffer);

/******************************************************************************
 * Function Name : TDAL_DMX_GetDemuxId
 *
 * Description   : this function is used to give te list of demuxId 
 *                 which must be used by client     
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct        
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
#ifndef USE_MULTI_PLATFORM_SUPPORT
tTDAL_DMX_Error     TDAL_DMX_GetDemuxId(uint8_t *nbDemuxId, tTDAL_DMX_DemuxId **demuxId);
#else  /* USE_MULTI_PLATFORM_SUPPORT */
// Provided additional platform ID for each demux ID.
tTDAL_DMX_Error     TDAL_DMX_GetDemuxId(uint8_t *nbDemuxId, tTDAL_DMX_DemuxId **demuxId, uint8_t **platformId);
#endif /* USE_MULTI_PLATFORM_SUPPORT */

#ifdef NAGRA_MEDIACAS
/******************************************************************************
 * Function Name : TDAL_DMX_Set_Channel_SessionID
 *
 * Description   : this function is used to pass transport session ID
 *                 that will be used to manage MediaCas desc session *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/

tTDAL_DMX_Error     TDAL_DMX_Set_Channel_SessionID(tTDAL_DMX_DemuxId DemuxId, uint32_t pid, void* sessionID, uint8_t size, uint8_t scramblingMode);

/******************************************************************************
 * Function Name : TDAL_DMX_Set_ScramblingMode
 *
 * Description   : this function is used to pass scrambling modes
 *                 that will be used to manage MediaCas desc session
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error     TDAL_DMX_Set_ScramblingMode(tTDAL_DMX_DemuxId DemuxId, uint8_t scramblingMode);
#endif /* NAGRA_MEDIACAS */

void TDAL_DMX_SetAPI(uint8_t demuxId, uint32_t api_type);

/******************************************************************************
 * Function Name : TDAL_DMX_GetDemuxByChannelId
 *
 * Description   : This function is used to obtain demux id that corresponds to the specified
 *                 channel id.
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error TDAL_DMX_GetDemuxByChannelId(tTDAL_DMX_ChannelId ChannelId,
    tTDAL_DMX_DemuxId *demuxId);

/******************************************************************************
 * Function Name : TDAL_DMX_RegisterTSErrorMonitoringCallback
 *
 * Description   : this function is used to register TS error monitoring callback
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_NOT_INITIALIZED
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error TDAL_DMX_RegisterTSErrorMonitoringCallback(
        tTDAL_DMX_DemuxId demuxId, int8_t *clientId,
        tTDAL_DMX_TSErrorMonitoringCallback pCallback);

/******************************************************************************
 * Function Name : TDAL_DMX_UnregisterTSErrorMonitoringCallback
 *
 * Description   : this function is used to unregister TS error monitoring callback
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_NOT_INITIALIZED
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error TDAL_DMX_UnregisterTSErrorMonitoringCallback(
        tTDAL_DMX_DemuxId demuxId, int8_t clientId);

/******************************************************************************
 * Function Name : TDAL_DMX_SetTSErrorMonitoringParameters
 *
 * Description   : this function is used to set TS error monitoring parameters.
 *                 If CCErrorNumber is set to 0, TS error monitoring will be disabled, otherwise it will be enabled.
 *                 TS error monitoring will be automatically disabled just before TS error monitoring callback is triggered.
 *
 * Side effects  :
 *          return  kTDAL_DMX_NO_ERROR if all is correct
 *                  kTDAL_DMX_ERROR_NOT_INITIALIZED
 *                  kTDAL_DMX_ERROR_BAD_PARAMETERS
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMX_Error TDAL_DMX_SetTSErrorMonitoringParameters(tTDAL_DMX_DemuxId demuxId,
        int8_t clientId, tTDAL_DMX_TSErrorMonitoringParameters* params);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DMX_H_*/

/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PTSM
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_ptsm.h $
 *            $Rev: 441 $
 *            $Date: 2011-09-28 13:56:52 +0200 (Wed, 28 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION : TDAL PTSM header.
 *
 *****************************************************************************/

#ifndef _TDAL_PTSM_H_
#define _TDAL_PTSM_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum {
    eTDAL_PTSM_NO_ERROR,
    eTDAL_PTSM_BAD_PARAMETER,
    eTDAL_PTSM_ERROR,
    eTDAL_PTSM_COMPARISON_PASSED,
    eTDAL_PTSM_IMMEDIATE_MATCH
}tTDAL_PTSM_ErrorCode;

typedef enum {
    eTDAL_PTSM_MAIN,
    eTDAL_PTSM_AUX
}tTDAL_PTSM_Path;

typedef enum {
    eTDAL_PTSM_PCR_STARTED,
    eTDAL_PSTM_TIME_OUT,            /* no PCR in the decoded stream */
    eTDAL_PTSM_PCR_DISCONTINUITY,   /* a PCR discontinuity has been detected */
    eTDAL_PTSM_COMPARISON_STOPPED   /* module was terminated while a comparison was running */
}tTDAL_PTSM_Status;

typedef struct{
    uint32_t clock_base_1msb;
    uint32_t clock_base_32lsb;
}tTDAL_PTSM_Pts;                    /* PTS in unit of 90 kHz (33bits) */

typedef int32_t  tTDAL_PTSM_Delay;  /* in milliseconds */

typedef uint32_t tTDAL_PTSM_Handle; /* client handle */

typedef uint16_t tTDAL_PTSM_DecoderId; /* Id of decoder which will be used for obtaining STC */

typedef uint32_t tTDAL_PTSM_ComparisonId;   /* Id of a comparison */

/* Callback types */
typedef void (*tTDAL_PTSM_CallbackMatch)(tTDAL_PTSM_ComparisonId comparisonId);
typedef void (*tTDAL_PTSM_CallbackError)(tTDAL_PTSM_Status status);

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Init ( void );

tTDAL_PTSM_ErrorCode TDAL_PTSM_Terminate ( void );

tTDAL_PTSM_ErrorCode TDAL_PTSM_GetCurrentStc(
    tTDAL_PTSM_DecoderId decoderId,
    tTDAL_PTSM_Pts*      pStc
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Register(
    tTDAL_PTSM_Handle*       pHandle,
    tTDAL_PTSM_CallbackMatch callbackMatch,
    tTDAL_PTSM_CallbackError callbackError
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_UnRegister(
    tTDAL_PTSM_Handle handle
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Diff_PTS_STC(
    tTDAL_PTSM_DecoderId decoderId,
    tTDAL_PTSM_Pts*      pPts,
    tTDAL_PTSM_Delay*    pDelay
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Start(
    tTDAL_PTSM_Handle       handle,
    tTDAL_PTSM_DecoderId    decoderId,
    tTDAL_PTSM_ComparisonId comparisonId,
    tTDAL_PTSM_Pts*         pPts,
    tTDAL_PTSM_Delay        delay
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Stop(
    tTDAL_PTSM_Handle       handle,
    tTDAL_PTSM_ComparisonId comparisonId
);





tTDAL_PTSM_ErrorCode TDAL_PTSM_Ext_GetCurrentStc(
    tTDAL_PTSM_Path path,
    tTDAL_PTSM_Pts* pStc
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Ext_Diff_PTS_STC(
    tTDAL_PTSM_Path     path,
    tTDAL_PTSM_Pts*     pPts,
    tTDAL_PTSM_Delay*   pDelay
);

tTDAL_PTSM_ErrorCode TDAL_PTSM_Ext_Start(
    tTDAL_PTSM_Path         path,
    tTDAL_PTSM_Handle       handle,
    tTDAL_PTSM_ComparisonId comparisonId,
    tTDAL_PTSM_Pts*         pPts,
    tTDAL_PTSM_Delay        delay
);



#ifdef __cplusplus
}
#endif


#endif /*_TDAL_PTSM_H_*/

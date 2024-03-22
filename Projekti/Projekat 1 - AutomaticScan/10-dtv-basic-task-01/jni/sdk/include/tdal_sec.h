/******************************************************************************
 *                         COPYRIGHT 2009 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME:     TDAL_SEC
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_sec.h $
 *      $Rev: 972 $
 *      $Date: 2011-12-22 18:14:44 +0100 (Thu, 22 Dec 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

#ifndef _TDAL_SEC_H_
#define _TDAL_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************
 *  Typedefs                                           *
 *******************************************************/
typedef enum
{
    eTDAL_SEC_NO_ERROR,
    eTDAL_SEC_NOT_DONE
}tTDAL_SEC_ErrorCode;

/*******************************************************
 *  Functions Declarations                             *
 *******************************************************/
tTDAL_SEC_ErrorCode TDAL_SEC_GetUniqueID( uint8_t *pBuffer, int *pSize);
tTDAL_SEC_ErrorCode TDAL_SEC_GetSeed( uint8_t *pBuffer, int *pSize);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_SEC_H_*/

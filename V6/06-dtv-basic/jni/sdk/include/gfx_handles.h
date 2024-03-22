/******************************************************************************
 *                         COPYRIGHT 2001 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: TELETEXT
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/packages/ttxt/core/trunk/inc/ttxt.h $
 *            $Rev: 55578 $
 *            $Date: 2009-02-19 10:23:43 +0100 (jeu. 19 févr. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 * header for the teletext module
 *
 *****************************************************************************/

#ifndef _GFX_HANDLE_
#define _GFX_HANDLE_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif
#include "tdal_gfx.h"
tTDAL_GFX_RegionHandle TTXT_handle;
tTDAL_GFX_RegionHandle	SubtitleHandle;
tTDAL_GFX_RegionHandle  mhegHandle;
#ifdef __cplusplus
}
#endif


#endif /*_GFX_HANDLE_*/

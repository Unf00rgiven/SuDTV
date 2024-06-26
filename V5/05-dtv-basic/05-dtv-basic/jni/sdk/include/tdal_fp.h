/******************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: TDAL FORNT PANEL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_fp.h $
 *            $Rev: 441 $
 *            $Date: 2011-09-28 13:56:52 +0200 (Wed, 28 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Front panel
 *
 *****************************************************************************/

#ifndef _TDAL_FP_H_
#define _TDAL_FP_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif



/*******************************************************/
/*              Defines                                */
/*******************************************************/

#define kTDAL_FP_NO_ID              -1
/* number to digit to manage */
#define kTDAL_FP_NB_DIGIT           4

#define kTDAL_FP_NO_KEY             0x00
#define kTDAL_FP_POWER_KEY          0x01
#define kTDAL_FP_SELECT_KEY         0x02
#define kTDAL_FP_MENU_KEY           0x04
#define kTDAL_FP_RIGHT_ARROW_KEY    0x08
#define kTDAL_FP_LEFT_ARROW_KEY     0x10
#define kTDAL_FP_UP_ARROW_KEY       0x20
#define kTDAL_FP_DOWN_ARROW_KEY     0x40
#define kTDAL_FP_EXIT_KEY           0x80
#define kTDAL_FP_PROGRAM_NEXT_KEY   0x0f
#define kTDAL_FP_PROGRAM_PREV_KEY	0xf0

#define kTDAL_FP_OK_STRING "OK"
#define kTDAL_FP_NOK_STRING "NOK"
#define kTDAL_FP_STRING_PVR_PLAY "PLAY"

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum
{
    eTDAL_FP_INFO_BOOTING,
    eTDAL_FP_INFO_ON,
    eTDAL_FP_INFO_OFF,
    eTDAL_FP_INFO_RECORD,
    eTDAL_FP_INFO_FLASH_ERASE,
    eTDAL_FP_INFO_FLASH_RW,
    eTDAL_FP_INFO_RCVDATA,
    eTDAL_FP_INFO_RCVINFRARED,
    eTDAL_FP_INFO_SCANNING,
    eTDAL_FP_INFO_HOUR,
    eTDAL_FP_INFO_MIN,
    eTDAL_FP_INFO_STRING,
    eTDAL_FP_INFO_VIDEO_START,
    eTDAL_FP_INFO_SHOW_MENU,
    eTDAL_FP_INFO_FE_LOCK,
    eTDAL_FP_INFO_DOWNLOAD,
    eTDAL_FP_INFO_ERROR,
    eTDAL_FP_INFO_CHANNELNUMBER,
    eTDAL_FP_INFO_PVR
} tTDAL_FP_Info;


typedef enum {
	eTDAL_FP_NO_ERROR,
	eTDAL_FP_ERROR
}tTDAL_FP_ErrorCode;

typedef int16_t tTDAL_FP_ClientId;

typedef uint8_t tTDAL_FP_KeyCode;

typedef enum {
	eTDAL_FP_KEY_PRESSED,
	eTDAL_FP_KEY_RELEASED,
	eTDAL_FP_KEY_HELD_DOWN
}tTDAL_FP_KeyStatus;

typedef void (*tTDAL_FP_CallbackFct) (
    tTDAL_FP_KeyCode   KeyCode,
    tTDAL_FP_KeyStatus KeyStatus
);

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
IMPORT tTDAL_FP_ErrorCode TDAL_FP_Init(
    void
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_Terminate(
    void
);

IMPORT tTDAL_FP_ClientId TDAL_FP_Register(
    tTDAL_FP_CallbackFct callback      /* I: Function called upon key events */
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_UnRegister(
    tTDAL_FP_ClientId clientId         /* I: Id of the client to unregister */
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_SetInfo(
	tTDAL_FP_Info eInfo,
	const void* param
);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_FP_H_*/

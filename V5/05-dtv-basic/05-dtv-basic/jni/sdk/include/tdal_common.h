/*******************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES       *
 *******************************************************
 *
 * MODULE NAME: TDAL_Common
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_common.h $
 *            $Rev: 11603 $
 *            $Date: 2013-11-08 14:11:41 +0100 (Fri, 08 Nov 2013) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 ********************************************************/

#ifndef _TDAL_COMMON_H_
#define _TDAL_COMMON_H_

/*******************************************************/
/*              Includes                                                                  */
/*******************************************************/


/*******************************************************/
/*              Defines                                                                    */
/*******************************************************/

#ifdef __DEBUG_MEM_LEAK__ /* Add utitily for memory leak debbuging */
#define TDAL_Malloc(_b)     TDAL_DebugMalloc(__FILE__,__LINE__,(_b))
#define TDAL_Free(_b)       TDAL_DebugFree(__FILE__,__LINE__,(_b))
#endif

/*******************************************************/
/*              Macros                                                                    */
/*******************************************************/


/*******************************************************/
/*              Typedefs                                                                 */
/*******************************************************/

/* TDAL error codes */
typedef enum
{
    TDAL_NO_ERR,    /* No error. */
    TDAL_TIMEOUT,   /* A timeout occurred. */
    TDAL_BAD_ARG,   /* Wrong argument value(s). */
    TDAL_NOT_DONE   /* Command not done. */
} TDAL_Error;

/* Module trace identifiers */
typedef enum
{
    eTDAL_UART = 200,
    eTDAL_DESC,
    eTDAL_VBI,
    eTDAL_SMC,
    eTDAL_PTSM,
    eTDAL_GFX,
    eTDAL_COMMON,
    eTDAL_DMX,
    eTDAL_AVS,
    eTDAL_KBD,
    eTDAL_NVM,
    eTDAL_CRYPT,
    eTDAL_FLA,
    eTDAL_DMD,
    eTDAL_DISP,
    eTDAL_OUTPUT,
    eTDAL_FP,
    eTDAL_AV,
    eTDAL_CI,
    eTDAL_DENC,
    eTDAL_TSOUT,
    eTDAL_TSIN,
    eTDAL_TS_ROUTE,
    eTDAL_HDMI,
    eTDAL_HDMI_CEC,
    eTDAL_TS_SOURCE,
    eTDAL_TS_SINK,
    eTDAL_ATA,
    eTDAL_FS,
    eTDAL_RPC,
    eTDAL_RPC_COMM,
    eTDAL_SPEAKERS,
    eTDAL_PANEL,
    eTDAL_VBI_SLICER,
    eTDAL_ANA_INPUT,
    eTDAL_ANA_AUD,
    eTDAL_ANA_VID,
    eTDAL_HDMI_IN,
    eTDAL_SCART_INPUT,
    eTDAL_SEC,
    eTDAL_PERSO,
    eTDAL_TS_MERGE,
    eTDAL_TS,
    eTDAL_AVSETTINGS,
    eTDAL_PVR,
    eTDAL_PVR_FILE,
    eTDAL_MARLIN_CRYPT,
    eTDAL_DMX_SW,
    eTDAL_LPM,
    eTDAL_ENC,
    eTDAL_MUXER,
    eTDAL_MUXER_PARSER
} tTDAL_ModuleTraceID;


typedef enum
{
    eTDAL_JUMP_BOOTLOADER,
    eTDAL_JUMP_APPLI,
    eTDAL_JUMP_MAX
}tTDAL_JUMP_Type;

typedef enum
{
	eTDAL_STANDBY_ON = 0, /*S0*/
	eTDAL_STANDBY_ACTIVE,
	eTDAL_STANDBY_PASSIVE,
	eTDAL_STANDBY_DEEP_SLEEP
}tTDAL_StandbyMode;


typedef enum
{
	eTDAL_POWER_ON_FRONT_POWER_KEY = 0,    /* 1 */
	eTDAL_POWER_ON_RCU_POWER_KEY,          /* 2 */
	eTDAL_POWER_ON_RCU_0_KEY = 0x10,       /* 0x10 */
	eTDAL_POWER_ON_RCU_1_KEY,              /* 0x11 */
	eTDAL_POWER_ON_RCU_2_KEY,              /* 0x12 */
	eTDAL_POWER_ON_RCU_3_KEY,              /* 0x13 */
	eTDAL_POWER_ON_RCU_4_KEY,              /* 0x14 */
	eTDAL_POWER_ON_RCU_5_KEY,              /* 0x15 */
	eTDAL_POWER_ON_RCU_6_KEY,              /* 0x16 */
	eTDAL_POWER_ON_RCU_7_KEY,              /* 0x17 */
	eTDAL_POWER_ON_RCU_8_KEY,              /* 0x18 */
	eTDAL_POWER_ON_RCU_9_KEY,              /* 0x19 */
	eTDAL_POWER_ON_AUTO_RESET = 0x20,
	eTDAL_POWER_ON_AC_POWER_SUPPLY,
	eTDAL_POWER_ON_WAKEUP_TIMER,
    eTDAL_POWER_ON_CEC_ACTIVATION,
	eTDAL_POWER_ON_MISC
}tTDAL_PowerOnStatus;


typedef void * TDAL_mutex_id;
typedef void * TDAL_sema_id;
typedef void * TDAL_task_id;
typedef void * TDAL_queue_id;

typedef int16_t   TDAL_queue_num;     /* queue depth */
typedef uint32_t TDAL_queue_size;      /* queue width */

/*******************************************************/
/*              Variables Declarations (IMPORT)                              */
/*******************************************************/


/*******************************************************/
/*              Functions Declarations                                             */
/*******************************************************/

 void TDAL_Init(void);
 void TDAL_Term(void);
 void TDAL_Reset(void);
 void TDAL_Jump(tTDAL_JUMP_Type eJumpType);

 void TDAL_EnterLowPowerTimeout(uint32_t timeout);
 uint8_t TDAL_GetPowerOnStatus(tTDAL_PowerOnStatus   *status,
                                                              uint32_t                        *lowPowerDuration,
                                                              uint32_t                         ulStandbyReference);
 void TDAL_SetMemoryPartition(void* pPartition);

 uint8_t TDAL_GetUpTime(uint32_t   *upTime);
 uint8_t TDAL_GetTime(uint32_t   *time);
 uint8_t TDAL_SetTime(uint32_t   time);

#ifdef __DEBUG_MEM_LEAK__ /* Add utitily for memory leak debbuging */
 void* TDAL_DebugMalloc(const char *zFile, int iLine, uint32_t memorySize);
 void TDAL_DebugFree(const char *zFile, int iLine, void*buffer);
#else
 void* TDAL_Malloc(uint32_t memorySize);
 void TDAL_Free(void*buffer);
#endif


 void TDAL_StackInfo(void);
 void TDAL_TaskStart(unsigned long, void*);
 void TDAL_TaskDelete(unsigned long, void*);
 void TDAL_MemoryInfo(void);

 void TDAL_CreateMutex(TDAL_mutex_id * MutexID);
 void TDAL_DeleteMutex(TDAL_mutex_id MutexID);
 void TDAL_LockMutex(TDAL_mutex_id MutexID);
 void TDAL_UnlockMutex(TDAL_mutex_id MutexID);

 void TDAL_CreateSemaphore(int count,TDAL_sema_id *semaphoreID);
 void TDAL_DeleteSemaphore(TDAL_sema_id semaphoreID);
 void TDAL_SignalSemaphore(TDAL_sema_id semaphoreID);
 void TDAL_WaitSemaphore(TDAL_sema_id semaphoreID);
 void TDAL_WaitSemaphoreTimeout(TDAL_sema_id semaphoreID, uint32_t timeout);

 void TDAL_CreateTask(TDAL_task_id *taskID, void (*entry) (void*), void *arg);
 void TDAL_DeleteTask(TDAL_task_id taskID);
 void TDAL_DelayTask(__attribute__((unused)) uint32_t timeInMs);

 void TDAL_CreateQueue(TDAL_queue_num  maxNumber,
                                                     TDAL_queue_size msgSize,
                                                     TDAL_queue_id   *queueID);
 void TDAL_DeleteQueue(TDAL_queue_id queueID);
 void TDAL_Enqueue(TDAL_queue_id queueID, void *message);
 void TDAL_Dequeue(TDAL_queue_id queueID, void *message);
 void TDAL_DequeueTimeout(TDAL_queue_id queueID, void *message, int ms, int *timeoutOccured);
 void TDAL_EnqueueTimeout(TDAL_queue_id queueID, void *message, int ms);
 void TDAL_DisconnectResources(void);

 void TDAL_SendDebugCommand(const char *debugCommand);

 void   TDAL_SetWakeUpTimer(uint32_t timeout);
 void   TDAL_GetWakeUpTime(uint32_t *timeout);
#if defined(SELECT_STB_PBI_SDQW_5105)
/*Common Interface to access to the PIO extended*/
extern void EXPANDED_PIO_OUTPUT(int PIN, int VALUE)      ;
#endif

 unsigned int TDAL_GetCurrentThreadHandle();

/*******************************************************/
/* POWER MANAGEMENT FUNCTIONS                          */
/*******************************************************/
TDAL_Error TDAL_COMMON_SetStandbyMode(tTDAL_StandbyMode newMode);
TDAL_Error TDAL_COMMON_GetStandbyMode(tTDAL_StandbyMode* mode);
TDAL_Error TDAL_COMMON_reboot_function(void);

#endif /*_TDAL_COMMON_H_*/

#include <stdio.h>
#include <string.h>  
#include <unistd.h>

#include "tdal_common.h"
#include "tdal_dmd.h"

#include "dtvplayer_utils.h"

static const tTDAL_DMD_FE FE_INSTANCE = eTDAL_DMD_FE_0;
// Tuner callback functions declaration
static void DtvPlayer_Tuner_NotifyStatusCbFn(tTDAL_DMD_FE ucFeID, tTDAL_DMD_NotifyStatus  status);
static void DtvPlayer_Tuner_NotifyProgressCbFn (tTDAL_DMD_FE ucFeID, tTDAL_DMD_NotifyProgress  progress);

/**
 * @brief This function is used to initialize tuner module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Init()
{
    tTDAL_DMD_Error dmd_err = eTDAL_DMD_NO_ERROR;

    // TODO: Call DMD Module initialization in SDK
    // NOTE 1: Find TDAL_DMD_Init function in API and provide appropriate params
    // NOTE 2: Check for errors
    // NOTE 3: Open tuner instance with finding TDAL_DMD_OpenFEInstance function in API and provide appropriate params
    // NOTE 3.1: use FE_INSTANCE as frontend dentifier everywhere
    // NOTE 3.2: provede DtvPlayer_Tuner_NotifyStatusCbFn as callback for tune status
    // NOTE 3.3: provede DtvPlayer_Tuner_NotifyProgressCbFn as callback for tune progress status

    dmd_err = TDAL_DMD_Init();
    if(dmd_err != eTDAL_DMD_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMD_Init failed. Error: [%d]", __FUNCTION__, __LINE__, dmd_err);
        return ePLAYER_ERROR_GENERAL;
    }

    // We can have multiple tuners on STB
    // Tuner numbers are contained in enum: tTDAL_DMD_FE
    // For this excersize we know we want to use only 1 tuner and support only "Single-Tuner" usage
    // so we can hardcode this value
    dmd_err = TDAL_DMD_OpenFEInstance(FE_INSTANCE, DtvPlayer_Tuner_NotifyStatusCbFn, DtvPlayer_Tuner_NotifyProgressCbFn);
    if(dmd_err != eTDAL_DMD_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMD_OpenFEInstance failed. Error: [%d]", __FUNCTION__, __LINE__, dmd_err);
        return ePLAYER_ERROR_GENERAL;
    }

    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to tune to desired frequency
 * 
 * @param frequency 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Tune(uint32_t frequency)
{
    tTDAL_DMD_Error dmd_err = eTDAL_DMD_NO_ERROR;
    tTDAL_DMD_TunData dummyTuneData;
    // Dummy tune data not used by tuner
    dummyTuneData.Ip.Protocol = eTDAL_DMD_IP_PROTOCOL_TCP;
    dummyTuneData.Ip.Source   = eTDAL_DMD_IP_SOURCE_RTP;
    dummyTuneData.Ip.pOther   = (uint8_t*) "file:///data/dtvdata/test.ts";
    // TODO: Call Tune in SDK
    // NOTE 1: Find TDAL_DMD_Tune function in API and provide appropriate params
    // NOTE 2: Check for errors
    // NOTE 2.1: Use FE_INSTANCE as frontend dentifier everywhere
    // NoTE 2.2: Use dummyTuneData to satisfy API
    dmd_err = TDAL_DMD_Tune(FE_INSTANCE, &dummyTuneData, frequency);
    if(dmd_err != eTDAL_DMD_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMD_Tune failed. Error: [%d]", __FUNCTION__, __LINE__, dmd_err);
        return ePLAYER_ERROR_GENERAL;
    }
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to untune from previously tuned frequency
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Untune()
{
    tTDAL_DMD_Error dmd_err = eTDAL_DMD_NO_ERROR;

    // TODO: Call Tune in SDK
    // NOTE 1: Find TDAL_DMD_Unlock function in API and provide appropriate params
    // NOTE 2: Check for errors
    // NOTE 2.1: use FE_INSTANCE as frontend dentifier everywhere
    dmd_err = TDAL_DMD_Unlock(FE_INSTANCE);
    if(dmd_err != eTDAL_DMD_NO_ERROR)
    {
        LOGE("[%s:%d] TDAL_DMD_Unlock failed. Error: [%d]", __FUNCTION__, __LINE__, dmd_err);
        return ePLAYER_ERROR_GENERAL;
    }

    // HACK: Flush sequence
    TDAL_DMD_Terminate();
    DtvPlayer_Tuner_Init();

    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to deinitialize tuner module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Tuner_Deinit()
{
    // NOTE 1: use FE_INSTANCE as frontend dentifier everywhere
    TDAL_DMD_CloseFEInstance(FE_INSTANCE);
    TDAL_DMD_Terminate();

    return ePLAYER_ERROR_OK;
}

/**
 * @brief Tuner status notification callback
 * 
 * @param ucFeID Frontend/tuner index where event has occured
 * @param status status enumeration describing kind of event that occured
 */
static void DtvPlayer_Tuner_NotifyStatusCbFn(tTDAL_DMD_FE ucFeID, tTDAL_DMD_NotifyStatus  status)
{
    // TODO: notice tuner states
    // NOTE 1: Check logcat output
    
    switch(status)
    {
        case eTDAL_DMD_LOCKED:
        {
            printf("TUNER LOCKED\n");
            LOGE("[%s:%d] callback called. Tuner LOCKED", __FUNCTION__, __LINE__);
            break;
        }
        case eTDAL_DMD_LOCK_FAILED:
        case eTDAL_DMD_FOUND:
        case eTDAL_DMD_END_OF_RANGE:
        case eTDAL_DMD_SIGNAL_LOST:
        case eTDAL_DMD_SIGNAL_RETURN:
        case eTDAL_DMD_SATIP_SERVER_DROPPED:
        case eTDAL_DMD_SATIP_SERVER_LIST_UPDATE:
        default:
        {
            printf("TUNER LOCK FAILED\n");
            LOGE("[%s:%d] callback called. Tuner NOT LOCKED. Status: [%d]", __FUNCTION__, __LINE__, status);
        }
    }
}

/**
 * @brief Tuner callback notification callback
 * 
 * @param ucFeID Frontend/tuner index where event has occured
 * @param progress progress of operation requested(scan progress)
 */
static void DtvPlayer_Tuner_NotifyProgressCbFn (tTDAL_DMD_FE ucFeID, tTDAL_DMD_NotifyProgress  progress) {
    // Not used but we need it to satisfy API
    LOGE("TUNER PROGRESS: %d", progress);
}

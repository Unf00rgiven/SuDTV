#include <jni.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

#include "tdal_common.h"
#include "tdal_dmd.h"
#include "tdal_disp.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_ts.h"


#include "dtvplayer_pipeline.h"


/**
 * @brief This function is used to initialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Init()
{
    tTDAL_TSROUTE_Error error;

    // DONE: Call Pipeline Module initialization in SDK
    error = TDAL_TSROUTE_Init();
    if(error != eTDAL_TSROUTE_NO_ERROR) {
        LOGE("[%s:%d] DtvPlayer_Pipeline_Init failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return -1;
    }
    // NOTE 1: Find TDAL_TSROUTE_Init function in API and provide appropriate params
    // NOTE 2: Check for errors
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to connect input data path
 * 
 * @param handleTuner tunner handle
 * @param handleDemux demux handle
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_ConnectInput()
{
    tTDAL_TSROUTE_Error error = eTDAL_TSROUTE_NO_ERROR;

    tTDAL_TS_Object tuner = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_SOURCE, eTDAL_TS_TYPE_TUNER, eTDAL_DMD_FE_0);
    tTDAL_TS_Object demux = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_DESTINATION, eTDAL_TS_TYPE_DEMUX, eTDAL_DMX_DEMUX0);

    // DONE: Connect input data path between tuner and demux hhere
    error = TDAL_TSROUTE_Connect(tuner, demux);
    if(error != eTDAL_TSROUTE_NO_ERROR) {
        LOGE("[%s:%d] DtvPlayer_Pipeline_ConnectInput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return -1;
    }
    // NOTE 1: Find TDAL_TSROUTE_Connect function in API and provide appropriate params
    // NOTE 2: Check for errors
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to disconnect input data path
 * Input data path is path between tuner and demux
 * 
 * @param handleTuner tunner handle
 * @param handleDemux demux handle
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_DisconnectInput()
{
    tTDAL_TSROUTE_Error error = eTDAL_TSROUTE_NO_ERROR;

    tTDAL_TS_Object tuner = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_SOURCE, eTDAL_TS_TYPE_TUNER, eTDAL_DMD_FE_0);
    tTDAL_TS_Object demux = mTDAL_TS_GetObjectId(eTDAL_TS_CLASS_DESTINATION, eTDAL_TS_TYPE_DEMUX, eTDAL_DMX_DEMUX0);

    // DONE: Connect input data path between tuner and demux hhere
    error = TDAL_TSROUTE_Disconnect(tuner, demux);
    if(error != eTDAL_TSROUTE_NO_ERROR) {
        LOGE("[%s:%d] DtvPlayer_Pipeline_DisconnectInput failed. Error: [%d]", __FUNCTION__, __LINE__, error);
        return -1;
    }
    // NOTE 1: Find TDAL_TSROUTE_Disconnect function in API and provide appropriate params
    // NOTE 2: Check for errors
    return ePLAYER_ERROR_OK;
}

/**
 * @brief This function is used to deinitialize pipeline module
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Pipeline_Deinit()
{
    TDAL_TSROUTE_Term();
    return ePLAYER_ERROR_OK;
}

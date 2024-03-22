#include <stdio.h>

#include "tkel.h"
#include "dtvplayer.h"
#include "dtvplayer_comm.h"
#include "dtvplayer_tuner.h"
#include "dtvplayer_demux.h"
#include "dtvplayer_pipeline.h"

#include "tdal_common.h"

/**
 * @brief Module entry point.
 * This function is used to initiate all modules that are being used
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Init()
{
    tErrorCode error;
    TKEL_Init();
    TDAL_Init();

    // Initialize player here
    error = DtvPlayer_Tuner_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner init failed!\n");
        return -1;
    } else {
        printf("Tuner init success!\n");
    }

    error = DtvPlayer_Demux_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux init failed!\n");
        return -1;
    } else {
        printf("Demux init success!\n");
    }

    // DONE: Initalize Pipeline Module here
    error = DtvPlayer_Pipeline_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline init failed!\n");
        return -1;
    }
    printf("Pipeline init success!\n");

    // NOTE 1: Check for errors

    // DONE: Initalize Pipeline input data path here
    error = DtvPlayer_Pipeline_ConnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline input data path failed!\n");
        return -1;
    }
    printf("Pipeline input data path success!\n");
    // NOTE 1: Check for errors

    return ePLAYER_ERROR_OK;
}

/**
 * @brief Module deinitialization.
 * This finction is used to deinitialize all used modules
 * 
 * @return tErrorCode 
 */
tErrorCode DtvPlayer_Deinit()
{
    tErrorCode error;

    // DONE: Disconnect Pipeline input data path here
    error = DtvPlayer_Pipeline_DisconnectInput();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline input data path disconnect failed!\n");
        return -1;
    }
    printf("Pipeline input data path disconnect success!\n");
    // NOTE 1: Check for errors

    // Initialize player here
    error = DtvPlayer_Tuner_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Tuner deinit failed!\n");
        return -1;
    } else {
        printf("Tuner deinit success!\n");
    }

    error = DtvPlayer_Demux_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Demux deinit failed!\n");
        return -1;
    } else {
        printf("Demux deinit success!\n");
    }

    // DONE: Terminate Pipeline Module here
    error = DtvPlayer_Pipeline_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Pipeline deinit failed!\n");
        return -1;
    }
    printf("Pipeline deinit success!\n");
    // NOTE 2: Check for errors

    TDAL_Term();
    return ePLAYER_ERROR_OK;
}


/**
 * @brief Use this function to tune to desired frequency
 * This function passes tune request into module where tune is actually implemented.
 * After this function is called, tuner will lock to desired frequency.
 * After this step is completed, it is safe to connect Tuner with DMX.
 * 
 * @param frequency input frequency
 */
void DtvPlayer_Tune(int frequency)
{
    tErrorCode error;
    if(frequency > 0 && frequency < 1e3)
    {
        frequency *= 1e6;
    } else if(frequency >= 1e3 && frequency < 1e6)
    {
        frequency *= 1e3;
    }

    error = DtvPlayer_Tuner_Tune(frequency);
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Tune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Tune success.", __FUNCTION__, __LINE__);
    }
}

/**
 * @brief Use this function to untune from previously tuned frequency
 * If data path between tuner and demux is connected, it is necessary to 
 * disconnect it first before untuning
 */
void DtvPlayer_Untune()
{
    tErrorCode error;

    error = DtvPlayer_Tuner_Untune();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
}


void DtvPlayer_Analyze()
{
    tErrorCode error;

    error = DtvPlayer_Demux_AnalyzePsi();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
}

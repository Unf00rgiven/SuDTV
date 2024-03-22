#include <stdio.h>

#include "dtvplayer.h"
#include "dtvplayer_comm.h"
#include "dtvplayer_tuner.h"

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

    // TODO: Initalize Player Modules here
    // NOTE 1: Initialize tuner module here
    // NOTE 2: Check initialization status in logcat
    // NOTE 3: Check for errors
    error = DtvPlayer_Tuner_Init();
    if (error != ePLAYER_ERROR_OK) {
        LOGE("Tuner initialization failed with error: %d", error);
        return error;
    }

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
    // TODO: Terminate Player Modules here
    // NOTE 1: Deinitialize tuner module
    // NOTE 2: Check for errors
    error = DtvPlayer_Tuner_Deinit();
    if (error != ePLAYER_ERROR_OK) {
        LOGE("Tuner deinitialization failed with error: %d", error);
    }

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

    // TODO: Execute tuning to specified frequency
    // NOTE 1: Call tune function from tuner module
    // NOTE 2: Check for errors
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

    // TODO: Execute untune from prebviously tuned frequency
    // NOTE 1: Call untune function from tuner module
    // NOTE 2: Check for errors
    error = DtvPlayer_Tuner_Untune();
    if(error != ePLAYER_ERROR_OK)
    {
        LOGE("[%s:%d] DtvPlayer_Tuner_Untune failed. Error: [%d]", __FUNCTION__, __LINE__, error);
    } else
    {
        LOGI("[%s:%d] DtvPlayer_Tuner_Untune success.", __FUNCTION__, __LINE__);
    }
}
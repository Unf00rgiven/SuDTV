#include <stdio.h>

#include "dtvplayer.h"
#include "dtvplayer_comm.h"

#include "tdal_common.h"

/**
 * @brief Module entry point.
 * This function is used to initiate all modules that are being used
 * 
 * @return tErrorCode 
 */
void TKEL_Init(void);

tErrorCode DtvPlayer_Init()
{
    // TODO: Initalize DTV SDK module
    // NOTE 1: Call TKEL_Init() first to enable logcat logs form SDK
    // NOTE 2: function TDAL_Init is used to handle main module initialization
    // NOTE 3: Check initialization status in logcat

    // Enable SDK logs
    TKEL_Init();

    // Initialize main module
    TDAL_Init();

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
    // TODO: Terminate DTV SDK module
    // NOTE 1: function TDAL_Term is used to handle main module initialization
    // NOTE 2: Check termination status in logcat
    TDAL_Term();

    return ePLAYER_ERROR_OK;
}

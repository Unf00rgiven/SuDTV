#include <stdio.h>

#include "dtvplayer.h"
#include "dtvplayer_comm.h"

int main() {
    tErrorCode error;

    // Initialize player here
    error = DtvPlayer_Init();
    if(error != ePLAYER_ERROR_OK) {
        printf("Player init failed!\n");
        return -1;
    } else {
        printf("\n--------------------\n");
        printf("Player init success!");
        printf("\n--------------------\n");
    }

    // Main command parser
    // Don't change!
    while (ePLAYER_ERROR_OK == DtvPlayer_Comm_Read());

    // Deinitialize player here
    error = DtvPlayer_Deinit();
    if(error != ePLAYER_ERROR_OK) {
        printf("Player deinit failed!\n");
        return -1;
    } else {
        printf("Player deinit success!\n");
    }
    return 0;
}
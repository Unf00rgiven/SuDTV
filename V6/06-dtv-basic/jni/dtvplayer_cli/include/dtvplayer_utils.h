#ifndef __DTV_PLAYER_UTILS_H__
#define __DTV_PLAYER_UTILS_H__

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "DtvPlayer", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "DtvPlayer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "DtvPlayer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "DtvPlayer", __VA_ARGS__))

#define MESSAGE_BUFFER_SIZE 1024

typedef enum errorCodes
{
    ePLAYER_ERROR_OK = 0,
    ePLAYER_ERROR_GENERAL = -1,
    ePLAYER_ERROR_BAD_ARGS = -2,
    ePLAYER_ERROR_NOT_INITIALIZED = -3,
    ePLAYER_ERROR_NOT_STARTED = -4,
} tErrorCode;

#endif // __DTV_PLAYER_UTILS_H__

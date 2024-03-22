LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= dtvplayer_cli
LOCAL_MODULE_TAGS := optional

LOCAL_SDK_VERSION := current

LOCAL_SRC_FILES := \
    source/main.c \
    source/dtvplayer_comm.c \
    source/dtvplayer_tuner.c \
    source/dtvplayer_demux.c \
    source/dtvplayer.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/../sdk/include


LOCAL_LDLIBS := -llog

LOCAL_SHARED_LIBRARIES := libchal

include $(BUILD_EXECUTABLE)
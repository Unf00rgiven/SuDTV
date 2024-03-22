LOCAL_PATH:= $(call my-dir)
###############################################################################
include $(CLEAR_VARS)

LOCAL_MODULE := libchal
LOCAL_SRC_FILES := libs/libchal.so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_TAGS := optional

include $(PREBUILT_SHARED_LIBRARY)
###############################################################################
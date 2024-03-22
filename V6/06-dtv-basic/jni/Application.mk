APP_ABI := armeabi-v7a

# Should the same as -platform and your minSdkVersion.
APP_PLATFORM := latest

APP_CFLAGS += -Wno-error=format-security
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -std=c++14

# TODO Should be removed
APP_ALLOW_MISSING_DEPS=true

#APP_BUILD_SCRIPT := $(call my-dir)/Android.mk

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := corenet

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/src/*.cpp)

#VisualGDBAndroid: AutoUpdateSourcesInNextLine
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

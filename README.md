RakNet_Android
==============

RakNet 4.081 移植到 Android 平台

直接添加文件：Application.mk、Android.mk

Android.mk内容如下：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := raknet

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/Source/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

针对Source目录下的文件进行编译
如果要添加第三方库，则直接添加。

Application.mk内容如下：
APP_MODULES := libraknet
APP_STL := gnustl_static  指定application里要链接的标准c++库。


node:
LOCAL_LDLIBS 	:= -llog 
LOCAL_LDFLAGS := -L./jni  -lraknet






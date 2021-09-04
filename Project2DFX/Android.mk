LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := Project2DFX
LOCAL_SRC_FILES := main.cpp searchlights.cpp mod/logger.cpp mod/config.cpp
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_C_INCLUDES += ./include
LOCAL_LDLIBS += -llog # ARM64 library requires it so...
include $(BUILD_SHARED_LIBRARY)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../../../thirdparty/SDL2-2.0.3

GEMMINER_SRC := ../../../src
THIRDPARTY_SRC := ../../../thirdparty

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(GEMMINER_SRC) \
	$(LOCAL_PATH)/$(THIRDPARTY_SRC)/stb


# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(GEMMINER_SRC)/main.cpp \
	$(GEMMINER_SRC)/base/sdlapp.cpp \
	$(GEMMINER_SRC)/base/timer.cpp \
	$(GEMMINER_SRC)/game/sprite_font.cpp \
	$(GEMMINER_SRC)/game/resource_manager.cpp \
	$(GEMMINER_SRC)/game/game.cpp \
	$(GEMMINER_SRC)/game/board.cpp \
	$(GEMMINER_SRC)/game/render_manager.cpp \
	$(GEMMINER_SRC)/game/animation_manager.cpp \
	$(GEMMINER_SRC)/game/timer_display.cpp \
	$(GEMMINER_SRC)/game/sprite.cpp

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)

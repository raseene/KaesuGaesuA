#APP_OPTIM := debug
APP_OPTIM := release

ifeq ($(APP_OPTIM),debug)
  APP_ABI := armeabi-v7a
else
  APP_ABI := armeabi armeabi-v7a
endif
APP_PLATFORM := android-14
APP_STL := gnustl_static
STLPORT_FORCE_REBUILD := true
APP_CFLAGS += -Ofast

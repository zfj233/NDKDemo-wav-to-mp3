LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := Hello
LOCAL_SRC_FILES := bitstream.c  lame.c  quantize_pvt.c  vbrquantize.c  encoder.c  mpglib_interface.c  reservoir.c  VbrTag.c  fft.c  newmdct.c  set_get.c  version.c  gain_analysis.c  presets.c  tables.c  Hello.c  psymodel.c  takehiro.c  id3tag.c  quantize.c  util.c
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)
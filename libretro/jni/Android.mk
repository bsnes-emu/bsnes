LOCAL_PATH := $(call my-dir)

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
   LOCAL_CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)

HAVE_NETWORK = 1
LOCAL_MODULE    := libretro

ifeq ($(TARGET_ARCH),arm)
LOCAL_CXXFLAGS += -DANDROID_ARM
LOCAL_ARM_MODE := arm
endif

ifeq ($(TARGET_ARCH),x86)
LOCAL_CXXFLAGS +=  -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
LOCAL_CXXFLAGS += -DANDROID_MIPS
endif

CORE_DIR := ../..

include ../Makefile.common

$(CORE_DIR)/libretro/%_boot.c: $(CORE_DIR)/BootROMs/prebuilt/%_boot.bin
        echo "/* AUTO-GENERATED */" > $@
        echo "const unsigned char $(notdir $(@:%.c=%))[] = {" >> $@
        hexdump -v -e '/1 "0x%02x, "' $< >> $@
        echo "};" >> $@
        echo "const unsigned $(notdir $(@:%.c=%))_length = sizeof($(notdir $(@:%.c=%)));" >> $@

LOCAL_SRC_FILES := $(SOURCES_CXX) $(SOURCES_C)
LOCAL_CFLAGS += -DINLINE=inline -DHAVE_STDINT_H -DHAVE_INTTYPES_H -D__LIBRETRO__ -DNDEBUG -D_USE_MATH_DEFINES  -DGB_INTERNAL

LOCAL_C_INCLUDES = $(INCFLAGS)

include $(BUILD_SHARED_LIBRARY)


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

CORE_DIR := $(realpath ../..)

include ../Makefile.common

LOCAL_SRC_FILES := $(SOURCES_CXX) $(SOURCES_C)
LOCAL_CFLAGS += -DINLINE=inline -DHAVE_STDINT_H -DHAVE_INTTYPES_H -D__LIBRETRO__ -DNDEBUG -D_USE_MATH_DEFINES  -DGB_INTERNAL -std=c99 -I$(CORE_DIR) -DSAMEBOY_CORE_VERSION=\"$(VERSION)\" -DDISABLE_DEBUGGER -DDISABLE_TIMEKEEPING -Wno-multichar
LOCAL_C_INCLUDES = $(INCFLAGS)

include $(BUILD_SHARED_LIBRARY)

$(CORE_DIR)/libretro/%_boot.c: $(CORE_DIR)/build/bin/BootROMs/%_boot.bin
	echo "/* AUTO-GENERATED */" > $@
	echo "const unsigned char $(notdir $(@:%.c=%))[] = {" >> $@
ifneq ($(findstring Haiku,$(shell uname -s)),)
	# turns out od is posix, hexdump is not hence is less portable
	# this is still rather ugly and could be done better I guess
	od -A none -t x1 -v $< | sed -e 's/^\ /0x/' -e 's/\ /,\ 0x/g' -e 's/$$/,/g' | tr '\n' ' ' >> $@
else
	hexdump -v -e '/1 "0x%02x, "' $< >> $@
endif
	echo "};" >> $@
	echo "const unsigned $(notdir $(@:%.c=%))_length = sizeof($(notdir $(@:%.c=%)));" >> $@

$(CORE_DIR)/build/bin/BootROMs/%_boot.bin:
	$(MAKE) -C .. $(patsubst $(CORE_DIR)/%,%,$@)
   

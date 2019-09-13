LOCAL_PATH := $(call my-dir)

SRCDIR := $(LOCAL_PATH)/../../..

INCFLAGS  := -I$(SRCDIR) -I$(SRCDIR)/bsnes
COREFLAGS := -fomit-frame-pointer -ffast-math -D__LIBRETRO__ $(INCFLAGS)
COREFLAGS += -DGB_INTERNAL -DDISABLE_DEBUGGER -DPLATFORM_ANDROID

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
  COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

SRCFILES := $(SRCDIR)/bsnes/target-libretro/libretro.cpp \
				$(SRCDIR)/bsnes/emulator/emulator.cpp \
				$(SRCDIR)/libco/libco.c \
				$(SRCDIR)/bsnes/filter/filter.cpp \
				$(SRCDIR)/bsnes/lzma/lzma.cpp \
				$(SRCDIR)/bsnes/sfc/interface/interface.cpp \
				$(SRCDIR)/bsnes/sfc/system/system.cpp \
				$(SRCDIR)/bsnes/sfc/controller/controller.cpp \
				$(SRCDIR)/bsnes/sfc/cartridge/cartridge.cpp \
				$(SRCDIR)/bsnes/sfc/memory/memory.cpp \
				$(SRCDIR)/bsnes/sfc/cpu/cpu.cpp \
				$(SRCDIR)/bsnes/sfc/smp/smp.cpp \
				$(SRCDIR)/bsnes/sfc/dsp/dsp.cpp \
				$(SRCDIR)/bsnes/sfc/ppu/ppu.cpp \
				$(SRCDIR)/bsnes/sfc/ppu-fast/ppu.cpp \
				$(SRCDIR)/bsnes/sfc/expansion/expansion.cpp \
				$(SRCDIR)/bsnes/sfc/coprocessor/coprocessor.cpp \
				$(SRCDIR)/bsnes/sfc/slot/slot.cpp \
				$(SRCDIR)/bsnes/gb/Core/apu.c \
				$(SRCDIR)/bsnes/gb/Core/camera.c \
				$(SRCDIR)/bsnes/gb/Core/display.c \
				$(SRCDIR)/bsnes/gb/Core/gb.c \
				$(SRCDIR)/bsnes/gb/Core/joypad.c \
				$(SRCDIR)/bsnes/gb/Core/mbc.c \
				$(SRCDIR)/bsnes/gb/Core/memory.c \
				$(SRCDIR)/bsnes/gb/Core/printer.c \
				$(SRCDIR)/bsnes/gb/Core/random.c \
				$(SRCDIR)/bsnes/gb/Core/rewind.c \
				$(SRCDIR)/bsnes/gb/Core/save_state.c \
				$(SRCDIR)/bsnes/gb/Core/sgb.c \
				$(SRCDIR)/bsnes/gb/Core/sm83_cpu.c \
				$(SRCDIR)/bsnes/gb/Core/symbol_hash.c \
				$(SRCDIR)/bsnes/gb/Core/timing.c \
				$(SRCDIR)/bsnes/processor/arm7tdmi/arm7tdmi.cpp \
				$(SRCDIR)/bsnes/processor/spc700/spc700.cpp \
				$(SRCDIR)/bsnes/processor/wdc65816/wdc65816.cpp

include $(CLEAR_VARS)
LOCAL_MODULE       := retro
LOCAL_SRC_FILES    := $(SRCFILES)
LOCAL_CPPFLAGS     := -std=c++17 $(COREFLAGS)
LOCAL_CFLAGS       := $(COREFLAGS)
LOCAL_LDFLAGS      := -Wl,-version-script=$(SRCDIR)/bsnes/target-libretro/link.T
LOCAL_CPP_FEATURES := exceptions rtti
include $(BUILD_SHARED_LIBRARY)

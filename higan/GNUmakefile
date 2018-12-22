target := bsnes
binary := application
build := performance
openmp := true
flags += -I. -I..

nall.path := ../nall
include $(nall.path)/GNUmakefile

ifeq ($(platform),windows)
  ifeq ($(binary),application)
    link += -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32
    link += -Wl,-enable-auto-import
    link += -Wl,-enable-runtime-pseudo-reloc
  else ifeq ($(binary),library)
    link += -shared
  endif
else ifeq ($(platform),macos)
  ifeq ($(binary),application)
  else ifeq ($(binary),library)
    flags += -fPIC
    link += -dynamiclib
  endif
else ifneq ($(filter $(platform),linux bsd),)
  ifeq ($(binary),application)
    flags += -march=native
    link += -Wl,-export-dynamic
    link += -lX11 -lXext
  else ifeq ($(binary),library)
    flags += -fPIC
    link += -shared
  endif
else
  $(error "unsupported platform")
endif

objects := libco emulator

obj/libco.o: ../libco/libco.c
obj/emulator.o: emulator/emulator.cpp

ifeq ($(target),higan)
  cores := fc sfc ms md pce msx gb gba ws ngp
endif

ifeq ($(target),bsnes)
  cores := sfc gb
endif

ifneq ($(filter $(cores),fc),)
  include fc/GNUmakefile
endif

ifneq ($(filter $(cores),sfc),)
  include sfc/GNUmakefile
endif

ifneq ($(filter $(cores),ms),)
  include ms/GNUmakefile
endif

ifneq ($(filter $(cores),md),)
  include md/GNUmakefile
endif

ifneq ($(filter $(cores),pce),)
  include pce/GNUmakefile
endif

ifneq ($(filter $(cores),msx),)
  include msx/GNUmakefile
endif

ifneq ($(filter $(cores),gb),)
  include gb/GNUmakefile
endif

ifneq ($(filter $(cores),gba),)
  include gba/GNUmakefile
endif

ifneq ($(filter $(cores),ws),)
  include ws/GNUmakefile
endif

ifneq ($(filter $(cores),ngp),)
  include ngp/GNUmakefile
endif

include processor/GNUmakefile

flags += $(foreach c,$(call strupper,$(cores)),-DCORE_$c)
ui := target-$(target)
include $(ui)/GNUmakefile
-include obj/*.d

clean:
	$(call delete,obj/*)
	$(call delete,out/*)

build := performance
openmp := true
include ../nall/GNUmakefile

binary := application
target := bsnes
objects := libco emulator audio video resource

flags += -I. -I..

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

compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(compiler) $(cflags) $(flags) $1 -c $< -o $@ -MMD -MP -MF $(@:.o=.d), \
      $(if $(filter %.cpp,$<), \
        $(compiler) $(cppflags) $(flags) $1 -c $< -o $@ -MMD -MP -MF $(@:.o=.d) \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

obj/libco.o: ../libco/libco.c
obj/emulator.o: emulator/emulator.cpp
obj/audio.o: audio/audio.cpp
obj/video.o: video/video.cpp
obj/resource.o: resource/resource.cpp

ui := target-$(target)
include $(ui)/GNUmakefile
-include obj/*.d

clean:
	$(call rm,out/*)
	$(call rm,obj/*)

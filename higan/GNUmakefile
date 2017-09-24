build := optimize
include ../nall/GNUmakefile

binary := application
target := tomoko
objects := libco emulator audio video resource

flags += -I. -I..

ifeq ($(platform),windows)
  link += -mwindows
  ifeq ($(binary),application)
    link += -mthreads -lpthread -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32
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
  flags += -fopenmp
  link += -fopenmp
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
      $(compiler) $(cflags) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(compiler) $(cppflags) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

obj/libco.o: ../libco/libco.c $(call rwildcard,../libco)
obj/emulator.o: emulator/emulator.cpp $(call rwildcard,emulator)
obj/audio.o: audio/audio.cpp $(call rwildcard,audio)
obj/video.o: video/video.cpp $(call rwildcard,video)
obj/resource.o: resource/resource.cpp $(call rwildcard,resource)

ui := target-$(target)
include $(ui)/GNUmakefile

clean:
	-@$(call delete,out/*)
	-@$(call delete,obj/*)

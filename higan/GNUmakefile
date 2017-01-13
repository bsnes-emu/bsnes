include ../nall/GNUmakefile

target := tomoko
# console := true

flags += -I. -I.. -O3
objects := libco emulator audio video resource

# profile-guided optimization mode
# pgo := instrument
# pgo := optimize

ifeq ($(pgo),instrument)
  flags += -fprofile-generate
  link += -lgcov
else ifeq ($(pgo),optimize)
  flags += -fprofile-use
endif

# platform
ifeq ($(platform),windows)
  ifeq ($(console),true)
    link += -mconsole
  else
    link += -mwindows
  endif
  link += -mthreads -lpthread -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32
  link += -Wl,-enable-auto-import
  link += -Wl,-enable-runtime-pseudo-reloc
else ifeq ($(platform),macosx)
  flags += -march=native
else ifneq ($(filter $(platform),linux bsd),)
  flags += -march=native -fopenmp
  link += -fopenmp
  link += -Wl,-export-dynamic
  link += -lX11 -lXext
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

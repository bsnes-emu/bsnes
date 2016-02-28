include ../nall/GNUmakefile

target := tomoko
# target := loki
profile := accuracy
# console := true

flags += -I. -I.. -O3
objects := libco

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
  link += -mthreads -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32
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

ui := target-$(target)
include $(ui)/GNUmakefile

clean:
	-@$(call delete,out/*)
	-@$(call delete,obj/*)

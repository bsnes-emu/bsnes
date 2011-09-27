include nall/Makefile

nes := nes
snes := snes
gameboy := gameboy
profile := accuracy
ui := ui

# options += console
# options += debugger

# compiler
c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -O3 -fomit-frame-pointer -I.
link    :=
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
ifeq ($(platform),x)
  # tree vectorization causes code generation errors with GCC 4.6.1
  flags += -fno-tree-vectorize
  link += -s -ldl -lX11 -lXext
else ifeq ($(platform),osx)
else ifeq ($(platform),win)
  link += $(if $(findstring console,$(options)),-mconsole,-mwindows)
  link += -mthreads -s -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32
  link += -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
else
  unknown_platform: help;
endif

flags := $(flags) $(foreach o,$(call strupper,$(options)),-D$o)

# implicit rules
compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(cpp) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

obj/libco.o: libco/libco.c libco/*

include $(ui)/Makefile

# targets
clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.a)
	-@$(call delete,obj/*.so)
	-@$(call delete,obj/*.dylib)
	-@$(call delete,obj/*.dll)
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

archive-all:
	tar -cjf bsnes.tar.bz2 data gameboy libco nall nes obj out phoenix ruby snes ui ui-libsnes Makefile cc.bat clean.bat sync.sh

help:;

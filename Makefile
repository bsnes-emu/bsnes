include nall/Makefile
gameboy := gameboy
ui := ui

# compiler
c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -O3 -fomit-frame-pointer -I.
link    :=
objects := libco

# profile-guided instrumentation
# flags += -fprofile-generate
# link += -lgcov

# profile-guided optimization
# flags += -fprofile-use

# platform
ifeq ($(platform),x)
  link += -s -ldl -lX11 -lXext
else ifeq ($(platform),osx)
else ifeq ($(platform),win)
  link += -mwindows
# link += -mconsole
  link += -mthreads -s -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32
  link += -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
else
  unknown_platform: help;
endif

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

include $(gameboy)/Makefile
include $(ui)/Makefile

objects := $(patsubst %,obj/%.o,$(objects))

# targets
build: ui_build $(objects)
ifeq ($(platform),osx)
	test -d ../bgameboy.app || mkdir -p ../bgameboy.app/Contents/MacOS
	$(strip $(cpp) -o ../bgameboy.app/Contents/MacOS/bgameboy $(objects) $(link))
else
	$(strip $(cpp) -o out/bgameboy $(objects) $(link))
endif

install:
ifeq ($(platform),x)
	install -D -m 755 out/bgameboy $(DESTDIR)$(prefix)/bin/bgameboy
	test -d ~/.bgameboy || mkdir ~/.bgameboy
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/bgameboy
endif

clean: ui_clean
	-@$(call delete,obj/*)

archive-all:
	tar -cjf bgameboy.tar.bz2 gameboy libco nall obj out phoenix ruby ui Makefile sync.sh

help:;

name := tomoko

processors := arm gsu hg51b lr35902 r6502 r65816 spc700 upd96050
include processor/GNUmakefile

include fc/GNUmakefile
include sfc/GNUmakefile
include gb/GNUmakefile
include gba/GNUmakefile

ui_objects := ui-tomoko ui-program
ui_objects += ui-library ui-presentation
ui_objects += ruby hiro

# platform
ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
else ifeq ($(platform),linux)
else ifeq ($(platform),bsd)
  ruby := video.glx video.xshm
  ruby += audio.openal audio.oss
  ruby += input.xlib
endif

# ruby
include ruby/GNUmakefile
link += $(rubylink)

# hiro
include hiro/GNUmakefile
link += $(hirolink)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/)
	$(compiler) $(rubyflags) -c $< -o $@

obj/hiro.o: hiro/hiro.cpp $(call rwildcard,hiro/)
	$(compiler) $(hiroflags) -c $< -o $@

obj/ui-tomoko.o: $(ui)/tomoko.cpp $(call rwildcard,$(ui)/)
obj/ui-program.o: $(ui)/program/program.cpp $(call rwildcard,$(ui)/)
obj/ui-library.o: $(ui)/library/library.cpp $(call rwildcard,$(ui)/)
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp $(call rwildcard,$(ui)/)

# targets
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))

install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
else
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/higan.png $(prefix)/share/icons/$(name).png
	mkdir -p ~/Emulation/System/
	cp -R profile/* ~/Emulation/System/
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
else
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/icons/$(name).png ]; then rm $(prefix)/share/icons/$(name).png; fi
endif

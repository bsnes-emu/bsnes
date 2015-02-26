name := higan

processors := arm gsu hg51b lr35902 r6502 r65816 spc700 upd96050
include processor/GNUmakefile
include fc/GNUmakefile
include sfc/GNUmakefile
include gb/GNUmakefile
include gba/GNUmakefile

ui_objects := ui-higan
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

obj/ui-higan.o: $(ui)/higan.cpp $(call rwildcard,$(ui)/)

# build
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))

install:
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/higan.png $(prefix)/share/icons/higan.png

uninstall:
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/icons/higan.png ]; then rm $(prefix)/share/icons/higan.png

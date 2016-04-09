name := loki
flags += -DDEBUGGER

include sfc/GNUmakefile
include processor/GNUmakefile

ui_objects := ui-loki ui-program
ui_objects += ui-terminal ui-presentation
ui_objects += ruby hiro
ui_objects += $(if $(call streq,$(platform),windows),ui-resource)

# platform
ifeq ($(platform),windows)
  ruby += video.gdi audio.directsound input.windows
else ifeq ($(platform),macosx)
  ruby += video.cgl audio.openal input.quartz
else ifeq ($(platform),linux)
  ruby += video.xshm audio.openal input.sdl
else ifeq ($(platform),bsd)
  ruby += video.xshm audio.oss input.sdl
endif

# ruby
include ../ruby/GNUmakefile
link += $(rubylink)

# hiro
include ../hiro/GNUmakefile
link += $(hirolink)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ruby.o: ../ruby/ruby.cpp $(call rwildcard,../ruby/)
	$(compiler) $(rubyflags) -c $< -o $@

obj/hiro.o: ../hiro/hiro.cpp $(call rwildcard,../hiro/)
	$(compiler) $(hiroflags) -c $< -o $@

obj/ui-loki.o: $(ui)/loki.cpp $(call rwildcard,$(ui)/)
obj/ui-program.o: $(ui)/program/program.cpp $(call rwildcard,$(ui)/)
obj/ui-terminal.o: $(ui)/terminal/terminal.cpp $(call rwildcard,$(ui)/)
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp $(call rwildcard,$(ui)/)

obj/ui-resource.o:
	windres data/resource.rc obj/ui-resource.o

# targets
build: $(objects)
	$(call unique,$(compiler) -o out/$(name) $(objects) $(link))

install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/higan.png $(prefix)/share/icons/$(name).png
	cp -R profile/* $(prefix)/share/$(name)/
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifneq ($(filter $(platform),linux bsd),)
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/icons/$(name).png ]; then rm $(prefix)/share/icons/$(name).png; fi
endif

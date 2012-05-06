options += debugger

processors := arm gsu hg51b r65816 spc700 upd96050
include processor/Makefile

include $(sfc)/Makefile
name := laevateinn

ui_objects := ui-main ui-settings ui-interface ui-debugger ui-tracer ui-window
ui_objects += ui-console ui-video ui-cpu ui-smp ui-memory ui-breakpoint ui-properties ui-vram
ui_objects += phoenix ruby
ui_objects += $(if $(call streq,$(platform),win),resource)

# platform
ifeq ($(platform),x)
  ruby := audio.alsa
else ifeq ($(platform),osx)
  ruby := audio.openal
else ifeq ($(platform),win)
  ruby := audio.xaudio2
endif

# phoenix
include phoenix/Makefile
link += $(phoenixlink)

# ruby
include ruby/Makefile
link += $(rubylink)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ui-main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(call rwildcard,$(ui)/*)
obj/ui-interface.o: $(ui)/interface/interface.cpp $(call rwildcard,$(ui)/*)
obj/ui-debugger.o: $(ui)/debugger/debugger.cpp $(call rwildcard,$(ui)/*)
obj/ui-tracer.o: $(ui)/tracer/tracer.cpp $(call rwildcard,$(ui)/*)
obj/ui-window.o: $(ui)/window/window.cpp $(call rwildcard,$(ui)/*)
obj/ui-console.o: $(ui)/console/console.cpp $(call rwildcard,$(ui)/*)
obj/ui-cpu.o: $(ui)/cpu/cpu.cpp $(call rwildcard,$(ui)/*)
obj/ui-smp.o: $(ui)/smp/smp.cpp $(call rwildcard,$(ui)/*)
obj/ui-video.o: $(ui)/video/video.cpp $(call rwildcard,$(ui)/*)
obj/ui-memory.o: $(ui)/memory/memory.cpp $(call rwildcard,$(ui)/*)
obj/ui-breakpoint.o: $(ui)/breakpoint/breakpoint.cpp $(call rwildcard,$(ui)/*)
obj/ui-properties.o: $(ui)/properties/properties.cpp $(call rwildcard,$(ui)/*)
obj/ui-vram.o: $(ui)/vram/vram.cpp $(call rwildcard,$(ui)/*)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubyflags))

obj/phoenix.o: phoenix/phoenix.cpp $(call rwildcard,phoenix/*)
	$(call compile,$(phoenixflags))

obj/resource.o: $(ui)/resource.rc
#	windres --target=pe-i386 $(ui)/resource.rc obj/resource.o
	windres $(ui)/resource.rc obj/resource.o

# targets
build: $(objects)
ifeq ($(platform),osx)
	test -d ../$(name).app || mkdir -p ../$(name).app/Contents/MacOS
	$(strip $(cpp) -o ../$(name).app/Contents/MacOS/$(name) $(objects) $(link))
else
	$(strip $(cpp) -o out/$(name) $(objects) $(link))
endif

install:
ifeq ($(USER),root)
	@echo Please do not run make install as root.
	@echo The installer needs to know your home directory to install important files.
else ifeq ($(platform),x)
	sudo install -D -m 755 out/$(name) $(DESTDIR)$(prefix)/bin/$(name)

	mkdir -p ~/.config/$(name)
	cp -R profile/* ~/.config/$(name)
	cp data/cheats.xml ~/.config/$(name)/cheats.xml
	chmod -R 777 ~/.config/$(name)
endif

uninstall:
ifeq ($(platform),x)
	sudo rm $(DESTDIR)$(prefix)/bin/$(name)
endif

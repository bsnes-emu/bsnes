name := higan

processors := arm gsu hg51b lr35902 r6502 r65816 spc700 upd96050
include processor/Makefile

include fc/Makefile
include sfc/Makefile
include gb/Makefile
include gba/Makefile
# include nds/Makefile

ui_objects := ui-ethos ui-configuration ui-interface ui-utility
ui_objects += ui-input ui-window ui-general ui-settings ui-tools
ui_objects += phoenix ruby
ui_objects += $(if $(call streq,$(platform),win),resource)

# platform
ifeq ($(platform),x)
  ruby := video.glx video.xv video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x
else ifeq ($(platform),osx)
  ruby :=
  ruby += audio.openal
  ruby += input.carbon
else ifeq ($(platform),win)
  ruby := video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.directsound audio.xaudio2
  ruby += input.rawinput input.directinput
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

obj/ui-ethos.o: $(ui)/ethos.cpp $(call rwildcard,$(ui)/)
obj/ui-configuration.o: $(ui)/configuration/configuration.cpp $(call rwildcard,$(ui)/)
obj/ui-interface.o: $(ui)/interface/interface.cpp $(call rwildcard,$(ui)/)
obj/ui-utility.o: $(ui)/utility/utility.cpp $(call rwildcard,$(ui)/)
obj/ui-input.o: $(ui)/input/input.cpp $(call rwildcard,$(ui)/)
obj/ui-window.o: $(ui)/window/window.cpp $(call rwildcard,$(ui)/)
obj/ui-general.o: $(ui)/general/general.cpp $(call rwildcard,$(ui)/)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(call rwildcard,$(ui)/)
obj/ui-tools.o: $(ui)/tools/tools.cpp $(call rwildcard,$(ui)/)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubyflags))

obj/phoenix.o: phoenix/phoenix.cpp $(call rwildcard,phoenix/*)
	$(call compile,$(phoenixflags))

obj/resource.o: $(ui)/resource.rc
ifeq ($(arch),win32)
	windres --target=pe-i386 $(ui)/resource.rc obj/resource.o
else
	windres $(ui)/resource.rc obj/resource.o
endif

# targets
build: $(objects)
ifeq ($(platform),x)
	$(strip $(cpp) -o out/$(name) $(objects) $(link))
else ifeq ($(platform),win)
	$(strip $(cpp) -shared -o out/phoenix.dll obj/phoenix.o $(phoenixlink))
	$(strip $(cpp) -o out/$(name) $(subst obj/phoenix.o,,$(objects)) $(link) -Lout -lphoenix)
else ifeq ($(platform),osx)
	test -d ../$(name).app || mkdir -p ../$(name).app/Contents/MacOS
	$(strip $(cpp) -o ../$(name).app/Contents/MacOS/$(name) $(objects) $(link))
endif

resource:
	sourcery $(ui)/resource/resource.bml $(ui)/resource/resource.cpp $(ui)/resource/resource.hpp

install:
ifeq ($(USER),root)
	@echo Please do not run make install as root.
	@echo The installer needs to know your home directory to install important files.
else ifeq ($(platform),x)
	sudo install -D -m 755 out/$(name) $(DESTDIR)$(prefix)/bin/$(name)
	sudo install -D -m 644 data/$(name).png $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
	sudo install -D -m 644 data/$(name).desktop $(DESTDIR)$(prefix)/share/applications/$(name).desktop

	mkdir -p ~/.config/$(name)
	cp -R profile/* ~/.config/$(name)
	cp data/cheats.bml ~/.config/$(name)/cheats.bml
	chmod -R 777 ~/.config/$(name)
endif

uninstall:
ifeq ($(platform),x)
	sudo rm $(DESTDIR)$(prefix)/bin/$(name)
endif

include $(nes)/Makefile
include $(snes)/Makefile
include $(gameboy)/Makefile
name := bsnes

ui_objects := ui-main ui-config ui-interface ui-input ui-utility
ui_objects += ui-window ui-general ui-settings ui-tools
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

obj/ui-main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/)
obj/ui-config.o: $(ui)/config/config.cpp $(call rwildcard,$(ui)/)
obj/ui-interface.o: $(ui)/interface/interface.cpp $(call rwildcard,$(ui)/)
obj/ui-input.o: $(ui)/input/input.cpp $(call rwildcard,$(ui)/)
obj/ui-utility.o: $(ui)/utility/utility.cpp $(call rwildcard,$(ui)/)
obj/ui-window.o: $(ui)/window/window.cpp $(call rwildcard,$(ui)/)
obj/ui-general.o: $(ui)/general/general.cpp $(call rwildcard,$(ui)/)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(call rwildcard,$(ui)/)
obj/ui-tools.o: $(ui)/tools/tools.cpp $(call rwildcard,$(ui)/)

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
ifeq ($(platform),x)
	install -D -m 755 out/$(name) $(DESTDIR)$(prefix)/bin/$(name)
	mkdir -p ~/.config/$(name)
	install -D -m 644 data/$(name).png $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
	install -D -m 644 data/$(name).desktop $(DESTDIR)$(prefix)/share/applications/$(name).desktop
	cp data/cheats.bml ~/.config/$(name)/cheats.bml
	chmod 777 ~/.config/$(name) ~/.config/$(name)/cheats.bml
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/$(name)
	rm $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
	rm $(DESTDIR)$(prefix)/share/applications/$(name).desktop
endif

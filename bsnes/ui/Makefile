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
  ifeq ($(phoenix),gtk)
    phoenix_compile = $(call compile,-DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`)
    link += `pkg-config --libs gtk+-2.0`
  else
    phoenix_compile = $(call compile,-DPHOENIX_QT `pkg-config --cflags QtCore QtGui`)
    link += `pkg-config --libs QtCore QtGui`
  endif

  ruby := video.glx video.xv video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x
else ifeq ($(platform),osx)
  phoenix_compile = $(call compile,-DPHOENIX_QT)
  link +=

  ruby :=
  ruby += audio.openal
  ruby += input.carbon
else ifeq ($(platform),win)
  phoenix_compile = $(call compile,-DPHOENIX_WINDOWS)
  link +=

  ruby := video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.directsound audio.xaudio2
  ruby += input.rawinput input.directinput
endif

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
	$(phoenix_compile)

obj/resource.o: $(ui)/resource.rc
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
	cp data/cheats.xml ~/.config/$(name)/cheats.xml
	chmod 777 ~/.config/$(name) ~/.config/$(name)/cheats.xml
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/$(name)
	rm $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
	rm $(DESTDIR)$(prefix)/share/applications/$(name).desktop
endif

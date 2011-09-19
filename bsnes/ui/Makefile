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

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  phoenix_compile = $(call compile,-DPHOENIX_QT)
  link +=

  ruby :=
  ruby += audio.openal
  ruby += input.carbon

  link += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  phoenix_compile = $(call compile,-DPHOENIX_WINDOWS)
  link +=

  ruby := video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.directsound audio.xaudio2
  ruby += input.rawinput input.directinput

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
endif

# ruby
rubyflags := $(if $(finstring .sdl,$(ruby)),`sdl-config --cflags`)

link += $(if $(findstring .sdl,$(ruby)),`sdl-config --libs`)
link += $(if $(findstring video.direct3d,$(ruby)),-ld3d9)
link += $(if $(findstring video.directdraw,$(ruby)),-lddraw)
link += $(if $(findstring video.glx,$(ruby)),-lGL)
link += $(if $(findstring video.wgl,$(ruby)),-lopengl32)
link += $(if $(findstring video.xv,$(ruby)),-lXv)
link += $(if $(findstring audio.alsa,$(ruby)),-lasound)
link += $(if $(findstring audio.ao,$(ruby)),-lao)
link += $(if $(findstring audio.directsound,$(ruby)),-ldsound)
link += $(if $(findstring audio.pulseaudio,$(ruby)),-lpulse)
link += $(if $(findstring audio.pulseaudiosimple,$(ruby)),-lpulse-simple)
link += $(if $(findstring audio.xaudio2,$(ruby)),-lole32)
link += $(if $(findstring input.directinput,$(ruby)),-ldinput8 -ldxguid)
link += $(if $(findstring input.rawinput,$(ruby)),-ldinput8 -ldxguid)

rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)

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
	$(call compile,$(rubydef) $(rubyflags))

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
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/$(name)
endif

#	install -D -m 644 data/$(name).png $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
#	install -D -m 644 data/$(name).desktop $(DESTDIR)$(prefix)/share/applications/$(name).desktop
#	cp data/cheats.xml ~/.config/$(name)/cheats.xml
#	chmod 777 ~/.config/$(name) ~/.config/$(name)/cheats.xml

#	rm $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
#	rm $(DESTDIR)$(prefix)/share/applications/$(name).desktop

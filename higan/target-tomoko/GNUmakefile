name := tomoko

processors := arm gsu hg51b lr35902 r6502 r65816 spc700 upd96050
include processor/GNUmakefile

include fc/GNUmakefile
include sfc/GNUmakefile
include gb/GNUmakefile
include gba/GNUmakefile

ui_objects := ui-tomoko ui-program ui-configuration ui-input
ui_objects += ui-settings ui-tools ui-presentation
ui_objects += ruby hiro
ui_objects += $(if $(call streq,$(platform),windows),ui-resource)

# platform
ifeq ($(platform),windows)
  ruby += video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.xaudio2 audio.directsound
  ruby += input.windows
else ifeq ($(platform),macosx)
  ruby += video.cgl
  ruby += audio.openal
  ruby += input.quartz input.carbon
else ifeq ($(platform),linux)
  ruby += video.glx video.xv video.xshm video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.udev input.sdl input.xlib
else ifeq ($(platform),bsd)
  ruby += video.glx video.xv video.xshm video.sdl
  ruby += audio.openal audio.oss
  ruby += input.sdl input.xlib
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

obj/libco.o: ../libco/libco.c $(call rwildcard,../libco/)

obj/ruby.o: ../ruby/ruby.cpp $(call rwildcard,../ruby/)
	$(compiler) $(rubyflags) -c $< -o $@

obj/hiro.o: ../hiro/hiro.cpp $(call rwildcard,../hiro/)
	$(compiler) $(hiroflags) -c $< -o $@

obj/ui-tomoko.o: $(ui)/tomoko.cpp $(call rwildcard,$(ui)/)
obj/ui-program.o: $(ui)/program/program.cpp $(call rwildcard,$(ui)/)
obj/ui-configuration.o: $(ui)/configuration/configuration.cpp $(call rwildcard,$(ui)/)
obj/ui-input.o: $(ui)/input/input.cpp $(call rwildcard,$(ui)/)
obj/ui-library.o: $(ui)/library/library.cpp $(call rwildcard,$(ui)/)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(call rwildcard,$(ui)/)
obj/ui-tools.o: $(ui)/tools/tools.cpp $(call rwildcard,$(ui)/)
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp $(call rwildcard,$(ui)/)

obj/ui-resource.o:
	windres data/resource.rc obj/ui-resource.o

# targets
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))
ifeq ($(platform),macosx)
	@if [ -d out/higan.app ]; then rm -r out/higan.app; fi
	mkdir -p out/higan.app/Contents/MacOS/
	mkdir -p out/higan.app/Contents/Resources/
	mv out/$(name) out/higan.app/Contents/MacOS/higan
	cp data/higan.plist out/higan.app/Contents/Info.plist
	sips -s format icns data/higan.png --out out/higan.app/Contents/Resources/higan.icns
endif

install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	mkdir -p ~/Library/Application\ Support/$(name)/
	mkdir -p ~/Emulation/System/
	cp -R out/higan.app /Applications/higan.app
	cp data/cheats.bml ~/Library/Application\ Support/$(name)/
	cp -R profile/* ~/Emulation/System/
else
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/$(name)/
	mkdir -p ~/Emulation/System/
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/higan.png $(prefix)/share/icons/$(name).png
	cp data/cheats.bml $(prefix)/$(name)/cheats.bml
	cp -R profile/* ~/Emulation/System/
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	if [ -d /Applications/higan.app ]; then rm -r /Applications/higan.app; fi
else
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/icons/$(name).png ]; then rm $(prefix)/share/icons/$(name).png; fi
endif

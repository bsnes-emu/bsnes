name := higan
flags += -DSFC_SUPERGAMEBOY

include fc/GNUmakefile
include sfc/GNUmakefile
include ms/GNUmakefile
include md/GNUmakefile
include pce/GNUmakefile
include gb/GNUmakefile
include gba/GNUmakefile
include ws/GNUmakefile
include processor/GNUmakefile

ui_objects := ui-tomoko ui-program ui-configuration ui-input
ui_objects += ui-settings ui-tools ui-presentation
ui_objects += ruby hiro
ui_objects += $(if $(call streq,$(platform),windows),ui-resource)

# platform
ifeq ($(platform),windows)
  ruby += video.direct3d  #video.wgl video.directdraw video.gdi
  ruby += audio.asio  #audio.wasapi audio.xaudio2 audio.directsound
  ruby += input.windows
else ifeq ($(platform),macosx)
  ruby += #video.cgl
  ruby += #audio.openal
  ruby += #input.quartz input.carbon
else ifeq ($(platform),linux)
  ruby += video.xshm  #video.glx video.xv video.xshm video.sdl
  ruby += audio.oss  #audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.xlib  #input.udev input.sdl input.xlib
else ifeq ($(platform),bsd)
  ruby += video.xshm  #video.glx video.xv video.xshm video.sdl
  ruby += audio.oss  #audio.alsa
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
	$(windres) data/resource.rc obj/ui-resource.o

# targets
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))
ifeq ($(platform),macosx)
	@if [ -d out/$(name).app ]; then rm -r out/$(name).app; fi
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp data/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns data/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	mkdir -p ~/Library/Application\ Support/$(name)/
	mkdir -p ~/Emulation/System/
	cp -R out/$(name).app /Applications/$(name).app
	cp -R systems/* ~/Library/Application\ Support/$(name)/
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/$(name)/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	cp out/$(name) $(prefix)/bin/$(name)
	cp -R systems/* $(prefix)/share/$(name)/
	cp data/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp data/$(name).png $(prefix)/share/icons/$(name).png
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	if [ -d /Applications/$(name).app ]; then rm -r /Applications/$(name).app; fi
else ifneq ($(filter $(platform),linux bsd),)
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/applications/$(name).desktop ]; then rm $(prefix)/share/applications/$(name).desktop; fi
	if [ -f $(prefix)/share/icons/$(name).png ]; then rm $(prefix)/share/icons/$(name).png; fi
endif

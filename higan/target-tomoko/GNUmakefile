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
  ruby += video.wgl video.direct3d video.directdraw video.gdi
  ruby += audio.asio audio.wasapi audio.xaudio2 audio.directsound
  ruby += input.windows
else ifeq ($(platform),macos)
  ruby += video.cgl
  ruby += audio.openal
  ruby += input.quartz input.carbon
else ifeq ($(platform),linux)
  ruby += video.glx video.xvideo video.xshm video.sdl
  ruby += audio.oss audio.alsa audio.openal audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.xlib input.udev
else ifeq ($(platform),bsd)
  ruby += video.glx video.xvideo video.xshm video.sdl
  ruby += audio.oss audio.openal
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
	$(windres) data/higan.rc obj/ui-resource.o

# targets
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))
ifeq ($(platform),macos)
	rm -rf out/$(name).app
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
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support/$(name)/
	mkdir -p ~/Emulation/System/
	cp -R out/$(name).app /Applications/$(name).app
	cp -R systems/* ~/Library/Application\ Support/$(name)/
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp out/$(name) $(prefix)/bin/$(name)
	cp -R systems/* $(prefix)/share/$(name)/
	cp data/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp data/$(name).png $(prefix)/share/icons/$(name).png
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(prefix)/bin/$(name)
	rm -f $(prefix)/share/applications/$(name).desktop
	rm -f $(prefix)/share/icons/$(name).png
endif

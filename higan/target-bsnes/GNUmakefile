name := bsnes
flags += -DSFC_SUPERGAMEBOY

include sfc/GNUmakefile
include gb/GNUmakefile
include processor/GNUmakefile

objects := ruby hiro $(if $(call streq,$(platform),windows),hiro-resource) $(objects)
objects += ui-bsnes ui-program ui-input ui-presentation
objects += ui-settings ui-tools ui-resource
objects := $(objects:%=obj/%.o)

ifeq ($(platform),windows)
  ruby += video.wgl video.direct3d video.directdraw video.gdi
  ruby += audio.asio audio.wasapi audio.xaudio2 audio.directsound
  ruby += input.windows
else ifeq ($(platform),macos)
  ruby += video.cgl
  ruby += audio.openal
  ruby += input.quartz input.carbon
else ifeq ($(platform),linux)
  ruby += video.glx video.xvideo video.xshm
  ruby += audio.oss audio.alsa audio.openal audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.xlib input.udev
else ifeq ($(platform),bsd)
  ruby += video.glx video.xvideo video.xshm
  ruby += audio.oss audio.openal
  ruby += input.sdl input.xlib
endif

ruby.path := ../ruby
include $(ruby.path)/GNUmakefile

hiro.path := ../hiro
hiro.resource := $(ui)/resource/bsnes.rc
include $(hiro.path)/GNUmakefile

obj/ui-bsnes.o: $(ui)/bsnes.cpp
obj/ui-program.o: $(ui)/program/program.cpp
obj/ui-input.o: $(ui)/input/input.cpp
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp
obj/ui-settings.o: $(ui)/settings/settings.cpp
obj/ui-tools.o: $(ui)/tools/tools.cpp
obj/ui-resource.o: $(ui)/resource/resource.cpp

# targets
all: $(objects)
	$(info Linking out/$(name) ...)
	+@$(strip $(compiler) -o out/$(name) $(objects) $(options) $(ruby.options) $(hiro.options))
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp $(ui)/resource/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns $(ui)/resource/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support/$(name)/
	cp -R out/$(name).app /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	mkdir -p $(prefix)/share/$(name)/locales/
	cp out/$(name) $(prefix)/bin/$(name)
	cp $(ui)/resource/locales/* $(prefix)/share/$(name)/locales/
	cp $(ui)/resource/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp $(ui)/resource/$(name).png $(prefix)/share/icons/$(name).png
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

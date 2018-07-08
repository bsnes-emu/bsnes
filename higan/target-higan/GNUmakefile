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

objects := ruby hiro $(objects)
objects += ui-higan ui-program ui-input
objects += ui-settings ui-tools ui-presentation ui-resource
objects += $(if $(call streq,$(platform),windows),ui-windows)
objects := $(objects:%=obj/%.o)

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
  ruby += video.glx video.xvideo video.xshm
  ruby += audio.oss audio.alsa audio.openal audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.xlib input.udev
else ifeq ($(platform),bsd)
  ruby += video.glx video.xvideo video.xshm
  ruby += audio.oss audio.openal
  ruby += input.sdl input.xlib
endif

# ruby
include ../ruby/GNUmakefile
link += $(rubylink)

obj/ruby.o: ../ruby/ruby.cpp $(call rwildcard,../ruby/)
	$(info Compiling $< ...)
	@$(compiler) $(rubyflags) -c $< -o $@

# hiro
include ../hiro/GNUmakefile
link += $(hirolink)

obj/hiro.o: ../hiro/hiro.cpp $(call rwildcard,../hiro/)
	$(info Compiling $< ...)
	@$(compiler) $(hiroflags) -c $< -o $@

# rules
obj/ui-higan.o: $(ui)/higan.cpp
obj/ui-program.o: $(ui)/program/program.cpp
obj/ui-input.o: $(ui)/input/input.cpp
obj/ui-settings.o: $(ui)/settings/settings.cpp
obj/ui-tools.o: $(ui)/tools/tools.cpp
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp
obj/ui-resource.o: $(ui)/resource/resource.cpp

obj/ui-windows.o: $(ui)/resource/higan.rc
	$(info Compiling $< ...)
	@$(windres) $(ui)/resource/higan.rc obj/ui-windows.o

# targets
default: information $(objects)
	$(info Linking out/$(name) ...)
	+@$(strip $(compiler) -o out/$(name) $(objects) $(link))
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
	mkdir -p ~/Library/Application\ Support/$(name)/systems/
	mkdir -p ~/Emulation/
	cp -R out/$(name).app /Applications/$(name).app
	cp -R systems/* ~/Library/Application\ Support/$(name)/systems/
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	mkdir -p $(prefix)/share/$(name)/systems/
	cp out/$(name) $(prefix)/bin/$(name)
	cp -R systems/* $(prefix)/share/$(name)/systems/
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

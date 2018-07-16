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

hiro.path := ../hiro
hiro.resource := $(ui)/resource/higan.rc
include $(hiro.path)/GNUmakefile

ruby.path := ../ruby
include $(ruby.path)/GNUmakefile

objects += ui-higan ui-program ui-input
objects += ui-settings ui-tools ui-presentation ui-resource
objects := $(objects:%=obj/%.o)

obj/ui-higan.o: $(ui)/higan.cpp
obj/ui-program.o: $(ui)/program/program.cpp
obj/ui-input.o: $(ui)/input/input.cpp
obj/ui-settings.o: $(ui)/settings/settings.cpp
obj/ui-tools.o: $(ui)/tools/tools.cpp
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp
obj/ui-resource.o: $(ui)/resource/resource.cpp

all: $(hiro.objects) $(ruby.objects) $(objects)
	$(info Linking out/$(name) ...)
	+@$(compiler) -o out/$(name) $(hiro.objects) $(ruby.objects) $(objects) $(hiro.options) $(ruby.options) $(options)
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp $(ui)/resource/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns $(ui)/resource/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

verbose: hiro.verbose ruby.verbose nall.verbose all;

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

name := bsnes

hiro.path := ../hiro
hiro.resource := $(ui)/resource/bsnes.rc
include $(hiro.path)/GNUmakefile

ruby.path := ../ruby
include $(ruby.path)/GNUmakefile

objects += ui-bsnes ui-program ui-input ui-presentation
objects += ui-settings ui-tools ui-resource
objects := $(objects:%=obj/%.o)

obj/ui-bsnes.o: $(ui)/bsnes.cpp
obj/ui-program.o: $(ui)/program/program.cpp
obj/ui-input.o: $(ui)/input/input.cpp
obj/ui-presentation.o: $(ui)/presentation/presentation.cpp
obj/ui-settings.o: $(ui)/settings/settings.cpp
obj/ui-tools.o: $(ui)/tools/tools.cpp
obj/ui-resource.o: $(ui)/resource/resource.cpp

all: $(hiro.objects) $(ruby.objects) $(objects)
	$(info Linking out/$(name) ...)
	+@$(compiler) -o out/$(name) $(hiro.objects) $(ruby.objects) $(objects) $(hiro.options) $(ruby.options) $(options)
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/MacOS/Database/
	mkdir -p out/$(name).app/Contents/MacOS/Firmware/
	mkdir -p out/$(name).app/Contents/MacOS/Shaders/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp Database/* out/$(name).app/Contents/MacOS/Database/
	cp -r ../shaders/* out/$(name).app/Contents/MacOS/Shaders/
	cp $(ui)/resource/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns $(ui)/resource/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

verbose: hiro.verbose ruby.verbose nall.verbose all;

install: all
ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support/$(name)/
	mkdir -p ~/Library/Application\ Support/$(name)/Database/
	mkdir -p ~/Library/Application\ Support/$(name)/Firmware/
	mkdir -p ~/Library/Application\ Support/$(name)/Shaders/
	cp -R out/$(name).app /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(DESTDIR)$(bindir)
	mkdir -p $(DESTDIR)$(datarootdir)/applications
	mkdir -p $(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps
	mkdir -p $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps
	mkdir -p $(DESTDIR)$(datadir)/$(name)/Database
	mkdir -p $(DESTDIR)$(datadir)/$(name)/Firmware
	mkdir -p $(DESTDIR)$(datadir)/$(name)/Shaders
	cp out/$(name) $(DESTDIR)$(bindir)/$(name)
	cp $(ui)/resource/$(name).desktop $(DESTDIR)$(datarootdir)/applications/$(name).desktop
	cp $(ui)/resource/$(name).png $(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps/$(name).png
	cp $(ui)/resource/$(name).svg $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps/$(name).svg
	cp Database/* $(DESTDIR)$(datadir)/$(name)/Database/
	cp -r ../shaders/* $(DESTDIR)$(datadir)/$(name)/Shaders/
endif

uninstall:
ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(DESTDIR)$(bindir)/$(name)
	rm -f $(DESTDIR)$(datarootdir)/applications/$(name).desktop
	rm -f $(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps/$(name).png
	rm -f $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps/$(name).svg
	rm -rf $(DESTDIR)$(datadir)/$(name)
endif

name := genius
build := stable
flags += -I..

nall.path := ../nall
include $(nall.path)/GNUmakefile

hiro.path := ../hiro
hiro.resource := data/$(name).rc
include $(hiro.path)/GNUmakefile

objects := obj/genius.o

obj/genius.o: genius.cpp

all: $(hiro.objects) $(objects)
	$(info Linking out/$(name) ...)
	+@$(compiler) -o out/$(name) $(hiro.objects) $(objects) $(hiro.options) $(options)
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp data/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns data/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

verbose: hiro.verbose nall.verbose all;

clean:
ifeq ($(platform),macos)
	rm -rf out/$(name).app
endif
	$(call delete,obj/*)
	$(call delete,out/*)

install: all
ifeq ($(platform),macos)
	cp -R out/$(name).app /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp data/$(name).png $(prefix)/share/icons/$(name).png
endif

uninstall:
ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(prefix)/bin/$(name)
	rm -f $(prefix)/share/applications/$(name).desktop
	rm -f $(prefix)/share/icons/$(name).png
endif

-include obj/*.d

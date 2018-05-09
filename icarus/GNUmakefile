build := performance
include ../nall/GNUmakefile
include ../hiro/GNUmakefile

name := icarus
flags += -I..

objects := obj/hiro.o
objects += obj/icarus.o
objects += $(if $(call streq,$(platform),windows),obj/resource.o)

all: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link) $(hirolink))
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp data/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns data/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

obj/hiro.o: ../hiro/hiro.cpp
	$(compiler) $(hiroflags) -o obj/hiro.o -c ../hiro/hiro.cpp

obj/icarus.o: icarus.cpp $(call rwildcard,core/) $(call rwildcard,heuristics/) $(call rwildcard,ui/)
	$(compiler) $(cppflags) $(flags) -o obj/icarus.o -c icarus.cpp

obj/resource.o:
	$(windres) data/$(name).rc obj/resource.o

clean:
ifeq ($(platform),macos)
	rm -rf out/$(name).app
endif
	$(call rm,obj/*)
	$(call rm,out/*)

install:
ifeq ($(platform),macos)
	cp -R out/$(name).app /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	mkdir -p $(prefix)/share/$(name)/database/
	mkdir -p $(prefix)/share/$(name)/firmware/
	cp out/$(name) $(prefix)/bin/$(name)
	cp data/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp data/$(name).png $(prefix)/share/icons/$(name).png
	cp -R database/* $(prefix)/share/$(name)/database/
	cp -R firmware/* $(prefix)/share/$(name)/firmware/
endif

uninstall:
ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(prefix)/bin/$(name)
	rm -f $(prefix)/share/applications/$(name).desktop
	rm -f $(prefix)/share/icons/$(name).png
endif

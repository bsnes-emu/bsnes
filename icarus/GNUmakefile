include ../nall/GNUmakefile
include ../hiro/GNUmakefile

flags += -I.. -O3
link +=

objects := obj/hiro.o
objects += obj/icarus.o
objects += $(if $(call streq,$(platform),windows),obj/resource.o)

all: $(objects)
	$(compiler) -o out/icarus $(objects) $(link) $(hirolink)
ifeq ($(platform),macosx)
	@if [ -d out/icarus.app ]; then rm -r out/icarus.app; fi
	mkdir -p out/icarus.app/Contents/MacOS/
	mv out/icarus out/icarus.app/Contents/MacOS/
	cp icarus.plist out/icarus.app/Contents/Info.plist
endif

obj/hiro.o: ../hiro/hiro.cpp
	$(compiler) $(hiroflags) -o obj/hiro.o -c ../hiro/hiro.cpp

obj/icarus.o: icarus.cpp $(call rwildcard,core/) $(call rwildcard,heuristics/) $(call rwildcard,ui/)
	$(compiler) $(cppflags) $(flags) -o obj/icarus.o -c icarus.cpp

obj/resource.o:
	windres ../hiro/windows/hiro.rc obj/resource.o

clean:
ifeq ($(platform),macosx)
	@if [ -d out/icarus.app ]; then rm out/icarus.app; fi
endif
	$(call delete,obj/*)
	$(call delete,out/*)

install:
ifeq ($(platform),macosx)
	cp -r out/icarus.app /Applications/icarus.app
else
	if [ -f out/icarus ]; then cp out/icarus $(prefix)/bin/icarus; fi
endif

uninstall:
ifeq ($(platform),macosx)
	if [ -d /Applications/icarus.app ]; then rm -r /Applications/icarus.app; fi
else
	if [ -f $(prefix)/bin/icarus ]; then rm $(prefix)/bin/icarus; fi
endif

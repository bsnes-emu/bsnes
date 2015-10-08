include ../nall/GNUmakefile
include ../hiro/GNUmakefile

flags += -I.. -O3
link +=
objects := obj/hiro.o obj/icarus.o
objects += $(if $(call streq,$(platform),windows),obj/resource.o)

all: $(objects)
	$(compiler) -o icarus $(objects) $(link) $(hirolink)

obj/hiro.o: ../hiro/hiro.cpp
	$(compiler) $(hiroflags) -o obj/hiro.o -c ../hiro/hiro.cpp

obj/icarus.o: icarus.cpp $(call rwildcard,core/) $(call rwildcard,heuristics/) $(call rwildcard,ui/)
	$(compiler) $(cppflags) $(flags) -o obj/icarus.o -c icarus.cpp

obj/resource.o:
	windres ../hiro/windows/hiro.rc obj/resource.o

clean:
	if [ -f ./icarus ]; then rm ./icarus; fi
	$(call delete,obj/*.o)

install:
	if [ -f ./icarus ]; then cp ./icarus $(prefix)/bin/icarus; fi

uninstall:
	if [ -f $(prefix)/bin/icarus ]; then rm $(prefix)/bin/icarus; fi

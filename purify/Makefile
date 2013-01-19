include nall/Makefile
include phoenix/Makefile

path := /usr/local/bin
flags := -I. -O3 -fomit-frame-pointer
link := -s
objects := obj/phoenix.o obj/purify.o

ifeq ($(platform),x)
  link += -Wl,-export-dynamic
else ifeq ($(platform),win)
  link += -mwindows
endif

all: build;

obj/phoenix.o: phoenix/phoenix.cpp
	$(cpp) $(flags) -o obj/phoenix.o -c phoenix/phoenix.cpp $(phoenixflags)

obj/purify.o: purify.cpp
	$(cpp) $(flags) -o obj/purify.o -c purify.cpp

build: $(objects)
ifeq ($(platform),x)
	$(cpp) $(link) -o purify $(objects) $(phoenixlink)
else ifeq ($(platform),win)
	windres phoenix/windows/phoenix.rc obj/phoenix-resource.o
	$(cpp) -shared -o phoenix.dll obj/phoenix.o $(phoenixlink)
	$(cpp) -o purify obj/purify.o obj/phoenix-resource.o $(link) -L. -lphoenix
endif

resource: force
	sourcery resource/resource.bml resource/resource.cpp resource/resource.hpp

clean:
	-@$(call delete,obj/*.o)

install: uninstall
ifeq ($(platform),x)
	sudo cp ./purify $(path)/purify
endif

uninstall:
ifeq ($(platform),x)
	if [ -f $(path)/purify ]; then sudo rm $(path)/purify; fi
endif

sync:
ifeq ($(shell id -un),byuu)
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./phoenix ]; then rm -r ./phoenix; fi
	cp -r ../nall ./nall
	cp -r ../phoenix ./phoenix
	rm -r nall/test
	rm -r phoenix/nall
	rm -r phoenix/test
endif

force:

include nall/Makefile
include phoenix/Makefile

path := /usr/local/lib
flags := -I. -O3 -fomit-frame-pointer

all:
	$(cpp) $(flags) -fPIC -o obj/ananke.o -c ananke.cpp
ifeq ($(platform),x)
	$(cpp) $(flags) -shared -Wl,-soname,libananke.so.1 -o libananke.so obj/ananke.o
else ifeq ($(platform),win)
	$(cpp) $(flags) -fPIC -o obj/phoenix.o -c phoenix/phoenix.cpp $(phoenixflags)
	$(cpp) $(flags) -shared -o phoenix.dll obj/phoenix.o $(phoenixlink)
	$(cpp) $(flags) -shared -o ananke.dll obj/ananke.o -L. -lphoenix
endif

resource: force
	sourcery resource/resource.bml resource/resource.cpp resource/resource.hpp

clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,*.so)

install: uninstall
ifeq ($(platform),x)
	if [ ! -d ~/.config/ananke ]; then mkdir ~/.config/ananke; fi
	sudo cp libananke.so $(path)/libananke.so.1
	sudo ln -s $(path)/libananke.so.1 $(path)/libananke.so
endif

uninstall:
ifeq ($(platform),x)
	if [ -f $(path)/libananke.so ]; then sudo rm $(path)/libananke.so; fi
	if [ -f $(path)/libananke.so.1 ]; then sudo rm $(path)/libananke.so.1; fi
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

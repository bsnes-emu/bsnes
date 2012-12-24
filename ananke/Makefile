include nall/Makefile
include phoenix/Makefile

path := /usr/local/lib
flags := -std=gnu++11 -I. -O3 -fomit-frame-pointer

all:
	$(cpp) $(flags) -fPIC -o ananke.o -c ananke.cpp
	$(cpp) $(flags) -shared -Wl,-soname,libananke.so.1 -o libananke.so ananke.o

resource: force
	sourcery resource/resource.bml resource/resource.cpp resource/resource.hpp

clean:
	-@$(call delete,*.o)
	-@$(call delete,*.so)

install: uninstall
	if [ ! -d ~/.config/ananke ]; then mkdir ~/.config/ananke; fi
	sudo cp libananke.so $(path)/libananke.so.1
	sudo ln -s $(path)/libananke.so.1 $(path)/libananke.so

uninstall:
	if [ -f $(path)/libananke.so ]; then sudo rm $(path)/libananke.so; fi
	if [ -f $(path)/libananke.so.1 ]; then sudo rm $(path)/libananke.so.1; fi

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

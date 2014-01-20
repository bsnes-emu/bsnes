include ../nall/Makefile
include ../phoenix/Makefile

path := /usr/local/lib
flags := $(flags) -O3 -fomit-frame-pointer -I..

all:
	$(compiler) $(cppflags) $(flags) -fPIC -o obj/ananke.o -c ananke.cpp
ifeq ($(platform),windows)
	$(compiler) $(phoenixflags) -fPIC -o obj/phoenix.o -c ../phoenix/phoenix.cpp
	$(compiler) $(link) -shared -o phoenix.dll obj/phoenix.o $(phoenixlink)
	$(compiler) $(link) -shared -o ananke.dll obj/ananke.o -L. -lphoenix
else ifeq ($(platform),macosx)
	$(compiler) $(link) -shared -dynamiclib -undefined suppress -flat_namespace -o libananke.dylib obj/ananke.o
else
	$(compiler) $(link) -shared -Wl,-soname,libananke.so.1 -o libananke.so obj/ananke.o
endif

resource: force
	sourcery resource/resource.bml resource/resource.cpp resource/resource.hpp

clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,*.dll)
	-@$(call delete,*.so)

install: uninstall
ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	if [ ! -d ~/Library/Application\ Support/ananke ]; then mkdir ~/Library/Application\ Support/ananke; fi
	sudo cp libananke.dylib $(path)/libananke.1.dylib
	sudo ln -s $(path)/libananke.1.dylib $(path)/libananke.dylib
else
	if [ ! -d ~/.config/ananke ]; then mkdir ~/.config/ananke; fi
	sudo cp libananke.so $(path)/libananke.so.1
	sudo ln -s $(path)/libananke.so.1 $(path)/libananke.so
endif

uninstall:
ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
	if [ -f $(path)/libananke.dylib ]; then sudo rm $(path)/libananke.dylib; fi
	if [ -f $(path)/libananke.1.dylib ]; then sudo rm $(path)/libananke.1.dylib; fi
else
	if [ -f $(path)/libananke.so ]; then sudo rm $(path)/libananke.so; fi
	if [ -f $(path)/libananke.so.1 ]; then sudo rm $(path)/libananke.so.1; fi
endif

force:

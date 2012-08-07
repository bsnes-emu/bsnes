include nall/Makefile
include phoenix/Makefile

application := purify
resource :=
flags := -std=gnu++0x -I. -O3 -fomit-frame-pointer
link := -s

ifeq ($(platform),win)
  resource := resource.o
  flags := -mwindows $(flags)
  link := -mwindows $(flags)
endif

all: phoenix.o $(application).o
ifeq ($(platform),win)
	windres phoenix/windows/phoenix.rc $(resource)
endif
	$(cpp) -o $(application) phoenix.o $(application).o $(resource) $(link) $(phoenixlink)

phoenix.o:
	$(cpp) -c -o phoenix.o phoenix/phoenix.cpp $(flags) $(phoenixflags)

$(application).o: $(application).cpp
	$(cpp) -c -o $(application).o $(application).cpp $(flags)

resource: force
	sourcery resource/resource.xml resource/resource.cpp resource/resource.hpp

install:
	sudo cp $(application) /usr/local/bin/$(application)

uninstall:
	sudo rm /usr/local/bin/$(application)

clean:
	-@$(call delete,*.o)

sync:
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./phoenix ]; then rm -r ./phoenix; fi
	cp -r ../nall ./nall
	cp -r ../phoenix ./phoenix
	rm -r nall/test
	rm -r phoenix/nall
	rm -r phoenix/test

force:

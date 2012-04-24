include nall/Makefile

application := purify
flags := -std=gnu++0x -I. -O3 -fomit-frame-pointer
link := -s

ifeq ($(platform),win)
  flags += -DPHOENIX_WINDOWS
  link += -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32
else ifeq ($(phoenix),qt)
  flags += -DPHOENIX_QT `pkg-config --cflags QtCore QtGui`
  link += `pkg-config --libs QtCore QtGui`
else
  flags += -DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`
  link += `pkg-config --libs gtk+-2.0`
endif

all: phoenix.o $(application).o
	$(cpp) -o $(application) phoenix.o $(application).o $(link)

phoenix.o: phoenix/phoenix.cpp
	$(cpp) -c -o phoenix.o phoenix/phoenix.cpp $(flags)

$(application).o: $(application).cpp
	$(cpp) -c -o $(application).o $(application).cpp $(flags)

install:
	sudo cp $(application) /usr/local/bin/$(application)

clean:
	$(delete) *.o

sync:
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./phoenix ]; then rm -r ./phoenix; fi
	cp -r ../nall ./nall
	cp -r ../phoenix ./phoenix
	rm -r nall/test
	rm -r phoenix/test

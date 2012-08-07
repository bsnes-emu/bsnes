# for Linux only; to synchronize includes and resources
# use cc32.bat and cc64.bat for Windows compilation

include nall/Makefile

resource: force
	sourcery resource/resource.xml resource/resource.cpp resource/resource.hpp

sync:
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./phoenix ]; then rm -r ./phoenix; fi
	cp -r ../nall ./nall
	cp -r ../phoenix ./phoenix
	rm -r nall/test
	rm -r phoenix/nall
	rm -r phoenix/test

force:

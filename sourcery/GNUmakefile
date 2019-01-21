name := sourcery
build := stable
flags += -I..

nall.path := ../nall
include $(nall.path)/GNUmakefile

objects := obj/sourcery.o

obj/sourcery.o: sourcery.cpp

all: $(objects)
	$(info Linking out/$(name) ...)
	+@$(compiler) -o out/$(name) $(objects) $(options)

verbose: nall.verbose all;

clean:
	$(call delete,obj/*)
	$(call delete,out/*)

install: all
	cp out/$(name) $(prefix)/bin/$(name)

uninstall:
	rm -f $(prefix)/bin/$(name)

include nall/Makefile

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -fPIC -O3 -I. -Iobj -fomit-frame-pointer
link    := -s
objects :=

objects += out/Pixellate2x.filter
objects += out/Scanline-Dark.filter
objects += out/Scanline-Light.filter
objects += out/Scale2x.filter
objects += out/LQ2x.filter
objects += out/HQ2x.filter

compile = $(cpp) $(link) $(flags) -o $@ -shared $<

%.filter: $<; $(call compile)

all: build;

out/Pixellate2x.filter: Pixellate2x/Pixellate2x.cpp Pixellate2x/*
out/Scanline-Dark.filter: Scanline/Scanline-Dark.cpp Scanline/*
out/Scanline-Light.filter: Scanline/Scanline-Light.cpp Scanline/*
out/Scale2x.filter: Scale2x/Scale2x.cpp Scale2x/*
out/LQ2x.filter: LQ2x/LQ2x.cpp LQ2x/*
out/HQ2x.filter: HQ2x/HQ2x.cpp HQ2x/*

build: $(objects)

install:
	mkdir -p ~/.config/batch/filters
	chmod 777 ~/.config/batch/filters
	cp out/*.filter ~/.config/batch/filters

clean:
	rm out/*.filter

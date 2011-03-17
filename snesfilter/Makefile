include nall/Makefile

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -fPIC -O3 -I. -Iobj -fomit-frame-pointer
link    := -s
objects :=

objects += out/Pixellate2x.filter
objects += out/Scanline-Black.filter
objects += out/Scanline-Dark.filter
objects += out/Scanline-Light.filter
objects += out/Scale2x.filter
objects += out/2xSaI.filter
objects += out/Super-2xSaI.filter
objects += out/Super-Eagle.filter
objects += out/LQ2x.filter
objects += out/HQ2x.filter
objects += out/NTSC-RF.filter
objects += out/NTSC-Composite.filter
objects += out/NTSC-SVideo.filter
objects += out/NTSC-RGB.filter

compile = $(cpp) $(link) $(flags) -o $@ -shared $<

%.filter: $<; $(call compile)

all: build;

out/Pixellate2x.filter: Pixellate2x/Pixellate2x.cpp Pixellate2x/*
out/Scanline-Black.filter: Scanline/Scanline-Black.cpp Scanline/*
out/Scanline-Dark.filter: Scanline/Scanline-Dark.cpp Scanline/*
out/Scanline-Light.filter: Scanline/Scanline-Light.cpp Scanline/*
out/Scale2x.filter: Scale2x/Scale2x.cpp Scale2x/*
out/2xSaI.filter: 2xSaI/2xSaI.cpp 2xSaI/*
out/Super-2xSaI.filter: 2xSaI/Super-2xSaI.cpp 2xSaI/*
out/Super-Eagle.filter: 2xSaI/Super-Eagle.cpp 2xSaI/*
out/LQ2x.filter: LQ2x/LQ2x.cpp LQ2x/*
out/HQ2x.filter: HQ2x/HQ2x.cpp HQ2x/*
out/NTSC-RF.filter: NTSC/NTSC-RF.cpp NTSC/*
out/NTSC-Composite.filter: NTSC/NTSC-Composite.cpp NTSC/*
out/NTSC-SVideo.filter: NTSC/NTSC-SVideo.cpp NTSC/*
out/NTSC-RGB.filter: NTSC/NTSC-RGB.cpp NTSC/*

build: $(objects)

install:
	mkdir -p ~/.config/bsnes/filters
	chmod 777 ~/.config/bsnes/filters
	cp out/*.filter ~/.config/bsnes/filters

clean:
	rm out/*.filter

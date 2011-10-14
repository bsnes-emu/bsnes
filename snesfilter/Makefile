include nall/Makefile

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -fPIC -O3 -I. -Iobj -fomit-frame-pointer
link    := -s
objects :=

ifeq ($(platform),x)
  flags += -fopenmp
endif

objects += out/Pixellate2x.filter
objects += out/Scanline-Dark.filter
objects += out/Scanline-Light.filter
objects += out/Scale2x.filter
objects += out/LQ2x.filter
objects += out/HQ2x.filter
objects += out/Overscan.filter
objects += out/Phosphor3x.filter

compile = $(cpp) $(link) $(flags) -o $@ -shared $<

%.filter: $<; $(call compile)

all: build;

out/Pixellate2x.filter: Pixellate2x/Pixellate2x.cpp Pixellate2x/*
out/Scanline-Dark.filter: Scanline/Scanline-Dark.cpp Scanline/*
out/Scanline-Light.filter: Scanline/Scanline-Light.cpp Scanline/*
out/Scale2x.filter: Scale2x/Scale2x.cpp Scale2x/*
out/LQ2x.filter: LQ2x/LQ2x.cpp LQ2x/*
out/HQ2x.filter: HQ2x/HQ2x.cpp HQ2x/*
out/Overscan.filter: Overscan/Overscan.cpp Overscan/*
out/Phosphor3x.filter: Phosphor3x/Phosphor3x.cpp Phosphor3x/*

build: $(objects)

install:
	mkdir -p ~/.config/bsnes/filters
	chmod 777 ~/.config/bsnes/filters
	cp out/*.filter ~/.config/bsnes/filters

clean:
	rm out/*.filter

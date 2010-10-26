include nall/Makefile

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler)) -std=gnu++0x
flags   := -O3 -I. -Iobj -fomit-frame-pointer -fPIC
link    := -s
objects :=

objects += out/pixellate2x.filter
objects += out/scanline-00.filter
objects += out/scanline-25.filter
objects += out/scanline-50.filter
objects += out/scanline-75.filter
objects += out/scale2x.filter
objects += out/2xsai.filter
objects += out/super-2xsai.filter
objects += out/super-eagle.filter
objects += out/lq2x.filter
objects += out/hq2x.filter
objects += out/ntsc-rf.filter
objects += out/ntsc-composite.filter
objects += out/ntsc-svideo.filter
objects += out/ntsc-rgb.filter

compile = $(cpp) $(link) $(flags) -o $@ -shared $<

%.filter: $<; $(call compile)

all: build;

out/pixellate2x.filter: pixellate2x/pixellate2x.cpp pixellate2x/*
out/scanline-00.filter: scanline/scanline-00.cpp scanline/*
out/scanline-25.filter: scanline/scanline-25.cpp scanline/*
out/scanline-50.filter: scanline/scanline-50.cpp scanline/*
out/scanline-75.filter: scanline/scanline-75.cpp scanline/*
out/scale2x.filter: scale2x/scale2x.cpp scale2x/*
out/2xsai.filter: 2xsai/2xsai.cpp 2xsai/*
out/super-2xsai.filter: 2xsai/super-2xsai.cpp 2xsai/*
out/super-eagle.filter: 2xsai/super-eagle.cpp 2xsai/*
out/lq2x.filter: lq2x/lq2x.cpp lq2x/*
out/hq2x.filter: hq2x/hq2x.cpp hq2x/*
out/ntsc-rf.filter: ntsc/ntsc-rf.cpp ntsc/*
out/ntsc-composite.filter: ntsc/ntsc-composite.cpp ntsc/*
out/ntsc-svideo.filter: ntsc/ntsc-svideo.cpp ntsc/*
out/ntsc-rgb.filter: ntsc/ntsc-rgb.cpp ntsc/*

build: $(objects)

install:

clean:

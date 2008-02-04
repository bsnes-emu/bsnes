include lib/nall/Makefile.string

prefix = /usr/local
arch = ARCH_LSB

################
### compiler ###
################

ifneq ($(findstring gcc,$(compiler)),) # GCC family
  flags = -O3 -fomit-frame-pointer -Ilib
  c = $(compiler) $(flags)
  cpp = $(subst cc,++,$(compiler)) $(flags)
  obj = o
  rule = -c $< -o $@
  link =
  mkbin = -o$1
  mkdef = -D$1
  mklib = -l$1
else ifeq ($(compiler),cl) # Visual C++
  flags = /nologo /wd4355 /wd4996 /O2 /EHsc /Ilib
  c = cl $(flags)
  cpp = cl $(flags)
  obj = obj
  rule = /c $< /Fo$@
  link = /link
  mkbin = /Fe$1
  mkdef = /D$1
  mklib = $1.lib
else
  unknown_compiler: help;
endif

##########
### os ###
##########

ifeq ($(platform),x) # X11
  ruby = video.glx video.xv video.sdl audio.openal audio.oss audio.ao input.sdl input.x
  arch += PLATFORM_X
  link += `pkg-config --libs gtk+-2.0`
  delete = rm -f $1
else ifeq ($(platform),win) # Windows
  ruby = video.direct3d video.directdraw video.gdi audio.directsound input.directinput
  arch += PLATFORM_WIN
  link += $(if $(findstring mingw,$(compiler)),-mwindows)
  link += $(call mklib,uuid)
  link += $(call mklib,kernel32)
  link += $(call mklib,user32)
  link += $(call mklib,gdi32)
  link += $(call mklib,shell32)
  link += $(call mklib,winmm)
  link += $(call mklib,comdlg32)
  link += $(call mklib,comctl32)
  delete = $(if $(findstring i586-mingw-gcc,$(compiler)),rm -f $1,del $(subst /,\,$1))
else
  unknown_platform: help;
endif

############
### ruby ###
############

link += $(if $(findstring video.direct3d,$(ruby)),$(call mklib,d3d9))
link += $(if $(findstring video.directdraw,$(ruby)),$(call mklib,ddraw))
link += $(if $(findstring video.glx,$(ruby)),$(call mklib,GL))
link += $(if $(findstring video.xv,$(ruby)),$(call mklib,Xv))
link += $(if $(findstring audio.ao,$(ruby)),$(call mklib,ao))
link += $(if $(findstring audio.directsound,$(ruby)),$(call mklib,dsound))
link += $(if $(findstring audio.openal,$(ruby)),$(call mklib,openal) $(call mklib,alut))
link += $(if $(findstring input.directinput,$(ruby)),$(call mklib,dinput8) $(call mklib,dxguid))
link += $(if $(findstring input.sdl,$(ruby)),`sdl-config --libs`)

####################################
### main target and dependencies ###
####################################

objects = main libco hiro ruby $(ruby) string reader cart cheat \
  memory smemory cpu scpu smp ssmp bdsp ppu bppu snes \
  bsx srtc sdd1 cx4 dsp1 dsp2 dsp3 dsp4 obc1 st010

ifeq ($(enable_gzip),true)
  objects += adler32 compress crc32 deflate gzio inffast inflate inftrees ioapi trees unzip zip zutil
  flags += $(call mkdef,GZIP_SUPPORT)
endif

ifeq ($(enable_jma),true)
  objects += jma jcrc32 lzmadec 7zlzma iiostrm inbyte lzma winout
  flags += $(call mkdef,JMA_SUPPORT)
endif

arch := $(patsubst %,$(call mkdef,%),$(arch))
objects := $(patsubst %,obj/%.$(obj),$(objects))
rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),$(call mkdef,$c))

# Windows resource file
ifeq ($(platform),win)
  ifeq ($(compiler),cl)
    objects += obj/bsnes.res
  else ifneq ($(findstring gcc,$(compiler)),)
    objects += obj/bsnesrc.$(obj)
  endif
endif

################
### implicit ###
################

compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $1 $(rule), \
      $(if $(filter %.cpp,$<), \
        $(cpp) $1 $(rule) \
      ) \
    ) \
  )

%.$(obj): $<; $(call compile)

all: build;

############
### main ###
############

obj/main.$(obj): ui/main.cpp config/* lib/nall/* lib/ruby/* ui/* ui/loader/* ui/settings/*
	$(call compile,$(arch))

obj/bsnes.res     : ui/bsnes.rc; rc /r /foobj/bsnes.res ui/bsnes.rc
obj/bsnesrc.$(obj): ui/bsnes.rc; windres -I data ui/bsnes.rc obj/bsnesrc.$(obj)

############
### ruby ###
############

obj/ruby.$(obj)             : lib/ruby/ruby.cpp lib/ruby/*
	$(call compile,$(rubydef))
obj/video.direct3d.$(obj)   : lib/ruby/video/direct3d.cpp lib/ruby/video/direct3d.*
obj/video.directdraw.$(obj) : lib/ruby/video/directdraw.cpp lib/ruby/video/directdraw.*
obj/video.gdi.$(obj)        : lib/ruby/video/gdi.cpp lib/ruby/video/gdi.*
obj/video.glx.$(obj)        : lib/ruby/video/glx.cpp lib/ruby/video/glx.*
obj/video.sdl.$(obj)        : lib/ruby/video/sdl.cpp lib/ruby/video/sdl.*
	$(call compile,`sdl-config --cflags`)
obj/video.xv.$(obj)         : lib/ruby/video/xv.cpp lib/ruby/video/xv.*
obj/audio.ao.$(obj)         : lib/ruby/audio/ao.cpp lib/ruby/audio/ao.*
obj/audio.directsound.$(obj): lib/ruby/audio/directsound.cpp lib/ruby/audio/directsound.*
obj/audio.openal.$(obj)     : lib/ruby/audio/openal.cpp lib/ruby/audio/openal.*
obj/audio.oss.$(obj)        : lib/ruby/audio/oss.cpp lib/ruby/audio/oss.*
obj/input.directinput.$(obj): lib/ruby/input/directinput.cpp lib/ruby/input/directinput.*
obj/input.sdl.$(obj)        : lib/ruby/input/sdl.cpp lib/ruby/input/sdl.*
	$(call compile,`sdl-config --cflags`)
obj/input.x.$(obj)          : lib/ruby/input/x.cpp lib/ruby/input/x.*

############
### hiro ###
############

obj/hiro.$(obj): lib/hiro.cpp lib/hiro.* lib/hiro_gtk/* lib/hiro_win/*
	$(call compile,$(if $(call streq,$(platform),x),`pkg-config --cflags gtk+-2.0`))

#################
### libraries ###
#################

obj/libco.$(obj): lib/libco.c lib/libco.* lib/libco/*
obj/string.$(obj): lib/nall/string.cpp lib/nall/*

#################
### utilities ###
#################

obj/reader.$(obj): reader/reader.cpp reader/*
obj/cart.$(obj)  : cart/cart.cpp cart/*
obj/cheat.$(obj) : cheat/cheat.cpp cheat/*

##############
### memory ###
##############

obj/memory.$(obj) : memory/memory.cpp memory/*
obj/bmemory.$(obj): memory/bmemory/bmemory.cpp memory/bmemory/* memory/bmemory/mapper/*
obj/smemory.$(obj): memory/smemory/smemory.cpp memory/smemory/* memory/smemory/mapper/*

###########
### cpu ###
###########

obj/cpu.$(obj) : cpu/cpu.cpp cpu/*
obj/scpu.$(obj): cpu/scpu/scpu.cpp cpu/scpu/* cpu/scpu/core/* cpu/scpu/dma/* cpu/scpu/memory/* cpu/scpu/mmio/* cpu/scpu/timing/*

###########
### smp ###
###########

obj/smp.$(obj) : smp/smp.cpp smp/*
obj/ssmp.$(obj): smp/ssmp/ssmp.cpp smp/ssmp/* smp/ssmp/core/* smp/ssmp/memory/* smp/ssmp/timing/*

###########
### dsp ###
###########

obj/adsp.$(obj): dsp/adsp/adsp.cpp dsp/adsp/*
obj/bdsp.$(obj): dsp/bdsp/bdsp.cpp dsp/bdsp/*

###########
### ppu ###
###########

obj/ppu.$(obj) : ppu/ppu.cpp ppu/*
obj/bppu.$(obj): ppu/bppu/bppu.cpp ppu/bppu/*

############
### snes ###
############

obj/snes.$(obj): snes/snes.cpp snes/* snes/scheduler/* snes/video/* snes/audio/* snes/input/*

#####################
### special chips ###
#####################

obj/bsx.$(obj)  : chip/bsx/bsx.cpp chip/bsx/*
obj/srtc.$(obj) : chip/srtc/srtc.cpp chip/srtc/*
obj/sdd1.$(obj) : chip/sdd1/sdd1.cpp chip/sdd1/*
obj/cx4.$(obj)  : chip/cx4/cx4.cpp chip/cx4/*
obj/dsp1.$(obj) : chip/dsp1/dsp1.cpp chip/dsp1/*
obj/dsp2.$(obj) : chip/dsp2/dsp2.cpp chip/dsp2/*
obj/dsp3.$(obj) : chip/dsp3/dsp3.cpp chip/dsp3/*
obj/dsp4.$(obj) : chip/dsp4/dsp4.cpp chip/dsp4/*
obj/obc1.$(obj) : chip/obc1/obc1.cpp chip/obc1/*
obj/st010.$(obj): chip/st010/st010.cpp chip/st010/*

############
### zlib ###
############

obj/adler32.$(obj) : reader/zlib/adler32.c reader/zlib/*
obj/compress.$(obj): reader/zlib/compress.c reader/zlib/*
obj/crc32.$(obj)   : reader/zlib/crc32.c reader/zlib/*
obj/deflate.$(obj) : reader/zlib/deflate.c reader/zlib/*
obj/gzio.$(obj)    : reader/zlib/gzio.c reader/zlib/*
obj/inffast.$(obj) : reader/zlib/inffast.c reader/zlib/*
obj/inflate.$(obj) : reader/zlib/inflate.c reader/zlib/*
obj/inftrees.$(obj): reader/zlib/inftrees.c reader/zlib/*
obj/ioapi.$(obj)   : reader/zlib/ioapi.c reader/zlib/*
obj/trees.$(obj)   : reader/zlib/trees.c reader/zlib/*
obj/unzip.$(obj)   : reader/zlib/unzip.c reader/zlib/*
obj/zip.$(obj)     : reader/zlib/zip.c reader/zlib/*
obj/zutil.$(obj)   : reader/zlib/zutil.c reader/zlib/*

###########
### jma ###
###########

obj/jma.$(obj)    : reader/jma/jma.cpp reader/jma/*
obj/jcrc32.$(obj) : reader/jma/jcrc32.cpp reader/jma/*
obj/lzmadec.$(obj): reader/jma/lzmadec.cpp reader/jma/*
obj/7zlzma.$(obj) : reader/jma/7zlzma.cpp reader/jma/*
obj/iiostrm.$(obj): reader/jma/iiostrm.cpp reader/jma/*
obj/inbyte.$(obj) : reader/jma/inbyte.cpp reader/jma/*
obj/lzma.$(obj)   : reader/jma/lzma.cpp reader/jma/*
obj/winout.$(obj) : reader/jma/winout.cpp reader/jma/*

###############
### targets ###
###############

build: $(objects)
	$(strip $(cpp) $(call mkbin,../bsnes) $(objects) $(link))

install:
	install -m 755 ../bsnes $(prefix)/bin/bsnes
	install -m 644 data/bsnes.png $(prefix)/share/icons/bsnes.png

clean:
	-@$(call delete,obj/*.$(obj))
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

help:
	@echo "Usage: $(MAKE) platform=(os) compiler=(cc) [options]"
	@echo ""
	@echo "Supported platforms:"
	@echo "  x   - Linux / BSD (x86, x86-64)"
	@echo "  win - Windows (x86, x86-64)"
	@echo ""
	@echo "Supported compilers:"
	@echo "  gcc              - GCC compiler"
	@echo "  mingw32-gcc      - MinGW compiler"
	@echo "  i586-mingw32-gcc - MinGW cross compiler"
	@echo "  cl               - Visual C++"
	@echo ""
	@echo "Available options:"
	@echo "  enable_gzip=[true|false] - Enable ZIP / GZ support (default=false)"
	@echo "  enable_jma=[true|false]  - Enable JMA support (default=false)"
	@echo ""
	@echo "Example: $(MAKE) platform=x compiler=gcc enable_gzip=true"
	@echo ""

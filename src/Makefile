include lib/nall/Makefile.string
prefix = /usr/local
ui = ui_qt

################
### compiler ###
################

ifneq ($(findstring gcc,$(compiler)),) # GCC family
  flags = -O3 -fomit-frame-pointer -Ilib
  # note: libco *requires* -fomit-frame-pointer on i386 arch
  libcoflags := $(flags) -static
  c = $(compiler)
  cpp = $(subst cc,++,$(compiler))
  obj = o
  rule = -c $< -o $@
  link = -s
  mkbin = -o$1
  mkdef = -D$1
  mkincpath = -I$1
  mklib = -l$1
  mklibpath = -L$1

  # profile-guided optimization:
  # flags += -fprofile-generate
  # link += -lgcov
  # flags += -fprofile-use
else ifeq ($(compiler),cl) # Visual C++
  flags = /nologo /wd4355 /wd4805 /wd4996 /Ox /GL /EHsc /Ilib
  libcoflags = $(flags)
  c = cl
  cpp = cl
  obj = obj
  rule = /c $< /Fo$@
  link = /link
  mkbin = /Fe$1
  mkdef = /D$1
  mkincpath = /I$1
  mklib = $1.lib
  mklibpath = /L$1
else
  unknown_compiler: help;
endif

##########
### os ###
##########

ifeq ($(platform),x) # X11
  ruby = video.glx video.xv video.sdl audio.alsa audio.openal audio.oss audio.pulseaudio audio.ao input.sdl input.x
  delete = rm -f $1
else ifeq ($(platform),win) # Windows
  mingw_link_flags = -mwindows
  # mingw_links_flags = -mconsole

  # enable static linking to Qt for Windows build
  mingw_link_flags += -enable-stdcall-fixup -Wl,-s -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc

  ruby = video.direct3d video.wgl video.directdraw video.gdi audio.directsound input.rawinput input.directinput
  delete = $(if $(findstring i586-mingw-gcc,$(compiler)),rm -f $1,del $(subst /,\,$1))
  link += $(if $(findstring mingw,$(compiler)),$(mingw_link_flags))
  link += $(call mklib,uuid)
  link += $(call mklib,kernel32)
  link += $(call mklib,user32)
  link += $(call mklib,gdi32)
  link += $(call mklib,shell32)
else
  unknown_platform: help;
endif

############
### ruby ###
############

rubyflags = $(if $(findstring .sdl,$(ruby)),`sdl-config --cflags`)
link += $(if $(findstring .sdl,$(ruby)),`sdl-config --libs`)

link += $(if $(findstring video.direct3d,$(ruby)),$(call mklib,d3d9))
link += $(if $(findstring video.directdraw,$(ruby)),$(call mklib,ddraw))
link += $(if $(findstring video.glx,$(ruby)),$(call mklib,GL))
link += $(if $(findstring video.wgl,$(ruby)),$(call mklib,opengl32))
link += $(if $(findstring video.xv,$(ruby)),$(call mklib,Xv))
link += $(if $(findstring audio.alsa,$(ruby)),$(call mklib,asound))
link += $(if $(findstring audio.ao,$(ruby)),$(call mklib,ao))
link += $(if $(findstring audio.directsound,$(ruby)),$(call mklib,dsound))
link += $(if $(findstring audio.openal,$(ruby)),$(if $(call streq,$(platform),x),$(call mklib,openal),$(call mklib,openal32)))
link += $(if $(findstring audio.pulseaudio,$(ruby)),$(call mklib,pulse-simple))
link += $(if $(findstring input.directinput,$(ruby)),$(call mklib,dinput8) $(call mklib,dxguid))
link += $(if $(findstring input.rawinput,$(ruby)),$(call mklib,xinput) $(call mklib,dinput8) $(call mklib,dxguid))

####################
### core objects ###
####################

objects = libco ruby libfilter string \
  reader cartridge cheat \
  memory smemory cpu cpucore scpu smp smpcore ssmp sdsp ppu bppu system \
  sgb sa1 bsx srtc sdd1 spc7110 cx4 dsp1 dsp2 dsp3 dsp4 obc1 st010

ifeq ($(enable_gzip),true)
  objects += adler32 compress crc32 deflate gzio inffast inflate inftrees ioapi trees unzip zip zutil
  flags += $(call mkdef,GZIP_SUPPORT)
endif

ifeq ($(enable_jma),true)
  objects += jma jcrc32 lzmadec 7zlzma iiostrm inbyte lzma winout
  flags += $(call mkdef,JMA_SUPPORT)
endif

sgbflags =
ifeq ($(sgb),gambatte)
  sgbflags += $(call mkdef,SGB_GAMBATTE)
  link += $(call mklibpath,lib/libgambatte)
  link += $(call mklib,gambatte)
endif

######################
### implicit rules ###
######################

compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $(flags) $1 $(rule), \
      $(if $(filter %.cpp,$<), \
        $(cpp) $(flags) $1 $(rule) \
      ) \
    ) \
  )

%.$(obj): $<;	$(call compile)

all: build;

include $(ui)/Makefile
objects := $(patsubst %,obj/%.$(obj),$(objects))
rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),$(call mkdef,$c))

#################
### libraries ###
#################

obj/ruby.$(obj): lib/ruby/ruby.cpp lib/ruby/* lib/ruby/video/* lib/ruby/audio/* lib/ruby/input/*
	$(call compile,$(rubydef) $(rubyflags))
obj/libco.$(obj): lib/libco/libco.c lib/libco/*
	$(c) $(libcoflags) $(rule)
obj/libfilter.$(obj): lib/libfilter/libfilter.cpp lib/libfilter/*
obj/string.$(obj): lib/nall/string.cpp lib/nall/*
obj/reader.$(obj): lib/reader/reader.cpp lib/reader/*

#################
### utilities ###
#################

obj/cartridge.$(obj): cartridge/cartridge.cpp cartridge/*
obj/cheat.$(obj)    : cheat/cheat.cpp cheat/*

##############
### memory ###
##############

obj/memory.$(obj) : memory/memory.cpp memory/*
obj/smemory.$(obj): memory/smemory/smemory.cpp memory/smemory/* memory/smemory/mapper/*

###########
### cpu ###
###########

obj/cpu.$(obj)    : cpu/cpu.cpp cpu/*
obj/cpucore.$(obj): cpu/core/core.cpp cpu/core/* cpu/core/disasm/*
obj/scpu.$(obj)   : cpu/scpu/scpu.cpp cpu/scpu/* cpu/scpu/dma/* cpu/scpu/memory/* cpu/scpu/mmio/* cpu/scpu/timing/*

###########
### smp ###
###########

obj/smp.$(obj)    : smp/smp.cpp smp/*
obj/smpcore.$(obj): smp/core/core.cpp smp/core/* smp/core/disasm/*
obj/ssmp.$(obj)   : smp/ssmp/ssmp.cpp smp/ssmp/* smp/ssmp/memory/* smp/ssmp/timing/*

###########
### dsp ###
###########

obj/adsp.$(obj): dsp/adsp/adsp.cpp dsp/adsp/*
obj/sdsp.$(obj): dsp/sdsp/sdsp.cpp dsp/sdsp/*

###########
### ppu ###
###########

obj/ppu.$(obj) : ppu/ppu.cpp ppu/*
obj/bppu.$(obj): ppu/bppu/bppu.cpp ppu/bppu/*

##############
### system ###
##############

obj/system.$(obj): system/system.cpp system/* system/scheduler/* system/video/* system/audio/* system/input/*

#####################
### special chips ###
#####################

obj/sgb.$(obj): chip/sgb/sgb.cpp chip/sgb/* chip/sgb/interface/*
	$(call compile,$(sgbflags))
obj/sa1.$(obj)    : chip/sa1/sa1.cpp chip/sa1/* chip/sa1/bus/* chip/sa1/dma/* chip/sa1/memory/* chip/sa1/mmio/*
obj/bsx.$(obj)    : chip/bsx/bsx.cpp chip/bsx/*
obj/srtc.$(obj)   : chip/srtc/srtc.cpp chip/srtc/*
obj/sdd1.$(obj)   : chip/sdd1/sdd1.cpp chip/sdd1/*
obj/spc7110.$(obj): chip/spc7110/spc7110.cpp chip/spc7110/*
obj/cx4.$(obj)    : chip/cx4/cx4.cpp chip/cx4/*
obj/dsp1.$(obj)   : chip/dsp1/dsp1.cpp chip/dsp1/*
obj/dsp2.$(obj)   : chip/dsp2/dsp2.cpp chip/dsp2/*
obj/dsp3.$(obj)   : chip/dsp3/dsp3.cpp chip/dsp3/*
obj/dsp4.$(obj)   : chip/dsp4/dsp4.cpp chip/dsp4/*
obj/obc1.$(obj)   : chip/obc1/obc1.cpp chip/obc1/*
obj/st010.$(obj)  : chip/st010/st010.cpp chip/st010/*

############
### zlib ###
############

obj/adler32.$(obj) : lib/zlib/adler32.c lib/zlib/*
obj/compress.$(obj): lib/zlib/compress.c lib/zlib/*
obj/crc32.$(obj)   : lib/zlib/crc32.c lib/zlib/*
obj/deflate.$(obj) : lib/zlib/deflate.c lib/zlib/*
obj/gzio.$(obj)    : lib/zlib/gzio.c lib/zlib/*
obj/inffast.$(obj) : lib/zlib/inffast.c lib/zlib/*
obj/inflate.$(obj) : lib/zlib/inflate.c lib/zlib/*
obj/inftrees.$(obj): lib/zlib/inftrees.c lib/zlib/*
obj/ioapi.$(obj)   : lib/zlib/ioapi.c lib/zlib/*
obj/trees.$(obj)   : lib/zlib/trees.c lib/zlib/*
obj/unzip.$(obj)   : lib/zlib/unzip.c lib/zlib/*
obj/zip.$(obj)     : lib/zlib/zip.c lib/zlib/*
obj/zutil.$(obj)   : lib/zlib/zutil.c lib/zlib/*

##############
### libjma ###
##############

obj/jma.$(obj)    : lib/libjma/jma.cpp lib/libjma/*
obj/jcrc32.$(obj) : lib/libjma/jcrc32.cpp lib/libjma/*
obj/lzmadec.$(obj): lib/libjma/lzmadec.cpp lib/libjma/*
obj/7zlzma.$(obj) : lib/libjma/7zlzma.cpp lib/libjma/*
obj/iiostrm.$(obj): lib/libjma/iiostrm.cpp lib/libjma/*
obj/inbyte.$(obj) : lib/libjma/inbyte.cpp lib/libjma/*
obj/lzma.$(obj)   : lib/libjma/lzma.cpp lib/libjma/*
obj/winout.$(obj) : lib/libjma/winout.cpp lib/libjma/*

###############
### targets ###
###############

build: ui_build $(objects)
	$(strip $(cpp) $(call mkbin,../bsnes) $(objects) $(link))

install:
	install -D -m 755 ../bsnes $(DESTDIR)$(prefix)/bin/bsnes
	install -D -m 644 data/bsnes.png $(DESTDIR)$(prefix)/share/pixmaps/bsnes.png
	install -D -m 644 data/bsnes.desktop $(DESTDIR)$(prefix)/share/applications/bsnes.desktop

clean: ui_clean
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

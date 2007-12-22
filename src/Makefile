######################
### bsnes makefile ###
######################

ifeq ($(PLATFORM),)
null_: help
endif

##################################
### platform-specific settings ###
##################################

PREFIX = /usr/local

ifeq ($(PLATFORM),x-gcc-x86)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86 -DUI_MIU `pkg-config --cflags gtk+-2.0`
AS = yasm
ASFLAGS = -f elf
LIBS = `pkg-config --libs gtk+-2.0` -lXv -lao
LIBCO = libco.x86
MIU = miu.gtk
VAI = video.xv.$(OBJ) video.gtk.$(OBJ) audio.ao.$(OBJ) input.x.$(OBJ)
endif

ifeq ($(PLATFORM),x-gcc-x86-64)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86_64 -DUI_MIU `pkg-config --cflags gtk+-2.0`
AS = yasm
ASFLAGS = -f elf64
LIBS = `pkg-config --libs gtk+-2.0` -lXv -lao
LIBCO = libco.x86-64
MIU = miu.gtk
VAI = video.xv.$(OBJ) video.gtk.$(OBJ) audio.ao.$(OBJ) input.x.$(OBJ)
endif

ifeq ($(PLATFORM),win-mingw-x86)
OS = win
CC = mingw32-gcc
CFLAGS = -mwindows -O3 -fomit-frame-pointer -DPLATFORM_WIN -DCOMPILER_GCC -DPROCESSOR_X86 -DUI_MIU
AS = nasm
ASFLAGS = -f win32 -DWIN
LIBS = -ld3d9 -lddraw -ldsound -ldinput8 -ldxguid -luuid -lkernel32 -luser32 -lgdi32 -lshell32 -lwinmm -lcomdlg32 -lcomctl32
LIBCO = libco.x86
MIU = miu.win
VAI = video.direct3d.$(OBJ) video.directdraw.$(OBJ) video.gdi.$(OBJ) audio.directsound.$(OBJ) input.directinput.$(OBJ)
endif

ifeq ($(PLATFORM),win-visualc-x86)
OS = win
CC = cl
CFLAGS = /nologo /wd4996 /O2 /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_MIU
AS = nasm
ASFLAGS = -f win32 -DWIN
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib shell32.lib winmm.lib comdlg32.lib comctl32.lib
LIBCO = libco.x86
MIU = miu.win
VAI = video.direct3d.$(OBJ) video.directdraw.$(OBJ) video.gdi.$(OBJ) audio.directsound.$(OBJ) input.directinput.$(OBJ)
endif

#####################################
### compiler / assembler switches ###
#####################################

ifeq ($(CC),gcc)
OUT = -obsnes
CPP = g++
OBJ = o
CARGS = -c $< -o $@
DEFINE = -D
endif

ifeq ($(CC),mingw32-gcc)
OUT = -obsnes
CPP = mingw32-g++
OBJ = o
CARGS = -c $< -o $@
DEFINE = -D
endif

ifeq ($(CC),cl)
OUT = /Febsnes
CPP = cl
OBJ = obj
CARGS = /c $< /Fo$@
DEFINE = /D
endif

ifeq ($(AS),nasm)
ASARGS = $< -o $@
endif

ifeq ($(AS),yasm)
ASARGS = $< -o $@
endif

###################
### OS switches ###
###################

ifeq ($(OS),unix)
RM = rm -f
endif

ifeq ($(OS),win)
OUT := $(OUT).exe
RM = del
endif

####################################
### main target and dependencies ###
####################################

OBJECTS = main.$(OBJ) $(LIBCO).$(OBJ) $(MIU).$(OBJ) $(VAI) bstring.$(OBJ) \
  reader.$(OBJ) cart.$(OBJ) cheat.$(OBJ) memory.$(OBJ) smemory.$(OBJ) \
  cpu.$(OBJ) scpu.$(OBJ) smp.$(OBJ) ssmp.$(OBJ) bdsp.$(OBJ) ppu.$(OBJ) \
  bppu.$(OBJ) snes.$(OBJ) bsx.$(OBJ) superfx.$(OBJ) srtc.$(OBJ) \
  sdd1.$(OBJ) cx4.$(OBJ) dsp1.$(OBJ) dsp2.$(OBJ) dsp3.$(OBJ) dsp4.$(OBJ) \
  obc1.$(OBJ) st010.$(OBJ)

ifeq ($(GZIP_SUPPORT),true)
  OBJECTS += adler32.$(OBJ) compress.$(OBJ) crc32.$(OBJ) deflate.$(OBJ) \
    gzio.$(OBJ) inffast.$(OBJ) inflate.$(OBJ) inftrees.$(OBJ) ioapi.$(OBJ) \
    trees.$(OBJ) unzip.$(OBJ) zip.$(OBJ) zutil.$(OBJ)
  CFLAGS += $(DEFINE)GZIP_SUPPORT
endif

ifeq ($(JMA_SUPPORT),true)
  OBJECTS += jma.$(OBJ) jcrc32.$(OBJ) lzmadec.$(OBJ) 7zlzma.$(OBJ) \
    iiostrm.$(OBJ) inbyte.$(OBJ) lzma.$(OBJ) winout.$(OBJ)
  CFLAGS += $(DEFINE)JMA_SUPPORT
endif

ifeq ($(OS),win)
  ifeq ($(CC),cl)
    OBJECTS += bsnes.res
  endif
  ifeq ($(CC),mingw32-gcc)
    OBJECTS += bsnesrc.o
  endif
endif

all: $(OBJECTS)
	$(CPP) $(OUT) $(CFLAGS) $(OBJECTS) $(LIBS) $(LINK)

######################
### implicit rules ###
######################

%.$(OBJ): $<
	$(if $(filter %.asm,$<),$(AS) $(ASFLAGS) $(ASARGS))
	$(if $(filter %.s,$<),$(AS) $(ASFLAGS) $(ASARGS))
	$(if $(filter %.c,$<),$(CC) $(CFLAGS) $(CARGS))
	$(if $(filter %.cpp,$<),$(CPP) $(CFLAGS) $(CARGS))

############
### main ###
############

main.$(OBJ): ui/main.cpp config/* \
ui/* ui/vai/* \
ui/miu/* ui/miu/loader/* ui/miu/settings/*

bsnes.res: ui/bsnes.rc ; rc /r /fobsnes.res ui/bsnes.rc
bsnesrc.o: ui/bsnes.rc ; windres -I data ui/bsnes.rc bsnesrc.o

##########
### ui ###
##########

video.direct3d.$(OBJ)   : ui/vai/video/video.direct3d.cpp ui/vai/video/*
video.directdraw.$(OBJ) : ui/vai/video/video.directdraw.cpp ui/vai/video/*
video.gdi.$(OBJ)        : ui/vai/video/video.gdi.cpp ui/vai/video/*
video.gtk.$(OBJ)        : ui/vai/video/video.gtk.cpp ui/vai/video/*
video.xv.$(OBJ)         : ui/vai/video/video.xv.cpp ui/vai/video/*
audio.ao.$(OBJ)         : ui/vai/audio/audio.ao.cpp ui/vai/audio/*
audio.directsound.$(OBJ): ui/vai/audio/audio.directsound.cpp ui/vai/audio/*
input.directinput.$(OBJ): ui/vai/input/input.directinput.cpp ui/vai/input/*
input.x.$(OBJ)          : ui/vai/input/input.x.cpp ui/vai/input/*

#############
### libco ###
#############

libco.x86.$(OBJ)   : lib/libco/libco.x86.asm lib/libco/*
libco.x86-64.$(OBJ): lib/libco/libco.x86-64.asm lib/libco/*
libco.pcc.$(OBJ)   : lib/libco/libco.ppc.s lib/libco/*
libco.ppc64.$(OBJ) : lib/libco/libco.ppc64.s lib/libco/*

###########
### miu ###
###########

miu.gtk.$(OBJ): lib/miu.gtk/miu.gtk.cpp lib/miu.gtk/*
miu.win.$(OBJ): lib/miu.win/miu.win.cpp lib/miu.win/*

#################
### libraries ###
#################

bstring.$(OBJ): lib/bstring.cpp lib/*

#################
### utilities ###
#################

reader.$(OBJ): reader/reader.cpp reader/*
cart.$(OBJ)  : cart/cart.cpp cart/*
cheat.$(OBJ) : cheat/cheat.cpp cheat/*

##############
### memory ###
##############

memory.$(OBJ) : memory/memory.cpp memory/*
bmemory.$(OBJ): memory/bmemory/bmemory.cpp memory/bmemory/* memory/bmemory/mapper/*
smemory.$(OBJ): memory/smemory/smemory.cpp memory/smemory/* memory/smemory/mapper/*

###########
### cpu ###
###########

cpu.$(OBJ) : cpu/cpu.cpp cpu/*
scpu.$(OBJ): cpu/scpu/scpu.cpp cpu/scpu/* cpu/scpu/core/* cpu/scpu/dma/* cpu/scpu/memory/* cpu/scpu/mmio/* cpu/scpu/timing/*

###########
### smp ###
###########

smp.$(OBJ) : smp/smp.cpp smp/*
ssmp.$(OBJ): smp/ssmp/ssmp.cpp smp/ssmp/* smp/ssmp/core/* smp/ssmp/memory/* smp/ssmp/timing/*

###########
### dsp ###
###########

adsp.$(OBJ): dsp/adsp/adsp.cpp dsp/adsp/*
bdsp.$(OBJ): dsp/bdsp/bdsp.cpp dsp/bdsp/*

###########
### ppu ###
###########

ppu.$(OBJ) : ppu/ppu.cpp ppu/*
bppu.$(OBJ): ppu/bppu/bppu.cpp ppu/bppu/*

############
### snes ###
############

snes.$(OBJ): snes/snes.cpp snes/* snes/scheduler/* snes/video/* snes/audio/* snes/input/*

#####################
### special chips ###
#####################

bsx.$(OBJ)    : chip/bsx/bsx.cpp chip/bsx/*
superfx.$(OBJ): chip/superfx/superfx.cpp chip/superfx/* chip/superfx/core/* chip/superfx/memory/*
srtc.$(OBJ)   : chip/srtc/srtc.cpp chip/srtc/*
sdd1.$(OBJ)   : chip/sdd1/sdd1.cpp chip/sdd1/*
cx4.$(OBJ)    : chip/cx4/cx4.cpp chip/cx4/*
dsp1.$(OBJ)   : chip/dsp1/dsp1.cpp chip/dsp1/*
dsp2.$(OBJ)   : chip/dsp2/dsp2.cpp chip/dsp2/*
dsp3.$(OBJ)   : chip/dsp3/dsp3.cpp chip/dsp3/*
dsp4.$(OBJ)   : chip/dsp4/dsp4.cpp chip/dsp4/*
obc1.$(OBJ)   : chip/obc1/obc1.cpp chip/obc1/*
st010.$(OBJ)  : chip/st010/st010.cpp chip/st010/*

############
### zlib ###
############

adler32.$(OBJ) : reader/zlib/adler32.c reader/zlib/*
compress.$(OBJ): reader/zlib/compress.c reader/zlib/*
crc32.$(OBJ)   : reader/zlib/crc32.c reader/zlib/*
deflate.$(OBJ) : reader/zlib/deflate.c reader/zlib/*
gzio.$(OBJ)    : reader/zlib/gzio.c reader/zlib/*
inffast.$(OBJ) : reader/zlib/inffast.c reader/zlib/*
inflate.$(OBJ) : reader/zlib/inflate.c reader/zlib/*
inftrees.$(OBJ): reader/zlib/inftrees.c reader/zlib/*
ioapi.$(OBJ)   : reader/zlib/ioapi.c reader/zlib/*
trees.$(OBJ)   : reader/zlib/trees.c reader/zlib/*
unzip.$(OBJ)   : reader/zlib/unzip.c reader/zlib/*
zip.$(OBJ)     : reader/zlib/zip.c reader/zlib/*
zutil.$(OBJ)   : reader/zlib/zutil.c reader/zlib/*

###########
### jma ###
###########

jma.$(OBJ)    : reader/jma/jma.cpp reader/jma/*
jcrc32.$(OBJ) : reader/jma/jcrc32.cpp reader/jma/*
lzmadec.$(OBJ): reader/jma/lzmadec.cpp reader/jma/*
7zlzma.$(OBJ) : reader/jma/7zlzma.cpp reader/jma/*
iiostrm.$(OBJ): reader/jma/iiostrm.cpp reader/jma/*
inbyte.$(OBJ) : reader/jma/inbyte.cpp reader/jma/*
lzma.$(OBJ)   : reader/jma/lzma.cpp reader/jma/*
winout.$(OBJ) : reader/jma/winout.cpp reader/jma/*

####################
### misc targets ###
####################

install:
	install -m 775 bsnes $(PREFIX)/bin/bsnes
	install -m 775 data/bsnes.png $(PREFIX)/share/icons/bsnes.png

clean:
	-@$(RM) *.$(OBJ)
	-@$(RM) *.res
	-@$(RM) *.pgd
	-@$(RM) *.pgc
	-@$(RM) *.ilk
	-@$(RM) *.pdb
	-@$(RM) *.manifest

help:
	@echo "Usage: $(MAKE) PLATFORM=platform [options]"
	@echo ""
	@echo "Available platform targets:"
	@echo "  x-gcc-x86       - Linux / BSD (x86) (requires yasm)"
	@echo "  x-gcc-x86-64    - Linux / BSD (x86-64) (requires yasm)"
	@echo "  win-mingw-x86   - Windows (x86) (requires nasm)"
	@echo "  win-visualc-x86 - Windows (x86) (requires nasm)"
	@echo ""
	@echo "Available options:"
	@echo "  GZIP_SUPPORT=[true|false] - Enable ZIP / GZ support (default=false)"
	@echo "  JMA_SUPPORT=[true|false]  - Enable JMA support (default=false)"
	@echo ""
	@echo "Example: $(MAKE) PLATFORM=x-gcc-lui GZIP_SUPPORT=true"
	@echo ""

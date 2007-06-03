######################
### bsnes makefile ###
######################

ifeq ($(PLATFORM),)
_null_: help
endif

##################################
### platform-specific settings ###
##################################

ifeq ($(PLATFORM),x-gcc-lui)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86 -DUI_LUI `pkg-config --cflags gtk+-2.0`
AS = nasm
ASFLAGS = -f elf
LIBS = `pkg-config --libs gtk+-2.0` -lXv -lao
LIBCO = libco_x86
LIBUI = libui_gtk
endif

ifeq ($(PLATFORM),x-gcc-lui-x64)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86_64 -DUI_LUI `pkg-config --cflags gtk+-2.0`
AS = yasm
ASFLAGS = -f elf64
LIBS = `pkg-config --libs gtk+-2.0` -lXv -lao
LIBCO = libco_x86_64
LIBUI = libui_gtk
endif

ifeq ($(PLATFORM),win-visualc-lui)
OS = win
CC = cl
CFLAGS = /nologo /wd4996 /O2 /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_LUI
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LIBCO = libco_x86
LIBUI = libui_win
endif

ifeq ($(PLATFORM),win-visualc-lui-pgi)
OS = win
CC = cl
CFLAGS = /nologo /wd4996 /O2 /GL /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_LUI
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LINK = /link /PGD:bsnes.pgd /LTCG:PGINSTRUMENT
LIBCO = libco_x86
LIBUI = libui_win
endif

ifeq ($(PLATFORM),win-visualc-lui-pgo)
OS = win
CC = cl
CFLAGS = /nologo /wd4996 /O2 /GL /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_LUI
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LINK = /link /PGD:bsnes.pgd /LTCG:PGOPTIMIZE
LIBCO = libco_x86
LIBUI = libui_win
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
LIBS += kernel32.lib user32.lib gdi32.lib shell32.lib winmm.lib comdlg32.lib comctl32.lib
endif

####################################
### main target and dependencies ###
####################################

OBJECTS = main.$(OBJ) $(LIBCO).$(OBJ) $(LIBUI).$(OBJ) \
  libstring.$(OBJ) \
  reader.$(OBJ) cart.$(OBJ) cheat.$(OBJ) memory.$(OBJ) bmemory.$(OBJ) \
  cpu.$(OBJ) scpu.$(OBJ) smp.$(OBJ) ssmp.$(OBJ) bdsp.$(OBJ) ppu.$(OBJ) \
  bppu.$(OBJ) snes.$(OBJ) srtc.$(OBJ) sdd1.$(OBJ) c4.$(OBJ) dsp1.$(OBJ) \
  dsp2.$(OBJ) obc1.$(OBJ)

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
endif

all: $(OBJECTS)
	$(CPP) $(OUT) $(CFLAGS) $(OBJECTS) $(LIBS) $(LINK)
#	mt -nologo -manifest bsnes.exe.manifest -outputresource:bsnes.exe;1

######################
### implicit rules ###
######################

%.$(OBJ): $<
	$(if $(filter %.asm,$<),$(AS) $(ASFLAGS) $(ASARGS))
	$(if $(filter %.c,$<),$(CC) $(CFLAGS) $(CARGS))
	$(if $(filter %.cpp,$<),$(CPP) $(CFLAGS) $(CARGS))

#########################
### platform-specific ###
#########################
main.$(OBJ): ui/main.cpp config/* ui/* ui/video/* ui/audio/* ui/input/* \
ui/lui/* ui/lui/settings/* \
ui/win/* ui/win/settings/* ui/win/debugger/*
bsnes.res  : ui/bsnes.rc ; rc /r /fobsnes.res ui/bsnes.rc

#############
### libco ###
#############
libco_x86.$(OBJ)   : lib/libco_x86.asm lib/*
libco_x86_64.$(OBJ): lib/libco_x86_64.asm lib/*

#############
### libui ###
#############
libui_gtk.$(OBJ): lib/libui_gtk.cpp lib/*
libui_win.$(OBJ): lib/libui_win.cpp lib/*

#################
### libraries ###
#################
libstring.$(OBJ): lib/libstring.cpp lib/*

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
srtc.$(OBJ): chip/srtc/srtc.cpp chip/srtc/*
sdd1.$(OBJ): chip/sdd1/sdd1.cpp chip/sdd1/*
c4.$(OBJ)  : chip/c4/c4.cpp chip/c4/*
dsp1.$(OBJ): chip/dsp1/dsp1.cpp chip/dsp1/*
dsp2.$(OBJ): chip/dsp2/dsp2.cpp chip/dsp2/*
obc1.$(OBJ): chip/obc1/obc1.cpp chip/obc1/*

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
clean:
	-@$(RM) *.$(OBJ)
	-@$(RM) *.res
	-@$(RM) *.pgd
	-@$(RM) *.pgc
	-@$(RM) *.ilk
	-@$(RM) *.pdb
	-@$(RM) *.manifest

help:
	@echo Please specify which platform to compile for with PLATFORM=platform_name

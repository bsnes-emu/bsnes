# bsnes makefile

##################################
### platform-specific settings ###
##################################

ifeq ($(PLATFORM),x-gcc-gtk)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86 -DUI_GTK `pkg-config --cflags gtk+-2.0` `sdl-config --cflags`
AS = nasm
ASFLAGS = -f elf
LIBS = `pkg-config --libs gtk+-2.0` `sdl-config --libs`
endif

ifeq ($(PLATFORM),x-gcc-sdl)
OS = unix
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -DPLATFORM_X -DCOMPILER_GCC -DPROCESSOR_X86 -DUI_SDL `sdl-config --cflags`
AS = nasm
ASFLAGS = -f elf
LIBS = `sdl-config --libs`
endif

ifeq ($(PLATFORM),win-visualc)
OS = win
CC = cl
CFLAGS = /nologo /O2 /EHsc /arch:SSE2 /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_WIN
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
endif

ifeq ($(PLATFORM),win-visualc-wip)
OS = win
CC = cl
CFLAGS = /nologo /O2 /GL /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_WIN
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LINK = /link /LTCG
endif

ifeq ($(PLATFORM),win-visualc-pgi)
OS = win
CC = cl
CFLAGS = /nologo /O2 /GL /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_WIN
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LINK = /link /PGD:bsnes.pgd /LTCG:PGINSTRUMENT
endif

ifeq ($(PLATFORM),win-visualc-pgo)
OS = win
CC = cl
CFLAGS = /nologo /O2 /GL /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_WIN
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = d3d9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
LINK = /link /PGD:bsnes.pgd /LTCG:PGOPTIMIZE
endif

ifeq ($(PLATFORM),win-visualc-sdl)
OS = win
CC = cl
CFLAGS = /nologo /O2 /EHsc /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86 /DUI_SDL
AS = nasm
ASFLAGS = -f win32 -DWIN32
LIBS = sdlmain.lib sdl.lib dsound.lib
endif

#####################################
### compiler / assembler switches ###
#####################################

ifeq ($(CC),gcc)
OUT = -obsnes
CXX = g++
OBJ = o
COBJFLAG = -c
endif

ifeq ($(CC),cl)
OUT = /Febsnes.exe
CXX = cl
OBJ = obj
COBJFLAG = /c
endif

ifeq ($(AS),nasm)
ASOBJFLAG = -o
endif

###################
### OS switches ###
###################

ifeq ($(OS),unix)
RM = rm -f
endif

ifeq ($(OS),win)
RM = del
LIBS += kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib
endif

#########################
### begin compilation ###
#########################

OBJS = main.$(OBJ) \
       libco.$(OBJ) libstring.$(OBJ) libconfig.$(OBJ) \
       reader.$(OBJ) cart.$(OBJ) cheat.$(OBJ) \
       memory.$(OBJ) bmemory.$(OBJ) \
       cpu.$(OBJ) scpu.$(OBJ) \
       smp.$(OBJ) ssmp.$(OBJ) \
       bdsp.$(OBJ) \
       ppu.$(OBJ) bppu.$(OBJ) \
       snes.$(OBJ) \
       srtc.$(OBJ) sdd1.$(OBJ) c4.$(OBJ) dsp1.$(OBJ) dsp2.$(OBJ) obc1.$(OBJ)
#      adler32.$(OBJ) compress.$(OBJ) crc32.$(OBJ) deflate.$(OBJ) gzio.$(OBJ) inffast.$(OBJ) \
#      inflate.$(OBJ) inftrees.$(OBJ) ioapi.$(OBJ) trees.$(OBJ) unzip.$(OBJ) zip.$(OBJ) zutil.$(OBJ) \
#      jma.$(OBJ) jcrc32.$(OBJ) lzmadec.$(OBJ) 7zlzma.$(OBJ) iiostrm.$(OBJ) inbyte.$(OBJ) lzma.$(OBJ) winout.$(OBJ)

ifeq ($(OS),win)
  ifeq ($(CC),cl)
    OBJS += bsnes.res
  endif
endif

all: $(OBJS)
	$(CXX) $(OUT) $(CFLAGS) $(OBJS) $(LIBS) $(LINK)
#	mt -nologo -manifest bsnes.exe.manifest -outputresource:bsnes.exe;1

clean:
	$(RM) *.$(OBJ)
	$(RM) *.res
	$(RM) *.pgd
	$(RM) *.pgc
	$(RM) *.ilk
	$(RM) *.pdb
	$(RM) *.manifest

#########################
### platform-specific ###
#########################
main.$(OBJ): *.cpp *.h video/* audio/* input/* ../config/* win/* win/settings/* win/debugger/* sdl/*
	$(CXX) $(CFLAGS) $(COBJFLAG) main.cpp

bsnes.res: bsnes.rc
	rc /r /fobsnes.res bsnes.rc

#################
### libraries ###
#################
libco.$(OBJ): ../lib/*
	$(AS) $(ASFLAGS) $(ASOBJFLAG) libco.$(OBJ) ../lib/libco_x86.asm
#	$(CXX) $(CFLAGS) $(COBJFLAG) /Folibco.$(OBJ) ../lib/libco_win32.cpp

libstring.$(OBJ): ../lib/*.cpp ../lib/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../lib/libstring.cpp

libconfig.$(OBJ): ../lib/*.cpp ../lib/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../lib/libconfig.cpp

##############
### memory ###
##############
memory.$(OBJ): ../memory/memory.cpp ../memory/memory.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../memory/memory.cpp

bmemory.$(OBJ): ../memory/bmemory/* ../memory/bmemory/mapper/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.$(OBJ): ../cpu/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../cpu/cpu.cpp

scpu.$(OBJ): ../cpu/scpu/* ../cpu/scpu/core/* ../cpu/scpu/dma/* ../cpu/scpu/memory/* ../cpu/scpu/mmio/* ../cpu/scpu/timing/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../cpu/scpu/scpu.cpp

###########
### smp ###
###########
smp.$(OBJ): ../smp/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../smp/smp.cpp

ssmp.$(OBJ): ../smp/ssmp/* ../smp/ssmp/core/* ../smp/ssmp/memory/* ../smp/ssmp/timing/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../smp/ssmp/ssmp.cpp

###########
### dsp ###
###########
bdsp.$(OBJ): ../dsp/bdsp/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../dsp/bdsp/bdsp.cpp

###########
### ppu ###
###########
ppu.$(OBJ): ../ppu/*.cpp ../ppu/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../ppu/ppu.cpp

bppu.$(OBJ): ../ppu/bppu/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../ppu/bppu/bppu.cpp

#################
### utilities ###
#################
reader.$(OBJ): ../reader/*.cpp ../reader/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../reader/reader.cpp

cart.$(OBJ): ../cart/*.cpp ../cart/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../cart/cart.cpp

cheat.$(OBJ): ../cheat/*.cpp ../cheat/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../cheat/cheat.cpp

############
### snes ###
############
snes.$(OBJ): ../snes/* ../snes/video/* ../snes/audio/* ../snes/input/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../snes/snes.cpp

#####################
### special chips ###
#####################
srtc.$(OBJ): ../chip/srtc/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/srtc/srtc.cpp

sdd1.$(OBJ): ../chip/sdd1/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/sdd1/sdd1.cpp

c4.$(OBJ): ../chip/c4/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/c4/c4.cpp

dsp1.$(OBJ): ../chip/dsp1/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/dsp1/dsp1.cpp

dsp2.$(OBJ): ../chip/dsp2/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/dsp2/dsp2.cpp

obc1.$(OBJ): ../chip/obc1/*
	$(CXX) $(CFLAGS) $(COBJFLAG) ../chip/obc1/obc1.cpp

############
### zlib ###
############
adler32.$(OBJ): ../reader/zlib/*.c ../reader/zlib/*.h
	$(CC) $(CFLAGS) $(COBJFLAG) ../reader/zlib/*.c

###########
### jma ###
###########
jma.$(OBJ): ../reader/jma/*.cpp ../reader/jma/*.h
	$(CXX) $(CFLAGS) $(COBJFLAG) ../reader/jma/*.cpp

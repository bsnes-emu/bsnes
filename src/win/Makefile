CC = cl
AS = nasm
CFLAGS = /nologo /O2 /arch:SSE2
#CFLAGS = /nologo /O2 /GL /arch:SSE2
#CFLAGS = /nologo /O2 /GL /EHsc
COMPILE = $(CC) $(CFLAGS) /c
ASSEMBLE = $(AS) -f win32

OBJS = main.obj \
       libco.obj libstring.obj libconfig.obj libbpf.obj \
       reader.obj cart.obj cheat.obj \
       memory.obj bmemory.obj \
       cpu.obj scpu.obj bcpu.obj \
       apu.obj sapu.obj bapu.obj \
       bdsp.obj \
       ppu.obj bppu.obj \
       snes.obj \
       srtc.obj sdd1.obj c4.obj dsp1.obj dsp2.obj obc1.obj
#      adler32.obj compress.obj crc32.obj deflate.obj gzio.obj inffast.obj \
#      inflate.obj inftrees.obj ioapi.obj trees.obj unzip.obj zip.obj zutil.obj \
#      jma.obj jcrc32.obj lzmadec.obj 7zlzma.obj iiostrm.obj inbyte.obj lzma.obj winout.obj
LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib \
       d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib

all: $(OBJS)
	rc /r /fobsnes.res bsnes.rc
	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS)
#	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS) /link /PGD:bsnes.pgd /LTCG:PGINSTRUMENT
#	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS) /link /PGD:bsnes.pgd /LTCG:PGOPTIMIZE

clean:
	del *.obj
	del *.res
	del *.pgd
	del *.pgc
	del *.ilk
	del *.pdb

######################
### win32-specific ###
######################
main.obj: *.cpp *.h video/* audio/* input/* settings/* debugger/* ../config/*
	$(COMPILE) main.cpp

#################
### libraries ###
#################
libco.obj: ../lib/*
	$(ASSEMBLE) -o libco.obj ../lib/libco_x86.asm

libstring.obj: ../lib/*.cpp ../lib/*.h
	$(COMPILE) ../lib/libstring.cpp

libconfig.obj: ../lib/*.cpp ../lib/*.h
	$(COMPILE) ../lib/libconfig.cpp

libbpf.obj: ../lib/*.cpp ../lib/*.h
	$(COMPILE) ../lib/libbpf.cpp

##############
### memory ###
##############
memory.obj: ../memory/memory.cpp ../memory/memory.h
	$(COMPILE) ../memory/memory.cpp

bmemory.obj: ../memory/bmemory/*
	$(COMPILE) ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.obj: ../cpu/*
	$(COMPILE) ../cpu/cpu.cpp

scpu.obj: ../cpu/scpu/* ../cpu/scpu/core/* ../cpu/scpu/dma/* ../cpu/scpu/memory/* ../cpu/scpu/mmio/* ../cpu/scpu/timing/*
	$(COMPILE) ../cpu/scpu/scpu.cpp

bcpu.obj: ../cpu/bcpu/*
	$(COMPILE) ../cpu/bcpu/bcpu.cpp

###########
### apu ###
###########
apu.obj: ../apu/*
	$(COMPILE) ../apu/apu.cpp

sapu.obj: ../apu/sapu/* ../apu/sapu/core/* ../apu/sapu/memory/* ../apu/sapu/timing/*
	$(COMPILE) ../apu/sapu/sapu.cpp

bapu.obj: ../apu/bapu/*
	$(COMPILE) ../apu/bapu/bapu.cpp

###########
### dsp ###
###########
bdsp.obj: ../dsp/bdsp/*
	$(COMPILE) ../dsp/bdsp/bdsp.cpp

###########
### ppu ###
###########
ppu.obj: ../ppu/*.cpp ../ppu/*.h
	$(COMPILE) ../ppu/ppu.cpp

bppu.obj: ../ppu/bppu/*
	$(COMPILE) ../ppu/bppu/bppu.cpp

#################
### utilities ###
#################
reader.obj: ../reader/*.cpp ../reader/*.h
	$(COMPILE) ../reader/reader.cpp

cart.obj: ../cart/*.cpp ../cart/*.h
	$(COMPILE) ../cart/cart.cpp

cheat.obj: ../cheat/*.cpp ../cheat/*.h
	$(COMPILE) ../cheat/cheat.cpp

############
### snes ###
############
snes.obj: ../snes/* ../snes/video/* ../snes/audio/* ../snes/input/*
	$(COMPILE) ../snes/snes.cpp

#####################
### special chips ###
#####################
srtc.obj: ../chip/srtc/*
	$(COMPILE) ../chip/srtc/srtc.cpp

sdd1.obj: ../chip/sdd1/*
	$(COMPILE) ../chip/sdd1/sdd1.cpp

c4.obj: ../chip/c4/*
	$(COMPILE) ../chip/c4/c4.cpp

dsp1.obj: ../chip/dsp1/*
	$(COMPILE) ../chip/dsp1/dsp1.cpp

dsp2.obj: ../chip/dsp2/*
	$(COMPILE) ../chip/dsp2/dsp2.cpp

obc1.obj: ../chip/obc1/*
	$(COMPILE) ../chip/obc1/obc1.cpp

############
### zlib ###
############
adler32.obj: ../reader/zlib/*.c ../reader/zlib/*.h
	$(COMPILE) ../reader/zlib/*.c

###########
### jma ###
###########
jma.obj: ../reader/jma/*.cpp ../reader/jma/*.h
	$(COMPILE) ../reader/jma/*.cpp

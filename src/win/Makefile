CC = cl
CFLAGS = /nologo /O2 /arch:SSE2 /wd4996
OBJS = main.obj \
       libstring.obj libconfig.obj libbpf.obj \
       reader.obj cart.obj cheat.obj \
       memory.obj bmemory.obj \
       cpu.obj bcpu.obj \
       apu.obj bapu.obj \
       bdsp.obj \
       ppu.obj bppu.obj \
       snes.obj \
       srtc.obj sdd1.obj c4.obj dsp2.obj obc1.obj
#      adler32.obj compress.obj crc32.obj deflate.obj gzio.obj inffast.obj \
#      inflate.obj inftrees.obj ioapi.obj trees.obj unzip.obj zip.obj zutil.obj \
#      jma.obj jcrc32.obj lzmadec.obj 7zlzma.obj iiostrm.obj inbyte.obj lzma.obj winout.obj
LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib \
       d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib

all: $(OBJS)
	rc /r /fobsnes.res bsnes.rc
	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS)
#/link /PGD:bsnes.pgd /LTCG:PGINSTRUMENT

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
	$(CC) $(CFLAGS) /c main.cpp

#################
### libraries ###
#################
libstring.obj: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) /c ../lib/libstring.cpp
libconfig.obj: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) /c ../lib/libconfig.cpp
libbpf.obj: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) /c ../lib/libbpf.cpp

##############
### memory ###
##############
memory.obj: ../memory/memory.cpp ../memory/memory.h
	$(CC) $(CFLAGS) /c ../memory/memory.cpp

bmemory.obj: ../memory/bmemory/*
	$(CC) $(CFLAGS) /c ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.obj: ../cpu/*.cpp ../cpu/*.h
	$(CC) $(CFLAGS) /c ../cpu/cpu.cpp

bcpu.obj: ../cpu/bcpu/*
	$(CC) $(CFLAGS) /c ../cpu/bcpu/bcpu.cpp

###########
### apu ###
###########
apu.obj: ../apu/*
	$(CC) $(CFLAGS) /c ../apu/apu.cpp

bapu.obj: ../apu/bapu/*
	$(CC) $(CFLAGS) /c ../apu/bapu/bapu.cpp

###########
### dsp ###
###########
bdsp.obj: ../dsp/bdsp/*
	$(CC) $(CFLAGS) /c ../dsp/bdsp/bdsp.cpp

###########
### ppu ###
###########
ppu.obj: ../ppu/*.cpp ../ppu/*.h
	$(CC) $(CFLAGS) /c ../ppu/ppu.cpp

bppu.obj: ../ppu/bppu/*
	$(CC) $(CFLAGS) /c ../ppu/bppu/bppu.cpp

#################
### utilities ###
#################
reader.obj: ../reader/*.cpp ../reader/*.h
	$(CC) $(CFLAGS) /c ../reader/reader.cpp

cart.obj: ../cart/*.cpp ../cart/*.h
	$(CC) $(CFLAGS) /c ../cart/cart.cpp

cheat.obj: ../cheat/*.cpp ../cheat/*.h
	$(CC) $(CFLAGS) /c ../cheat/cheat.cpp

############
### snes ###
############
snes.obj: ../snes/* ../snes/video/* ../snes/audio/* ../snes/input/*
	$(CC) $(CFLAGS) /c ../snes/snes.cpp

#####################
### special chips ###
#####################
srtc.obj: ../chip/srtc/*.cpp ../chip/srtc/*.h
	$(CC) $(CFLAGS) /c ../chip/srtc/srtc.cpp

sdd1.obj: ../chip/sdd1/*.cpp ../chip/sdd1/*.h
	$(CC) $(CFLAGS) /c ../chip/sdd1/sdd1.cpp

c4.obj: ../chip/c4/*.cpp ../chip/c4/*.h
	$(CC) $(CFLAGS) /c ../chip/c4/c4.cpp

dsp2.obj: ../chip/dsp2/*.cpp ../chip/dsp2/*.h
	$(CC) $(CFLAGS) /c ../chip/dsp2/dsp2.cpp

obc1.obj: ../chip/obc1/*.cpp ../chip/obc1/*.h
	$(CC) $(CFLAGS) /c ../chip/obc1/obc1.cpp

############
### zlib ###
############
adler32.obj: ../reader/zlib/*.c ../reader/zlib/*.h
	$(CC) $(CFLAGS) /c ../reader/zlib/*.c

###########
### jma ###
###########
jma.obj: ../reader/jma/*.cpp ../reader/jma/*.h
	$(CC) $(CFLAGS) /c ../reader/jma/*.cpp

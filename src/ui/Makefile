ifeq ($(PLATFORM),win-visualc)
CC = cl
CXX = cl
AS = nasm
RM = del
OBJ = obj
CFLAGS = /nologo /O2 /arch:SSE2 /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86
COMPILE = $(CC) $(CFLAGS) /c
ASSEMBLE = $(AS) -f win32
LINK =
LIBS =  kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib \
	d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
endif

ifeq ($(PLATFORM),win-visualc-wip)
CC = cl
CXX = cl
AS = nasm
RM = del
OBJ = obj
CFLAGS = /nologo /O2 /GL /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86
COMPILE = $(CC) $(CFLAGS) /c
ASSEMBLE = $(AS) -f win32
LINK = /link /LTCG
LIBS =  kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib \
	d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
endif

ifeq ($(PLATFORM),win-visualc-pgi)
CC = cl
CXX = cl
AS = nasm
RM = del
OBJ = obj
CFLAGS = /nologo /O2 /GL /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86
COMPILE = $(CC) $(CFLAGS) /c
ASSEMBLE = $(AS) -f win32
LINK = /link /PGD:bsnes.pgd /LTCG:PGINSTRUMENT
LIBS =  kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib \
	d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
endif

ifeq ($(PLATFORM),win-visualc-pgo)
CC = cl
CXX = cl
AS = nasm
RM = del
OBJ = obj
CFLAGS = /nologo /O2 /arch:SSE2 /DPLATFORM_WIN /DCOMPILER_VISUALC /DPROCESSOR_X86
COMPILE = $(CC) $(CFLAGS) /c
ASSEMBLE = $(AS) -f win32
LINK = /link /PGD:bsnes.pgd /LTCG:PGOPTIMIZE
LIBS =  kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib \
	d3d9.lib d3dx9.lib ddraw.lib dsound.lib dinput8.lib dxguid.lib
endif

OBJS =  main.$(OBJ) \
	libco.$(OBJ) libstring.$(OBJ) libconfig.$(OBJ) \
	reader.$(OBJ) cart.$(OBJ) cheat.$(OBJ) \
	memory.$(OBJ) bmemory.$(OBJ) \
	cpu.$(OBJ) scpu.$(OBJ) bcpu.$(OBJ) \
	apu.$(OBJ) sapu.$(OBJ) bapu.$(OBJ) \
	bdsp.$(OBJ) \
	ppu.$(OBJ) bppu.$(OBJ) \
	snes.$(OBJ) \
	srtc.$(OBJ) sdd1.$(OBJ) c4.$(OBJ) dsp1.$(OBJ) dsp2.$(OBJ) obc1.$(OBJ)
#	adler32.$(OBJ) compress.$(OBJ) crc32.$(OBJ) deflate.$(OBJ) gzio.$(OBJ) inffast.$(OBJ) \
#	inflate.$(OBJ) inftrees.$(OBJ) ioapi.$(OBJ) trees.$(OBJ) unzip.$(OBJ) zip.$(OBJ) zutil.$(OBJ) \
#	jma.$(OBJ) jcrc32.$(OBJ) lzmadec.$(OBJ) 7zlzma.$(OBJ) iiostrm.$(OBJ) inbyte.$(OBJ) lzma.$(OBJ) winout.$(OBJ)

all: $(OBJS)
	rc /r /fobsnes.res bsnes.rc
	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS) $(LINK)

clean:
	$(RM) *.$(OBJ)
	$(RM) *.res
	$(RM) *.pgd
	$(RM) *.pgc
	$(RM) *.ilk
	$(RM) *.pdb

#########################
### platform-specific ###
#########################
main.$(OBJ): *.cpp *.h video/* audio/* input/* ../config/* win/* win/settings/* win/debugger/*
	$(COMPILE) main.cpp

#################
### libraries ###
#################
libco.$(OBJ): ../lib/*
	$(ASSEMBLE) -o libco.$(OBJ) ../lib/libco_x86.asm

libstring.$(OBJ): ../lib/*.cpp ../lib/*.h
	$(COMPILE) ../lib/libstring.cpp

libconfig.$(OBJ): ../lib/*.cpp ../lib/*.h
	$(COMPILE) ../lib/libconfig.cpp

##############
### memory ###
##############
memory.$(OBJ): ../memory/memory.cpp ../memory/memory.h
	$(COMPILE) ../memory/memory.cpp

bmemory.$(OBJ): ../memory/bmemory/* ../memory/bmemory/mapper/*
	$(COMPILE) ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.$(OBJ): ../cpu/*
	$(COMPILE) ../cpu/cpu.cpp

scpu.$(OBJ): ../cpu/scpu/* ../cpu/scpu/core/* ../cpu/scpu/dma/* ../cpu/scpu/memory/* ../cpu/scpu/mmio/* ../cpu/scpu/timing/*
	$(COMPILE) ../cpu/scpu/scpu.cpp

bcpu.$(OBJ): ../cpu/bcpu/*
	$(COMPILE) ../cpu/bcpu/bcpu.cpp

###########
### apu ###
###########
apu.$(OBJ): ../apu/*
	$(COMPILE) ../apu/apu.cpp

sapu.$(OBJ): ../apu/sapu/* ../apu/sapu/core/* ../apu/sapu/memory/* ../apu/sapu/timing/*
	$(COMPILE) ../apu/sapu/sapu.cpp

bapu.$(OBJ): ../apu/bapu/*
	$(COMPILE) ../apu/bapu/bapu.cpp

###########
### dsp ###
###########
bdsp.$(OBJ): ../dsp/bdsp/*
	$(COMPILE) ../dsp/bdsp/bdsp.cpp

###########
### ppu ###
###########
ppu.$(OBJ): ../ppu/*.cpp ../ppu/*.h
	$(COMPILE) ../ppu/ppu.cpp

bppu.$(OBJ): ../ppu/bppu/*
	$(COMPILE) ../ppu/bppu/bppu.cpp

#################
### utilities ###
#################
reader.$(OBJ): ../reader/*.cpp ../reader/*.h
	$(COMPILE) ../reader/reader.cpp

cart.$(OBJ): ../cart/*.cpp ../cart/*.h
	$(COMPILE) ../cart/cart.cpp

cheat.$(OBJ): ../cheat/*.cpp ../cheat/*.h
	$(COMPILE) ../cheat/cheat.cpp

############
### snes ###
############
snes.$(OBJ): ../snes/* ../snes/video/* ../snes/audio/* ../snes/input/*
	$(COMPILE) ../snes/snes.cpp

#####################
### special chips ###
#####################
srtc.$(OBJ): ../chip/srtc/*
	$(COMPILE) ../chip/srtc/srtc.cpp

sdd1.$(OBJ): ../chip/sdd1/*
	$(COMPILE) ../chip/sdd1/sdd1.cpp

c4.$(OBJ): ../chip/c4/*
	$(COMPILE) ../chip/c4/c4.cpp

dsp1.$(OBJ): ../chip/dsp1/*
	$(COMPILE) ../chip/dsp1/dsp1.cpp

dsp2.$(OBJ): ../chip/dsp2/*
	$(COMPILE) ../chip/dsp2/dsp2.cpp

obc1.$(OBJ): ../chip/obc1/*
	$(COMPILE) ../chip/obc1/obc1.cpp

############
### zlib ###
############
adler32.$(OBJ): ../reader/zlib/*.c ../reader/zlib/*.h
	$(COMPILE) ../reader/zlib/*.c

###########
### jma ###
###########
jma.$(OBJ): ../reader/jma/*.cpp ../reader/jma/*.h
	$(COMPILE) ../reader/jma/*.cpp

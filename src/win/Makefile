CC = cl
CFLAGS = /nologo /O2 /Ogityb2 /Gr /Gs
OBJS = winmain.obj \
       libstring.obj libconfig.obj \
       reader.obj \
       memory.obj bmemory.obj \
       cpu.obj bcpu.obj \
       apu.obj bapu.obj bapuskip.obj \
       ppu.obj bppu.obj \
       snes.obj
LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib ddraw.lib dxguid.lib

all: $(OBJS)
	rc /r /fobsnes.res bsnes.rc
	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) bsnes.res $(LIBS)

clean:
	del *.obj

######################
### win32-specific ###
######################
winmain.obj: *.cpp *.h
	$(CC) $(CFLAGS) /c winmain.cpp

#################
### libraries ###
#################
libstring.obj: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) /c ../lib/libstring.cpp
libconfig.obj: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) /c ../lib/libconfig.cpp

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

bapuskip.obj: ../apu/bapuskip/*
	$(CC) $(CFLAGS) /c ../apu/bapuskip/bapuskip.cpp

###########
### ppu ###
###########
ppu.obj: ../ppu/*.cpp ../ppu/*.h
	$(CC) $(CFLAGS) /c ../ppu/ppu.cpp

bppu.obj: ../ppu/bppu/*
	$(CC) $(CFLAGS) /c ../ppu/bppu/bppu.cpp

##############
### reader ###
##############
reader.obj: ../reader/reader.cpp ../reader/reader.h
	$(CC) $(CFLAGS) /c ../reader/reader.cpp

############
### snes ###
############
snes.obj: ../snes/*.cpp ../snes/*.h
	$(CC) $(CFLAGS) /c ../snes/snes.cpp

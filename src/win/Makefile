CC = cl
CFLAGS = /nologo /O2 /Ob2
OBJS = winmain.obj \
       clock.obj bclock.obj \
       memory.obj bmemory.obj \
       reader.obj \
       cpu.obj bcpu.obj \
       ppu.obj bppu.obj \
       snes.obj
LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib ddraw.lib

all: $(OBJS)
	$(CC) /Febsnes_g2.exe $(CFLAGS) $(OBJS) $(LIBS)

clean:
	del *.obj

######################
### win32-specific ###
######################
winmain.obj: *.cpp *.h
	$(CC) $(CFLAGS) /c winmain.cpp

#############
### clock ###
#############
clock.obj: ../clock/*.cpp ../clock/*.h
	$(CC) $(CFLAGS) /c ../clock/clock.cpp

bclock.obj: ../clock/bclock/*
	$(CC) $(CFLAGS) /c ../clock/bclock/bclock.cpp

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

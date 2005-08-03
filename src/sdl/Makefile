CC = c++
CFLAGS = -O2
OBJS = sdlmain.o \
       libstring.o libconfig.o \
       reader.o \
       memory.o bmemory.o \
       cpu.o bcpu.o \
       apu.o bapu.o bapuskip.o \
       ppu.o bppu.o \
       snes.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) `sdl11-config --cflags --libs` -o bsnes_sdl

clean:
	rm *.o

####################
### sdl-specific ###
####################
sdlmain.o: *.cpp *.h
	$(CC) $(CFLAGS) -c sdlmain.cpp `sdl11-config --cflags`

#################
### libraries ###
#################
libstring.o: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) -c ../lib/libstring.cpp
libconfig.o: ../lib/*.cpp ../lib/*.h
	$(CC) $(CFLAGS) -c ../lib/libconfig.cpp

##############
### memory ###
##############
memory.o: ../memory/memory.cpp ../memory/memory.h
	$(CC) $(CFLAGS) -c ../memory/memory.cpp

bmemory.o: ../memory/bmemory/*
	$(CC) $(CFLAGS) -c ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.o: ../cpu/*.cpp ../cpu/*.h
	$(CC) $(CFLAGS) -c ../cpu/cpu.cpp

bcpu.o: ../cpu/bcpu/*
	$(CC) $(CFLAGS) -c ../cpu/bcpu/bcpu.cpp

###########
### apu ###
###########
apu.o: ../apu/*
	$(CC) $(CFLAGS) -c ../apu/apu.cpp

bapu.o: ../apu/bapu/*
	$(CC) $(CFLAGS) -c ../apu/bapu/bapu.cpp

bapuskip.o: ../apu/bapuskip/*
	$(CC) $(CFLAGS) -c ../apu/bapuskip/bapuskip.cpp

###########
### ppu ###
###########
ppu.o: ../ppu/*.cpp ../ppu/*.h
	$(CC) $(CFLAGS) -c ../ppu/ppu.cpp

bppu.o: ../ppu/bppu/*
	$(CC) $(CFLAGS) -c ../ppu/bppu/bppu.cpp

##############
### reader ###
##############
reader.o: ../reader/reader.cpp ../reader/reader.h
	$(CC) $(CFLAGS) -c ../reader/reader.cpp

############
### snes ###
############
snes.o: ../snes/*.cpp ../snes/*.h
	$(CC) $(CFLAGS) -c ../snes/snes.cpp

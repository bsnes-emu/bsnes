CC = cc
CXX = c++
CFLAGS = -O3 -fomit-frame-pointer -ffast-math
CXXFLAGS = $(CFLAGS) -fno-rtti
OBJS = sdlmain.o \
       libstring.o libconfig.o libbpf.o \
       reader.o cart.o \
       memory.o bmemory.o \
       cpu.o bcpu.o \
       apu.o bapu.o \
       bdsp.o \
       ppu.o bppu.o \
       snes.o \
       srtc.o sdd1.o c4.o \
       adler32.o compress.o crc32.o deflate.o gzio.o inffast.o \
       inflate.o inftrees.o ioapi.o trees.o unzip.o zip.o zutil.o \
       jma.o jcrc32.o lzmadec.o 7zlzma.o iiostrm.o inbyte.o lzma.o winout.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) `sdl11-config --cflags --libs` -o bsnes_sdl

clean:
	rm *.o

####################
### sdl-specific ###
####################
sdlmain.o: *.cpp *.h
	$(CXX) $(CXXFLAGS) -c sdlmain.cpp `sdl11-config --cflags`

#################
### libraries ###
#################
libstring.o: ../lib/*.cpp ../lib/*.h
	$(CXX) $(CXXFLAGS) -c ../lib/libstring.cpp
libconfig.o: ../lib/*.cpp ../lib/*.h
	$(CXX) $(CXXFLAGS) -c ../lib/libconfig.cpp
libbpf.o: ../lib/*.cpp ../lib/*.h
	$(CXX) $(CXXFLAGS) -c ../lib/libbpf.cpp

##############
### memory ###
##############
memory.o: ../memory/memory.cpp ../memory/memory.h
	$(CXX) $(CXXFLAGS) -c ../memory/memory.cpp

bmemory.o: ../memory/bmemory/*
	$(CXX) $(CXXFLAGS) -c ../memory/bmemory/bmemory.cpp

###########
### cpu ###
###########
cpu.o: ../cpu/*.cpp ../cpu/*.h
	$(CXX) $(CXXFLAGS) -c ../cpu/cpu.cpp

bcpu.o: ../cpu/bcpu/*
	$(CXX) $(CXXFLAGS) -c ../cpu/bcpu/bcpu.cpp

###########
### apu ###
###########
apu.o: ../apu/*
	$(CXX) $(CXXFLAGS) -c ../apu/apu.cpp

bapu.o: ../apu/bapu/*
	$(CXX) $(CXXFLAGS) -c ../apu/bapu/bapu.cpp

###########
### dsp ###
###########
bdsp.o: ../dsp/bdsp/*
	$(CXX) $(CXXFLAGS) -c ../dsp/bdsp/bdsp.cpp

###########
### ppu ###
###########
ppu.o: ../ppu/*.cpp ../ppu/*.h
	$(CXX) $(CXXFLAGS) -c ../ppu/ppu.cpp

bppu.o: ../ppu/bppu/*
	$(CXX) $(CXXFLAGS) -c ../ppu/bppu/bppu.cpp

#################
### utilities ###
#################
reader.o: ../reader/*.cpp ../reader/*.h
	$(CXX) $(CXXFLAGS) -c ../reader/reader.cpp

cart.o: ../cart/*.cpp ../cart/*.h
	$(CXX) $(CXXFLAGS) -c ../cart/cart.cpp

############
### snes ###
############
snes.o: ../snes/*.cpp ../snes/*.h
	$(CXX) $(CXXFLAGS) -c ../snes/snes.cpp

#####################
### special chips ###
#####################
srtc.o: ../chip/srtc/*.cpp ../chip/srtc/*.h
	$(CXX) $(CXXFLAGS) -c ../chip/srtc/srtc.cpp

sdd1.o: ../chip/sdd1/*.cpp ../chip/sdd1/*.h
	$(CXX) $(CXXFLAGS) -c ../chip/sdd1/sdd1.cpp

c4.o: ../chip/c4/*.cpp ../chip/c4/*.h
	$(CXX) $(CXXFLAGS) -c ../chip/c4/c4.cpp

############
### zlib ###
############
adler32.o: ../reader/zlib/*.c ../reader/zlib/*.h
	$(CC) $(CFLAGS) -c ../reader/zlib/*.c

###########
### jma ###
###########
jma.o: ../reader/jma/*.cpp ../reader/jma/*.h
	$(CXX) $(CXXFLAGS) -c ../reader/jma/*.cpp

CC = cl
CFLAGS = /nologo /O2
OBJS = main.obj timing.obj g65816.obj d65816.obj spc700.obj dspc700.obj memory.obj mmio.obj bridge.obj gui.obj libstr.obj
LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib ddraw.lib

all: $(OBJS)
	$(CC) /Febsnes.exe $(CFLAGS) $(OBJS) $(LIBS)

clean:
	del *.obj

main.obj: main.cpp main.h
	$(CC) $(CFLAGS) /c main.cpp

timing.obj: timing/timing.cpp timing/timing.h
	$(CC) $(CFLAGS) /c timing/timing.cpp

g65816.obj: cpu/g65816*.cpp cpu/g65816.h
	$(CC) $(CFLAGS) /c cpu/g65816.cpp

d65816.obj: cpu/d65816.cpp
	$(CC) $(CFLAGS) /c cpu/d65816.cpp

spc700.obj: apu/spc700*.cpp apu/spc700*.h
	$(CC) $(CFLAGS) /c apu/spc700.cpp

dspc700.obj: apu/dspc700.cpp
	$(CC) $(CFLAGS) /c apu/dspc700.cpp

memory.obj: mem/memory.cpp
	$(CC) $(CFLAGS) /c mem/memory.cpp

mmio.obj: ppu/mmio.cpp ppu/ppu*.cpp
	$(CC) $(CFLAGS) /c ppu/mmio.cpp

bridge.obj: bridge/bridge.cpp bridge/bridge.h
	$(CC) $(CFLAGS) /c bridge/bridge.cpp

gui.obj: win/gui*.cpp win/render*.cpp
	$(CC) $(CFLAGS) /c win/gui.cpp

libstr.obj: misc/libstr.cpp
	$(CC) $(CFLAGS) /c misc/libstr.cpp

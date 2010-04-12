snes_core = sMemory sCPU sSMP aDSP bPPU

snes_objects := libco
snes_objects += snes-system
snes_objects += snes-cartridge snes-cheat
snes_objects += snes-memory snes-cpu snes-cpucore snes-smp snes-smpcore snes-dsp snes-ppu
snes_objects += $(foreach n,$(call strlower,$(snes_core)),snes-$n)
snes_objects += snes-supergameboy snes-superfx snes-sa1
snes_objects += snes-bsx snes-srtc snes-sdd1 snes-spc7110
snes_objects += snes-cx4 snes-dsp1 snes-dsp2 snes-dsp3 snes-dsp4
snes_objects += snes-obc1 snes-st0010 snes-st0011 snes-st0018 snes-msu1
objects += $(snes_objects)

# libco
obj/libco.o: libco/libco.c libco/*

# libsnes
obj/libsnes.o: snes/libsnes/libsnes.cpp snes/libsnes/*

# system
obj/snes-system.o: snes/system/system.cpp $(call rwildcard,snes/system/)

# memory
obj/snes-memory.o : snes/memory/memory.cpp snes/memory/*
obj/snes-smemory.o: snes/memory/smemory/smemory.cpp $(call rwildcard,snes/memory/smemory/)

# cpu
obj/snes-cpu.o    : snes/cpu/cpu.cpp snes/cpu/*
obj/snes-cpucore.o: snes/cpu/core/core.cpp $(call rwildcard,snes/cpu/core/)
obj/snes-scpu.o   : snes/cpu/scpu/scpu.cpp $(call rwildcard,snes/cpu/scpu/)

# smp
obj/snes-smp.o    : snes/smp/smp.cpp snes/smp/*
obj/snes-smpcore.o: snes/smp/core/core.cpp $(call rwildcard,snes/smp/core/)
obj/snes-ssmp.o   : snes/smp/ssmp/ssmp.cpp $(call rwildcard,snes/smp/ssmp/)

# dsp
obj/snes-dsp.o : snes/dsp/dsp.cpp snes/dsp/*
obj/snes-sdsp.o: snes/dsp/sdsp/sdsp.cpp snes/dsp/sdsp/*
obj/snes-adsp.o: snes/dsp/adsp/adsp.cpp snes/dsp/adsp/*

# ppu
obj/snes-ppu.o : snes/ppu/ppu.cpp snes/ppu/*
obj/snes-sppu.o: snes/ppu/sppu/sppu.cpp $(call rwildcard,snes/ppu/sppu/)
obj/snes-bppu.o: snes/ppu/bppu/bppu.cpp $(call rwildcard,snes/ppu/bppu/)

# utilities
obj/snes-cartridge.o: snes/cartridge/cartridge.cpp snes/cartridge/*
obj/snes-cheat.o    : snes/cheat/cheat.cpp snes/cheat/*

# special chips
obj/snes-supergameboy.o: snes/chip/supergameboy/supergameboy.cpp $(call rwildcard,snes/chip/supergameboy/)
obj/snes-superfx.o     : snes/chip/superfx/superfx.cpp $(call rwildcard,snes/chip/superfx/)
obj/snes-sa1.o         : snes/chip/sa1/sa1.cpp $(call rwildcard,snes/chip/sa1/)
obj/snes-bsx.o         : snes/chip/bsx/bsx.cpp snes/chip/bsx/*
obj/snes-srtc.o        : snes/chip/srtc/srtc.cpp snes/chip/srtc/*
obj/snes-sdd1.o        : snes/chip/sdd1/sdd1.cpp snes/chip/sdd1/*
obj/snes-spc7110.o     : snes/chip/spc7110/spc7110.cpp snes/chip/spc7110/*
obj/snes-cx4.o         : snes/chip/cx4/cx4.cpp snes/chip/cx4/*
obj/snes-dsp1.o        : snes/chip/dsp1/dsp1.cpp snes/chip/dsp1/*
obj/snes-dsp2.o        : snes/chip/dsp2/dsp2.cpp snes/chip/dsp2/*
obj/snes-dsp3.o        : snes/chip/dsp3/dsp3.cpp snes/chip/dsp3/*
obj/snes-dsp4.o        : snes/chip/dsp4/dsp4.cpp snes/chip/dsp4/*
obj/snes-obc1.o        : snes/chip/obc1/obc1.cpp snes/chip/obc1/*
obj/snes-st0010.o      : snes/chip/st0010/st0010.cpp snes/chip/st0010/*
obj/snes-st0011.o      : snes/chip/st0011/st0011.cpp snes/chip/st0011/*
obj/snes-st0018.o      : snes/chip/st0018/st0018.cpp snes/chip/st0018/*
obj/snes-msu1.o        : snes/chip/msu1/msu1.cpp snes/chip/msu1/*

# library
snes_objects := $(patsubst %,obj/%.o,$(snes_objects))

library: $(snes_objects) obj/libsnes.o
ifeq ($(platform),x)
	ar rcs obj/libsnes.a $(snes_objects) obj/libsnes.o
	$(cpp) -o obj/libsnes.so -shared -Wl,-soname,libsnes.so.1 $(snes_objects) obj/libsnes.o
else ifeq ($(platform),osx)
	ar rcs obj/libsnes.a $(snes_objects) obj/libsnes.o
	$(cpp) -o obj/libsnes.dylib -shared -dynamiclib $(snes_objects) obj/libsnes.o
else ifeq ($(platform),win)
	$(cpp) -o obj/snes.dll -shared -Wl,--out-implib,libsnes.a $(snes_objects) obj/libsnes.o
endif

library-install:
ifeq ($(platform),x)
	install -D -m 755 obj/libsnes.a $(DESTDIR)$(prefix)/lib/libsnes.a
	install -D -m 755 obj/libsnes.so $(DESTDIR)$(prefix)/lib/libsnes.so
	ldconfig -n $(DESTDIR)$(prefix)/lib
else ifeq ($(platform),osx)
	cp obj/libsnes.dylib /usr/local/lib/libsnes.dylib
endif

library-uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/lib/libsnes.a
	rm $(DESTDIR)$(prefix)/lib/libsnes.so
else ifeq ($(platform),osx)
	rm /usr/local/lib/libsnes.dylib
endif

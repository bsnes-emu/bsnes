snes_objects := snes-system
snes_objects += snes-cartridge snes-cheat
snes_objects += snes-memory snes-cpucore snes-smpcore
snes_objects += snes-cpu snes-smp snes-dsp snes-ppu
snes_objects += snes-icd2 snes-superfx snes-sa1 snes-necdsp
snes_objects += snes-bsx snes-srtc snes-sdd1 snes-spc7110 snes-cx4
snes_objects += snes-obc1 snes-st0018 snes-sufamiturbo
snes_objects += snes-msu1 snes-serial
objects += $(snes_objects)

ifeq ($(profile),accuracy)
  flags += -DPROFILE_ACCURACY
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/dsp
  snesppu := $(snes)/ppu
else ifeq ($(profile),compatibility)
  flags += -DPROFILE_COMPATIBILITY
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu-compatibility
else ifeq ($(profile),performance)
  flags += -DPROFILE_PERFORMANCE
  snescpu := $(snes)/alt/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu-performance
endif

obj/libsnes.o: $(snes)/libsnes/libsnes.cpp $(snes)/libsnes/*

obj/snes-system.o   : $(snes)/system/system.cpp $(call rwildcard,$(snes)/system/) $(call rwildcard,$(snes)/video/)
obj/snes-memory.o   : $(snes)/memory/memory.cpp $(call rwildcard,$(snes)/memory/)
obj/snes-cpucore.o  : $(snes)/cpu/core/core.cpp $(call rwildcard,$(snes)/cpu/core/)
obj/snes-smpcore.o  : $(snes)/smp/core/core.cpp $(call rwildcard,$(snes)/smp/core/)
obj/snes-cpu.o      : $(snescpu)/cpu.cpp $(call rwildcard,$(snescpu)/)
obj/snes-smp.o      : $(snessmp)/smp.cpp $(call rwildcard,$(snessmp)/)
obj/snes-dsp.o      : $(snesdsp)/dsp.cpp $(call rwildcard,$(snesdsp)/)
obj/snes-ppu.o      : $(snesppu)/ppu.cpp $(call rwildcard,$(snesppu)/)
obj/snes-cartridge.o: $(snes)/cartridge/cartridge.cpp $(snes)/cartridge/*
obj/snes-cheat.o    : $(snes)/cheat/cheat.cpp $(snes)/cheat/*

obj/snes-icd2.o       : $(snes)/chip/icd2/icd2.cpp $(call rwildcard,$(snes)/chip/icd2/)
obj/snes-superfx.o    : $(snes)/chip/superfx/superfx.cpp $(call rwildcard,$(snes)/chip/superfx/)
obj/snes-sa1.o        : $(snes)/chip/sa1/sa1.cpp $(call rwildcard,$(snes)/chip/sa1/)
obj/snes-necdsp.o     : $(snes)/chip/necdsp/necdsp.cpp $(call rwildcard,$(snes)/chip/necdsp/)
obj/snes-bsx.o        : $(snes)/chip/bsx/bsx.cpp $(call rwildcard,$(snes)/chip/bsx/)
obj/snes-srtc.o       : $(snes)/chip/srtc/srtc.cpp $(snes)/chip/srtc/*
obj/snes-sdd1.o       : $(snes)/chip/sdd1/sdd1.cpp $(snes)/chip/sdd1/*
obj/snes-spc7110.o    : $(snes)/chip/spc7110/spc7110.cpp $(snes)/chip/spc7110/*
obj/snes-cx4.o        : $(snes)/chip/cx4/cx4.cpp $(snes)/chip/cx4/*
obj/snes-obc1.o       : $(snes)/chip/obc1/obc1.cpp $(snes)/chip/obc1/*
obj/snes-st0018.o     : $(snes)/chip/st0018/st0018.cpp $(snes)/chip/st0018/*
obj/snes-sufamiturbo.o: $(snes)/chip/sufamiturbo/sufamiturbo.cpp $(snes)/chip/sufamiturbo/*
obj/snes-msu1.o       : $(snes)/chip/msu1/msu1.cpp $(snes)/chip/msu1/*
obj/snes-serial.o     : $(snes)/chip/serial/serial.cpp $(snes)/chip/serial/*

###########
# library #
###########

snes_objects := $(patsubst %,obj/%.o,$(snes_objects))

library: $(snes_objects) obj/libsnes.o
ifeq ($(platform),x)
	ar rcs out/libsnes.a $(snes_objects) obj/libsnes.o
	$(cpp) -o out/libsnes.so -shared -Wl,-soname,libsnes.so.1 $(snes_objects) obj/libsnes.o
else ifeq ($(platform),osx)
	ar rcs out/libsnes.a $(snes_objects) obj/libsnes.o
	$(cpp) -o out/libsnes.dylib -install_name @executable_path/../Libraries/libsnes.dylib -shared -dynamiclib $(snes_objects) obj/libsnes.o
else ifeq ($(platform),win)
	$(cpp) -o out/snes.dll -shared -Wl,--out-implib,libsnes.a $(snes_objects) obj/libsnes.o
endif

library-install:
ifeq ($(platform),x)
	install -D -m 755 out/libsnes.a $(DESTDIR)$(prefix)/lib/libsnes.a
	install -D -m 755 out/libsnes.so $(DESTDIR)$(prefix)/lib/libsnes.so
	ldconfig -n $(DESTDIR)$(prefix)/lib
else ifeq ($(platform),osx)
	cp out/libsnes.dylib /usr/local/lib/libsnes.dylib
endif

library-uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/lib/libsnes.a
	rm $(DESTDIR)$(prefix)/lib/libsnes.so
else ifeq ($(platform),osx)
	rm /usr/local/lib/libsnes.dylib
endif

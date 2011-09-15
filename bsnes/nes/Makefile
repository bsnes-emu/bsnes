nes_objects := nes-interface nes-system nes-scheduler
nes_objects += nes-mapper nes-cartridge nes-memory
nes_objects += nes-cpu nes-apu nes-ppu
nes_objects += nes-cheat
objects += $(nes_objects)

obj/nes-interface.o: $(nes)/interface/interface.cpp $(call rwildcard,$(nes)/interface/)
obj/nes-system.o: $(nes)/system/system.cpp $(call rwildcard,$(nes)/system/)
obj/nes-scheduler.o: $(nes)/scheduler/scheduler.cpp $(call rwildcard,$(nes)/scheduler/)
obj/nes-mapper.o: $(nes)/mapper/mapper.cpp $(call rwildcard,$(nes)/mapper/)
obj/nes-cartridge.o: $(nes)/cartridge/cartridge.cpp $(call rwildcard,$(nes)/cartridge/)
obj/nes-memory.o: $(nes)/memory/memory.cpp $(call rwildcard,$(nes)/memory/)
obj/nes-cpu.o: $(nes)/cpu/cpu.cpp $(call rwildcard,$(nes)/cpu/)
obj/nes-apu.o: $(nes)/apu/apu.cpp $(call rwildcard,$(nes)/apu/)
obj/nes-ppu.o: $(nes)/ppu/ppu.cpp $(call rwildcard,$(nes)/ppu/)
obj/nes-cheat.o: $(nes)/cheat/cheat.cpp $(call rwildcard,$(nes)/cheat/)

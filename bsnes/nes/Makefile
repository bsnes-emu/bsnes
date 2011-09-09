nes_objects := nes-system nes-scheduler
nes_objects += nes-cartridge nes-memory
nes_objects += nes-cpu nes-ppu
objects += $(nes_objects)

obj/nes-system.o: $(nes)/system/system.cpp $(call rwildcard,$(nes)/system/)
obj/nes-scheduler.o: $(nes)/scheduler/scheduler.cpp $(call rwildcard,$(nes)/scheduler/)
obj/nes-cartridge.o: $(nes)/cartridge/cartridge.cpp $(call rwildcard,$(nes)/cartridge/)
obj/nes-memory.o: $(nes)/memory/memory.cpp $(call rwildcard,$(nes)/memory/)
obj/nes-cpu.o: $(nes)/cpu/cpu.cpp $(call rwildcard,$(nes)/cpu/)
obj/nes-ppu.o: $(nes)/ppu/ppu.cpp $(call rwildcard,$(nes)/ppu/)

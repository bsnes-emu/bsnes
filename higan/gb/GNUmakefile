processors += lr35902

objects += gb-interface gb-system
objects += gb-memory gb-cartridge
objects += gb-cpu gb-ppu gb-apu

obj/gb-interface.o: gb/interface/interface.cpp $(call rwildcard,gb/interface/)
obj/gb-system.o:    gb/system/system.cpp $(call rwildcard,gb/system/)
obj/gb-cartridge.o: gb/cartridge/cartridge.cpp $(call rwildcard,gb/cartridge/)
obj/gb-memory.o:    gb/memory/memory.cpp $(call rwildcard,gb/memory/)
obj/gb-cpu.o:       gb/cpu/cpu.cpp $(call rwildcard,gb/cpu/)
obj/gb-ppu.o:       gb/ppu/ppu.cpp $(call rwildcard,gb/ppu/)
obj/gb-apu.o:       gb/apu/apu.cpp $(call rwildcard,gb/apu/)

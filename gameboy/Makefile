gameboy_objects := libco
gameboy_objects += gameboy-system gameboy-cartridge gameboy-memory gameboy-cpu
objects += $(gameboy_objects)

obj/libco.o: libco/libco.c libco/*

obj/gameboy-system.o: $(gameboy)/system/system.cpp $(call rwildcard,$(gameboy)/system/)
obj/gameboy-cartridge.o: $(gameboy)/cartridge/cartridge.cpp $(call rwildcard,$(gameboy)/cartridge/)
obj/gameboy-memory.o: $(gameboy)/memory/memory.cpp $(call rwildcard,$(gameboy)/memory/)
obj/gameboy-cpu.o: $(gameboy)/cpu/cpu.cpp $(call rwildcard,$(gameboy)/cpu/)

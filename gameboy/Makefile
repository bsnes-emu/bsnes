gameboy_objects := libco
gameboy_objects += gameboy-system gameboy-scheduler
gameboy_objects += gameboy-memory gameboy-cartridge
gameboy_objects += gameboy-cpu gameboy-lcd
objects += $(gameboy_objects)

obj/libco.o: libco/libco.c libco/*

obj/gameboy-system.o: $(gameboy)/system/system.cpp $(call rwildcard,$(gameboy)/system/)
obj/gameboy-scheduler.o: $(gameboy)/scheduler/scheduler.cpp $(call rwildcard,$(gameboy)/scheduler/)
obj/gameboy-cartridge.o: $(gameboy)/cartridge/cartridge.cpp $(call rwildcard,$(gameboy)/cartridge/)
obj/gameboy-memory.o: $(gameboy)/memory/memory.cpp $(call rwildcard,$(gameboy)/memory/)
obj/gameboy-cpu.o: $(gameboy)/cpu/cpu.cpp $(call rwildcard,$(gameboy)/cpu/)
obj/gameboy-lcd.o: $(gameboy)/lcd/lcd.cpp $(call rwildcard,$(gameboy)/lcd/)

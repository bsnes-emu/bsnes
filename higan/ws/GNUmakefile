processors += v30mz

objects += ws-interface ws-system ws-scheduler
objects += ws-memory ws-eeprom ws-cartridge
objects += ws-cpu ws-ppu ws-apu

obj/ws-interface.o: ws/interface/interface.cpp $(call rwildcard,ws/interface/)
obj/ws-system.o:    ws/system/system.cpp $(call rwildcard,ws/system/)
obj/ws-scheduler.o: ws/scheduler/scheduler.cpp $(call rwildcard,ws/scheduler/)
obj/ws-memory.o:    ws/memory/memory.cpp $(call rwildcard,ws/memory/)
obj/ws-eeprom.o:    ws/eeprom/eeprom.cpp $(call rwildcard,ws/eeprom/)
obj/ws-cartridge.o: ws/cartridge/cartridge.cpp $(call rwildcard,ws/cartridge/)
obj/ws-cpu.o:       ws/cpu/cpu.cpp $(call rwildcard,ws/cpu/)
obj/ws-ppu.o:       ws/ppu/ppu.cpp $(call rwildcard,ws/ppu/)
obj/ws-apu.o:       ws/apu/apu.cpp $(call rwildcard,ws/apu/)

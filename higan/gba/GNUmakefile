processors += arm7tdmi

objects += gba-memory gba-interface gba-system
objects += gba-cartridge gba-player
objects += gba-cpu gba-ppu gba-apu

obj/gba-memory.o:    gba/memory/memory.cpp $(call rwildcard,gba/memory)
obj/gba-interface.o: gba/interface/interface.cpp $(call rwildcard,gba/interface)
obj/gba-system.o:    gba/system/system.cpp $(call rwildcard,gba/system)
obj/gba-cartridge.o: gba/cartridge/cartridge.cpp $(call rwildcard,gba/cartridge)
obj/gba-player.o:    gba/player/player.cpp $(call rwildcard,gba/player)
obj/gba-cpu.o:       gba/cpu/cpu.cpp $(call rwildcard,gba/cpu)
obj/gba-ppu.o:       gba/ppu/ppu.cpp $(call rwildcard,gba/ppu)
obj/gba-apu.o:       gba/apu/apu.cpp $(call rwildcard,gba/apu)

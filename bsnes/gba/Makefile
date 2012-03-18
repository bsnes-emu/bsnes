gba_objects := gba-interface gba-system gba-scheduler
gba_objects += gba-cartridge gba-memory
gba_objects += gba-cpu gba-ppu gba-apu
gba_objects += gba-video
objects += $(gba_objects)

obj/gba-interface.o: $(gba)/interface/interface.cpp $(call rwildcard,$(gba)/interface)
obj/gba-system.o: $(gba)/system/system.cpp $(call rwildcard,$(gba)/system)
obj/gba-scheduler.o: $(gba)/scheduler/scheduler.cpp $(call rwildcard,$(gba)/scheduler)
obj/gba-cartridge.o: $(gba)/cartridge/cartridge.cpp $(call rwildcard,$(gba)/cartridge)
obj/gba-memory.o: $(gba)/memory/memory.cpp $(call rwildcard,$(gba)/memory)
obj/gba-cpu.o: $(gba)/cpu/cpu.cpp $(call rwildcard,$(gba)/cpu)
obj/gba-ppu.o: $(gba)/ppu/ppu.cpp $(call rwildcard,$(gba)/ppu)
obj/gba-apu.o: $(gba)/apu/apu.cpp $(call rwildcard,$(gba)/apu)
obj/gba-video.o: $(gba)/video/video.cpp $(call rwildcard,$(gba)/video)

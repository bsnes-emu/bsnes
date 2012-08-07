nds_objects := nds-interface
nds_objects += nds-cpu nds-apu nds-ppu nds-gpu nds-video
nds_objects += nds-memory nds-system nds-slot1 nds-slot2 nds-wifi
objects += $(nds_objects)

obj/nds-interface.o: $(nds)/interface/interface.cpp $(call rwildcard,$(nds)/interface)
obj/nds-cpu.o:       $(nds)/cpu/cpu.cpp $(call rwildcard,$(nds)/cpu)
obj/nds-apu.o:       $(nds)/apu/apu.cpp $(call rwildcard,$(nds)/apu)
obj/nds-ppu.o:       $(nds)/ppu/ppu.cpp $(call rwildcard,$(nds)/ppu)
obj/nds-gpu.o:       $(nds)/gpu/gpu.cpp $(call rwildcard,$(nds)/gpu)
obj/nds-video.o:     $(nds)/video/video.cpp $(call rwildcard,$(nds)/video)
obj/nds-memory.o:    $(nds)/memory/memory.cpp $(call rwildcard,$(nds)/memory)
obj/nds-system.o:    $(nds)/system/system.cpp $(call rwildcard,$(nds)/system)
obj/nds-slot1.o:     $(nds)/slot1/slot1.cpp $(call rwildcard,$(nds)/slot1)
obj/nds-slot2.o:     $(nds)/slot2/slot2.cpp $(call rwildcard,$(nds)/slot2)
obj/nds-wifi.o:      $(nds)/wifi/wifi.cpp $(call rwildcard,$(nds)/wifi)
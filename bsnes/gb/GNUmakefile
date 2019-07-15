processors += sm83

objects += gb-interface gb-system
objects += gb-memory gb-cartridge
objects += gb-cpu gb-ppu gb-apu

obj/gb-interface.o: gb/interface/interface.cpp
obj/gb-system.o:    gb/system/system.cpp
obj/gb-cartridge.o: gb/cartridge/cartridge.cpp
obj/gb-memory.o:    gb/memory/memory.cpp
obj/gb-cpu.o:       gb/cpu/cpu.cpp
obj/gb-ppu.o:       gb/ppu/ppu.cpp
obj/gb-apu.o:       gb/apu/apu.cpp

flags += -DGB_INTERNAL -Wno-multichar -Dtypeof=__typeof__
options += -I../sameboy

objects += sb-apu sb-camera sb-debugger sb-display sb-gb sb-joypad sb-mbc
objects += sb-memory sb-printer sb-random sb-rewind sb-save_state sb-sgb
objects += sb-sm83_cpu sb-sm83_disassembler sb-symbol_hash sb-timing

obj/sb-apu.o:               ../sameboy/Core/apu.c
obj/sb-camera.o:            ../sameboy/Core/camera.c
obj/sb-debugger.o:          ../sameboy/Core/debugger.c
obj/sb-display.o:           ../sameboy/Core/display.c
obj/sb-gb.o:                ../sameboy/Core/gb.c
obj/sb-joypad.o:            ../sameboy/Core/joypad.c
obj/sb-mbc.o:               ../sameboy/Core/mbc.c
obj/sb-memory.o:            ../sameboy/Core/memory.c
obj/sb-printer.o:           ../sameboy/Core/printer.c
obj/sb-random.o:            ../sameboy/Core/random.c
obj/sb-rewind.o:            ../sameboy/Core/rewind.c
obj/sb-save_state.o:        ../sameboy/Core/save_state.c
obj/sb-sgb.o:               ../sameboy/Core/sgb.c
obj/sb-sm83_cpu.o:          ../sameboy/Core/sm83_cpu.c
obj/sb-sm83_disassembler.o: ../sameboy/Core/sm83_disassembler.c
obj/sb-symbol_hash.o:       ../sameboy/Core/symbol_hash.c
obj/sb-timing.o:            ../sameboy/Core/timing.c

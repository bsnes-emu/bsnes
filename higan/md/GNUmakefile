processors += m68k z80

objects += md-interface
objects += md-cpu md-apu md-vdp md-psg md-ym2612
objects += md-system md-cartridge

obj/md-interface.o: md/interface/interface.cpp $(call rwildcard,md/interface)
obj/md-cpu.o:       md/cpu/cpu.cpp $(call rwildcard,md/cpu)
obj/md-apu.o:       md/apu/apu.cpp $(call rwildcard,md/apu)
obj/md-vdp.o:       md/vdp/vdp.cpp $(call rwildcard,md/vdp)
obj/md-psg.o:       md/psg/psg.cpp $(call rwildcard,md/psg)
obj/md-ym2612.o:    md/ym2612/ym2612.cpp $(call rwildcard,md/ym2612)
obj/md-system.o:    md/system/system.cpp $(call rwildcard,md/system)
obj/md-cartridge.o: md/cartridge/cartridge.cpp $(call rwildcard,md/cartridge)

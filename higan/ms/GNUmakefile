processors += z80

objects += ms-interface
objects += ms-cpu ms-vdp ms-psg
objects += ms-system ms-cartridge ms-bus
objects += ms-controller

obj/ms-interface.o:  ms/interface/interface.cpp $(call rwildcard,ms/interface)
obj/ms-cpu.o:        ms/cpu/cpu.cpp $(call rwildcard,ms/cpu)
obj/ms-vdp.o:        ms/vdp/vdp.cpp $(call rwildcard,ms/vdp)
obj/ms-psg.o:        ms/psg/psg.cpp $(call rwildcard,ms/psg)
obj/ms-system.o:     ms/system/system.cpp $(call rwildcard,ms/system)
obj/ms-cartridge.o:  ms/cartridge/cartridge.cpp $(call rwildcard,ms/cartridge)
obj/ms-bus.o:        ms/bus/bus.cpp $(call rwildcard,ms/bus)
obj/ms-controller.o: ms/controller/controller.cpp $(call rwildcard,ms/controller)

processors += huc6280

objects += pce-interface
objects += pce-cpu pce-vpc pce-vce pce-vdc pce-psg
objects += pce-system pce-cartridge
objects += pce-controller

obj/pce-interface.o:  pce/interface/interface.cpp $(call rwildcard,pce/interface)
obj/pce-cpu.o:        pce/cpu/cpu.cpp $(call rwildcard,pce/cpu)
obj/pce-vpc.o:        pce/vpc/vpc.cpp $(call rwildcard,pce/vpc)
obj/pce-vce.o:        pce/vce/vce.cpp $(call rwildcard,pce/vce)
obj/pce-vdc.o:        pce/vdc/vdc.cpp $(call rwildcard,pce/vdc)
obj/pce-psg.o:        pce/psg/psg.cpp $(call rwildcard,pce/psg)
obj/pce-system.o:     pce/system/system.cpp $(call rwildcard,pce/system)
obj/pce-cartridge.o:  pce/cartridge/cartridge.cpp $(call rwildcard,pce/cartridge)
obj/pce-controller.o: pce/controller/controller.cpp $(call rwildcard,pce/controller)

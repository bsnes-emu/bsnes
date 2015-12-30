sfc_objects := sfc-interface sfc-system sfc-controller
sfc_objects += sfc-cartridge sfc-cheat
sfc_objects += sfc-memory sfc-cpu sfc-smp sfc-dsp sfc-ppu
sfc_objects += sfc-satellaview sfc-eboot
sfc_objects += sfc-icd2 sfc-mcc sfc-nss sfc-event
sfc_objects += sfc-sa1 sfc-superfx
sfc_objects += sfc-armdsp sfc-hitachidsp sfc-necdsp
sfc_objects += sfc-epsonrtc sfc-sharprtc
sfc_objects += sfc-spc7110 sfc-sdd1 sfc-obc1
sfc_objects += sfc-msu1
sfc_objects += sfc-bsmemory sfc-sufamiturbo
objects += $(sfc_objects)

ifeq ($(profile),accuracy)
  flags += -DPROFILE_ACCURACY
  sfccpu := $(sfc)/cpu
  sfcsmp := $(sfc)/smp
  sfcdsp := $(sfc)/dsp
  sfcppu := $(sfc)/ppu
else ifeq ($(profile),balanced)
  flags += -DPROFILE_BALANCED
  sfccpu := $(sfc)/cpu
  sfcsmp := $(sfc)/smp
  sfcdsp := $(sfc)/alt/dsp
  sfcppu := $(sfc)/alt/ppu-balanced
else ifeq ($(profile),performance)
  flags += -DPROFILE_PERFORMANCE
  sfccpu := $(sfc)/alt/cpu
  sfcsmp := $(sfc)/alt/smp
  sfcdsp := $(sfc)/alt/dsp
  sfcppu := $(sfc)/alt/ppu-performance
else
  $(error unknown Super Famicom profile)
endif

obj/sfc-interface.o:   $(sfc)/interface/interface.cpp $(call rwildcard,$(sfc)/interface)
obj/sfc-system.o:      $(sfc)/system/system.cpp $(call rwildcard,$(sfc)/system/)
obj/sfc-controller.o:  $(sfc)/controller/controller.cpp $(call rwildcard,$(sfc)/controller/)
obj/sfc-cartridge.o:   $(sfc)/cartridge/cartridge.cpp $(call rwildcard,$(sfc)/cartridge/)
obj/sfc-cheat.o:       $(sfc)/cheat/cheat.cpp $(call rwildcard,$(sfc)/cheat/)
obj/sfc-memory.o:      $(sfc)/memory/memory.cpp $(call rwildcard,$(sfc)/memory/)
obj/sfc-cpu.o:         $(sfccpu)/cpu.cpp $(call rwildcard,$(sfccpu)/)
obj/sfc-smp.o:         $(sfcsmp)/smp.cpp $(call rwildcard,$(sfcsmp)/)
obj/sfc-dsp.o:         $(sfcdsp)/dsp.cpp $(call rwildcard,$(sfcdsp)/)
obj/sfc-ppu.o:         $(sfcppu)/ppu.cpp $(call rwildcard,$(sfcppu)/)

obj/sfc-satellaview.o: $(sfc)/expansion/satellaview/satellaview.cpp $(call rwildcard,$(sfc)/expansion/satellaview/)
obj/sfc-eboot.o:       $(sfc)/expansion/eboot/eboot.cpp $(call rwildcard,$(sfc)/expansion/eboot/)

obj/sfc-icd2.o:        $(sfc)/coprocessor/icd2/icd2.cpp $(call rwildcard,$(sfc)/coprocessor/icd2/)
obj/sfc-mcc.o:         $(sfc)/coprocessor/mcc/mcc.cpp $(call rwildcard,$(sfc)/coprocessor/mcc/)
obj/sfc-nss.o:         $(sfc)/coprocessor/nss/nss.cpp $(call rwildcard,$(sfc)/coprocessor/nss/)
obj/sfc-event.o:       $(sfc)/coprocessor/event/event.cpp $(call rwildcard,$(sfc)/coprocessor/event/)

obj/sfc-sa1.o:         $(sfc)/coprocessor/sa1/sa1.cpp $(call rwildcard,$(sfc)/coprocessor/sa1/)
obj/sfc-superfx.o:     $(sfc)/coprocessor/superfx/superfx.cpp $(call rwildcard,$(sfc)/coprocessor/superfx/)

obj/sfc-armdsp.o:      $(sfc)/coprocessor/armdsp/armdsp.cpp $(call rwildcard,$(sfc)/coprocessor/armdsp/)
obj/sfc-hitachidsp.o:  $(sfc)/coprocessor/hitachidsp/hitachidsp.cpp $(call rwildcard,$(sfc)/coprocessor/hitachidsp/)
obj/sfc-necdsp.o:      $(sfc)/coprocessor/necdsp/necdsp.cpp $(call rwildcard,$(sfc)/coprocessor/necdsp/)

obj/sfc-epsonrtc.o:    $(sfc)/coprocessor/epsonrtc/epsonrtc.cpp $(call rwildcard,$(sfc)/coprocessor/epsonrtc/)
obj/sfc-sharprtc.o:    $(sfc)/coprocessor/sharprtc/sharprtc.cpp $(call rwildcard,$(sfc)/coprocessor/sharprtc/)

obj/sfc-spc7110.o:     $(sfc)/coprocessor/spc7110/spc7110.cpp $(call rwildcard,$(sfc)/coprocessor/spc7110/)
obj/sfc-sdd1.o:        $(sfc)/coprocessor/sdd1/sdd1.cpp $(call rwildcard,$(sfc)/coprocessor/sdd1/)
obj/sfc-obc1.o:        $(sfc)/coprocessor/obc1/obc1.cpp $(call rwildcard,$(sfc)/coprocessor/obc1/)

obj/sfc-msu1.o:        $(sfc)/coprocessor/msu1/msu1.cpp $(call rwildcard,$(sfc)/coprocessor/msu1/)

obj/sfc-bsmemory.o:    $(sfc)/slot/bsmemory/bsmemory.cpp $(call rwildcard,$(sfc)/slot/bsmemory/)
obj/sfc-sufamiturbo.o: $(sfc)/slot/sufamiturbo/sufamiturbo.cpp $(call rwildcard,$(sfc)/slot/sufamiturbo/)

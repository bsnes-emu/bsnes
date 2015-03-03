sfc_objects := sfc-interface sfc-system sfc-controller
sfc_objects += sfc-cartridge sfc-cheat
sfc_objects += sfc-memory sfc-cpu sfc-smp sfc-dsp sfc-ppu
sfc_objects += sfc-satellaviewbase
sfc_objects += sfc-icd2 sfc-bsx sfc-nss sfc-event
sfc_objects += sfc-sa1 sfc-superfx
sfc_objects += sfc-armdsp sfc-hitachidsp sfc-necdsp
sfc_objects += sfc-epsonrtc sfc-sharprtc
sfc_objects += sfc-spc7110 sfc-sdd1 sfc-obc1
sfc_objects += sfc-msu1
sfc_objects += sfc-satellaviewcart sfc-sufamiturbo
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

obj/sfc-interface.o:       $(sfc)/interface/interface.cpp $(call rwildcard,$(sfc)/interface)
obj/sfc-system.o:          $(sfc)/system/system.cpp $(call rwildcard,$(sfc)/system/)
obj/sfc-controller.o:      $(sfc)/controller/controller.cpp $(call rwildcard,$(sfc)/controller/)
obj/sfc-cartridge.o:       $(sfc)/cartridge/cartridge.cpp $(call rwildcard,$(sfc)/cartridge/)
obj/sfc-cheat.o:           $(sfc)/cheat/cheat.cpp $(call rwildcard,$(sfc)/cheat/)
obj/sfc-memory.o:          $(sfc)/memory/memory.cpp $(call rwildcard,$(sfc)/memory/)
obj/sfc-cpu.o:             $(sfccpu)/cpu.cpp $(call rwildcard,$(sfccpu)/)
obj/sfc-smp.o:             $(sfcsmp)/smp.cpp $(call rwildcard,$(sfcsmp)/)
obj/sfc-dsp.o:             $(sfcdsp)/dsp.cpp $(call rwildcard,$(sfcdsp)/)
obj/sfc-ppu.o:             $(sfcppu)/ppu.cpp $(call rwildcard,$(sfcppu)/)

obj/sfc-satellaviewbase.o: $(sfc)/base/satellaview/satellaview.cpp $(call rwildcard,$(sfc)/base/satellaview/)

obj/sfc-icd2.o:            $(sfc)/chip/icd2/icd2.cpp $(call rwildcard,$(sfc)/chip/icd2/)
obj/sfc-bsx.o:             $(sfc)/chip/bsx/bsx.cpp $(call rwildcard,$(sfc)/chip/bsx/)
obj/sfc-nss.o:             $(sfc)/chip/nss/nss.cpp $(call rwildcard,$(sfc)/chip/nss/)
obj/sfc-event.o:           $(sfc)/chip/event/event.cpp $(call rwildcard,$(sfc)/chip/event/)

obj/sfc-sa1.o:             $(sfc)/chip/sa1/sa1.cpp $(call rwildcard,$(sfc)/chip/sa1/)
obj/sfc-superfx.o:         $(sfc)/chip/superfx/superfx.cpp $(call rwildcard,$(sfc)/chip/superfx/)

obj/sfc-armdsp.o:          $(sfc)/chip/armdsp/armdsp.cpp $(call rwildcard,$(sfc)/chip/armdsp/)
obj/sfc-hitachidsp.o:      $(sfc)/chip/hitachidsp/hitachidsp.cpp $(call rwildcard,$(sfc)/chip/hitachidsp/)
obj/sfc-necdsp.o:          $(sfc)/chip/necdsp/necdsp.cpp $(call rwildcard,$(sfc)/chip/necdsp/)

obj/sfc-epsonrtc.o:        $(sfc)/chip/epsonrtc/epsonrtc.cpp $(call rwildcard,$(sfc)/chip/epsonrtc/)
obj/sfc-sharprtc.o:        $(sfc)/chip/sharprtc/sharprtc.cpp $(call rwildcard,$(sfc)/chip/sharprtc/)

obj/sfc-spc7110.o:         $(sfc)/chip/spc7110/spc7110.cpp $(call rwildcard,$(sfc)/chip/spc7110/)
obj/sfc-sdd1.o:            $(sfc)/chip/sdd1/sdd1.cpp $(call rwildcard,$(sfc)/chip/sdd1/)
obj/sfc-obc1.o:            $(sfc)/chip/obc1/obc1.cpp $(call rwildcard,$(sfc)/chip/obc1/)

obj/sfc-msu1.o:            $(sfc)/chip/msu1/msu1.cpp $(call rwildcard,$(sfc)/chip/msu1/)

obj/sfc-satellaviewcart.o: $(sfc)/slot/satellaview/satellaview.cpp $(call rwildcard,$(sfc)/slot/satellaview/)
obj/sfc-sufamiturbo.o:     $(sfc)/slot/sufamiturbo/sufamiturbo.cpp $(call rwildcard,$(sfc)/slot/sufamiturbo/)

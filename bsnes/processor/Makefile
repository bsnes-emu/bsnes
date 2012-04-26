processor_objects :=
processor_objects += $(if $(findstring arm,$(processors)),processor-arm)
processor_objects += $(if $(findstring hg51b,$(processors)),processor-hg51b)
processor_objects += $(if $(findstring lr35902,$(processors)),processor-lr35902)
processor_objects += $(if $(findstring upd96050,$(processors)),processor-upd96050)
objects += $(processor_objects)

processor := processor
obj/processor-arm.o:      $(processor)/arm/arm.cpp $(call rwildcard,$(processor)/arm)
obj/processor-hg51b.o:    $(processor)/hg51b/hg51b.cpp $(call rwildcard,$(processor)/hg51b)
obj/processor-lr35902.o:  $(processor)/lr35902/lr35902.cpp $(call rwildcard,$(processor)/lr35902)
obj/processor-upd96050.o: $(processor)/upd96050/upd96050.cpp $(call rwildcard,$(processor)/upd96050)

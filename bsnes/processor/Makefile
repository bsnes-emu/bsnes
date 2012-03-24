processor_objects :=
processor_objects += $(if $(findstring arm,$(processors)),processor-arm)
processor_objects += $(if $(findstring hg51b,$(processors)),processor-hg51b)
objects += $(processor_objects)

processor := processor
obj/processor-arm.o:   $(processor)/arm/arm.cpp $(call rwildcard,$(processor)/arm)
obj/processor-hg51b.o: $(processor)/hg51b/hg51b.cpp $(call rwildcard,$(processor)/hg51b)

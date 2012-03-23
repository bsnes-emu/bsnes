processor_objects :=
processor_objects += $(if $(findstring arm,$(processors)),processor-arm)
objects += $(processor_objects)

processor := processor
obj/processor-arm.o: $(processor)/arm/arm.cpp $(call rwildcard,$(processor)/arm)

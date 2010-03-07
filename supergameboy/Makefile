include nall/Makefile

c     := $(compiler) -std=gnu99
cpp   := $(subst cc,++,$(compiler)) -std=gnu++0x
flags := -O3 -fomit-frame-pointer -I. -Icommon -Ilibgambatte/include -Ilibgambatte/src
link  :=

ifeq ($(platform),osx)
  flags := -fPIC $(flags)
else ifeq ($(platform),x)
  flags := -fPIC $(flags)
  link += -s
endif

objects := supergameboy
objects += bitmap_font colorconversion cpu gambatte initstate interrupter
objects += memory rtc sound state_osd_elements statesaver video
objects += channel1 channel2 channel3 channel4 duty_unit envelope_unit length_counter
objects += basic_add_event break_event irq_event ly_counter lyc_irq
objects += m3_extra_cycles mode3_event mode0_irq mode1_irq mode2_irq
objects += sc_reader scx_reader sprite_mapper we_master_checker we wx_reader wy
objects += catrom2x catrom3x kreed2xsai maxsthq2x maxsthq3x file

compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(cpp) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

objects := $(patsubst %,obj/%.o,$(objects))

####################
### supergameboy ###
####################

obj/supergameboy.o: supergameboy.cpp *.cpp *.hpp $(call rwildcard,interface/)

###################
### libgambatte ###
###################

obj/bitmap_font.o: libgambatte/src/bitmap_font.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/colorconversion.o: libgambatte/src/colorconversion.cpp  $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/cpu.o: libgambatte/src/cpu.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/gambatte.o: libgambatte/src/gambatte.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/initstate.o: libgambatte/src/initstate.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/interrupter.o: libgambatte/src/interrupter.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/memory.o: libgambatte/src/memory.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/rtc.o: libgambatte/src/rtc.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/sound.o: libgambatte/src/sound.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/state_osd_elements.o: libgambatte/src/state_osd_elements.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/statesaver.o: libgambatte/src/statesaver.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/video.o: libgambatte/src/video.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)

obj/channel1.o: libgambatte/src/sound/channel1.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/channel2.o: libgambatte/src/sound/channel2.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/channel3.o: libgambatte/src/sound/channel3.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/channel4.o: libgambatte/src/sound/channel4.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/duty_unit.o: libgambatte/src/sound/duty_unit.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/envelope_unit.o: libgambatte/src/sound/envelope_unit.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/length_counter.o: libgambatte/src/sound/length_counter.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)

obj/basic_add_event.o: libgambatte/src/video/basic_add_event.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/break_event.o: libgambatte/src/video/break_event.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/irq_event.o: libgambatte/src/video/irq_event.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/ly_counter.o: libgambatte/src/video/ly_counter.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/lyc_irq.o: libgambatte/src/video/lyc_irq.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/m3_extra_cycles.o: libgambatte/src/video/m3_extra_cycles.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/mode3_event.o: libgambatte/src/video/mode3_event.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/mode0_irq.o: libgambatte/src/video/mode0_irq.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/mode1_irq.o: libgambatte/src/video/mode1_irq.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/mode2_irq.o: libgambatte/src/video/mode2_irq.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/sc_reader.o: libgambatte/src/video/sc_reader.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/scx_reader.o: libgambatte/src/video/scx_reader.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/sprite_mapper.o: libgambatte/src/video/sprite_mapper.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/we_master_checker.o: libgambatte/src/video/we_master_checker.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/we.o: libgambatte/src/video/we.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/wx_reader.o: libgambatte/src/video/wx_reader.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/wy.o: libgambatte/src/video/wy.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)

obj/catrom2x.o: libgambatte/src/video/filters/catrom2x.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/catrom3x.o: libgambatte/src/video/filters/catrom3x.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/kreed2xsai.o: libgambatte/src/video/filters/kreed2xsai.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/maxsthq2x.o: libgambatte/src/video/filters/maxsthq2x.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)
obj/maxsthq3x.o: libgambatte/src/video/filters/maxsthq3x.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)

obj/file.o: libgambatte/src/file/file.cpp $(call rwildcard,common/) $(call rwildcard,libgambatte/)

###############
### targets ###
###############

build: $(objects)
ifeq ($(platform),win)
	$(cpp) $(link) -o supergameboy.dll -shared -Wl,--out-implib,libsupergameboy.a $(objects) $(qtlib)
else ifeq ($(platform),osx)
	ar rcs libsupergameboy.a $(objects)
	$(cpp) $(link) -o libsupergameboy.dylib -shared -dynamiclib $(objects) $(qtlib)
else
	ar rcs libsupergameboy.a $(objects)
	$(cpp) $(link) -o libsupergameboy.so -shared -Wl,-soname,libsupergameboy.so.1 $(objects) $(qtlib)
endif

install:
ifeq ($(platform),osx)
	cp libsupergameboy.dylib /usr/local/lib/libsupergameboy.dylib
else
	install -D -m 755 libsupergameboy.a $(DESTDIR)$(prefix)/lib
	install -D -m 755 libsupergameboy.so $(DESTDIR)$(prefix)/lib
	ldconfig -n $(DESTDIR)$(prefix)/lib
endif

clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,libsupergameboy.a)
	-@$(call delete,supergameboy.dll)
	-@$(call delete,libsupergameboy.dylib)
	-@$(call delete,libsupergameboy.so)

cc = g++

cflags  = -O2 -fomit-frame-pointer -Wall -Wextra
cflags += -fno-exceptions -fno-rtti
cflags += -DHAVE_STDINT_H -DCHAR_WIDTH_8
cflags += -Isrc -Iinclude -Icommon

headers  = src/*.h src/file/*.h src/sound/*.h src/video/*.h src/video/filters/*.h
headers += include/*.h
headers += common/*.h common/resample/*.h

source = \
  src/bitmap_font.cpp \
  src/colorconversion.cpp \
  src/cpu.cpp \
  src/gambatte.cpp \
  src/initstate.cpp \
  src/interrupter.cpp \
  src/memory.cpp \
  src/rtc.cpp \
  src/sound.cpp \
  src/state_osd_elements.cpp \
  src/statesaver.cpp \
  src/video.cpp \
  src/sound/channel1.cpp \
  src/sound/channel2.cpp \
  src/sound/channel3.cpp \
  src/sound/channel4.cpp \
  src/sound/duty_unit.cpp \
  src/sound/envelope_unit.cpp \
  src/sound/length_counter.cpp \
  src/video/basic_add_event.cpp \
  src/video/break_event.cpp \
  src/video/irq_event.cpp \
  src/video/ly_counter.cpp \
  src/video/lyc_irq.cpp \
  src/video/m3_extra_cycles.cpp \
  src/video/mode3_event.cpp \
  src/video/mode0_irq.cpp \
  src/video/mode1_irq.cpp \
  src/video/mode2_irq.cpp \
  src/video/sc_reader.cpp \
  src/video/scx_reader.cpp \
  src/video/sprite_mapper.cpp \
  src/video/we_master_checker.cpp \
  src/video/we.cpp \
  src/video/wx_reader.cpp \
  src/video/wy.cpp \
  src/video/filters/catrom2x.cpp \
  src/video/filters/catrom3x.cpp \
  src/video/filters/kreed2xsai.cpp \
  src/video/filters/maxsthq2x.cpp \
  src/video/filters/maxsthq3x.cpp \
  src/file/file.cpp \

all: build;

clean:
	-@rm obj/*.o
	-@rm libgambatte.a

$(foreach item,$(source),$(eval obj/$(notdir $(basename $(item))).o: $(item) $(headers)))
objects := $(foreach item,$(source),obj/$(notdir $(basename $(item))).o)

%.o: $<
	$(cc) -o $@ -c $< $(cflags)

build: $(objects)
	ar rcs libgambatte.a $(objects)

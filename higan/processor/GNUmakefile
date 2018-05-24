processors := $(call unique,$(processors))

objects += $(if $(findstring arm7tdmi,$(processors)),processor-arm7tdmi)
objects += $(if $(findstring gsu,$(processors)),processor-gsu)
objects += $(if $(findstring hg51b,$(processors)),processor-hg51b)
objects += $(if $(findstring huc6280,$(processors)),processor-huc6280)
objects += $(if $(findstring lr35902,$(processors)),processor-lr35902)
objects += $(if $(findstring m68k,$(processors)),processor-m68k)
objects += $(if $(findstring mos6502,$(processors)),processor-mos6502)
objects += $(if $(findstring spc700,$(processors)),processor-spc700)
objects += $(if $(findstring upd96050,$(processors)),processor-upd96050)
objects += $(if $(findstring v30mz,$(processors)),processor-v30mz)
objects += $(if $(findstring wdc65816,$(processors)),processor-wdc65816)
objects += $(if $(findstring z80,$(processors)),processor-z80)

obj/processor-arm7tdmi.o: processor/arm7tdmi/arm7tdmi.cpp
obj/processor-gsu.o:      processor/gsu/gsu.cpp
obj/processor-hg51b.o:    processor/hg51b/hg51b.cpp
obj/processor-huc6280.o:  processor/huc6280/huc6280.cpp
obj/processor-lr35902.o:  processor/lr35902/lr35902.cpp
obj/processor-m68k.o:     processor/m68k/m68k.cpp
obj/processor-mos6502.o:  processor/mos6502/mos6502.cpp
obj/processor-spc700.o:   processor/spc700/spc700.cpp
obj/processor-upd96050.o: processor/upd96050/upd96050.cpp
obj/processor-v30mz.o:    processor/v30mz/v30mz.cpp
obj/processor-wdc65816.o: processor/wdc65816/wdc65816.cpp
obj/processor-z80.o:      processor/z80/z80.cpp

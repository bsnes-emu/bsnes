include lib/nall/Makefile

ui := ui_qt
qtlibs := $(strip QtCore QtGui $(if $(findstring osx,$(platform)),QtOpenGL))
include lib/nall/qt/Makefile

################
### compiler ###
################

c     := $(compiler) --std=gnu99
cpp   := $(subst cc,++,$(compiler)) -std=gnu++0x
flags := -O3 -fomit-frame-pointer -Ilib
link  :=

# profile-guided instrumentation:
# flags += -fprofile-generate
# link += -lgcov

# profile-guided optimization:
# flags += -fprofile-use

################
### platform ###
################

ifeq ($(platform),x)
  link += -s -ldl -lX11 -lXext

  ruby := video.glx video.xv video.qtraster video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  ruby := video.qtopengl video.qtraster
  ruby += audio.openal
  ruby += input.carbon

  link += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  link += -mwindows -mthreads
# link += -mconsole -mthreads
  link += -s -luuid -lkernel32 -luser32 -lgdi32 -lshell32
  link += -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc

  ruby := video.direct3d video.wgl video.directdraw video.gdi video.qtraster
  ruby += audio.directsound
  ruby += input.rawinput input.directinput

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
else
  unknown_platform: help;
endif

############
### ruby ###
############

rubyflags := $(if $(finstring .sdl,$(ruby)),`sdl-config --cflags`)
rubyflags += $(if $(findstring .qt,$(ruby)),$(qtinc))

link += $(if $(findstring .sdl,$(ruby)),`sdl-config --libs`)
link += $(if $(findstring video.direct3d,$(ruby)),-ld3d9)
link += $(if $(findstring video.directdraw,$(ruby)),-lddraw)
link += $(if $(findstring video.glx,$(ruby)),-lGL)
link += $(if $(findstring video.wgl,$(ruby)),-lopengl32)
link += $(if $(findstring video.xv,$(ruby)),-lXv)
link += $(if $(findstring audio.alsa,$(ruby)),-lasound)
link += $(if $(findstring audio.ao,$(ruby)),-lao)
link += $(if $(findstring audio.directsound,$(ruby)),-ldsound)
link += $(if $(findstring audio.pulseaudio,$(ruby)),-lpulse)
link += $(if $(findstring audio.pulseaudiosimple,$(ruby)),-lpulse-simple)
link += $(if $(findstring input.directinput,$(ruby)),-ldinput8 -ldxguid)
link += $(if $(findstring input.rawinput,$(ruby)),-ldinput8 -ldxguid)

###############
### objects ###
###############

objects := libco ruby
objects += system cartridge cheat
objects += memory smemory cpu cpucore scpu smp smpcore ssmp dsp sdsp ppu bppu
objects += supergameboy superfx sa1
objects += bsx srtc sdd1 spc7110 cx4 dsp1 dsp2 dsp3 dsp4 obc1 st0010 st0011 st0018
objects += msu1

######################
### implicit rules ###
######################

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

include $(ui)/Makefile
objects := $(patsubst %,obj/%.o,$(objects))
rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)

#################
### libraries ###
#################

obj/ruby.o: lib/ruby/ruby.cpp $(call rwildcard,lib/ruby/*)
	$(call compile,$(rubydef) $(rubyflags))
obj/libco.o: lib/libco/libco.c lib/libco/*

#################
### utilities ###
#################

obj/cartridge.o: cartridge/cartridge.cpp cartridge/*
obj/cheat.o    : cheat/cheat.cpp cheat/*

##############
### memory ###
##############

obj/memory.o : memory/memory.cpp memory/*
obj/smemory.o: memory/smemory/smemory.cpp $(call rwildcard,memory/smemory/)

###########
### cpu ###
###########

obj/cpu.o    : cpu/cpu.cpp cpu/*
obj/cpucore.o: cpu/core/core.cpp $(call rwildcard,cpu/core/)
obj/scpu.o   : cpu/scpu/scpu.cpp $(call rwildcard,cpu/scpu/)

###########
### smp ###
###########

obj/smp.o    : smp/smp.cpp smp/*
obj/smpcore.o: smp/core/core.cpp $(call rwildcard,smp/core/)
obj/ssmp.o   : smp/ssmp/ssmp.cpp $(call rwildcard,smp/ssmp/)

###########
### dsp ###
###########

obj/dsp.o : dsp/dsp.cpp dsp/*
obj/adsp.o: dsp/adsp/adsp.cpp dsp/adsp/*
obj/sdsp.o: dsp/sdsp/sdsp.cpp dsp/sdsp/*

###########
### ppu ###
###########

obj/ppu.o : ppu/ppu.cpp ppu/*
obj/bppu.o: ppu/bppu/bppu.cpp $(call rwildcard,ppu/bppu/)

##############
### system ###
##############

obj/system.o: system/system.cpp $(call rwildcard,system/)

#####################
### special chips ###
#####################

obj/supergameboy.o: chip/supergameboy/supergameboy.cpp $(call rwildcard,chip/supergameboy/)
obj/superfx.o     : chip/superfx/superfx.cpp $(call rwildcard,chip/superfx/)
obj/sa1.o         : chip/sa1/sa1.cpp $(call rwildcard,chip/sa1/)
obj/bsx.o         : chip/bsx/bsx.cpp chip/bsx/*
obj/srtc.o        : chip/srtc/srtc.cpp chip/srtc/*
obj/sdd1.o        : chip/sdd1/sdd1.cpp chip/sdd1/*
obj/spc7110.o     : chip/spc7110/spc7110.cpp chip/spc7110/*
obj/cx4.o         : chip/cx4/cx4.cpp chip/cx4/*
obj/dsp1.o        : chip/dsp1/dsp1.cpp chip/dsp1/*
obj/dsp2.o        : chip/dsp2/dsp2.cpp chip/dsp2/*
obj/dsp3.o        : chip/dsp3/dsp3.cpp chip/dsp3/*
obj/dsp4.o        : chip/dsp4/dsp4.cpp chip/dsp4/*
obj/obc1.o        : chip/obc1/obc1.cpp chip/obc1/*
obj/st0010.o      : chip/st0010/st0010.cpp chip/st0010/*
obj/st0011.o      : chip/st0011/st0011.cpp chip/st0011/*
obj/st0018.o      : chip/st0018/st0018.cpp chip/st0018/*
obj/msu1.o        : chip/msu1/msu1.cpp chip/msu1/*

###############
### targets ###
###############

build: ui_build $(objects)
ifeq ($(platform),osx)
	test -d ../bsnes.app || mkdir -p ../bsnes.app/Contents/MacOS
	$(strip $(cpp) -o ../bsnes.app/Contents/MacOS/bsnes $(objects) $(link))
else
	$(strip $(cpp) -o ../bsnes $(objects) $(link))
endif

install:
	install -D -m 755 ../bsnes $(DESTDIR)$(prefix)/bin/bsnes
	install -D -m 644 data/bsnes.png $(DESTDIR)$(prefix)/share/pixmaps/bsnes.png
	install -D -m 644 data/bsnes.desktop $(DESTDIR)$(prefix)/share/applications/bsnes.desktop

uninstall:
	rm $(DESTDIR)$(prefix)/bin/bsnes
	rm $(DESTDIR)$(prefix)/share/pixmaps/bsnes.png
	rm $(DESTDIR)$(prefix)/share/applications/bsnes.desktop

clean: ui_clean
	-@$(call delete,obj/*.o)
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

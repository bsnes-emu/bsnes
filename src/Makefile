include lib/nall/Makefile
include lib/nall/Makefile-qt
ui = ui_qt

################
### compiler ###
################

c     := $(compiler)
cpp   := $(subst cc,++,$(compiler))
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
  link += -s

  ruby := video.glx video.xv video.sdl video.qtimage
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.ao
  ruby += input.sdl input.x

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  ruby := video.qtimage
  ruby += audio.openal
  ruby += input.carbon

  link += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  link += -mwindows
# link += -mconsole
  link += -s -luuid -lkernel32 -luser32 -lgdi32 -lshell32
  # statically link Qt for Windows build
  link += -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc

  ruby := video.direct3d video.wgl video.directdraw video.gdi video.qtimage
  ruby += audio.directsound
  ruby += input.rawinput input.directinput

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
else
  unknown_platform: help;
endif

############
### ruby ###
############

rubyflags := $(call ifhas,.sdl,$(ruby),`sdl-config --cflags`)
rubyflags += $(call ifhas,.qt,$(ruby),$(qtinc))

link += $(call ifhas,.sdl,$(ruby),`sdl-config --libs`)
link += $(call ifhas,video.direct3d,$(ruby),-ld3d9)
link += $(call ifhas,video.directdraw,$(ruby),-lddraw)
link += $(call ifhas,video.glx,$(ruby),-lGL)
link += $(call ifhas,video.wgl,$(ruby),-lopengl32)
link += $(call ifhas,video.xv,$(ruby),-lXv)
link += $(call ifhas,audio.alsa,$(ruby),-lasound)
link += $(call ifhas,audio.ao,$(ruby),-lao)
link += $(call ifhas,audio.directsound,$(ruby),-ldsound)
link += $(call ifhas,audio.pulseaudio,$(ruby),-lpulse-simple)
link += $(call ifhas,input.directinput,$(ruby),-ldinput8 -ldxguid)
link += $(call ifhas,input.rawinput,$(ruby),-ldinput8 -ldxguid)

####################
### core objects ###
####################

objects := libco ruby libreader libfilter
objects += system cartridge cheat
objects += memory smemory cpu cpucore scpu smp smpcore ssmp sdsp ppu bppu
objects += sgb superfx sa1
objects += bsx srtc sdd1 spc7110 cx4 dsp1 dsp2 dsp3 dsp4 obc1 st010 st011 st018

ifeq ($(enable_gzip),true)
  objects += adler32 compress crc32 deflate gzio inffast inflate inftrees ioapi trees unzip zip zutil
  flags += -DGZIP_SUPPORT
endif

ifeq ($(enable_jma),true)
  objects += jma jcrc32 lzmadec 7zlzma iiostrm inbyte lzma winout
  flags += -DJMA_SUPPORT
endif

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
	$(c) -O3 -fomit-frame-pointer -static -Ilib -c $< -o $@
obj/libreader.o: lib/libreader/libreader.cpp lib/libreader/*
obj/libfilter.o: lib/libfilter/libfilter.cpp lib/libfilter/*

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

obj/adsp.o: dsp/adsp/adsp.cpp dsp/adsp/*
obj/sdsp.o: dsp/sdsp/sdsp.cpp dsp/sdsp/*

###########
### ppu ###
###########

obj/ppu.o : ppu/ppu.cpp ppu/*
obj/bppu.o: ppu/bppu/bppu.cpp ppu/bppu/*

##############
### system ###
##############

obj/system.o: system/system.cpp $(call rwildcard,system/)

#####################
### special chips ###
#####################

obj/sgb.o    : chip/sgb/sgb.cpp $(call rwildcard,chip/sgb/)
obj/superfx.o: chip/superfx/superfx.cpp $(call rwildcard,chip/superfx/)
obj/sa1.o    : chip/sa1/sa1.cpp $(call rwildcard,chip/sa1/)
obj/bsx.o    : chip/bsx/bsx.cpp chip/bsx/*
obj/srtc.o   : chip/srtc/srtc.cpp chip/srtc/*
obj/sdd1.o   : chip/sdd1/sdd1.cpp chip/sdd1/*
obj/spc7110.o: chip/spc7110/spc7110.cpp chip/spc7110/*
obj/cx4.o    : chip/cx4/cx4.cpp chip/cx4/*
obj/dsp1.o   : chip/dsp1/dsp1.cpp chip/dsp1/*
obj/dsp2.o   : chip/dsp2/dsp2.cpp chip/dsp2/*
obj/dsp3.o   : chip/dsp3/dsp3.cpp chip/dsp3/*
obj/dsp4.o   : chip/dsp4/dsp4.cpp chip/dsp4/*
obj/obc1.o   : chip/obc1/obc1.cpp chip/obc1/*
obj/st010.o  : chip/st010/st010.cpp chip/st010/*
obj/st011.o  : chip/st011/st011.cpp chip/st011/*
obj/st018.o  : chip/st018/st018.cpp chip/st018/*

############
### zlib ###
############

obj/adler32.o : lib/zlib/adler32.c lib/zlib/*
obj/compress.o: lib/zlib/compress.c lib/zlib/*
obj/crc32.o   : lib/zlib/crc32.c lib/zlib/*
obj/deflate.o : lib/zlib/deflate.c lib/zlib/*
obj/gzio.o    : lib/zlib/gzio.c lib/zlib/*
obj/inffast.o : lib/zlib/inffast.c lib/zlib/*
obj/inflate.o : lib/zlib/inflate.c lib/zlib/*
obj/inftrees.o: lib/zlib/inftrees.c lib/zlib/*
obj/ioapi.o   : lib/zlib/ioapi.c lib/zlib/*
obj/trees.o   : lib/zlib/trees.c lib/zlib/*
obj/unzip.o   : lib/zlib/unzip.c lib/zlib/*
obj/zip.o     : lib/zlib/zip.c lib/zlib/*
obj/zutil.o   : lib/zlib/zutil.c lib/zlib/*

##############
### libjma ###
##############

obj/jma.o    : lib/libjma/jma.cpp lib/libjma/*
obj/jcrc32.o : lib/libjma/jcrc32.cpp lib/libjma/*
obj/lzmadec.o: lib/libjma/lzmadec.cpp lib/libjma/*
obj/7zlzma.o : lib/libjma/7zlzma.cpp lib/libjma/*
obj/iiostrm.o: lib/libjma/iiostrm.cpp lib/libjma/*
obj/inbyte.o : lib/libjma/inbyte.cpp lib/libjma/*
obj/lzma.o   : lib/libjma/lzma.cpp lib/libjma/*
obj/winout.o : lib/libjma/winout.cpp lib/libjma/*

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

clean: ui_clean
	-@$(call delete,obj/*.o)
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

help:
	@echo "Usage: $(MAKE) platform=(os) compiler=(cc) [options]"
	@echo ""
	@echo "Supported platforms:"
	@echo "  x   - Linux / BSD (x86, x86-64)"
	@echo "  win - Windows (x86, x86-64)"
	@echo ""
	@echo "Supported compilers:"
	@echo "  gcc              - GCC compiler"
	@echo "  mingw32-gcc      - MinGW compiler"
	@echo "  i586-mingw32-gcc - MinGW cross compiler"
	@echo ""
	@echo "Available options:"
	@echo "  enable_gzip=[true|false] - Enable ZIP / GZ support (default=false)"
	@echo "  enable_jma=[true|false]  - Enable JMA support (default=false)"
	@echo ""
	@echo "Example: $(MAKE) platform=x compiler=gcc enable_gzip=true"
	@echo ""

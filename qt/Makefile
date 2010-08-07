qtlibs := $(strip QtCore QtGui $(if $(findstring osx,$(platform)),QtOpenGL))
include nall/qt/Makefile

ui_objects := ui-main ui-base ui-cartridge ui-debugger ui-input ui-movie ui-settings ui-state ui-tools
ui_objects += ruby
ui_objects += $(if $(call streq,$(platform),win),resource)
link += $(qtlib)

headers := $(call rwildcard,$(ui)/,%.hpp)
moc_headers := $(call rwildcard,nall/qt/,%.moc.hpp) $(call rwildcard,$(ui)/,%.moc.hpp)
moc_objects := $(foreach f,$(moc_headers),obj/$(notdir $(patsubst %.moc.hpp,%.moc,$f)))
qt_compile = $(call compile,-Iobj $(qtinc))

# platform
ifeq ($(platform),x)
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
  ruby := video.direct3d video.wgl video.directdraw video.gdi video.qtraster
  ruby += audio.directsound
  ruby += input.rawinput input.directinput

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
else
  unknown_platform: help;
endif

# ruby
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

rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)

# rules
objects := $(ui_objects) $(objects)

# automatically run moc on all .moc.hpp (MOC header) files
%.moc: $<; $(moc) -i $< -o $@

# automatically generate %.moc build rules
__list = $(moc_headers)
$(foreach f,$(moc_objects), \
  $(eval __file = $(word 1,$(__list))) \
  $(eval __list = $(wordlist 2,$(words $(__list)),$(__list))) \
  $(eval $f: $(__file)) \
)

obj/ui-main.o: $(ui)/main.cpp $(headers) $(wildcard $(ui)/*.cpp) $(wildcard $(ui)/link/*.cpp) $(wildcard $(ui)/platform/*.cpp) $(wildcard $(ui)/utility/*.cpp); $(qt_compile)
obj/ui-base.o: $(ui)/base/base.cpp $(headers) $(wildcard $(ui)/base/*.cpp); $(qt_compile)
obj/ui-cartridge.o: $(ui)/cartridge/cartridge.cpp $(headers) $(wildcard $(ui)/cartridge/*.cpp); $(qt_compile)
obj/ui-debugger.o: $(ui)/debugger/debugger.cpp $(headers) $(call rwildcard,$(ui)/debugger/,%.cpp); $(qt_compile)
obj/ui-input.o: $(ui)/input/input.cpp $(headers) $(wildcard $(ui)/input/*.cpp); $(qt_compile)
obj/ui-movie.o: $(ui)/movie/movie.cpp $(headers) $(wildcard $(ui)/movie/*.cpp); $(qt_compile)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(headers) $(wildcard $(ui)/settings/*.cpp); $(qt_compile)
obj/ui-state.o: $(ui)/state/state.cpp $(headers) $(wildcard $(ui)/state/*.cpp); $(qt_compile)
obj/ui-tools.o: $(ui)/tools/tools.cpp $(headers) $(wildcard $(ui)/tools/*.cpp); $(qt_compile)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubydef) $(rubyflags))

obj/resource.rcc: $(ui)/resource/resource.qrc $(ui)/data/*
	$(rcc) $(ui)/resource/resource.qrc -o obj/resource.rcc

obj/resource.o: $(ui)/resource/resource.rc
	windres $(ui)/resource/resource.rc obj/resource.o

# targets
ui_build: obj/resource.rcc $(moc_objects);
ui_clean:
	-@$(call delete,obj/*.rcc)
	-@$(call delete,obj/*.moc)

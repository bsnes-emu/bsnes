include $(gameboy)/Makefile

ui_objects := ui-main ui-utility
ui_objects += ruby phoenix

# platform
ifeq ($(platform),x)
# phoenix_compile = $(call compile,-DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`)
# link += `pkg-config --libs gtk+-2.0`

  phoenix_compile = $(call compile,-DPHOENIX_QT `pkg-config --cflags QtCore QtGui`)
  link += `pkg-config --libs QtCore QtGui`

  ruby := video.glx video.xv video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  phoenix_compile = $(call compile,-DPHOENIX_QT)
  link +=

  ruby :=
  ruby += audio.openal
  ruby += input.carbon

  link += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  phoenix_compile = $(call compile,-DPHOENIX_WINDOWS)
  link +=

  ruby := video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.directsound audio.xaudio2
  ruby += input.rawinput input.directinput

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
endif

# ruby
rubyflags := $(if $(finstring .sdl,$(ruby)),`sdl-config --cflags`)

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
link += $(if $(findstring audio.xaudio2,$(ruby)),-lole32)
link += $(if $(findstring input.directinput,$(ruby)),-ldinput8 -ldxguid)
link += $(if $(findstring input.rawinput,$(ruby)),-ldinput8 -ldxguid)

rubydef := $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ui-main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/*); $(phoenix_compile)
obj/ui-utility.o: $(ui)/utility/utility.cpp $(call rwildcard,$(ui)/utility/*); $(phoenix_compile)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubydef) $(rubyflags))

obj/phoenix.o: phoenix/phoenix.cpp $(call rwildcard,phoenix/*)
	$(phoenix_compile)

# targets
build: $(objects)
ifeq ($(platform),osx)
	test -d ../bgameboy.app || mkdir -p ../bgameboy.app/Contents/MacOS
	$(strip $(cpp) -o ../bgameboy.app/Contents/MacOS/bgameboy $(objects) $(link))
else
	$(strip $(cpp) -o out/bgameboy $(objects) $(link))
endif

install:
ifeq ($(platform),x)
	install -D -m 755 out/bgameboy $(DESTDIR)$(prefix)/bin/bgameboy
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/bgameboy
endif

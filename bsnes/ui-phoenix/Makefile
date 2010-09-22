ui_objects := ui-main ui-general ui-settings ui-utility ui-cartridge
ui_objects += ruby phoenix
ui_objects += $(if $(call streq,$(platform),win),resource)

# platform
ifeq ($(platform),x)
  flags += -DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`
  link += `pkg-config --libs gtk+-2.0`

  ruby := video.glx video.xv video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x

  link += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  ruby :=
  ruby += audio.openal
  ruby += input.carbon

  link += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  flags += -DPHOENIX_WINDOWS
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

obj/ui-main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/*)
obj/ui-general.o: $(ui)/general/general.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/general/*)
obj/ui-settings.o: $(ui)/settings/settings.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/settings/*)
obj/ui-utility.o: $(ui)/utility/utility.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/utility/*)
obj/ui-cartridge.o: $(ui)/cartridge/cartridge.cpp $(call rwildcard,$(ui)/*.hpp) $(call rwildcard,$(ui)/cartridge/*)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubydef) $(rubyflags))

obj/phoenix.o: phoenix/phoenix.cpp $(call rwildcard,phoenix/*)

obj/resource.o: $(ui)/resource.rc
	windres $(ui)/resource.rc obj/resource.o

# targets
ui_build:;

ui_clean:;

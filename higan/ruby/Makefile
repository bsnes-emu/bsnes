ifeq ($(platform),osx)
  rubyflags = $(objcppflags) $(flags)
else
  rubyflags = $(cppflags) $(flags)
endif

rubyflags += $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)
rubyflags += $(if $(finstring .sdl,$(ruby)),`sdl-config --cflags`)

rubylink =

rubylink += $(if $(findstring video.cgl,$(ruby)),-framework OpenGL)
rubylink += $(if $(findstring video.direct3d,$(ruby)),-ld3d9)
rubylink += $(if $(findstring video.directdraw,$(ruby)),-lddraw)
rubylink += $(if $(findstring video.glx,$(ruby)),-lGL)
rubylink += $(if $(findstring video.wgl,$(ruby)),-lopengl32)
rubylink += $(if $(findstring video.xv,$(ruby)),-lXv)

rubylink += $(if $(findstring audio.alsa,$(ruby)),-lasound)
rubylink += $(if $(findstring audio.ao,$(ruby)),-lao)
rubylink += $(if $(findstring audio.directsound,$(ruby)),-ldsound)
rubylink += $(if $(findstring audio.pulseaudio,$(ruby)),-lpulse)
rubylink += $(if $(findstring audio.pulseaudiosimple,$(ruby)),-lpulse-simple)
rubylink += $(if $(findstring audio.xaudio2,$(ruby)),-lole32)

rubylink += $(if $(findstring input.directinput,$(ruby)),-ldinput8 -ldxguid)
rubylink += $(if $(findstring input.rawinput,$(ruby)),-ldinput8 -ldxguid)

rubylink += $(if $(findstring .sdl,$(ruby)),`sdl-config --libs`)

ifeq ($(platform),x)
  rubylink += $(if $(findstring audio.openal,$(ruby)),-lopenal)
else ifeq ($(platform),osx)
  rubylink += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else ifeq ($(platform),win)
  rubylink += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
endif

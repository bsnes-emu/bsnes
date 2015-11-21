ifeq ($(platform),macosx)
  rubyflags = $(objcppflags) $(flags)
else
  rubyflags = $(cppflags) $(flags)
endif

rubyflags += $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)
rubyflags += $(if $(findstring .sdl,$(ruby)),$(shell sdl-config --cflags))

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
rubylink += $(if $(findstring audio.wasapi,$(ruby)),-lavrt -luuid)
rubylink += $(if $(findstring audio.xaudio2,$(ruby)),-lole32)

rubylink += $(if $(findstring input.udev,$(ruby)),-ludev)
rubylink += $(if $(findstring input.windows,$(ruby)),-ldinput8 -ldxguid)

rubylink += $(if $(findstring .sdl,$(ruby)),$(shell sdl-config --libs))

ifeq ($(platform),windows)
  rubylink += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
else ifeq ($(platform),macosx)
  rubylink += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
else
  rubylink += $(if $(findstring audio.openal,$(ruby)),-lopenal)
endif

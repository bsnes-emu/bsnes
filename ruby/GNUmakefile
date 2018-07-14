ifeq ($(platform),macos)
  ruby.flags := $(flags.objcpp)
else
  ruby.flags := $(flags.cpp)
endif

ruby.flags += $(foreach c,$(subst .,_,$(call strupper,$(ruby))),-D$c)
ruby.flags += $(if $(findstring input.sdl,$(ruby)),$(shell sdl2-config --cflags))

ruby.options :=

ruby.options += $(if $(findstring video.cgl,$(ruby)),-framework OpenGL)
ruby.options += $(if $(findstring video.direct3d,$(ruby)),-ld3d9)
ruby.options += $(if $(findstring video.directdraw,$(ruby)),-lddraw)
ruby.options += $(if $(findstring video.glx,$(ruby)),-lGL)
ruby.options += $(if $(findstring video.wgl,$(ruby)),-lopengl32)
ruby.options += $(if $(findstring video.xvideo,$(ruby)),-lXv)

ruby.options += $(if $(findstring audio.alsa,$(ruby)),-lasound)
ruby.options += $(if $(findstring audio.ao,$(ruby)),-lao)
ruby.options += $(if $(findstring audio.directsound,$(ruby)),-ldsound)
ruby.options += $(if $(findstring audio.pulseaudio,$(ruby)),-lpulse)
ruby.options += $(if $(findstring audio.pulseaudiosimple,$(ruby)),-lpulse-simple)
ruby.options += $(if $(findstring audio.wasapi,$(ruby)),-lavrt -luuid)
ruby.options += $(if $(findstring audio.xaudio2,$(ruby)),-lole32)

ruby.options += $(if $(findstring input.sdl,$(ruby)),$(shell sdl2-config --libs))
ruby.options += $(if $(findstring input.udev,$(ruby)),-ludev)
ruby.options += $(if $(findstring input.windows,$(ruby)),-ldinput8 -ldxguid)

ifeq ($(platform),windows)
  ruby.options += $(if $(findstring audio.openal,$(ruby)),-lopenal32)
endif

ifeq ($(platform),macos)
  ruby.options += $(if $(findstring audio.openal,$(ruby)),-framework OpenAL)
endif

ifeq ($(platform),linux)
  ruby.options += -lX11 -lXext
  ruby.options += $(if $(findstring audio.openal,$(ruby)),-lopenal)
endif

ifeq ($(platform),bsd)
  ruby.options += -lX11 -lXext
  ruby.options += $(if $(findstring audio.openal,$(ruby)),-lopenal)
endif

$(object.path)/ruby.o: $(ruby.path)/ruby.cpp $(call rwildcard,$(ruby.path)) $(call rwildcard,$(nall.path))
	$(info Compiling $< ...)
	@$(compiler) $(ruby.flags) $(flags) -c $< -o $@

options += debugger

include $(snes)/Makefile
name := laevateinn

ui_objects := ui-main ui-interface ui-debugger ui-tracer ui-console
ui_objects += ui-video ui-cpu ui-smp ui-memory ui-breakpoint
ui_objects += phoenix ruby
ui_objects += $(if $(call streq,$(platform),win),resource)

# platform
ifeq ($(platform),x)
  ruby := audio.alsa
else ifeq ($(platform),osx)
  ruby := audio.openal
else ifeq ($(platform),win)
  ruby := audio.xaudio2
endif

# phoenix
include phoenix/Makefile
link += $(phoenixlink)

# ruby
include ruby/Makefile
link += $(rubylink)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ui-main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/)
obj/ui-interface.o: $(ui)/interface/interface.cpp $(call rwildcard,$(ui)/*)
obj/ui-debugger.o: $(ui)/debugger/debugger.cpp $(call rwildcard,$(ui)/*)
obj/ui-tracer.o: $(ui)/tracer/tracer.cpp $(call rwildcard,$(ui)/*)
obj/ui-console.o: $(ui)/console/console.cpp $(call rwildcard,$(ui)/*)
obj/ui-cpu.o: $(ui)/cpu/cpu.cpp $(call rwildcard,$(ui)/*)
obj/ui-smp.o: $(ui)/smp/smp.cpp $(call rwildcard,$(ui)/*)
obj/ui-video.o: $(ui)/video/video.cpp $(call rwildcard,$(ui)/*)
obj/ui-memory.o: $(ui)/memory/memory.cpp $(call rwildcard,$(ui)/*)
obj/ui-breakpoint.o: $(ui)/breakpoint/breakpoint.cpp $(call rwildcard,$(ui)/*)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(call compile,$(rubyflags))

obj/phoenix.o: phoenix/phoenix.cpp $(call rwildcard,phoenix/*)
	$(call compile,$(phoenixflags))

obj/resource.o: $(ui)/resource.rc
#	windres --target=pe-i386 $(ui)/resource.rc obj/resource.o
	windres $(ui)/resource.rc obj/resource.o

# targets
build: $(objects)
ifeq ($(platform),osx)
	test -d ../$(name).app || mkdir -p ../$(name).app/Contents/MacOS
	$(strip $(cpp) -o ../$(name).app/Contents/MacOS/$(name) $(objects) $(link))
else
	$(strip $(cpp) -o out/$(name) $(objects) $(link))
endif

install:
ifeq ($(platform),x)
	install -D -m 755 out/$(name) $(DESTDIR)$(prefix)/bin/$(name)
	mkdir -p ~/.config/$(name)
#	install -D -m 644 data/$(name).png $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
#	install -D -m 644 data/$(name).desktop $(DESTDIR)$(prefix)/share/applications/$(name).desktop
	cp data/cheats.xml ~/.config/$(name)/cheats.xml
	chmod 777 ~/.config/$(name) ~/.config/$(name)/cheats.xml
endif

uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/bin/$(name)
#	rm $(DESTDIR)$(prefix)/share/pixmaps/$(name).png
#	rm $(DESTDIR)$(prefix)/share/applications/$(name).desktop
endif

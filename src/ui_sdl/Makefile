objects := ui-main libsnes $(objects)
link += `sdl-config --libs`

obj/ui-main.o: $(ui)/main.cpp
	$(call compile,`sdl-config --cflags`)
obj/libsnes.o: snes/libsnes/libsnes.cpp snes/libsnes/*

ui_build:;
ui_clean:;

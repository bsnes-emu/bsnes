#define INTERFACE_MAIN

#include "../base.h"
#include "main.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * hardware abstraction layer
 *****/

#include "video/video.cpp"
#include "audio/audio.cpp"
#include "input/input.cpp"

#include "inputmgr.cpp"
#include "interface.cpp"

/*****
 * platform abstraction layer
 *****/

#if defined(UI_LUI)
  #include "lui/main.cpp"
#elif defined(UI_WIN)
  #include "win/main.cpp"
#elif defined(UI_SDL)
  #include "sdl/main.cpp"
#else
  #error "unsupported platform"
#endif

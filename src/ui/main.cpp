#define INTERFACE_MAIN

#include "../base.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * hardware abstraction layer
 *****/

#include "vai/video.h"
#include "vai/audio.h"
#include "vai/input.h"

Video *uiVideo;
Audio *uiAudio;
Input *uiInput;

#include "inputmgr.cpp"
#include "interface.cpp"

/*****
 * platform abstraction layer
 *****/

#if defined(UI_MIU)
  #include "../lib/miu.h"
  using namespace ns_miu;
  #include "miu/main.cpp"
#else
  #error "unsupported platform"
#endif

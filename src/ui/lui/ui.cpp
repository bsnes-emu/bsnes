#include "ui_main.cpp"
#include "ui_about.cpp"
#include "settings/ui_settings.cpp"

#if defined(PLATFORM_WIN)
  #include "../video/d3d.h"
  #include "../video/d3d.cpp"
  #include "../video/ddraw.h"
  #include "../video/ddraw.cpp"
  #include "../audio/dsound.h"
  #include "../audio/dsound.cpp"
  #include "../input/dinput.h"
  #include "../input/dinput.cpp"
#elif defined(PLATFORM_X)
  #include "../video/xv.h"
  #include "../video/xv.cpp"
  #include "../video/sdl.h"
  #include "../video/sdl.cpp"
  #include "../video/gtk.h"
  #include "../video/gtk.cpp"
  #include "../audio/ao.h"
  #include "../audio/ao.cpp"
#endif

void ui_init() {
  window_main.setup();
  window_about.setup();
  window_settings.setup();

  window_main.show();
  while(ui::events_pending())ui::run();

#if defined(PLATFORM_WIN)
  uiVideo = new VideoD3D(window_main.canvas.handle());
  uiAudio = new AudioDS(window_main.handle());
  uiInput = new InputDI(window_main.handle());
#elif defined(PLATFORM_X)
  uiVideo = new VideoXv((unsigned long)window_main.canvas.handle());
  uiAudio = new AudioAO();
  ((AudioAO*)uiAudio)->set_driver("oss");
  uiInput = new Input();
#endif

  uiVideo->init();
  uiAudio->init();
  uiInput->init();
}

void ui_term() {
  window_main.hide();

  uiVideo->term();
  uiAudio->term();
  uiInput->term();

  safe_delete(uiVideo);
  safe_delete(uiAudio);
  safe_delete(uiInput);
}

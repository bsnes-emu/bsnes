#include "ui_main.cpp"
#include "ui_about.cpp"

#include "settings/ui_settings.cpp"
#include "settings/ui_rastersettings.cpp"
#include "settings/ui_inputconfig.cpp"
#include "settings/ui_cheateditor.cpp"
#include "settings/ui_advanced.cpp"

#if defined(PLATFORM_WIN)
  #include "../video/d3d.h"
  #include "../video/d3d.cpp"
  #include "../video/ddraw.h"
  #include "../video/ddraw.cpp"
  #include "../video/gdi.cpp"
  #include "../audio/dsound.h"
  #include "../audio/dsound.cpp"
  #include "../input/dinput.cpp"
#elif defined(PLATFORM_X)
  #include "../video/xv.cpp"
  #include "../video/gtk.cpp"
  #include "../audio/ao.cpp"
#endif

#include "../input/inputui.cpp"

void ui_init() {
  window_main.setup();
  window_about.setup();

  window_raster_settings.setup();
  window_input_config.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_video_settings();
  window_main.show();
  while(ui::events_pending()) { ui::run(); }

#if defined(PLATFORM_WIN)
  uiVideo =
    config::system.video == "none" ? (Video*)new Video() :
    config::system.video == "gdi"  ? (Video*)new VideoGDI(window_main.view.handle()) :
    config::system.video == "dd"   ? (Video*)new VideoDD(window_main.view.handle()) :
                                     (Video*)new VideoD3D(window_main.view.handle());
  uiAudio =
    config::system.audio == "none" ? (Audio*)new Audio() :
                                     (Audio*)new AudioDS(window_main.handle());
  uiInput =
    config::system.input == "none" ? (Input*)new Input() :
    config::system.input == "ui"   ? (Input*)new InputUI() :
                                     (Input*)new InputDI(window_main.handle());
#elif defined(PLATFORM_X)
  uiVideo =
    config::system.video == "none" ? (Video*)new Video() :
    config::system.video == "gtk"  ? (Video*)new VideoGTK(window_main.view.handle()) :
                                     (Video*)new VideoXv((unsigned long)window_main.view.x_handle());
  uiAudio =
    config::system.audio == "none" ? (Audio*)new Audio() :
                                     (Audio*)new AudioAO(config::system.audio_flags);
  uiInput =
    config::system.input == "none" ? (Input*)new Input() :
                                     (Input*)new InputUI();
#endif

  uiVideo->init();
  uiAudio->init();
  uiInput->init();

  window_main.setup_menu();
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

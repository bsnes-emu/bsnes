#include "ui_main.cpp"
#include "ui_about.cpp"

#include "loader/ui_bsxloader.cpp"
#include "loader/ui_stloader.cpp"

#include "settings/ui_settings.cpp"
#include "settings/ui_videosettings.cpp"
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
  #include "../input/xinput.cpp"
#endif

void ui_init() {
  window_main.setup();
  window_about.setup();

  window_bsxloader.setup();
  window_stloader.setup();

  window_video_settings.setup();
  window_raster_settings.setup();
  window_input_config.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_video_settings(); //call first time to resize main window and update menubar
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
                                     (Input*)new InputX();
#endif

  uiVideo->init();
  uiAudio->init();
  uiInput->init();

  event::update_video_settings(); //call second time to update uiVideo->settings
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

#include "ui_main.cpp"
#include "ui_about.cpp"

#include "loader/ui_bsxloader.cpp"
#include "loader/ui_stloader.cpp"

#include "settings/ui_settings.cpp"
#include "settings/ui_rastersettings.cpp"
#include "settings/ui_inputconfig.cpp"
#include "settings/ui_cheateditor.cpp"
#include "settings/ui_advanced.cpp"

#if defined(PLATFORM_WIN)
  #include "../vai/video/video.direct3d.h"
  #include "../vai/video/video.directdraw.h"
  #include "../vai/video/video.gdi.h"
  #include "../vai/audio/audio.directsound.h"
  #include "../vai/input/input.directinput.h"
#elif defined(PLATFORM_X)
  #include "../vai/video/video.xv.h"
  #include "../vai/video/video.gtk.h"
  #include "../vai/audio/audio.ao.h"
  #include "../vai/input/input.x.h"
#endif

void ui_init() {
  window_main.setup();
  window_about.setup();

  window_bsxloader.setup();
  window_stloader.setup();

  window_raster_settings.setup();
  window_input_config.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_video_settings(); //call first time to resize main window and update menubar
  window_main.show();
  while(miu().pending()) miu().run();

#if defined(PLATFORM_WIN)
  if(config::system.video == "none") {
    uiVideo = new Video();
  } else if(config::system.video == "gdi") {
    uiVideo = new VideoGDI();
  } else if(config::system.video == "directdraw") {
    uiVideo = new VideoDD();
  } else {
    uiVideo = new VideoD3D();
  }

  if(config::system.audio == "none") {
    uiAudio = new Audio();
  } else {
    uiAudio = new AudioDS();
  }

  if(config::system.input == "none") {
    uiInput = new Input();
  } else {
    uiInput = new InputDI();
  }
#elif defined(PLATFORM_X)
  if(config::system.video == "none") {
    uiVideo = new Video();
  } else if(config::system.video == "gtk") {
    uiVideo = new VideoGTK();
  } else {
    uiVideo = new VideoXv();
  }

  if(config::system.audio == "none") {
    uiAudio = new Audio();
  } else {
    uiAudio = new AudioAO();
  }

  if(config::system.input == "none") {
    uiInput = new Input();
  } else {
    uiInput = new InputX();
  }
#endif

//needed only by VideoGDI (default is RGB565)
  if(config::system.video == "gdi") snes.set_video_pixel_format(SNES::PIXELFORMAT_RGB555);

  uiVideo->set(Video::Handle, window_main.view.handle());
  uiVideo->set(Video::Synchronize, false);
  uiAudio->set(Audio::Handle, window_main.handle());
  uiAudio->set(Audio::Synchronize, config::system.regulate_speed);
  uiAudio->set(Audio::Frequency, 32000);
  uiInput->set(Input::Handle, window_main.handle());

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

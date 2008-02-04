#include "ui_main.cpp"
#include "ui_about.cpp"
#include "ui_message.cpp"

#include "loader/ui_bsxloader.cpp"
#include "loader/ui_stloader.cpp"

#include "settings/ui_settings.cpp"
#include "settings/ui_rastersettings.cpp"
#include "settings/ui_inputconfig.cpp"
#include "settings/ui_cheateditor.cpp"
#include "settings/ui_advanced.cpp"

void ui_init() {
  window_main.setup();
  window_about.setup();
  window_message.setup();

  window_bsxloader.setup();
  window_stloader.setup();

  window_raster_settings.setup();
  window_input_config.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_video_settings(); //call first time to resize main window and update menubar
  window_main.show();
  while(hiro().pending()) hiro().run();

  //needed only by VideoGDI (default is RGB565)
  if(config::system.video == "gdi") snes.set_video_pixel_format(SNES::PIXELFORMAT_RGB555);

  video.driver(config::system.video);
  audio.driver(config::system.audio);
  input.driver(config::system.input);

  video.set(Video::Handle, window_main.view.handle());
  video.set(Video::Synchronize, false);
  audio.set(Audio::Handle, window_main.handle());
  audio.set(Audio::Synchronize, (bool)config::system.regulate_speed);
  audio.set(Audio::Frequency, 32000);
  input.set(Input::Handle, window_main.handle());

  video.init();
  audio.init();
  input.init();

  event::update_video_settings(); //call second time to update uiVideo->settings

  //UI setup complete, hook keyboard callbacks
  snesinterface.input_ready = bind(&MainWindow::input_ready, &window_main);
  input_manager.on_keydown = bind(&event::keydown);
  input_manager.on_keyup = bind(&event::keyup);
}

void ui_term() {
  window_main.hide();

  video.term();
  audio.term();
  input.term();
}

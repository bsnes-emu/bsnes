#include "resource.cpp"

#include "base/main.cpp"
#include "base/about.cpp"

#include "loader/bsxloader.cpp"
#include "loader/stloader.cpp"

#include "settings/settings.cpp"
#include "settings/videosettings.cpp"
#include "settings/inputconfig.cpp"
#include "settings/pathsettings.cpp"
#include "settings/cheateditor.cpp"
#include "settings/advanced.cpp"

void ui_init() {
  hiro().disable_screensaver();
  resource::init();

  window_main.setup();
  window_about.setup();

  window_bsxloader.setup();
  window_stloader.setup();

  window_video_settings.setup();
  window_input_config.setup();
  window_path_settings.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_opacity();
  event::update_video_settings(); //call first time to resize main window and update menubar
  window_main.show();
  while(hiro().pending()) hiro().run();

  video.driver(config::system.video);
  audio.driver(config::system.audio);
  input.driver(config::system.input);

  video.set(Video::Handle, window_main.view.handle());
  video.set(Video::Synchronize, false);
  audio.set(Audio::Handle, window_main.handle());
  input.set(Input::Handle, window_main.handle());
  input.set(Input::AnalogAxisResistance, config::input.analog_axis_resistance);
  //sets Audio::Synchronize and Audio::Frequency
  event::update_emulation_speed(config::system.emulation_speed);

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

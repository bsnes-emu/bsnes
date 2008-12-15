#include "resource.cpp"

#include "base/main.cpp"
#include "base/textview.cpp"
#include "base/about.cpp"

#include "loader/bsxloader.cpp"
#include "loader/stloader.cpp"

#include "settings/settings.cpp"
#include "settings/driverselect.cpp"
#include "settings/videosettings.cpp"
#include "settings/audiosettings.cpp"
#include "settings/inputconfig.cpp"
#include "settings/pathsettings.cpp"
#include "settings/cheateditor.cpp"
#include "settings/advanced.cpp"

void ui_init() {
  hiro().disable_screensaver();
  resource::init();

  //ruby can initialize with "", but driver selection window needs active driver name
  if(config::system.video == "") config::system.video = video.default_driver();
  if(config::system.audio == "") config::system.audio = audio.default_driver();
  if(config::system.input == "") config::system.input = input.default_driver();

  if(config::system.invoke_crash_handler == false) {
    video.driver(config::system.video);
    audio.driver(config::system.audio);
    input.driver(config::system.input);
  }

  window_main.setup();
  window_textview.setup();
  window_about.setup();

  window_bsxloader.setup();
  window_stloader.setup();

  window_driver_select.setup();
  window_video_settings.setup();
  window_audio_settings.setup();
  window_input_config.setup();
  window_path_settings.setup();
  window_cheat_editor.setup();
  window_advanced.setup();
  window_settings.setup();

  event::update_opacity();
  event::update_video_settings(); //call first time to resize main window and update menubar

  if(config::system.invoke_crash_handler == true) {
    //application crashed during driver setup on last run ...
    //allow selection of new drivers, and then exit emulator.
    window_settings.show();
    while(window_settings.visible()) {
      while(hiro().pending()) hiro().run();
      usleep(20 * 1000);
    }
    config::system.invoke_crash_handler = false;
    event::quit();
    return;
  }

  window_main.show();
  while(hiro().pending()) hiro().run();

  //detect crashes during driver initialization
  config::system.invoke_crash_handler = true;
  config::config().save(config::bsnes_cfg);

  video.set(Video::Handle, window_main.view.handle());
  video.set(Video::Synchronize, config::video.synchronize);
  audio.set(Audio::Handle, window_main.handle());
  audio.set(Audio::Synchronize, config::audio.synchronize);
  audio.set(Audio::Volume, config::audio.volume);
  audio.set(Audio::Frequency, config::audio.output_frequency);
  audio.set(Audio::Latency, config::audio.latency);
  input.set(Input::Handle, window_main.handle());
  input.set(Input::AnalogAxisResistance, config::input.analog_axis_resistance);

  //sets Audio::Resample, Audio::ResampleOutputFrequency and Audio::ResampleInputFrequency
  event::update_emulation_speed(config::system.emulation_speed);

  video.init();
  audio.init();
  input.init();

  video.clear();
  audio.clear();

  //if code has reached this point, driver initialized successfully
  config::system.invoke_crash_handler = false;
  config::config().save(config::bsnes_cfg);

  event::update_video_settings(); //call second time to update video class settings

  //UI setup complete, hook input callbacks
  snesinterface.input_ready = bind(&MainWindow::input_ready, &window_main);
  input_manager.on_input = bind(&event::input_event);
}

void ui_term() {
  window_main.hide();

  video.term();
  audio.term();
  input.term();
}

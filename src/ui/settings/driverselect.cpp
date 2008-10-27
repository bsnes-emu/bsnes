uintptr_t DriverSelectWindow::video_change(event_t) {
  lstring part;
  split(part, ";", video.driver_list());
  config::system.video = part[cvideo.get_selection()];
  return true;
}

uintptr_t DriverSelectWindow::audio_change(event_t) {
  lstring part;
  split(part, ";", audio.driver_list());
  config::system.audio = part[caudio.get_selection()];
  return true;
}

uintptr_t DriverSelectWindow::input_change(event_t) {
  lstring part;
  split(part, ";", input.driver_list());
  config::system.input = part[cinput.get_selection()];
  return true;
}

void DriverSelectWindow::setup() {
  create(0, 475, 355);

  //this is only displayed if application crashed on last run
  crash_message.create(0, 475, 36, string()
    << translate["Warning: bsnes crashed on last startup due to incompatible driver settings."] << "\n"
    << translate["Please select a different driver configuration below."]
  );

  lstring part;

  lvideo.create(0, 155, 18, translate["Video driver:"]);
  cvideo.create(0, 155, 25);

  split(part, ";", video.driver_list());
  for(unsigned i = 0; i < count(part); i++) {
    cvideo.add_item(translate[string() << "{{videodriver}}" << part[i]]);
    if(part[i] == config::system.video) cvideo.set_selection(i);
  }

  laudio.create(0, 155, 18, translate["Audio driver:"]);
  caudio.create(0, 155, 25);

  split(part, ";", audio.driver_list());
  for(unsigned i = 0; i < count(part); i++) {
    caudio.add_item(translate[string() << "{{audiodriver}}" << part[i]]);
    if(part[i] == config::system.audio) caudio.set_selection(i);
  }

  linput.create(0, 155, 18, translate["Input driver:"]);
  cinput.create(0, 155, 25);

  split(part, ";", input.driver_list());
  for(unsigned i = 0; i < count(part); i++) {
    cinput.add_item(translate[string() << "{{inputdriver}}" << part[i]]);
    if(part[i] == config::system.input) cinput.set_selection(i);
  }

  video_caps.create(0, 475, 18);

  video_sync.create(0, 155, 18, translate["{{video}}Synchronize"]);
  video_sync.disable();
  video_filter.create(0, 155, 18, translate["Hardware filtering"]);
  video_filter.disable();

  audio_caps.create(0, 475, 18);

  audio_sync.create(0, 155, 18, translate["{{audio}}Synchronize"]);
  audio_sync.disable();
  audio_freq.create(0, 155, 18, translate["Frequency control"]);
  audio_freq.disable();
  audio_latency.create(0, 155, 18, translate["Latency control"]);
  audio_latency.disable();

  input_caps.create(0, 475, 18);

  input_keyboard.create(0, 155, 18, translate["Keyboard support"]);
  input_keyboard.disable();
  input_mouse.create(0, 155, 18, translate["Mouse support"]);
  input_mouse.disable();
  input_joypad.create(0, 155, 18, translate["Joypad support"]);
  input_joypad.disable();

  restart_message.create(0, 475, 36,
    translate["Note: bsnes must be restarted for changes to take effect."]);

  bool crashed = config::system.invoke_crash_handler;

  string t = translate["Capabilities of active video driver ($):"];
  replace(t, "$", config::system.video);
  video_caps.set_text(t);

  video_sync.check(video.cap(Video::Synchronize));
  video_filter.check(video.cap(Video::Filter));

  t = translate["Capabilities of active audio driver ($):"];
  replace(t, "$", config::system.audio);
  audio_caps.set_text(t);

  audio_sync.check(audio.cap(Audio::Synchronize));
  audio_freq.check(audio.cap(Audio::Frequency));
  audio_latency.check(audio.cap(Audio::Latency));

  t = translate["Capabilities of active input driver ($):"];
  replace(t, "$", config::system.input);
  input_caps.set_text(t);

  input_keyboard.check(input.cap(Input::KeyboardSupport));
  input_mouse.check(input.cap(Input::MouseSupport));
  input_joypad.check(input.cap(Input::JoypadSupport));

  unsigned y = 0;

  if(crashed == true) {
    attach(crash_message, 0, y); y += 36 + 5;
  }

  attach(lvideo,   0, y);
  attach(laudio, 160, y);
  attach(linput, 320, y); y += 18;

  attach(cvideo,   0, y);
  attach(caudio, 160, y);
  attach(cinput, 320, y); y += 25 + 5;

  if(crashed == false) {
    attach(video_caps,       0, y); y += 18;
    attach(video_sync,       0, y);
    attach(video_filter,   160, y); y += 18 + 5;

    attach(audio_caps,       0, y); y += 18;
    attach(audio_sync,       0, y);
    attach(audio_freq,     160, y);
    attach(audio_latency,  320, y); y += 18 + 5;

    attach(input_caps,       0, y); y += 18;
    attach(input_keyboard,   0, y);
    attach(input_mouse,    160, y);
    attach(input_joypad,   320, y); y += 18 + 5;

    attach(restart_message,  0, y); y += 36 + 5;
  }

  cvideo.on_change = bind(&DriverSelectWindow::video_change, this);
  caudio.on_change = bind(&DriverSelectWindow::audio_change, this);
  cinput.on_change = bind(&DriverSelectWindow::input_change, this);
}

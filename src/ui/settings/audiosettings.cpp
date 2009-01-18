uintptr_t AudioSettingsWindow::volume_change(event_t) {
  config.audio.volume = 25 + cvolume.get_selection() * 25;
  audio.set(Audio::Volume, config.audio.volume);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::latency_change(event_t) {
  config.audio.latency = 20 + clatency.get_selection() * 20;
  audio.set(Audio::Latency, config.audio.latency);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::frequency_change(event_t) {
  switch(cfrequency.get_selection()) {
    case 0: config.audio.output_frequency = 32000; break;
    case 1: config.audio.output_frequency = 44100; break; default:
    case 2: config.audio.output_frequency = 48000; break;
    case 3: config.audio.output_frequency = 96000; break;
  }

  audio.set(Audio::Frequency, config.audio.output_frequency);
  event::update_emulation_speed(config.system.emulation_speed);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::skew_change(event_t) {
  config.audio.input_frequency = sskew.get_position() + 32000 - 200;
  event::update_emulation_speed(config.system.emulation_speed);

  sync_ui();
  return true;
}

void AudioSettingsWindow::sync_ui() {
  cvolume.set_selection((config.audio.volume - 25) / 25);

  unsigned position;
       if(config.audio.output_frequency <= 32000) position = 0;
  else if(config.audio.output_frequency <= 44100) position = 1;
  else if(config.audio.output_frequency <= 48000) position = 2;
  else position = 3;
  cfrequency.set_selection(position);

  clatency.set_selection((config.audio.latency - 20) / 20);

  lskew.set_text(string() << translate["{{audio}}Frequency adjust:"] << " " << (int)config.audio.input_frequency << "hz");
  sskew.set_position(config.audio.input_frequency - 32000 + 200);
}

void AudioSettingsWindow::setup() {
  create(0, 451, 370);

  lvolume.create(0, 147, 18, translate["{{audio}}Volume:"]);
  cvolume.create(0, 147, 25);
  cvolume.add_item( "25%");
  cvolume.add_item( "50%");
  cvolume.add_item( "75%");
  cvolume.add_item("100%");
  if(config.misc.show_advanced_options == true) {
    cvolume.add_item("125%");
    cvolume.add_item("150%");
    cvolume.add_item("175%");
    cvolume.add_item("200%");
  }

  lfrequency.create(0, 147, 18, translate["{{audio}}Frequency:"]);
  cfrequency.create(0, 147, 25);
  cfrequency.add_item("32Khz");
  cfrequency.add_item("44.1Khz");
  cfrequency.add_item("48Khz");
  if(config.misc.show_advanced_options == true) {
    cfrequency.add_item("96Khz");
  }

  llatency.create(0, 147, 18, translate["{{audio}}Latency:"]);
  clatency.create(0, 147, 25);
  clatency.add_item( "20ms");
  clatency.add_item( "40ms");
  clatency.add_item( "60ms");
  clatency.add_item( "80ms");
  clatency.add_item("100ms");
  clatency.add_item("120ms");
  if(config.misc.show_advanced_options == true) {
    clatency.add_item("140ms");
    clatency.add_item("160ms");
  }

  lskew.create(0, 451, 18, translate["{{audio}}Frequency adjust:"]);
  sskew.create(0, 451, 30, 401);

  note.create(0, 451, 54, string()
  << translate["{{audio}}Frequency adjust is used to improve video sync timing."] << "\n"
  << translate["{{audio}}Lower value to clean audio output."] << "\n"
  << translate["{{audio}}Raise value to smooth video output."]
  );

  unsigned y = 0;

  attach(lvolume,      0, y);
  attach(lfrequency, 152, y);
  attach(llatency,   304, y); y += 18;

  attach(cvolume,      0, y);
  attach(cfrequency, 152, y);
  attach(clatency,   304, y); y += 30;

  attach(lskew,        0, y); y += 18;
  attach(sskew,        0, y); y += 30;

  attach(note,         0, y);

  cvolume.on_change    = bind(&AudioSettingsWindow::volume_change,    this);
  cfrequency.on_change = bind(&AudioSettingsWindow::frequency_change, this);
  clatency.on_change   = bind(&AudioSettingsWindow::latency_change,   this);
  sskew.on_change      = bind(&AudioSettingsWindow::skew_change,      this);

  sync_ui();
}

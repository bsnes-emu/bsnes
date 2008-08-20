uintptr_t AudioSettingsWindow::volume_change(event_t) {
  config::audio.volume = 10 + volume.get_position();
  audio.set(Audio::Volume, config::audio.volume);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::latency_change(event_t) {
  config::audio.latency = 25 + latency.get_position() * 5;
  audio.set(Audio::Latency, config::audio.latency);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::outputfrequency_change(event_t) {
  switch(outputfrequency.get_position()) { default:
    case 0: config::audio.output_frequency = 32000; break;
    case 1: config::audio.output_frequency = 44100; break;
    case 2: config::audio.output_frequency = 48000; break;
    case 3: config::audio.output_frequency = 96000; break;
  }

  audio.set(Audio::Frequency, config::audio.output_frequency);
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::inputfrequency_change(event_t) {
  config::audio.input_frequency = inputfrequency.get_position() + 32000 - 200;
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

void AudioSettingsWindow::sync_ui() {
  lvolume.set_text(string()
    << translate["Volume:"] << " "
    << (int)config::audio.volume << "%"
  );
  volume.set_position(config::audio.volume - 10);

  llatency.set_text(string()
    << translate["Latency:"] << " "
    << (int)config::audio.latency << "ms"
  );
  latency.set_position((config::audio.latency - 25) / 5);

  loutputfrequency.set_text(string()
    << translate["PC output frequency:"] << " "
    << (int)config::audio.output_frequency << "hz"
  );
  unsigned position;
       if(config::audio.output_frequency <= 32000) position = 0;
  else if(config::audio.output_frequency <= 44100) position = 1;
  else if(config::audio.output_frequency <= 48000) position = 2;
  else                                             position = 3;
  outputfrequency.set_position(position);

  int input_freq = config::audio.input_frequency - 32000;
  string adjust;
  if(input_freq > 0) adjust << "+";
  adjust << input_freq;
  if(config::advanced.enable == true) {
    adjust << " (" << (int)config::audio.input_frequency << "hz)";
  }
  linputfrequency.set_text(string()
    << translate["SNES input frequency adjust:"] << " "
    << adjust
  );
  inputfrequency.set_position(input_freq + 200);
}

void AudioSettingsWindow::setup() {
  create(0, 475, 355);

  lvolume.create(0, 475, 18);
  //if advanced UI mode is disabled, volume range is 10% - 100%
  //if enabled, volume range is 10% - 200%
  //101%+ is hidden by default, as it can cause audio quality degradation due to clamping
  volume.create(0, 475, 30, (config::advanced.enable == false ? 91 : 191));

  llatency.create(0, 475, 18);
  latency.create(0, 475, 30, 31);

  loutputfrequency.create(0, 475, 18);
  outputfrequency.create(0, 475, 30, 4);

  linputfrequency.create(0, 475, 18);
  inputfrequency.create(0, 475, 30, 401);

  unsigned y = 0;
  attach(lvolume, 0, y); y += 18;
  attach(volume,  0, y); y += 30;
  if(audio.cap(Audio::Latency) == true) {
    attach(llatency, 0, y); y += 18;
    attach(latency,  0, y); y += 30;
  }
  if(audio.cap(Audio::Frequency) == true) {
    attach(loutputfrequency, 0, y); y += 18;
    attach(outputfrequency,  0, y); y += 30;
  }
  attach(linputfrequency, 0, y); y += 18;
  attach(inputfrequency,  0, y); y += 30;

  volume.on_change = bind(&AudioSettingsWindow::volume_change, this);
  latency.on_change = bind(&AudioSettingsWindow::latency_change, this);
  outputfrequency.on_change = bind(&AudioSettingsWindow::outputfrequency_change, this);
  inputfrequency.on_change = bind(&AudioSettingsWindow::inputfrequency_change, this);

  sync_ui();
}

uintptr_t AudioSettingsWindow::volume_change(event_t) {
  config::audio.volume = 10 + svolume.get_position();
  audio.set(Audio::Volume, config::audio.volume);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::latency_change(event_t) {
  config::audio.latency = 25 + slatency.get_position() * 5;
  audio.set(Audio::Latency, config::audio.latency);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::output_change(event_t) {
  switch(soutput.get_position()) {
    case 0: config::audio.output_frequency = 32000; break;
    case 1: config::audio.output_frequency = 44100; break; default:
    case 2: config::audio.output_frequency = 48000; break;
  }

  audio.set(Audio::Frequency, config::audio.output_frequency);
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::input_change(event_t) {
  config::audio.input_frequency = sinput.get_position() + 32000 - 200;
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::volume_tick(event_t) {
  char t[256];
  evolume.get_text(t, sizeof t);
  config::audio.volume = strdec(t);
  audio.set(Audio::Volume, config::audio.volume);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::latency_tick(event_t) {
  char t[256];
  elatency.get_text(t, sizeof t);
  config::audio.latency = strdec(t);
  audio.set(Audio::Latency, config::audio.latency);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::output_tick(event_t) {
  char t[256];
  eoutput.get_text(t, sizeof t);
  config::audio.output_frequency = strdec(t);
  audio.set(Audio::Frequency, config::audio.output_frequency);
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

uintptr_t AudioSettingsWindow::input_tick(event_t) {
  char t[256];
  einput.get_text(t, sizeof t);
  config::audio.input_frequency = strdec(t);
  event::update_emulation_speed(config::system.emulation_speed);

  sync_ui();
  return true;
}

void AudioSettingsWindow::sync_ui() {
  lvolume.set_text(string()
    << translate["Volume:"] << " "
    << (int)config::audio.volume << "%"
  );

  llatency.set_text(string()
    << translate["Latency:"] << " "
    << (int)config::audio.latency << "ms"
  );

  loutput.set_text(string()
    << translate["Frequency:"] << " "
    << (int)config::audio.output_frequency << "hz"
  );

  if(config::advanced.enable == false) {
    int input_freq = config::audio.input_frequency - 32000;
    string adjust;
    if(input_freq > 0) adjust << "+";
    adjust << input_freq << "hz";
    linput.set_text(string()
      << translate["Frequency adjust:"] << " "
      << adjust
    );
  } else {
    linput.set_text(string() << translate["Frequency adjust:"] << " " << (int)config::audio.input_frequency << "hz");
  }

  if(config::advanced.enable == false) {
    svolume.set_position(config::audio.volume - 10);
    slatency.set_position((config::audio.latency - 25) / 5);
    unsigned position;
         if(config::audio.output_frequency <= 32000) position = 0;
    else if(config::audio.output_frequency <= 44100) position = 1;
    else position = 2;
    soutput.set_position(position);
    sinput.set_position(config::audio.input_frequency - 32000 + 200);
  } else {
    evolume.set_text(string() << (int)config::audio.volume);
    elatency.set_text(string() << (int)config::audio.latency);
    eoutput.set_text(string() << (int)config::audio.output_frequency);
    einput.set_text(string() << (int)config::audio.input_frequency);
  }
}

void AudioSettingsWindow::setup() {
  create(0, 475, 355);

  lvolume.create(0, 235, 18);
  svolume.create(0, 475, 30, 91);
  evolume.create(0, 130, 25);
  bvolume.create(0, 100, 25, translate["{{audio}}Set"]);

  llatency.create(0, 235, 18);
  slatency.create(0, 475, 30, 26);
  elatency.create(0, 130, 25);
  blatency.create(0, 100, 25, translate["{{audio}}Set"]);

  loutput.create(0, 235, 18);
  soutput.create(0, 475, 30, 3);
  eoutput.create(0, 130, 25);
  boutput.create(0, 100, 25, translate["{{audio}}Set"]);

  linput.create(0, 235, 18);
  sinput.create(0, 475, 30, 401);
  einput.create(0, 130, 25);
  binput.create(0, 100, 25, translate["{{audio}}Set"]);

  note.create(0, 475, 54, string()
  << translate["{{audio}}Frequency adjust is used to improve video sync timing."] << "\n"
  << translate["{{audio}}Lower value to clean audio output."] << "\n"
  << translate["{{audio}}Raise value to smooth video output."]
  );

  unsigned y = 0;
  if(config::advanced.enable == false) {
    attach(lvolume, 0, y); y += 18;
    attach(svolume, 0, y); y += 30;
    if(audio.cap(Audio::Latency) == true) {
      attach(llatency, 0, y); y += 18;
      attach(slatency, 0, y); y += 30;
    }
    if(audio.cap(Audio::Frequency) == true) {
      attach(loutput, 0, y); y += 18;
      attach(soutput, 0, y); y += 30;
    }
    attach(linput, 0, y); y += 18;
    attach(sinput, 0, y); y += 30;
  } else {
    attach(lvolume,    0, y);
    attach(llatency, 240, y); y += 18;

    attach(evolume,    0, y);
    attach(bvolume,  135, y);
    attach(elatency, 240, y);
    attach(blatency, 375, y); y += 25 + 5;

    attach(loutput,    0, y);
    attach(linput,   240, y); y += 18;

    attach(eoutput,    0, y);
    attach(boutput,  135, y);
    attach(einput,   240, y);
    attach(binput,   375, y); y += 25 + 5;

    if(audio.cap(Audio::Latency) == false) {
      elatency.disable();
      blatency.disable();
    }

    if(audio.cap(Audio::Frequency) == false) {
      eoutput.disable();
      boutput.disable();
    }
  }

  attach(note, 0, y);

  svolume.on_change = bind(&AudioSettingsWindow::volume_change, this);
  slatency.on_change = bind(&AudioSettingsWindow::latency_change, this);
  soutput.on_change = bind(&AudioSettingsWindow::output_change, this);
  sinput.on_change = bind(&AudioSettingsWindow::input_change, this);

  bvolume.on_tick = bind(&AudioSettingsWindow::volume_tick, this);
  blatency.on_tick = bind(&AudioSettingsWindow::latency_tick, this);
  boutput.on_tick = bind(&AudioSettingsWindow::output_tick, this);
  binput.on_tick = bind(&AudioSettingsWindow::input_tick, this);

  sync_ui();
}

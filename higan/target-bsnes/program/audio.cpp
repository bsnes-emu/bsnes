auto Program::updateAudioDriver(Window parent) -> void {
  auto changed = (bool)audio;
  audio.create(settings.audio.driver);
  audio.setContext(presentation.viewport.handle());
  audio.setChannels(2);
  if(changed) {
    settings.audio.device = audio.device();
    settings.audio.frequency = audio.frequency();
    settings.audio.latency = audio.latency();
  }
  updateAudioExclusive();
  updateAudioDevice();
  updateAudioBlocking();
  updateAudioDynamic();

  if(!audio.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings.audio.driver, "] audio driver."
    }).setParent(parent).error();
    settings.audio.driver = "None";
    return updateAudioDriver(parent);
  }
}

auto Program::updateAudioExclusive() -> void {
  audio.setExclusive(settings.audio.exclusive);
  updateAudioFrequency();
  updateAudioLatency();
}

auto Program::updateAudioDevice() -> void {
  audio.clear();
  if(!audio.hasDevice(settings.audio.device)) {
    settings.audio.device = audio.device();
  }
  audio.setDevice(settings.audio.device);
  updateAudioFrequency();
  updateAudioLatency();
}

auto Program::updateAudioBlocking() -> void {
  audio.clear();
  audio.setBlocking(settings.audio.blocking);
}

auto Program::updateAudioDynamic() -> void {
  audio.setDynamic(settings.audio.dynamic);
}

auto Program::updateAudioFrequency() -> void {
  audio.clear();
  if(!audio.hasFrequency(settings.audio.frequency)) {
    settings.audio.frequency = audio.frequency();
  }
  audio.setFrequency(settings.audio.frequency);
  double frequency = settings.audio.frequency + settings.audio.skew;
  if(!settings.video.blocking && settings.audio.blocking) {
    for(auto item : presentation.speedGroup.objects<MenuRadioItem>()) {
      if(item.checked()) frequency *= item.property("multiplier").real();
    }
  }
  Emulator::audio.setFrequency(frequency);
}

auto Program::updateAudioLatency() -> void {
  audio.clear();
  if(!audio.hasLatency(settings.audio.latency)) {
    settings.audio.latency = audio.latency();
  }
  audio.setLatency(settings.audio.latency);
}

auto Program::updateAudioEffects() -> void {
  double volume = settings.audio.mute ? 0.0 : settings.audio.volume * 0.01;
  Emulator::audio.setVolume(volume);

  double balance = max(-1.0, min(+1.0, (settings.audio.balance - 50) / 50.0));
  Emulator::audio.setBalance(balance);
}

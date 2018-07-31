auto Program::updateAudioDriver(Window parent) -> void {
  auto changed = (bool)audio;
  audio.create(settings["Audio/Driver"].text());
  audio.setContext(presentation->viewport.handle());
  audio.setChannels(2);
  if(changed) {
    settings["Audio/Device"].setValue(audio.device());
    settings["Audio/Frequency"].setValue(audio.frequency());
    settings["Audio/Latency"].setValue(audio.latency());
  }
  updateAudioExclusive();
  updateAudioDevice();
  updateAudioBlocking();
  updateAudioDynamic();

  if(!audio.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Audio/Driver"].text(), "] audio driver."
    }).setParent(parent).error();
    settings["Audio/Driver"].setValue("None");
    return updateAudioDriver(parent);
  }
}

auto Program::updateAudioExclusive() -> void {
  audio.setExclusive(settings["Audio/Exclusive"].boolean());
  updateAudioFrequency();
  updateAudioLatency();
}

auto Program::updateAudioDevice() -> void {
  audio.clear();
  if(!audio.hasDevice(settings["Audio/Device"].text())) {
    settings["Audio/Device"].setValue(audio.device());
  }
  audio.setDevice(settings["Audio/Device"].text());
  updateAudioFrequency();
  updateAudioLatency();
}

auto Program::updateAudioBlocking() -> void {
  audio.clear();
  audio.setBlocking(settings["Audio/Blocking"].boolean());
}

auto Program::updateAudioDynamic() -> void {
  audio.setDynamic(settings["Audio/Dynamic"].boolean());
}

auto Program::updateAudioFrequency() -> void {
  audio.clear();
  if(!audio.hasFrequency(settings["Audio/Frequency"].real())) {
    settings["Audio/Frequency"].setValue(audio.frequency());
  }
  audio.setFrequency(settings["Audio/Frequency"].real());
  double frequency = settings["Audio/Frequency"].real() + settings["Audio/Skew"].integer();
  for(auto item : presentation->speedGroup.objects<MenuRadioItem>()) {
    if(item.checked()) frequency *= item.property("multiplier").real();
  }
  Emulator::audio.setFrequency(frequency);
}

auto Program::updateAudioLatency() -> void {
  audio.clear();
  if(!audio.hasLatency(settings["Audio/Latency"].natural())) {
    settings["Audio/Latency"].setValue(audio.latency());
  }
  audio.setLatency(settings["Audio/Latency"].natural());
}

auto Program::updateAudioEffects() -> void {
  double volume = settings["Audio/Mute"].boolean() ? 0.0 : settings["Audio/Volume"].natural() * 0.01;
  Emulator::audio.setVolume(volume);

  double balance = max(-1.0, min(+1.0, (settings["Audio/Balance"].integer() - 50) / 50.0));
  Emulator::audio.setBalance(balance);
}

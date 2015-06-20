auto Program::powerCycle() -> void {
  if(!emulator) return;
  emulator->power();
  showMessage("Power cycled");
}

auto Program::softReset() -> void {
  if(!emulator) return;
  if(!emulator->information.resettable) return powerCycle();
  emulator->reset();
  showMessage("System reset");
}

auto Program::showMessage(const string& text) -> void {
  statusTime = time(0);
  statusMessage = text;
}

auto Program::updateStatusText() -> void {
  time_t currentTime = time(0);

  string text;
  if((currentTime - statusTime) <= 2) {
    text = statusMessage;
  } else if(!emulator || emulator->loaded() == false) {
    text = "No cartridge loaded";
  } else if(pause) {
    text = "Paused";
  } else {
    text = statusText;
  }

  if(text != presentation->statusBar.text()) {
    presentation->statusBar.setText(text);
  }
}

auto Program::updateVideoFilter() -> void {
  if(config->video.filter == "None") video->set(Video::Filter, Video::FilterNearest);
  if(config->video.filter == "Blur") video->set(Video::Filter, Video::FilterLinear);
}

auto Program::updateVideoPalette() -> void {
  if(!emulator) return;
  emulator->paletteUpdate(config->video.colorEmulation
  ? Emulator::Interface::PaletteMode::Emulation
  : Emulator::Interface::PaletteMode::Standard
  );
}

auto Program::updateAudio() -> void {
  if(!audio) return;
  audio->set(Audio::Frequency, config->audio.frequency);
  audio->set(Audio::Latency, config->audio.latency);
  if(auto resampler = config->audio.resampler) {
    if(resampler == "Linear" ) dsp.setResampler(DSP::ResampleEngine::Linear);
    if(resampler == "Hermite") dsp.setResampler(DSP::ResampleEngine::Hermite);
    if(resampler == "Sinc"   ) dsp.setResampler(DSP::ResampleEngine::Sinc);
  }
  dsp.setResamplerFrequency(config->audio.frequency);
  dsp.setVolume(config->audio.mute ? 0.0 : config->audio.volume * 0.01);
  updateDSP();
}

auto Program::updateDSP() -> void {
  if(!emulator) return;
  if(!config->video.synchronize) return dsp.setFrequency(emulator->audioFrequency());

  double inputRatio = emulator->audioFrequency() / emulator->videoFrequency();
  double outputRatio = config->timing.audio / config->timing.video;
  dsp.setFrequency(inputRatio / outputRatio * config->audio.frequency);
}

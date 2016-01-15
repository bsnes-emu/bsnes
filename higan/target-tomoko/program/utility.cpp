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
  if(settings["Video/Driver"].text() == "OpenGL"
  && settings["Video/Shader"].text() != "None"
  && directory::exists(settings["Video/Shader"].text())
  ) {
    video->set(Video::Filter, Video::FilterNearest);
    video->set(Video::Shader, settings["Video/Shader"].text());
  } else {
    video->set(Video::Filter, settings["Video/Filter"].text() == "Blur" ? Video::FilterLinear : Video::FilterNearest);
    video->set(Video::Shader, (string)"");
  }
}

auto Program::updateAudio() -> void {
  if(!audio) return;
  audio->clear();
  audio->set(Audio::Frequency, settings["Audio/Frequency"].natural());
  audio->set(Audio::Latency, settings["Audio/Latency"].natural());
  if(settings["Audio/Resampler"].text() == "Linear" ) dsp.setResampler(DSP::ResampleEngine::Linear);
  if(settings["Audio/Resampler"].text() == "Hermite") dsp.setResampler(DSP::ResampleEngine::Hermite);
  if(settings["Audio/Resampler"].text() == "Sinc"   ) dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(settings["Audio/Frequency"].natural());
  updateAudioVolume();
  updateDSP();
}

auto Program::updateAudioVolume() -> void {
  dsp.setVolume(settings["Audio/Mute"].boolean() ? 0.0 : settings["Audio/Volume"].natural() * 0.01);
}

auto Program::updateDSP() -> void {
  if(!emulator) return;
  if(!settings["Video/Synchronize"].boolean()) return dsp.setFrequency(emulator->audioFrequency());

  double inputRatio = emulator->audioFrequency() / emulator->videoFrequency();
  double outputRatio = settings["Timing/Audio"].real() / settings["Timing/Video"].real();
  dsp.setFrequency(inputRatio / outputRatio * settings["Audio/Frequency"].natural());
}

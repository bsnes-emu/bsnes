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

auto Program::connectDevices() -> void {
  if(!emulator) return;
  for(auto& port : emulator->port) {
    auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
    auto name = settings(path).text();
    for(auto& device : port.device) {
      if(device.name == name) {
        emulator->connect(port.id, device.id);
        break;
      }
    }
  }
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
  } else if(pause || (!presentation->focused() && settings["Input/FocusLoss/Pause"].boolean())) {
    text = "Paused";
  } else {
    text = statusText;
  }

  if(text != presentation->statusBar.text()) {
    presentation->statusBar.setText(text);
  }
}

auto Program::updateVideoShader() -> void {
  if(settings["Video/Driver"].text() == "OpenGL"
  && settings["Video/Shader"].text() != "None"
  && settings["Video/Shader"].text() != "Blur"
  && directory::exists(settings["Video/Shader"].text())
  ) {
    video->set(Video::Filter, Video::FilterNearest);
    video->set(Video::Shader, settings["Video/Shader"].text());
  } else {
    video->set(Video::Filter, settings["Video/Shader"].text() == "Blur" ? Video::FilterLinear : Video::FilterNearest);
    video->set(Video::Shader, (string)"");
  }
}

auto Program::updateAudio() -> void {
  if(!audio) return;
  audio->clear();
  audio->set(Audio::Latency, (uint)settings["Audio/Latency"].natural());
  if(settings["Audio/Resampler"].text() == "Linear" ) dsp.setResampler(DSP::ResampleEngine::Linear);
  if(settings["Audio/Resampler"].text() == "Hermite") dsp.setResampler(DSP::ResampleEngine::Hermite);
  if(settings["Audio/Resampler"].text() == "Sinc"   ) dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(audio->get(Audio::Frequency).get<uint>());
  updateAudioMode();
  updateAudioVolume();
  updateDSP();
}

auto Program::updateAudioMode() -> void {
  audio->set(Audio::Exclusive, settings["Audio/Exclusive"].boolean());
}

auto Program::updateAudioVolume() -> void {
  dsp.setVolume(settings["Audio/Mute"].boolean() ? 0.0 : settings["Audio/Volume"].natural() * 0.01);
}

auto Program::updateDSP() -> void {
  if(!emulator) return;
  return dsp.setFrequency(emulator->audioFrequency());
}

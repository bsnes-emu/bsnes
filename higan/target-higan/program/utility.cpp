auto Program::initializeVideoDriver() -> void {
  if(!Video::hasDrivers().find(settings["Video/Driver"].text())) {
    settings["Video/Driver"].setValue(Video::safestDriver());
  }
  video = new Video;
  video->create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());

  video->setBlocking(settings["Video/Synchronize"].boolean());

  if(!video->ready()) {
    MessageDialog().setText("Failed to initialize video driver").warning();
    video = new Video;
    video->create("None");
  }

  presentation->clearViewport();
}

auto Program::initializeAudioDriver() -> void {
  if(!Audio::hasDrivers().find(settings["Audio/Driver"].text())) {
    settings["Audio/Driver"].setValue("None");
  }
  audio = new Audio;
  audio->create(settings["Audio/Driver"].text());
  audio->setContext(presentation->viewport.handle());

  if(!audio->hasDevices().find(settings["Audio/Device"].text())) {
    settings["Audio/Device"].setValue(audio->device());
  }
  audio->setDevice(settings["Audio/Device"].text());

  if(!audio->hasFrequencies().find(settings["Audio/Frequency"].natural())) {
    settings["Audio/Frequency"].setValue(audio->frequency());
  }
  audio->setFrequency(settings["Audio/Frequency"].natural());

  if(!audio->hasLatencies().find(settings["Audio/Latency"].natural())) {
    settings["Audio/Latency"].setValue(audio->latency());
  }
  audio->setLatency(settings["Audio/Latency"].natural());

  audio->setChannels(2);
  audio->setExclusive(settings["Audio/Exclusive"].boolean());
  audio->setBlocking(settings["Audio/Synchronize"].boolean());

  if(!audio->ready()) {
    MessageDialog().setText("Failed to initialize audio driver").warning();
    audio = new Audio;
    audio->create("None");
  }

  Emulator::audio.setFrequency(settings["Audio/Frequency"].real());
}

auto Program::initializeInputDriver() -> void {
  if(!Input::hasDrivers().find(settings["Input/Driver"].text())) {
    settings["Input/Driver"].setValue("None");
  }
  input = new Input;
  input->create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());

  if(!input->ready()) {
    MessageDialog().setText("Failed to initialize input driver").warning();
    input = new Input;
    input->create("None");
  }
}

auto Program::softReset() -> void {
  if(!emulator || !emulator->information().resettable) return;
  emulator->reset();
  showMessage("System has been soft reset");
}

auto Program::powerCycle() -> void {
  if(!emulator) return;
  emulator->power();
  showMessage("System has been power cycled");
}

auto Program::togglePause() -> void {
  program->pause = !program->pause;
  presentation->pauseEmulation.setChecked(program->pause);
  if(program->pause) {
    showMessage("Emulation has been paused");
  } else {
    showMessage("Emulation has been unpaused");
  }
}

auto Program::rotateDisplay() -> void {
  if(!emulator) return;
  if(!emulator->cap("Rotate Display")) return showMessage("Display rotation not supported");
  auto rotate = emulator->get("Rotate Display");
  emulator->set("Rotate Display", !rotate.get<bool>());
  presentation->resizeViewport();
  showMessage("Display rotated");
}

auto Program::showMessage(const string& text) -> void {
  statusTime = time(nullptr);
  statusMessage = text;
}

auto Program::updateStatusText() -> void {
  string message;
  if(chrono::timestamp() - statusTime <= 2) {
    message = statusMessage;
  }
  if(message != presentation->statusMessage.text()) {
    presentation->statusMessage.setText(message);
  }

  string info;
  if(!emulator || !emulator->loaded()) {
    info = "Unloaded";
  } else if(pause || (!focused() && settingsManager->input.pauseEmulation.checked())) {
    info = "Paused";
  } else {
    info = statusInfo;
  }
  if(info != presentation->statusInfo.text()) {
    presentation->statusInfo.setText(info);
  }
}

auto Program::updateVideoPalette() -> void {
  double saturation = settings["Video/Saturation"].natural() / 100.0;
  double gamma = settings["Video/Gamma"].natural() / 100.0;
  double luminance = settings["Video/Luminance"].natural() / 100.0;
  Emulator::video.setSaturation(saturation);
  Emulator::video.setGamma(gamma);
  Emulator::video.setLuminance(luminance);
  Emulator::video.setPalette();
}

auto Program::updateVideoShader() -> void {
  if(settings["Video/Driver"].text() == "OpenGL"
  && settings["Video/Shader"].text() != "None"
  && settings["Video/Shader"].text() != "Blur"
  && directory::exists(settings["Video/Shader"].text())
  ) {
    video->setSmooth(false);
    video->setShader(settings["Video/Shader"].text());
  } else {
    video->setSmooth(settings["Video/Shader"].text() == "Blur");
    video->setShader("");
  }
}

auto Program::updateAudioDriver() -> void {
  if(!audio) return;
  audio->clear();
  audio->setExclusive(settings["Audio/Exclusive"].boolean());
  audio->setFrequency(settings["Audio/Frequency"].real());
  audio->setLatency(settings["Audio/Latency"].natural());
  Emulator::audio.setFrequency(settings["Audio/Frequency"].real());
}

auto Program::updateAudioEffects() -> void {
  auto volume = settings["Audio/Mute"].boolean() ? 0.0 : settings["Audio/Volume"].natural() * 0.01;
  Emulator::audio.setVolume(volume);

  auto balance = max(-1.0, min(1.0, (settings["Audio/Balance"].integer() - 50) / 50.0));
  Emulator::audio.setBalance(balance);
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video->exclusive()) return true;
  if(presentation && presentation->focused()) return true;
  return false;
}

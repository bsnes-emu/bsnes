auto Program::initializeVideoDriver() -> void {
  video = Video::create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());
  video->setExclusive(false);
  video->setBlocking(settings["Video/Blocking"].boolean());

  if(video->ready()) {
    presentation->clearViewport();
    updateVideoShader();
  }

  video->onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) presentation->clearViewport();
  });
}

auto Program::initializeAudioDriver() -> void {
  audio = Audio::create(settings["Audio/Driver"].text());
  audio->setContext(presentation->viewport.handle());
  audio->setExclusive(false);
  audio->setBlocking(settings["Audio/Blocking"].boolean());
  if(!audio->availableDevices().find(settings["Audio/Device"].text())) {
    settings["Audio/Device"].setValue(audio->availableDevices()(0));
  }
  audio->setDevice(settings["Audio/Device"].text());
  if(!audio->availableFrequencies().find(settings["Audio/Frequency"].real())) {
    settings["Audio/Frequency"].setValue(audio->availableFrequencies()(0));
  }
  audio->setFrequency(settings["Audio/Frequency"].real());
  if(!audio->availableLatencies().find(settings["Audio/Latency"].natural())) {
    settings["Audio/Latency"].setValue(audio->availableLatencies()(0));
  }
  audio->setLatency(settings["Audio/Latency"].natural());
  audio->setChannels(2);
}

auto Program::initializeInputDriver() -> void {
  input = Input::create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());
}

auto Program::updateVideoShader() -> void {
  if(settings["Video/Driver"].text() == "OpenGL"
  && settings["Video/Shader"].text() != "None"
  && settings["Video/Shader"].text() != "Blur"
  ) {
    video->setSmooth(false);
    video->setShader(settings["Video/Shader"].text());
  } else {
    video->setSmooth(settings["Video/Shader"].text() == "Blur");
    video->setShader("");
  }
}

auto Program::connectDevices() -> void {
  for(auto& port : emulator->ports) {
    auto path = string{"Emulator/", port.name}.replace(" ", "");
    auto name = settings(path).text();
    for(auto& device : port.devices) {
      if(device.name == name) {
        emulator->connect(port.id, device.id);
        break;
      }
    }
  }
}

auto Program::applyHacks() -> void {
  bool fastPPU = settingsWindow->advanced.fastPPUOption.checked();
  bool fastDSP = settingsWindow->advanced.fastDSPOption.checked();

  auto label = superNintendo.label;
  if(label == "AIR STRIKE PATROL" || label == "DESERT FIGHTER") fastPPU = false;
  if(label == "KOUSHIEN_2") fastDSP = false;
  if(label == "RENDERING RANGER R2") fastDSP = false;

  emulator->set("Fast PPU", fastPPU);
  emulator->set("Fast DSP", fastDSP);
}

auto Program::showMessage(string text) -> void {
  statusTime = chrono::timestamp();
  statusMessage = text;
}

auto Program::showFrameRate(string text) -> void {
  statusFrameRate = text;
}

auto Program::updateStatus() -> void {
  string message;
  if(chrono::timestamp() - statusTime <= 2) {
    message = statusMessage;
  }
  message.prepend("  ");
  if(message != presentation->statusLeft.text()) {
    presentation->statusLeft.setText(message);
  }

  string frameRate;
  if(!emulator->loaded()) {
    frameRate = "Unloaded";
  } else if(presentation->pauseEmulation.checked()) {
    frameRate = "Paused";
  } else if(!focused() && settingsWindow->input.pauseEmulation.checked()) {
    frameRate = "Paused";
  } else {
    frameRate = statusFrameRate;
  }
  frameRate.append("  ");
  if(frameRate != presentation->statusRight.text()) {
    presentation->statusRight.setText(frameRate);
  }
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video && video->exclusive()) return true;
  if(presentation && presentation->focused()) return true;
  return false;
}

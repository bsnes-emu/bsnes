auto Program::initializeVideoDriver() -> void {
  video = Video::create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());
  video->setExclusive(false);
  video->setBlocking(settings["Video/Blocking"].boolean());
  if(!video->ready()) {
    MessageDialog().setText("Failed to initialize video driver").warning();
    video = Video::create("None");
  }
  presentation->clearViewport();
  updateVideoShader();
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
  if(!audio->ready()) {
    MessageDialog().setText("Failed to initialize audio driver").warning();
    audio = Audio::create("None");
  }
}

auto Program::initializeInputDriver() -> void {
  input = Input::create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());
  if(!input->ready()) {
    MessageDialog().setText("Failed to initialize input driver").warning();
    input = Input::create("None");
  }
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

auto Program::showMessage(string text) -> void {
  statusTime = chrono::timestamp();
  statusMessage = text;
}

auto Program::updateMessage() -> void {
  uint64 currentTime = chrono::timestamp();

  string text;
  if((currentTime - statusTime) <= 2) {
    text = statusMessage;
  } else if(!emulator->loaded()) {
    text = "No game loaded";
  } else if(presentation->pauseEmulation.checked()) {
    text = "Paused";
  } else if(!focused() && settingsWindow->input.pauseEmulation.checked()) {
    text = "Paused";
  } else {
    text = statusText;
  }

  if(text != presentation->statusBar.text()) {
    presentation->statusBar.setText(text);
  }
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video->exclusive()) return true;
  if(presentation && presentation->focused()) return true;
  return false;
}

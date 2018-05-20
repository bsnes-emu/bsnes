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
  input = Input::create(settings["Input/Driver"].boolean());
  input->setContext(presentation->viewport.handle());
  if(!input->ready()) {
    MessageDialog().setText("Failed to initialize input driver").warning();
    input = Input::create("None");
  }
}

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
  audio->setChannels(2);
  audio->setFrequency(48000.0);
  audio->setLatency(0);
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

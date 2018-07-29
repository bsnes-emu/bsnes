auto Program::updateVideoDriver(Window parent) -> void {
  auto changed = (bool)video;
  video = Video::create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());
  if(changed) {
    settings["Video/Format"].setValue(video->defaultFormat());
  }
  updateVideoExclusive();
  updateVideoBlocking();
  updateVideoFlush();
  updateVideoFormat();
  updateVideoShader();

  if(video->ready()) {
    presentation->clearViewport();
    updateVideoShader();
  }

  video->onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) presentation->clearViewport();
  });

  if(!video->ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Video/Driver"].text(), "] video driver."
    }).setParent(parent).error();
    settings["Video/Driver"].setValue("None");
    return updateVideoDriver(parent);
  }

  presentation->updateShaders();
}

auto Program::updateVideoExclusive() -> void {
  //only enabled in fullscreen mode via Presentation::toggleFullScreen()
  video->setExclusive(false);
}

auto Program::updateVideoBlocking() -> void {
  video->setBlocking(settings["Video/Blocking"].boolean());
}

auto Program::updateVideoFlush() -> void {
  video->setFlush(settings["Video/Flush"].boolean());
}

auto Program::updateVideoFormat() -> void {
  if(!video->availableFormats().find(settings["Video/Format"].text())) {
    settings["Video/Format"].setValue(video->defaultFormat());
  }
  video->setFormat(settings["Video/Format"].text());
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

auto Program::updateVideoPalette() -> void {
  emulator->configure("video/colorEmulation", false);
  double luminance = settings["Video/Luminance"].natural() / 100.0;
  double saturation = settings["Video/Saturation"].natural() / 100.0;
  double gamma = settings["Video/Gamma"].natural() / 100.0;
  Emulator::video.setLuminance(luminance);
  Emulator::video.setSaturation(saturation);
  Emulator::video.setGamma(gamma);
  Emulator::video.setPalette();
}

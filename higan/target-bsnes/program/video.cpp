auto Program::updateVideoDriver() -> void {
  video = Video::create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());
  video->setExclusive(false);
  updateVideoBlocking();
  updateVideoShader();

  if(video->ready()) {
    presentation->clearViewport();
    updateVideoShader();
  }

  video->onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) presentation->clearViewport();
  });

  settingsWindow->advanced.updateVideoDriver();

  if(!video->ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Video/Driver"].text(), "] video driver."
    }).error();
    settings["Video/Driver"].setValue("None");
    return updateVideoDriver();
  }

  presentation->updateShaders();
}

auto Program::updateVideoBlocking() -> void {
  video->setBlocking(settings["Video/Blocking"].boolean());
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
  emulator->set("Color Emulation", false);
  double luminance = settings["Video/Luminance"].natural() / 100.0;
  double saturation = settings["Video/Saturation"].natural() / 100.0;
  double gamma = settings["Video/Gamma"].natural() / 100.0;
  Emulator::video.setLuminance(luminance);
  Emulator::video.setSaturation(saturation);
  Emulator::video.setGamma(gamma);
  Emulator::video.setPalette();
}

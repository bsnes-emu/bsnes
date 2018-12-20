auto Program::updateVideoDriver(Window parent) -> void {
  auto changed = (bool)video;
  video.create(settings.video.driver);
  video.setContext(presentation.viewport.handle());
  if(changed) {
    settings.video.format = video.format();
  }
  updateVideoExclusive();
  updateVideoBlocking();
  updateVideoFlush();
  updateVideoFormat();
  updateVideoShader();

  if(video.ready()) {
    presentation.configureViewport();
    presentation.clearViewport();
    updateVideoShader();
  }

  video.onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) presentation.clearViewport();
  });

  if(!video.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings.video.driver, "] video driver."
    }).setParent(parent).error();
    settings.video.driver = "None";
    return updateVideoDriver(parent);
  }

  presentation.updateShaders();
}

auto Program::updateVideoExclusive() -> void {
  //only enabled in fullscreen mode via Presentation::toggleFullScreen()
  video.setExclusive(false);
}

auto Program::updateVideoBlocking() -> void {
  video.setBlocking(settings.video.blocking);
}

auto Program::updateVideoFlush() -> void {
  video.setFlush(settings.video.flush);
}

auto Program::updateVideoFormat() -> void {
  if(!video.hasFormat(settings.video.format)) {
    settings.video.format = video.format();
  }
  video.setFormat(settings.video.format);
  Emulator::video.setDepth(settings.video.format == "RGB30" ? 30 : 24);
  Emulator::video.setPalette();
}

auto Program::updateVideoShader() -> void {
  video.setShader(settings.video.shader);
}

auto Program::updateVideoPalette() -> void {
  emulator->configure("Video/ColorEmulation", false);
  Emulator::video.setLuminance(settings.video.luminance / 100.0);
  Emulator::video.setSaturation(settings.video.saturation / 100.0);
  Emulator::video.setGamma(settings.video.gamma / 100.0);
  Emulator::video.setPalette();
}

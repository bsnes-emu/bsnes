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
    video.clear();
    updateVideoShader();
  }

  video.onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) video.clear();
  });

  if(!video.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings.video.driver, "] video driver."
    }).setAlignment(parent).error();
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
}

auto Program::updateVideoShader() -> void {
  video.setShader(settings.video.shader);
}

auto Program::updateVideoPalette() -> void {
  double luminance = settings.video.luminance / 100.0;
  double saturation = settings.video.saturation / 100.0;
  double gamma = settings.video.gamma / 100.0;

  uint depth = 24;
  if(video.format() == "RGB30") depth = 30;

  for(uint color : range(32768)) {
    uint16 r = (color >> 10) & 31;
    uint16 g = (color >>  5) & 31;
    uint16 b = (color >>  0) & 31;

    r = r << 3 | r >> 2; r = r << 8 | r << 0;
    g = g << 3 | g >> 2; g = g << 8 | g << 0;
    b = b << 3 | b >> 2; b = b << 8 | b << 0;

    if(saturation != 1.0) {
      uint16 grayscale = uclamp<16>((r + g + b) / 3);
      double inverse = max(0.0, 1.0 - saturation);
      r = uclamp<16>(r * saturation + grayscale * inverse);
      g = uclamp<16>(g * saturation + grayscale * inverse);
      b = uclamp<16>(b * saturation + grayscale * inverse);
    }

    if(gamma != 1.0) {
      double reciprocal = 1.0 / 32767.0;
      r = r > 32767 ? r : uint16(32767 * pow(r * reciprocal, gamma));
      g = g > 32767 ? g : uint16(32767 * pow(g * reciprocal, gamma));
      b = b > 32767 ? b : uint16(32767 * pow(b * reciprocal, gamma));
    }

    if(luminance != 1.0) {
      r = uclamp<16>(r * luminance);
      g = uclamp<16>(g * luminance);
      b = uclamp<16>(b * luminance);
    }

    switch(depth) {
    case 24: palette[color] = r >> 8 << 16 | g >> 8 <<  8 | b >> 8 << 0; break;
    case 30: palette[color] = r >> 6 << 20 | g >> 6 << 10 | b >> 6 << 0; break;
    }

    r >>= 1;
    g >>= 1;
    b >>= 1;

    switch(depth) {
    case 24: palettePaused[color] = r >> 8 << 16 | g >> 8 <<  8 | b >> 8 << 0; break;
    case 30: palettePaused[color] = r >> 6 << 20 | g >> 6 << 10 | b >> 6 << 0; break;
    }
  }

  emulator->configure("Video/ColorEmulation", false);
}

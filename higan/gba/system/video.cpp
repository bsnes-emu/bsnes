auto System::configureVideo() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();
}

auto System::configureVideoPalette() -> void {
  Emulator::video.setPalette(1 << 15, [&](uint32 color) -> uint64 {
    uint R = color.bits( 0, 4);
    uint G = color.bits( 5, 9);
    uint B = color.bits(10,14);

    uint64 r = image::normalize(R, 5, 16);
    uint64 g = image::normalize(G, 5, 16);
    uint64 b = image::normalize(B, 5, 16);

    if(settings.colorEmulation) {
      double lcdGamma = 4.0, outGamma = 2.2;
      double lb = pow(B / 31.0, lcdGamma);
      double lg = pow(G / 31.0, lcdGamma);
      double lr = pow(R / 31.0, lcdGamma);
      r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    }

    return r << 32 | g << 16 | b << 0;
  });
}

auto System::configureVideoEffects() -> void {
  Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
}

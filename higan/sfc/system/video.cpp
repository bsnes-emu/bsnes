auto System::configureVideo() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();
}

auto System::configureVideoPalette() -> void {
  Emulator::video.setPalette(1 << 19, [&](uint32 color) -> uint64 {
    uint r = color.bits( 0, 4);
    uint g = color.bits( 5, 9);
    uint b = color.bits(10,14);
    uint l = color.bits(15,18);

    double L = (1.0 + l) / 16.0 * (l ? 1.0 : 0.5);
    uint64 R = L * image::normalize(r, 5, 16);
    uint64 G = L * image::normalize(g, 5, 16);
    uint64 B = L * image::normalize(b, 5, 16);

    if(settings.colorEmulation) {
      static const uint8 gammaRamp[32] = {
        0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
        0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
        0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
        0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
      };
      R = L * gammaRamp[r] * 0x0101;
      G = L * gammaRamp[g] * 0x0101;
      B = L * gammaRamp[b] * 0x0101;
    }

    return R << 32 | G << 16 | B << 0;
  });
}

auto System::configureVideoEffects() -> void {
  Emulator::video.setEffect(Emulator::Video::Effect::ColorBleed, settings.blurEmulation);
}

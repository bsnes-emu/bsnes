auto System::configureVideoPalette() -> void {
  Emulator::video.setPalette();
}

auto System::configureVideoEffects() -> void {
  Emulator::video.setEffect(Emulator::Video::Effect::ColorBleed, settings.blurEmulation);
}

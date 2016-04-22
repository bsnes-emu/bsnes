auto System::configureVideoPalette() -> void {
  if(sgb()) return;
  Emulator::video.setPalette();
}

auto System::configureVideoEffects() -> void {
  if(sgb()) return;
  Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
}

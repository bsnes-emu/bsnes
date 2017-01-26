auto System::configureVideoPalette() -> void {
  if(model() == Model::SuperGameBoy) return;
  Emulator::video.setPalette();
}

auto System::configureVideoEffects() -> void {
  if(model() == Model::SuperGameBoy) return;
  Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
}

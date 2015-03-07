auto Program::updateVideoFilter() -> void {
  if(config().video.filter == "None") video.set(Video::Filter, Video::FilterNearest);
  if(config().video.filter == "Blur") video.set(Video::Filter, Video::FilterLinear);
}

auto Program::updateVideoPalette() -> void {
  if(!activeEmulator) return;
  emulator().paletteUpdate(config().video.colorEmulation
  ? Emulator::Interface::PaletteMode::Emulation
  : Emulator::Interface::PaletteMode::Standard
  );
}

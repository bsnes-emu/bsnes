auto Program::showMessage(const string& text) -> void {
  statusTime = time(0);
  statusMessage = text;
}

auto Program::updateStatusText() -> void {
  time_t currentTime = time(0);

  string text;
  if((currentTime - statusTime) <= 2) {
    text = statusMessage;
  } else if(!activeEmulator || emulator().loaded() == false) {
    text = "No cartridge loaded";
  } else if(0) {
    text = "Paused";
  } else {
    text = statusText;
  }

  if(text != presentation->statusBar.text()) {
    presentation->statusBar.setText(text);
  }
}

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

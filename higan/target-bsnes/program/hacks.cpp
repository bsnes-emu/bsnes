auto Program::applyHacks() -> void {
  bool fastPPU = settingsWindow->advanced.fastPPUOption.checked();
  bool fastDSP = settingsWindow->advanced.fastDSPOption.checked();

  auto label = superFamicom.label;
  if(label == "AIR STRIKE PATROL" || label == "DESERT FIGHTER") fastPPU = false;
  if(label == "KOUSHIEN_2") fastDSP = false;
  if(label == "RENDERING RANGER R2") fastDSP = false;

  emulator->set("Fast PPU", fastPPU);
  emulator->set("Fast DSP", fastDSP);
}

auto Program::applyHackOverclockSuperFX() -> void {
  //todo: implement a better way of detecting SuperFX games
  //todo: apply multiplier changes on reset, not just on game load?
  double multiplier = settingsWindow->advanced.superFXValue.text().natural() / 100.0;

  auto label = superFamicom.label;
  if(label == "NIDAN MORITASHOGI2") return;  //ST018 uses same clock speed as SuperFX

  auto document = BML::unserialize(superFamicom.manifest);

  //GSU-1, GSU-2 have a 21440000hz oscillator
  if(auto oscillator = document["game/board/oscillator"]) {
    if(oscillator["frequency"].text() == "21440000") {
      oscillator["frequency"].setValue(uint(21440000 * multiplier));
      superFamicom.manifest = BML::serialize(document);
    }
    return;
  }

  //MARIO CHIP 1 uses CPU oscillator; force it to use its own crystal to overclock it
  bool marioChip1 = false;
  if(label == "STAR FOX" || label == "STAR WING") marioChip1 = true;
  if(marioChip1) {
    document("game/board/oscillator/frequency").setValue(uint(21440000 * multiplier));
    superFamicom.manifest = BML::serialize(document);
  }
}

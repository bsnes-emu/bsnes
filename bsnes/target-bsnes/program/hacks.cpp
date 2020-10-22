auto Program::hackCompatibility() -> void {
  string entropy = settings.emulator.hack.entropy;
  bool fastPPU = settings.emulator.hack.ppu.fast;
  bool fastPPUNoSpriteLimit = settings.emulator.hack.ppu.noSpriteLimit;
  bool fastDSP = settings.emulator.hack.dsp.fast;
  bool coprocessorDelayedSync = settings.emulator.hack.coprocessor.delayedSync;
  uint renderCycle = 512;

  auto title = superFamicom.title;
  auto region = superFamicom.region;

  //relies on mid-scanline rendering techniques
  if(title == "AIR STRIKE PATROL" || title == "DESERT FIGHTER") fastPPU = false;

  //the dialogue text is blurry due to an issue in the scanline-based renderer's color math support
  if(title == "マーヴェラス") fastPPU = false;

  //stage 2 uses pseudo-hires in a way that's not compatible with the scanline-based renderer
  if(title == "SFC クレヨンシンチャン") fastPPU = false;

  //title screen game select (after choosing a game) changes OAM tiledata address mid-frame
  //this is only supported by the cycle-based PPU renderer
  if(title == "Winter olympics") fastPPU = false;

  //title screen shows remnants of the flag after choosing a language with the scanline-based renderer
  if(title == "WORLD CUP STRIKER") fastPPU = false;

  //relies on cycle-accurate writes to the echo buffer
  if(title == "KOUSHIEN_2") fastDSP = false;

  //will hang immediately
  if(title == "RENDERING RANGER R2") fastDSP = false;

  //will hang sometimes in the "Bach in Time" stage
  if(title == "BUBSY II" && region == "PAL") fastDSP = false;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "ADVENTURES OF FRANKEN" && region == "PAL") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "FIREPOWER 2000" || title == "SUPER SWIV") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "NHL '94" || title == "NHL PROHOCKEY'94") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "Sugoro Quest++") renderCycle = 128;

  if(settings.emulator.hack.hotfixes) {
    //this game transfers uninitialized memory into video RAM: this can cause a row of invalid tiles
    //to appear in the background of stage 12. this one is a bug in the original game, so only enable
    //it if the hotfixes option has been enabled.
    if(title == "The Hurricanes") entropy = "None";

    //Frisky Tom attract sequence sometimes hangs when WRAM is initialized to pseudo-random patterns
    if(title == "ニチブツ・アーケード・クラシックス") entropy = "None";
  }

  emulator->configure("Hacks/Entropy", entropy);
  emulator->configure("Hacks/PPU/Fast", fastPPU);
  emulator->configure("Hacks/PPU/NoSpriteLimit", fastPPUNoSpriteLimit);
  emulator->configure("Hacks/PPU/RenderCycle", renderCycle);
  emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
  emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  emulator->configure("Hacks/DSP/Fast", fastDSP);
  emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorDelayedSync);
}

auto Program::hackPatchMemory(vector<uint8_t>& data) -> void {
  auto title = superFamicom.title;

  if(title == "Satellaview BS-X" && data.size() >= 0x100000) {
    //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN) (1.1)
    //disable limited play check for BS Memory flash cartridges
    //benefit: allow locked out BS Memory flash games to play without manual header patching
    //detriment: BS Memory ROM cartridges will cause the game to hang in the load menu
    if(data[0x4a9b] == 0x10) data[0x4a9b] = 0x80;
    if(data[0x4d6d] == 0x10) data[0x4d6d] = 0x80;
    if(data[0x4ded] == 0x10) data[0x4ded] = 0x80;
    if(data[0x4e9a] == 0x10) data[0x4e9a] = 0x80;
  }
}

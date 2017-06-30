//this should only be called by CPU::PPUcounter::tick();
//keeps track of previous counter positions in history table
auto PPUcounter::tick() -> void {
  status.hcounter += 2;  //increment by smallest unit of time
  if(status.hcounter == lineclocks()) {
    status.hcounter = 0;
    vcounterTick();
  }

  history.index = (history.index + 1) & 2047;
  history.field   [history.index] = status.field;
  history.vcounter[history.index] = status.vcounter;
  history.hcounter[history.index] = status.hcounter;
}

//this should only be called by PPU::PPUcounter::tick(n);
//allows stepping by more than the smallest unit of time
auto PPUcounter::tick(uint clocks) -> void {
  status.hcounter += clocks;
  if(status.hcounter >= lineclocks()) {
    status.hcounter -= lineclocks();
    vcounterTick();
  }
}

//internal
auto PPUcounter::vcounterTick() -> void {
  if(++status.vcounter == 128) status.interlace = ppu.interlace();

  if((Region::NTSC() && status.interlace == 0 && status.vcounter == 262)
  || (Region::NTSC() && status.interlace == 1 && status.vcounter == 263)
  || (Region::NTSC() && status.interlace == 1 && status.vcounter == 262 && status.field == 1)
  || (Region::PAL()  && status.interlace == 0 && status.vcounter == 312)
  || (Region::PAL()  && status.interlace == 1 && status.vcounter == 313)
  || (Region::PAL()  && status.interlace == 1 && status.vcounter == 312 && status.field == 1)
  ) {
    status.vcounter = 0;
    status.field = !status.field;
  }
  if(scanline) scanline();
}

auto PPUcounter::field() const -> bool { return status.field; }
auto PPUcounter::vcounter() const -> uint16 { return status.vcounter; }
auto PPUcounter::hcounter() const -> uint16 { return status.hcounter; }

auto PPUcounter::field(uint offset) const -> bool { return history.field[(history.index - (offset >> 1)) & 2047]; }
auto PPUcounter::vcounter(uint offset) const -> uint16 { return history.vcounter[(history.index - (offset >> 1)) & 2047]; }
auto PPUcounter::hcounter(uint offset) const -> uint16 { return history.hcounter[(history.index - (offset >> 1)) & 2047]; }

//one PPU dot = 4 CPU clocks
//
//PPU dots 323 and 327 are 6 CPU clocks long.
//this does not apply to NTSC non-interlace scanline 240 on odd fields. this is
//because the PPU skips one dot to alter the color burst phase of the video signal.
//
//dot 323 range = {1292, 1294, 1296}
//dot 327 range = {1310, 1312, 1314}

auto PPUcounter::hdot() const -> uint16 {
  if(Region::NTSC() && status.interlace == 0 && vcounter() == 240 && field() == 1) {
    return (hcounter() >> 2);
  } else {
    return (hcounter() - ((hcounter() > 1292) << 1) - ((hcounter() > 1310) << 1)) >> 2;
  }
}

auto PPUcounter::lineclocks() const -> uint16 {
  if(Region::NTSC() && status.interlace == 0 && vcounter() == 240 && field() == 1) return 1360;
  return 1364;
}

auto PPUcounter::reset() -> void {
  status.interlace = 0;
  status.field     = 0;
  status.vcounter  = 0;
  status.hcounter  = 0;
  history.index    = 0;

  for(auto n : range(2048)) {
    history.field   [n] = 0;
    history.vcounter[n] = 0;
    history.hcounter[n] = 0;
  }
}

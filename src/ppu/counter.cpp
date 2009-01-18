#ifdef PPU_CPP

//wrappers to allow PPUcounter::tick()/tock() to be inlined
bool PPUcounter::region() const { return snes.region() == SNES::NTSC ? 0 : 1; }
bool PPUcounter::interlace() const { return ppu.interlace(); }
void PPUcounter::scanline() { cpu.scanline(); }

//one PPU dot = 4 CPU clocks
//
//PPU dots 323 and 327 are 6 CPU clocks long.
//this does not apply to NTSC non-interlace scanline 240 on odd fields. this is
//because the PPU skips one dot to alter the color burst phase of the video signal.
//
//dot 323 range = { 1292, 1294, 1296 }
//dot 327 range = { 1310, 1312, 1314 }

uint16 PPUcounter::hdot() const {
  if(region() == 0 && interlace() == false && status.vcounter == 240 && status.field == 1) {
    return (status.hcounter >> 2);
  } else {
    return (status.hcounter - ((status.hcounter > 1292) << 1) - ((status.hcounter > 1310) << 1)) >> 2;
  }
}

uint16 PPUcounter::lineclocks() const {
  if(region() == 0 && interlace() == false && vcounter() == 240 && status.field == 1) return 1360;
  return 1364;
}

uint16 PPUcounter::ilineclocks() const {
  if(region() == 0 && interlace() == false && ivcounter() == 240 && status.field == 1) return 1360;
  return 1364;
}

void PPUcounter::reset() {
  status.field    = 0;
  status.vcounter = 0;
  status.hcounter = 0;

  history.index   = 0;
  history.ppudiff = 0;

  for(unsigned i = 0; i < 2048; i++) {
    history.field   [i] = 0;
    history.vcounter[i] = 0;
    history.hcounter[i] = 0;
  }
}

#endif

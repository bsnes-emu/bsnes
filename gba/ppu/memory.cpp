auto PPU::vram_read(unsigned mode, uint32 addr) -> uint32 {
  addr &= (addr & 0x10000) ? 0x17fff : 0x0ffff;

  if(mode & Word) {
    addr &= ~3;
    return vram[addr + 0] << 0 | vram[addr + 1] << 8 | vram[addr + 2] << 16 | vram[addr + 3] << 24;
  } else if(mode & Half) {
    addr &= ~1;
    return vram[addr + 0] << 0 | vram[addr + 1] << 8;
  } else if(mode & Byte) {
    return vram[addr];
  }

  return 0;  //should never occur
}

auto PPU::vram_write(unsigned mode, uint32 addr, uint32 word) -> void {
  addr &= (addr & 0x10000) ? 0x17fff : 0x0ffff;

  if(mode & Word) {
    addr &= ~3;
    vram[addr + 0] = word >>  0;
    vram[addr + 1] = word >>  8;
    vram[addr + 2] = word >> 16;
    vram[addr + 3] = word >> 24;
  } else if(mode & Half) {
    addr &= ~1;
    vram[addr + 0] = word >>  0;
    vram[addr + 1] = word >>  8;
  } else if(mode & Byte) {
    //8-bit writes to OBJ section of VRAM are ignored
    if(regs.control.bgmode <= 2 && addr >= 0x10000) return;
    if(regs.control.bgmode <= 5 && addr >= 0x14000) return;

    addr &= ~1;
    vram[addr + 0] = (uint8)word;
    vram[addr + 1] = (uint8)word;
  }
}

auto PPU::pram_read(unsigned mode, uint32 addr) -> uint32 {
  if(mode & Word) return pram_read(Half, addr & ~2) << 0 | pram_read(Half, addr | 2) << 16;
  if(mode & Byte) return pram_read(Half, addr) >> ((addr & 1) * 8);
  return pram[addr >> 1 & 511];
}

auto PPU::pram_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(mode & Word) {
    pram_write(Half, addr & ~2, word >>  0);
    pram_write(Half, addr |  2, word >> 16);
    return;
  }

  if(mode & Byte) {
    word = (uint8)word;
    return pram_write(Half, addr, word << 8 | word << 0);
  }

  pram[addr >> 1 & 511] = (uint16)word;
}

auto PPU::oam_read(unsigned mode, uint32 addr) -> uint32 {
  if(mode & Word) return oam_read(Half, addr & ~2) << 0 | oam_read(Half, addr | 2) << 16;
  if(mode & Byte) return oam_read(Half, addr) >> ((addr & 1) * 8);

  auto& obj = object[addr >> 3 & 127];
  auto& par = objectparam[addr >> 5 & 31];

  switch(addr & 6) {

  case 0: return (
    (obj.y          <<  0)
  | (obj.affine     <<  8)
  | (obj.affinesize <<  9)
  | (obj.mode       << 10)
  | (obj.mosaic     << 12)
  | (obj.colors     << 13)
  | (obj.shape      << 14)
  );

  case 2: return (
    (obj.x           <<  0)
  | (obj.affineparam <<  9)
  | (obj.hflip       << 12)
  | (obj.vflip       << 13)
  | (obj.size        << 14)
  );

  case 4: return (
    (obj.character <<  0)
  | (obj.priority  << 10)
  | (obj.palette   << 12)
  );

  case 6:
    switch(addr >> 3 & 3) {
    case 0: return par.pa;
    case 1: return par.pb;
    case 2: return par.pc;
    case 3: return par.pd;
    }

  }
}

auto PPU::oam_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(mode & Word) {
    oam_write(Half, addr & ~2, word >>  0);
    oam_write(Half, addr |  2, word >> 16);
    return;
  }

  if(mode & Byte) return;  //8-bit writes to OAM are ignored

  auto& obj = object[addr >> 3 & 127];
  auto& par = objectparam[addr >> 5 & 31];
  switch(addr & 6) {

  case 0:
    obj.y          = word >>  0;
    obj.affine     = word >>  8;
    obj.affinesize = word >>  9;
    obj.mode       = word >> 10;
    obj.mosaic     = word >> 12;
    obj.colors     = word >> 13;
    obj.shape      = word >> 14;
    break;

  case 2:
    obj.x           = word >>  0;
    obj.affineparam = word >>  9;
    obj.hflip       = word >> 12;
    obj.vflip       = word >> 13;
    obj.size        = word >> 14;
    break;

  case 4:
    obj.character = word >>  0;
    obj.priority  = word >> 10;
    obj.palette   = word >> 12;
    break;

  case 6:
    switch(addr >> 3 & 3) {
    case 0: par.pa = word; break;
    case 1: par.pb = word; break;
    case 2: par.pc = word; break;
    case 3: par.pd = word; break;
    }

  }

  static uint widths[] = {
     8, 16, 32, 64,
    16, 32, 32, 64,
     8,  8, 16, 32,
     8,  8,  8,  8,  //invalid modes
  };

  static uint heights[] = {
     8, 16, 32, 64,
     8,  8, 16, 32,
    16, 32, 32, 64,
     8,  8,  8,  8,  //invalid modes
  };

  obj.width  = widths [obj.shape * 4 + obj.size];
  obj.height = heights[obj.shape * 4 + obj.size];
}

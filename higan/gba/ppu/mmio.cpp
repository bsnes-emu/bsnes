auto PPU::read(uint32 addr) -> uint8 {
  switch(addr) {

  //DISPCNT
  case 0x0400'0000: return regs.control >> 0;
  case 0x0400'0001: return regs.control >> 8;

  //GRSWP
  case 0x0400'0002: return regs.greenswap;
  case 0x0400'0003: return 0u;

  //DISPSTAT
  case 0x0400'0004: return regs.status >> 0;
  case 0x0400'0005: return regs.status >> 8;

  //VCOUNT
  case 0x0400'0006: return regs.vcounter >> 0;
  case 0x0400'0007: return regs.vcounter >> 8;

  //BG0CNT,BG1CNT,BG2CNT,BG3CNT
  case 0x0400'0008: case 0x0400'0009:
  case 0x0400'000a: case 0x0400'000b:
  case 0x0400'000c: case 0x0400'000d:
  case 0x0400'000e: case 0x0400'000f: {
    auto& bg = regs.bg[(addr >> 1) & 3];
    unsigned shift = (addr & 1) * 8;
    return bg.control >> shift;
  }

  //WININ
  case 0x0400'0048: return regs.windowflags[In0];
  case 0x0400'0049: return regs.windowflags[In1];
  case 0x0400'004a: return regs.windowflags[Out];
  case 0x0400'004b: return regs.windowflags[Obj];

  //BLTCNT
  case 0x0400'0050: return regs.blend.control >> 0;
  case 0x0400'0051: return regs.blend.control >> 8;

  //BLDALPHA
  case 0x0400'0052: return regs.blend.eva;
  case 0x0400'0053: return regs.blend.evb;

  //BLDY is write-only

  }

  return 0u;
}

auto PPU::write(uint32 addr, uint8 byte) -> void {
  switch(addr) {

  //DISPCNT
  case 0x0400'0000: regs.control = (regs.control & 0xff00) | (byte << 0); return;
  case 0x0400'0001: regs.control = (regs.control & 0x00ff) | (byte << 8); return;

  //GRSWP
  case 0x0400'0002: regs.greenswap = byte >> 0; return;
  case 0x0400'0003: return;

  //DISPSTAT
  case 0x0400'0004:
    regs.status.irqvblank       = byte >> 3;
    regs.status.irqhblank       = byte >> 4;
    regs.status.irqvcoincidence = byte >> 5;
    return;
  case 0x0400'0005:
    regs.status.vcompare = byte;
    return;

  //BG0CNT,BG1CNT,BG2CNT,BG3CNT
  case 0x0400'0008: case 0x0400'0009:
  case 0x0400'000a: case 0x0400'000b:
  case 0x0400'000c: case 0x0400'000d:
  case 0x0400'000e: case 0x0400'000f: {
    auto& bg = regs.bg[(addr >> 1) & 3];
    unsigned shift = (addr & 1) * 8;
    if(addr == 0x04000009 || addr == 0x0400000b) byte &= 0xdf;  //clear affine wrap for BG0,1
    bg.control = (bg.control & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG0HOFS,BG1HOFS,BG2BOFS,BG3HOFS
  case 0x0400'0010: case 0x0400'0011:
  case 0x0400'0014: case 0x0400'0015:
  case 0x0400'0018: case 0x0400'0019:
  case 0x0400'001c: case 0x0400'001d: {
    auto& bg = regs.bg[(addr >> 2) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.hoffset = (bg.hoffset & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG0VOFS,BG1VOFS,BG2VOFS,BG3VOFS
  case 0x0400'0012: case 0x0400'0013:
  case 0x0400'0016: case 0x0400'0017:
  case 0x0400'001a: case 0x0400'001b:
  case 0x0400'001e: case 0x0400'001f: {
    auto& bg = regs.bg[(addr >> 2) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.voffset = (bg.voffset & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2PA,BG3PA
  case 0x0400'0020: case 0x0400'0021:
  case 0x0400'0030: case 0x0400'0031: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.pa = (bg.pa & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2PB,BG3PB
  case 0x0400'0022: case 0x0400'0023:
  case 0x0400'0032: case 0x0400'0033: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.pb = (bg.pb & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2PC,BG3PC
  case 0x0400'0024: case 0x0400'0025:
  case 0x0400'0034: case 0x0400'0035: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.pc = (bg.pc & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2PD,BG3PD
  case 0x0400'0026: case 0x0400'0027:
  case 0x0400'0036: case 0x0400'0037: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 1) * 8;
    bg.pd = (bg.pd & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2X_L,BG2X_H,BG3X_L,BG3X_H
  case 0x0400'0028: case 0x0400'0029: case 0x0400'002a: case 0x0400'002b:
  case 0x0400'0038: case 0x0400'0039: case 0x0400'003a: case 0x0400'003b: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 3) * 8;
    bg.lx = bg.x = (bg.x & ~(255 << shift)) | (byte << shift);
    return;
  }

  //BG2Y_L,BG2Y_H,BG3Y_L,BG3Y_H
  case 0x0400'002c: case 0x0400'002d: case 0x0400'002e: case 0x0400'002f:
  case 0x0400'003c: case 0x0400'003d: case 0x0400'003e: case 0x0400'003f: {
    auto& bg = regs.bg[(addr >> 4) & 3];
    unsigned shift = (addr & 3) * 8;
    bg.ly = bg.y = (bg.y & ~(255 << shift)) | (byte << shift);
    return;
  }

  //WIN0H
  case 0x0400'0040: regs.window[0].x2 = byte; return;
  case 0x0400'0041: regs.window[0].x1 = byte; return;

  //WIN1H
  case 0x0400'0042: regs.window[1].x2 = byte; return;
  case 0x0400'0043: regs.window[1].x1 = byte; return;

  //WIN0V
  case 0x0400'0044: regs.window[0].y2 = byte; return;
  case 0x0400'0045: regs.window[0].y1 = byte; return;

  //WIN1V
  case 0x0400'0046: regs.window[1].y2 = byte; return;
  case 0x0400'0047: regs.window[1].y1 = byte; return;

  //WININ
  case 0x0400'0048: regs.windowflags[In0] = byte; return;
  case 0x0400'0049: regs.windowflags[In1] = byte; return;

  //WINOUT
  case 0x0400'004a: regs.windowflags[Out] = byte; return;
  case 0x0400'004b: regs.windowflags[Obj] = byte; return;

  //MOSAIC
  case 0x0400'004c:
    regs.mosaic.bghsize = byte >> 0;
    regs.mosaic.bgvsize = byte >> 4;
    return;
  case 0x0400'004d:
    regs.mosaic.objhsize = byte >> 0;
    regs.mosaic.objvsize = byte >> 4;
    return;

  //BLDCNT
  case 0x0400'0050: regs.blend.control = (regs.blend.control & 0xff00) | (byte << 0); return;
  case 0x0400'0051: regs.blend.control = (regs.blend.control & 0x00ff) | (byte << 8); return;

  //BLDALPHA
  case 0x0400'0052: regs.blend.eva = byte & 0x1f; return;
  case 0x0400'0053: regs.blend.evb = byte & 0x1f; return;

  //BLDY
  case 0x0400'0054: regs.blend.evy = byte & 0x1f; return;
  case 0x0400'0055: return;

  }
}

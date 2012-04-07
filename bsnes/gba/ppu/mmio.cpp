uint8 PPU::read(uint32 addr) {
  switch(addr) {

  //DISPCNT
  case 0x04000000: return regs.control >> 0;
  case 0x04000001: return regs.control >> 8;

  //GRSWP
  case 0x04000002: return regs.greenswap;
  case 0x04000003: return 0u;

  //DISPSTAT
  case 0x04000004: return regs.status >> 0;
  case 0x04000005: return regs.status >> 8;

  //VCOUNT
  case 0x04000006: return regs.vcounter >> 0;
  case 0x04000007: return regs.vcounter >> 8;

  //BG0CNT
  case 0x04000008: return regs.bg[0].control >> 0;
  case 0x04000009: return regs.bg[0].control >> 8;

  //BG1CNT
  case 0x0400000a: return regs.bg[1].control >> 0;
  case 0x0400000b: return regs.bg[1].control >> 8;

  //BG2CNT
  case 0x0400000c: return regs.bg[2].control >> 0;
  case 0x0400000d: return regs.bg[2].control >> 8;

  //BG3CNT
  case 0x0400000e: return regs.bg[3].control >> 0;
  case 0x0400000f: return regs.bg[3].control >> 8;

  //WININ
  case 0x04000048: return regs.window[0].in;
  case 0x04000049: return regs.window[1].in;
  case 0x0400004a: return regs.window[0].out;
  case 0x0400004b: return regs.windowobj.in;

  //BLTCNT
  case 0x04000050: return regs.blend.control >> 0;
  case 0x04000051: return regs.blend.control >> 8;

  }

  return 0u;
}

void PPU::write(uint32 addr, uint8 byte) {
  switch(addr) {

  //DISPCNT
  case 0x04000000: regs.control = (regs.control & 0xff00) | (byte << 0); return;
  case 0x04000001: regs.control = (regs.control & 0x00ff) | (byte << 8); return;

  //GRSWP
  case 0x04000002: regs.greenswap = byte & 0x01; return;
  case 0x04000003: return;

  //DISPSTAT
  case 0x04000004:
    regs.status.irqvblank       = byte & (1 << 3);
    regs.status.irqhblank       = byte & (1 << 4);
    regs.status.irqvcoincidence = byte & (1 << 5);
    return;
  case 0x04000005:
    regs.status.vcoincidence = byte;
    return;

  //BG0CNT
  case 0x04000008: regs.bg[0].control = (regs.bg[0].control & 0xff00) | (byte << 0); return;
  case 0x04000009: regs.bg[0].control = (regs.bg[0].control & 0x00ff) | (byte << 8); return;

  //BG1CNT
  case 0x0400000a: regs.bg[1].control = (regs.bg[1].control & 0xff00) | (byte << 0); return;
  case 0x0400000b: regs.bg[1].control = (regs.bg[1].control & 0x00ff) | (byte << 8); return;

  //BG2CNT
  case 0x0400000c: regs.bg[2].control = (regs.bg[2].control & 0xff00) | (byte << 0); return;
  case 0x0400000d: regs.bg[2].control = (regs.bg[2].control & 0x00ff) | (byte << 8); return;

  //BG3CNT
  case 0x0400000e: regs.bg[3].control = (regs.bg[3].control & 0xff00) | (byte << 0); return;
  case 0x0400000f: regs.bg[3].control = (regs.bg[3].control & 0x00ff) | (byte << 8); return;

  //BG0HOFS
  case 0x04000010: regs.bg[0].hoffset = (regs.bg[0].hoffset & 0xff00) | (byte << 0); return;
  case 0x04000011: regs.bg[0].hoffset = (regs.bg[0].hoffset & 0x00ff) | (byte << 8); return;

  //BG0VOFS
  case 0x04000012: regs.bg[0].voffset = (regs.bg[0].voffset & 0xff00) | (byte << 0); return;
  case 0x04000013: regs.bg[0].voffset = (regs.bg[0].voffset & 0x00ff) | (byte << 8); return;

  //BG1HOFS
  case 0x04000014: regs.bg[1].hoffset = (regs.bg[1].hoffset & 0xff00) | (byte << 0); return;
  case 0x04000015: regs.bg[1].hoffset = (regs.bg[1].hoffset & 0x00ff) | (byte << 8); return;

  //BG1VOFS
  case 0x04000016: regs.bg[1].voffset = (regs.bg[1].voffset & 0xff00) | (byte << 0); return;
  case 0x04000017: regs.bg[1].voffset = (regs.bg[1].voffset & 0x00ff) | (byte << 8); return;

  //BG2HOFS
  case 0x04000018: regs.bg[2].hoffset = (regs.bg[2].hoffset & 0xff00) | (byte << 0); return;
  case 0x04000019: regs.bg[2].hoffset = (regs.bg[2].hoffset & 0x00ff) | (byte << 8); return;

  //BG2VOFS
  case 0x0400001a: regs.bg[2].voffset = (regs.bg[2].voffset & 0xff00) | (byte << 0); return;
  case 0x0400001b: regs.bg[2].voffset = (regs.bg[2].voffset & 0x00ff) | (byte << 8); return;

  //BG3HOFS
  case 0x0400001c: regs.bg[3].hoffset = (regs.bg[3].hoffset & 0xff00) | (byte << 0); return;
  case 0x0400001d: regs.bg[3].hoffset = (regs.bg[3].hoffset & 0x00ff) | (byte << 8); return;

  //BG3VOFS
  case 0x0400001e: regs.bg[3].voffset = (regs.bg[3].voffset & 0xff00) | (byte << 0); return;
  case 0x0400001f: regs.bg[3].voffset = (regs.bg[3].voffset & 0x00ff) | (byte << 8); return;

  //BG2PA
  case 0x04000020: regs.bg[2].pa = (regs.bg[2].pa & 0xff00) | (byte << 0); return;
  case 0x04000021: regs.bg[2].pa = (regs.bg[2].pa & 0x00ff) | (byte << 8); return;

  //BG2PB
  case 0x04000022: regs.bg[2].pb = (regs.bg[2].pb & 0xff00) | (byte << 0); return;
  case 0x04000023: regs.bg[2].pb = (regs.bg[2].pb & 0x00ff) | (byte << 8); return;

  //BG2PC
  case 0x04000024: regs.bg[2].pc = (regs.bg[2].pc & 0xff00) | (byte << 0); return;
  case 0x04000025: regs.bg[2].pc = (regs.bg[2].pc & 0x00ff) | (byte << 8); return;

  //BG2PD
  case 0x04000026: regs.bg[2].pd = (regs.bg[2].pd & 0xff00) | (byte << 0); return;
  case 0x04000027: regs.bg[2].pd = (regs.bg[2].pd & 0x00ff) | (byte << 8); return;

  //BG2X_L
  case 0x04000028: regs.bg[2].lx = regs.bg[2].x = (regs.bg[2].x & 0xffffff00) | (byte <<  0); return;
  case 0x04000029: regs.bg[2].lx = regs.bg[2].x = (regs.bg[2].x & 0xffff00ff) | (byte <<  8); return;

  //BG2X_H
  case 0x0400002a: regs.bg[2].lx = regs.bg[2].x = (regs.bg[2].x & 0xff00ffff) | (byte << 16); return;
  case 0x0400002b: regs.bg[2].lx = regs.bg[2].x = (regs.bg[2].x & 0x00ffffff) | (byte << 24); return;

  //BG2Y_L
  case 0x0400002c: regs.bg[2].ly = regs.bg[2].y = (regs.bg[2].y & 0xffffff00) | (byte <<  0); return;
  case 0x0400002d: regs.bg[2].ly = regs.bg[2].y = (regs.bg[2].y & 0xffff00ff) | (byte <<  8); return;

  //BG2Y_H
  case 0x0400002e: regs.bg[2].ly = regs.bg[2].y = (regs.bg[2].y & 0xff00ffff) | (byte << 16); return;
  case 0x0400002f: regs.bg[2].ly = regs.bg[2].y = (regs.bg[2].y & 0x00ffffff) | (byte << 24); return;

  //BG3PA
  case 0x04000030: regs.bg[3].pa = (regs.bg[3].pa & 0xff00) | (byte << 0); return;
  case 0x04000031: regs.bg[3].pa = (regs.bg[3].pa & 0x00ff) | (byte << 8); return;

  //BG3PB
  case 0x04000032: regs.bg[3].pb = (regs.bg[3].pb & 0xff00) | (byte << 0); return;
  case 0x04000033: regs.bg[3].pb = (regs.bg[3].pb & 0x00ff) | (byte << 8); return;

  //BG3PC
  case 0x04000034: regs.bg[3].pc = (regs.bg[3].pc & 0xff00) | (byte << 0); return;
  case 0x04000035: regs.bg[3].pc = (regs.bg[3].pc & 0x00ff) | (byte << 8); return;

  //BG3PD
  case 0x04000036: regs.bg[3].pd = (regs.bg[3].pd & 0xff00) | (byte << 0); return;
  case 0x04000037: regs.bg[3].pd = (regs.bg[3].pd & 0x00ff) | (byte << 8); return;

  //BG3X_L
  case 0x04000038: regs.bg[3].lx = regs.bg[3].x = (regs.bg[3].x & 0xffffff00) | (byte <<  0); return;
  case 0x04000039: regs.bg[3].lx = regs.bg[3].x = (regs.bg[3].x & 0xffff00ff) | (byte <<  8); return;

  //BG3X_H
  case 0x0400003a: regs.bg[3].lx = regs.bg[3].x = (regs.bg[3].x & 0xff00ffff) | (byte << 16); return;
  case 0x0400003b: regs.bg[3].lx = regs.bg[3].x = (regs.bg[3].x & 0x00ffffff) | (byte << 24); return;

  //BG3Y_L
  case 0x0400003c: regs.bg[3].ly = regs.bg[3].y = (regs.bg[3].y & 0xffffff00) | (byte <<  0); return;
  case 0x0400003d: regs.bg[3].ly = regs.bg[3].y = (regs.bg[3].y & 0xffff00ff) | (byte <<  8); return;

  //BG3Y_H
  case 0x0400003e: regs.bg[3].ly = regs.bg[3].y = (regs.bg[3].y & 0xff00ffff) | (byte << 16); return;
  case 0x0400003f: regs.bg[3].ly = regs.bg[3].y = (regs.bg[3].y & 0x00ffffff) | (byte << 24); return;

  //WIN0H
  case 0x04000040: regs.window[0].x2 = byte; return;
  case 0x04000041: regs.window[0].x1 = byte; return;

  //WIN1H
  case 0x04000042: regs.window[1].x2 = byte; return;
  case 0x04000043: regs.window[1].x1 = byte; return;

  //WIN0V
  case 0x04000044: regs.window[0].y2 = byte; return;
  case 0x04000045: regs.window[0].y1 = byte; return;

  //WIN1V
  case 0x04000046: regs.window[1].y2 = byte; return;
  case 0x04000047: regs.window[1].y1 = byte; return;

  //WININ
  case 0x04000048: regs.window[0].in = byte; return;
  case 0x04000049: regs.window[1].in = byte; return;

  //WINOUT
  case 0x0400004a: regs.window[0].out = regs.window[1].out = byte; return;
  case 0x0400004b: regs.windowobj.in = byte; return;

  //MOSAIC
  case 0x0400004c:
    regs.mosaic.bghsize = byte >> 0;
    regs.mosaic.bgvsize = byte >> 4;
    return;
  case 0x0400004d:
    regs.mosaic.objhsize = byte >> 0;
    regs.mosaic.objvsize = byte >> 4;
    return;

  //BLDCNT
  case 0x04000050: regs.blend.control = (regs.blend.control & 0xff00) | (byte << 0); return;
  case 0x04000051: regs.blend.control = (regs.blend.control & 0x00ff) | (byte << 8); return;

  //BLDALPHA
  case 0x04000052: regs.blend.eva = std::min(16, byte & 0x1f); return;
  case 0x04000053: regs.blend.evb = std::min(16, byte & 0x1f); return;

  //BLDY
  case 0x04000054: regs.blend.evy = std::min(16, byte & 0x1f); return;
  case 0x04000055: return;

  }
}

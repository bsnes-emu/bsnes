void PPU::render_objects() {
  if(regs.control.enableobj == false) return;

  for(signed n = 127; n >= 0; n--) {
    auto &obj = object[n];
    uint8 py = regs.vcounter - obj.y;
    if(py >= obj.height << obj.affinesize) continue;  //offscreen
    if(obj.affine == 0 && obj.affinesize == 1) continue;  //hidden

    if(obj.affine == 0) render_object_linear(obj);
    if(obj.affine == 1) render_object_affine(obj);
  }
}

void PPU::render_object_linear(Object &obj) {
  uint8 py = regs.vcounter - obj.y;
  if(obj.vflip) py ^= obj.height - 1;

  unsigned rowsize = regs.control.objmapping == 0 ? 32 >> obj.colors : obj.width / 8;
  unsigned baseaddr = 0x10000 + obj.character * 32;
  uint9 sx = obj.x;

  for(unsigned x = 0; x < obj.width; x++, sx++) {
    if(sx >= 240) continue;
    unsigned px = x;
    if(obj.hflip) px ^= obj.width - 1;

    unsigned offset = (py / 8) * rowsize + (px / 8);
    if(obj.colors == 0) offset = baseaddr + offset * 32 + (py & 7) * 4 + (px & 7) / 2;
    if(obj.colors == 1) offset = baseaddr + offset * 64 + (py & 7) * 8 + (px & 7);

    uint8 color = vram[offset];
    if(obj.colors == 0) color = (px & 1) ? color >> 4 : color & 15;

    if(color) {
      if(obj.colors == 0) layer[obj.priority][sx] = { true, palette(256 + obj.palette * 16 + color) };
      if(obj.colors == 1) layer[obj.priority][sx] = { true, palette(256 + color) };
    }
  }
}

void PPU::render_object_affine(Object &obj) {
  uint8 py = regs.vcounter - obj.y;

  unsigned rowsize = regs.control.objmapping == 0 ? 32 >> obj.colors : obj.width / 8;
  unsigned baseaddr = 0x10000 + obj.character * 32;
  uint9 sx = obj.x;

  int16 pa = objectparam[obj.affineparam].pa;
  int16 pb = objectparam[obj.affineparam].pb;
  int16 pc = objectparam[obj.affineparam].pc;
  int16 pd = objectparam[obj.affineparam].pd;

  //center-of-sprite coordinates
  int16 centerx = obj.width  / 2;
  int16 centery = obj.height / 2;

  //origin coordinates (top-left of sprite)
  int28 originx = -(centerx << obj.affinesize);
  int28 originy = -(centery << obj.affinesize) + py;

  int28 fx = originx * pa + originy * pb;
  int28 fy = originx * pc + originy * pd;

  for(unsigned x = 0; x < (obj.width << obj.affinesize); x++, sx++) {
    unsigned px = (fx >> 8) + centerx;
    unsigned py = (fy >> 8) + centery;

    if(sx < 240 && px < obj.width && py < obj.height) {
      unsigned offset = (py / 8) * rowsize + (px / 8);
      if(obj.colors == 0) offset = baseaddr + offset * 32 + (py & 7) * 4 + (px & 7) / 2;
      if(obj.colors == 1) offset = baseaddr + offset * 64 + (py & 7) * 8 + (px & 7);

      uint8 color = vram[offset];
      if(obj.colors == 0) color = (px & 1) ? color >> 4 : color & 15;

      if(color) {
        if(obj.colors == 0) layer[obj.priority][sx] = { true, palette(256 + obj.palette * 16 + color) };
        if(obj.colors == 1) layer[obj.priority][sx] = { true, palette(256 + color) };
      }
    }

    fx += pa;
    fy += pc;
  }
}

uint32 PPU::oam_read(uint32 addr, uint32 size) {
  uint32 word = 0;

  switch(size) {
  case Word:
    addr &= ~3;
    word |= oam_read(addr + 0) <<  0;
    word |= oam_read(addr + 1) <<  8;
    word |= oam_read(addr + 2) << 16;
    word |= oam_read(addr + 3) << 24;
    break;
  case Half:
    word |= oam_read(addr + 0) <<  0;
    word |= oam_read(addr + 1) <<  8;
    break;
  case Byte:
    word |= oam_read(addr + 0) <<  0;
    break;
  }

  return word;
}

//16-bit bus (8-bit writes are ignored)
void PPU::oam_write(uint32 addr, uint32 size, uint32 word) {
  switch(size) {
  case Word:
    addr &= ~3;
    oam_write(addr + 0, word >>  0);
    oam_write(addr + 1, word >>  8);
    oam_write(addr + 2, word >> 16);
    oam_write(addr + 3, word >> 24);
    break;
  case Half:
    addr &= ~1;
    oam_write(addr + 0, word >>  0);
    oam_write(addr + 1, word >>  8);
    break;
  }
}

uint8 PPU::oam_read(uint32 addr) {
  auto &obj = object[(addr >> 3) & 127];
  auto &par = objectparam[(addr >> 5) & 31];

  switch(addr & 7) {
  case 0: return (obj.y);
  case 1: return (obj.affine << 0) + (obj.affinesize << 1) + (obj.mode << 2) + (obj.mosaic << 4) + (obj.colors << 5) + (obj.shape << 6);
  case 2: return (obj.x >> 0);
  case 3: return (obj.x >> 8) + (obj.affineparam << 1) + (obj.hflip << 4) + (obj.vflip << 5) + (obj.size << 6);
  case 4: return (obj.character >> 0);
  case 5: return (obj.character >> 8) + (obj.priority << 2) + (obj.palette << 4);
  case 6:
    switch((addr >> 3) & 3) {
    case 0: return par.pa >> 0;
    case 1: return par.pb >> 0;
    case 2: return par.pc >> 0;
    case 3: return par.pd >> 0;
    }
  case 7:
    switch((addr >> 3) & 3) {
    case 0: return par.pa >> 8;
    case 1: return par.pb >> 8;
    case 2: return par.pc >> 8;
    case 3: return par.pd >> 8;
    }
  }
}

void PPU::oam_write(uint32 addr, uint8 byte) {
  auto &obj = object[(addr >> 3) & 127];
  auto &par = objectparam[(addr >> 5) & 31];

  switch(addr & 7) {
  case 0:
    obj.y = byte;
    break;
  case 1:
    obj.affine     = byte >> 0;
    obj.affinesize = byte >> 1;
    obj.mode       = byte >> 2;
    obj.mosaic     = byte >> 4;
    obj.colors     = byte >> 5;
    obj.shape      = byte >> 6;
    break;
  case 2:
    obj.x = (obj.x & 0xff00) | (byte << 0);
    break;
  case 3:
    obj.x = (obj.x & 0x00ff) | (byte << 8);
    obj.affineparam = byte >> 1;
    obj.hflip       = byte >> 4;
    obj.vflip       = byte >> 5;
    obj.size        = byte >> 6;
    break;
  case 4:
    obj.character = (obj.character & 0xff00) | (byte << 0);
    break;
  case 5:
    obj.character = (obj.character & 0x00ff) | (byte << 8);
    obj.priority = byte >> 2;
    obj.palette  = byte >> 4;
    break;
  case 6:
    switch((addr >> 3) & 3) {
    case 0: par.pa = (par.pa & 0xff00) | (byte << 0); break;
    case 1: par.pb = (par.pb & 0xff00) | (byte << 0); break;
    case 2: par.pc = (par.pc & 0xff00) | (byte << 0); break;
    case 3: par.pd = (par.pd & 0xff00) | (byte << 0); break;
    }
    break;
  case 7:
    switch((addr >> 3) & 3) {
    case 0: par.pa = (par.pa & 0x00ff) | (byte << 8); break;
    case 1: par.pb = (par.pb & 0x00ff) | (byte << 8); break;
    case 2: par.pc = (par.pc & 0x00ff) | (byte << 8); break;
    case 3: par.pd = (par.pd & 0x00ff) | (byte << 8); break;
    }
    break;
  }

  static unsigned widths[] = {
     8, 16, 32, 64,
    16, 32, 32, 64,
     8,  8, 16, 32,
     0,  0,  0,  0,  //8?
  };

  static unsigned heights[] = {
     8, 16, 32, 64,
     8,  8, 16, 32,
    16, 32, 32, 64,
     0,  0,  0,  0,  //8?
  };

  obj.width  = widths [obj.shape * 4 + obj.size];
  obj.height = heights[obj.shape * 4 + obj.size];
}

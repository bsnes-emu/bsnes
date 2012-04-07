void PPU::render_objects() {
  if(regs.control.enableobj == false) return;

  for(signed n = 127; n >= 0; n--) {
    auto &obj = object[n];
    uint16 attr0 = oam.read(n * 8 + 0, Half);
    uint16 attr1 = oam.read(n * 8 + 2, Half);
    uint16 attr2 = oam.read(n * 8 + 4, Half);

    obj.y           = attr0 >>  0;
    obj.affine      = attr0 >>  8;
    obj.affinesize  = attr0 >>  9;
    obj.mode        = attr0 >> 10;
    obj.mosaic      = attr0 >> 12;
    obj.colors      = attr0 >> 13;
    obj.shape       = attr0 >> 14;

    obj.x           = attr1 >>  0;
    obj.affineparam = attr1 >>  9;
    obj.hflip       = attr1 >> 12;
    obj.vflip       = attr1 >> 13;
    obj.size        = attr1 >> 14;

    obj.character   = attr2 >>  0;
    obj.priority    = attr2 >> 10;
    obj.palette     = attr2 >> 12;

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

    uint8 py = regs.vcounter - obj.y;
    if(py >= obj.height << obj.affinesize) continue;
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

  int16 pa = oam.read(obj.affineparam * 32 + 0x06, Half);
  int16 pb = oam.read(obj.affineparam * 32 + 0x0e, Half);
  int16 pc = oam.read(obj.affineparam * 32 + 0x16, Half);
  int16 pd = oam.read(obj.affineparam * 32 + 0x1e, Half);

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

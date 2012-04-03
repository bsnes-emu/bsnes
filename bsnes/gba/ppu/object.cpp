void PPU::render_objects() {
  for(unsigned n = 0; n < 240; n++) pixel[n].exists = false;

  for(unsigned n = 0; n < 128; n++) {
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

    if(regs.vcounter < obj.y) continue;
    if(regs.vcounter >= (obj.y + obj.height) & 255) continue;

    if(obj.affine == 0 && obj.affinesize == 1) continue;  //invalid mode
    if(obj.affine == 0) render_object_linear(obj);
    if(obj.affine == 1) render_object_affine(obj);
  }
}

void PPU::render_object_linear(Object &obj) {
  unsigned py = regs.vcounter - obj.y;
  if(obj.vflip) py ^= obj.height - 1;

  unsigned rowsize = regs.control.objmapping == 0 ? 32 >> obj.colors : obj.width / 8;
  unsigned baseaddr = 0x10000 + obj.character * 32;
  uint9 sx = obj.x;

  for(unsigned x = 0; x < obj.width; x++) {
    if(sx++ >= 240) continue;
    unsigned px = x;
    if(obj.hflip) px ^= obj.width - 1;

    unsigned offset = (py / 8) * rowsize + (px / 8);
    if(obj.colors == 0) offset = baseaddr + offset * 32 + (py & 7) * 4 + (px & 7) / 2;
    if(obj.colors == 1) offset = baseaddr + offset * 64 + (py & 7) * 8 + (px & 7);

    uint8 color = vram[offset];
    if(obj.colors == 0) color = (px & 1) ? color >> 4 : color & 15;
    if(color == 0) continue;  //transparent

    if(obj.colors == 0) pixel[sx] = { true, palette(256 + obj.palette * 16 + color), obj.priority };
    if(obj.colors == 1) pixel[sx] = { true, palette(256 + color), obj.priority };
  }
}

void PPU::render_object_affine(Object &obj) {
}

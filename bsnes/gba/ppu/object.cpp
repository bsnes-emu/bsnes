void PPU::render_objects() {
  if(regs.control.enable[OBJ] == false) return;

  for(unsigned n = 0; n < 128; n++) {
    auto &obj = object[n];
    uint8 py = regs.vcounter - obj.y;
    if(py >= obj.height << obj.affinesize) continue;  //offscreen
    if(obj.affine == 0 && obj.affinesize == 1) continue;  //hidden

    if(obj.affine == 0) render_object_linear(obj);
    if(obj.affine == 1) render_object_affine(obj);
  }
}

void PPU::render_object_linear(Object &obj) {
  auto &output = layer[OBJ];
  uint8 py = regs.vcounter - obj.y;
  if(obj.vflip) py ^= obj.height - 1;

  unsigned rowsize = regs.control.objmapping == 0 ? 32 >> obj.colors : obj.width / 8;
  unsigned baseaddr = 0x10000 + obj.character * 32;
  uint9 sx = obj.x;

  for(unsigned x = 0; x < obj.width; x++, sx++) {
    unsigned px = x;
    if(obj.hflip) px ^= obj.width - 1;

    if(sx < 240) {
      render_object_pixel(obj, sx, px, py, rowsize, baseaddr);
    }
  }
}

void PPU::render_object_affine(Object &obj) {
  auto &output = layer[OBJ];
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
      render_object_pixel(obj, sx, px, py, rowsize, baseaddr);
    }

    fx += pa;
    fy += pc;
  }
}

void PPU::render_object_pixel(Object &obj, unsigned x, unsigned px, unsigned py, unsigned rowsize, unsigned baseaddr) {
  auto &output = layer[OBJ];

  unsigned offset = (py / 8) * rowsize + (px / 8);
  if(obj.colors == 0) offset = baseaddr + offset * 32 + (py & 7) * 4 + (px & 7) / 2;
  if(obj.colors == 1) offset = baseaddr + offset * 64 + (py & 7) * 8 + (px & 7);

  uint8 color = vram[offset];
  if(obj.colors == 0) color = (px & 1) ? color >> 4 : color & 15;

  if(color == 0) return;  //transparent

  if(obj.mode == 2) {
    windowmask[Obj][x] = true;
    return;
  }

  if(output[x].enable == false || obj.priority < output[x].priority) {
    if(obj.colors == 0) output[x] = { true, obj.mode == 1, obj.priority, pram[256 + obj.palette * 16 + color] };
    if(obj.colors == 1) output[x] = { true, obj.mode == 1, obj.priority, pram[256 + color] };
  }
}

/*
//px,py = pixel coordinates within sprite [0,0 - width,height)
//fx,fy = affine pixel coordinates
//pa,pb,pc,pd = affine pixel adjustments
//x,y = adjusted coordinates within sprite (linear = vflip/hflip, affine = rotation/zoom)
auto PPU::renderObject(ObjectInfo& obj) -> void {
  uint8 py = regs.vcounter - obj.y;
  if(obj.affine == 0 && obj.affinesize == 1) return;  //hidden
  if(py >= obj.height << obj.affinesize) return;  //offscreen

  auto& output = layer[OBJ];
  uint rowsize = regs.control.objmapping == 0 ? 32 >> obj.colors : obj.width / 8;
  uint baseaddr = obj.character * 32;

  if(obj.mosaic && regs.mosaic.objvsize) {
    int mosaicy = (regs.vcounter / (1 + regs.mosaic.objvsize)) * (1 + regs.mosaic.objvsize);
    py = obj.y >= 160 || mosaicy - obj.y >= 0 ? mosaicy - obj.y : 0;
  }

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

  //fractional pixel coordinates
  int28 fx = originx * pa + originy * pb;
  int28 fy = originx * pc + originy * pd;

  for(uint px = 0; px < (obj.width << obj.affinesize); px++) {
    uint x, y;
    if(obj.affine == 0) {
      x = px;
      y = py;
      if(obj.hflip) x ^= obj.width  - 1;
      if(obj.vflip) y ^= obj.height - 1;
    } else {
      x = (fx >> 8) + centerx;
      y = (fy >> 8) + centery;
    }

    uint9 ox = obj.x + px;
    if(ox < 240 && x < obj.width && y < obj.height) {
      uint offset = (y / 8) * rowsize + (x / 8);
      offset = offset * 64 + (y & 7) * 8 + (x & 7);

      uint8 color = readObjectVRAM(baseaddr + (offset >> !obj.colors));
      if(obj.colors == 0) color = (x & 1) ? color >> 4 : color & 15;
      if(color) {
        if(obj.mode & 2) {
          windowmask[Obj][ox] = true;
        } else if(output[ox].enable == false || obj.priority < output[ox].priority) {
          if(obj.colors == 0) color = obj.palette * 16 + color;
          output[ox].write(true, obj.priority, pram[256 + color], obj.mode == 1, obj.mosaic);
        }
      }
    }

    fx += pa;
    fy += pc;
  }
}
*/

auto PPU::Objects::scanline(uint y) -> void {
  for(auto& pixel : buffer) pixel = {};
  if(ppu.blank() || !io.enable) return;

  for(auto& object : ppu.object) {
    uint8 py = y - object.y;
    if(object.affine == 0 && object.affineSize == 1) continue;  //hidden
    if(py >= object.height << object.affineSize) continue;  //offscreen

    uint rowSize = io.mapping == 0 ? 32 >> object.colors : object.width >> 3;
    uint baseAddress = object.character << 5;

    if(object.mosaic && io.mosaicHeight) {
      int mosaicY = (y / (1 + io.mosaicHeight)) * (1 + io.mosaicHeight);
      py = object.y >= 160 || mosaicY - object.y >= 0 ? mosaicY - object.y : 0;
    }

    int16 pa = ppu.objectParam[object.affineParam].pa;
    int16 pb = ppu.objectParam[object.affineParam].pb;
    int16 pc = ppu.objectParam[object.affineParam].pc;
    int16 pd = ppu.objectParam[object.affineParam].pd;

    //center-of-sprite coordinates
    int16 centerX = object.width  >> 1;
    int16 centerY = object.height >> 1;

    //origin coordinates (top-left of sprite)
    int28 originX = -(centerX << object.affineSize);
    int28 originY = -(centerY << object.affineSize) + py;

    //fractional pixel coordinates
    int28 fx = originX * pa + originY * pb;
    int28 fy = originX * pc + originY * pd;

    for(uint px : range(object.width << object.affineSize)) {
      uint sx, sy;
      if(!object.affine) {
        sx = px ^ (object.hflip ? object.width  - 1 : 0);
        sy = py ^ (object.vflip ? object.height - 1 : 0);
      } else {
        sx = (fx >> 8) + centerX;
        sy = (fy >> 8) + centerY;
      }

      uint9 bx = object.x + px;
      if(bx < 240 && sx < object.width && sy < object.height) {
        uint offset = (sy >> 3) * rowSize + (sx >> 3);
        offset = offset * 64 + (sy & 7) * 8 + (sx & 7);

        uint8 color = ppu.readObjectVRAM(baseAddress + (offset >> !object.colors));
        if(object.colors == 0) color = sx & 1 ? color >> 4 : color & 15;
        if(color) {
          if(object.mode & 2) {
            buffer[bx].window = true;
          } else if(!buffer[bx].enable || object.priority < buffer[bx].priority) {
            if(object.colors == 0) color = object.palette * 16 + color;
            buffer[bx].enable = true;
            buffer[bx].priority = object.priority;
            buffer[bx].color = ppu.pram[256 + color];
            buffer[bx].translucent = object.mode == 1;
            buffer[bx].mosaic = object.mosaic;
          }
        }
      }

      fx += pa;
      fy += pc;
    }
  }
}

auto PPU::Objects::run(uint x, uint y) -> void {
  output = {};
  if(ppu.blank() || !io.enable) return;

  output = buffer[x];
}

auto PPU::Objects::power() -> void {
  memory::fill(&io, sizeof(IO));
}

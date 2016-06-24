auto PPUcounter::serialize(serializer& s) -> void {
  s.integer(status.interlace);
  s.integer(status.field);
  s.integer(status.vcounter);
  s.integer(status.hcounter);

  s.array(history.field);
  s.array(history.vcounter);
  s.array(history.hcounter);
  s.integer(history.index);
}

auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);
  PPUcounter::serialize(s);

  s.integer(vram.size);
  s.array(vram.data, vram.size);
  s.array(oam);
  s.array(cgram);

  s.integer(ppu1.version);
  s.integer(ppu1.mdr);

  s.integer(ppu2.version);
  s.integer(ppu2.mdr);

  s.integer(display.interlace);
  s.integer(display.overscan);

  s.integer(latch.vram);
  s.integer(latch.oam);
  s.integer(latch.cgram);
  s.integer(latch.bgofs);
  s.integer(latch.mode7);
  s.integer(latch.counters);
  s.integer(latch.hcounter);
  s.integer(latch.vcounter);

  s.integer(latch.oamAddress);
  s.integer(latch.cgramAddress);

  s.integer(r.displayDisable);
  s.integer(r.displayBrightness);

  s.integer(r.oamBaseAddress);
  s.integer(r.oamAddress);
  s.integer(r.oamPriority);

  s.integer(r.bgPriority);
  s.integer(r.bgMode);

  s.integer(r.hoffsetMode7);
  s.integer(r.voffsetMode7);

  s.integer(r.vramIncrementMode);
  s.integer(r.vramMapping);
  s.integer(r.vramIncrementSize);

  s.integer(r.vramAddress);

  s.integer(r.repeatMode7);
  s.integer(r.vflipMode7);
  s.integer(r.hflipMode7);

  s.integer(r.m7a);
  s.integer(r.m7b);
  s.integer(r.m7c);
  s.integer(r.m7d);
  s.integer(r.m7x);
  s.integer(r.m7y);

  s.integer(r.cgramAddress);

  s.integer(r.extbg);
  s.integer(r.pseudoHires);
  s.integer(r.overscan);
  s.integer(r.interlace);

  s.integer(r.hcounter);
  s.integer(r.vcounter);

  bg1.serialize(s);
  bg2.serialize(s);
  bg3.serialize(s);
  bg4.serialize(s);
  obj.serialize(s);
  window.serialize(s);
  screen.serialize(s);
}

auto PPU::Background::serialize(serializer& s) -> void {
  s.integer(r.tiledataAddress);
  s.integer(r.screenAddress);
  s.integer(r.screenSize);
  s.integer(r.mosaic);
  s.integer(r.tileSize);

  s.integer(r.mode);
  s.array(r.priority);

  s.integer(r.aboveEnable);
  s.integer(r.belowEnable);

  s.integer(r.hoffset);
  s.integer(r.voffset);

  s.integer(latch.hoffset);
  s.integer(latch.voffset);

  s.integer(output.above.priority);
  s.integer(output.above.palette);
  s.integer(output.above.tile);

  s.integer(output.below.priority);
  s.integer(output.below.palette);
  s.integer(output.below.tile);

  s.integer(x);
  s.integer(y);

  s.integer(mosaic.priority);
  s.integer(mosaic.palette);
  s.integer(mosaic.tile);

  s.integer(mosaic.vcounter);
  s.integer(mosaic.voffset);
  s.integer(mosaic.hcounter);
  s.integer(mosaic.hoffset);

  s.integer(tileCounter);
  s.integer(tile);
  s.integer(priority);
  s.integer(paletteNumber);
  s.integer(paletteIndex);
  s.array(data);
}

auto PPU::Object::serialize(serializer& s) -> void {
  s.integer(r.aboveEnable);
  s.integer(r.belowEnable);
  s.integer(r.interlace);

  s.integer(r.baseSize);
  s.integer(r.nameSelect);
  s.integer(r.tiledataAddress);
  s.integer(r.firstSprite);

  s.array(r.priority);

  s.integer(r.timeOver);
  s.integer(r.rangeOver);

  s.integer(t.x);
  s.integer(t.y);

  s.integer(t.itemCount);
  s.integer(t.tileCount);

  s.integer(t.active);
  for(auto p : range(2)) {
    for(auto n : range(32)) {
      s.integer(t.item[p][n].valid);
      s.integer(t.item[p][n].index);
    }
    for(auto n : range(34)) {
      s.integer(t.tile[p][n].valid);
      s.integer(t.tile[p][n].x);
      s.integer(t.tile[p][n].priority);
      s.integer(t.tile[p][n].palette);
      s.integer(t.tile[p][n].hflip);
      s.integer(t.tile[p][n].data);
    }
  }

  s.integer(output.above.priority);
  s.integer(output.above.palette);

  s.integer(output.below.priority);
  s.integer(output.below.palette);

  for(auto n : range(128)) {
    s.integer(list[n].x);
    s.integer(list[n].y);
    s.integer(list[n].character);
    s.integer(list[n].nameSelect);
    s.integer(list[n].vflip);
    s.integer(list[n].hflip);
    s.integer(list[n].priority);
    s.integer(list[n].palette);
    s.integer(list[n].size);
  }
}

auto PPU::Window::serialize(serializer& s) -> void {
  s.integer(r.bg1.oneEnable);
  s.integer(r.bg1.oneInvert);
  s.integer(r.bg1.twoEnable);
  s.integer(r.bg1.twoInvert);
  s.integer(r.bg1.mask);
  s.integer(r.bg1.aboveEnable);
  s.integer(r.bg1.belowEnable);

  s.integer(r.bg2.oneEnable);
  s.integer(r.bg2.oneInvert);
  s.integer(r.bg2.twoEnable);
  s.integer(r.bg2.twoInvert);
  s.integer(r.bg2.mask);
  s.integer(r.bg2.aboveEnable);
  s.integer(r.bg2.belowEnable);

  s.integer(r.bg3.oneEnable);
  s.integer(r.bg3.oneInvert);
  s.integer(r.bg3.twoEnable);
  s.integer(r.bg3.twoInvert);
  s.integer(r.bg3.mask);
  s.integer(r.bg3.aboveEnable);
  s.integer(r.bg3.belowEnable);

  s.integer(r.bg4.oneEnable);
  s.integer(r.bg4.oneInvert);
  s.integer(r.bg4.twoEnable);
  s.integer(r.bg4.twoInvert);
  s.integer(r.bg4.mask);
  s.integer(r.bg4.aboveEnable);
  s.integer(r.bg4.belowEnable);

  s.integer(r.obj.oneEnable);
  s.integer(r.obj.oneInvert);
  s.integer(r.obj.twoEnable);
  s.integer(r.obj.twoInvert);
  s.integer(r.obj.mask);
  s.integer(r.obj.aboveEnable);
  s.integer(r.obj.belowEnable);

  s.integer(r.col.oneEnable);
  s.integer(r.col.oneInvert);
  s.integer(r.col.twoEnable);
  s.integer(r.col.twoInvert);
  s.integer(r.col.mask);
  s.integer(r.col.aboveMask);
  s.integer(r.col.belowMask);

  s.integer(r.oneLeft);
  s.integer(r.oneRight);
  s.integer(r.twoLeft);
  s.integer(r.twoRight);

  s.integer(output.above.colorEnable);
  s.integer(output.below.colorEnable);

  s.integer(x);
}

auto PPU::Screen::serialize(serializer& s) -> void {
  s.integer(r.blendMode);
  s.integer(r.directColor);

  s.integer(r.colorMode);
  s.integer(r.colorHalve);
  s.integer(r.bg1.colorEnable);
  s.integer(r.bg2.colorEnable);
  s.integer(r.bg3.colorEnable);
  s.integer(r.bg4.colorEnable);
  s.integer(r.obj.colorEnable);
  s.integer(r.back.colorEnable);

  s.integer(r.colorBlue);
  s.integer(r.colorGreen);
  s.integer(r.colorRed);

  s.integer(math.above.color);
  s.integer(math.above.colorEnable);
  s.integer(math.below.color);
  s.integer(math.below.colorEnable);
  s.integer(math.transparent);
  s.integer(math.blendMode);
  s.integer(math.colorHalve);
}

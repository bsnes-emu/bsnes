auto PPU::Window::scanline() -> void {
  x = 0;
}

auto PPU::Window::run() -> void {
  bool one = (x >= r.oneLeft && x <= r.oneRight);
  bool two = (x >= r.twoLeft && x <= r.twoRight);
  x++;

  if(test(r.bg1.oneEnable, one ^ r.bg1.oneInvert, r.bg1.twoEnable, two ^ r.bg1.twoInvert, r.bg1.mask)) {
    if(r.bg1.aboveEnable) ppu.bg1.output.above.priority = 0;
    if(r.bg1.belowEnable) ppu.bg1.output.below.priority = 0;
  }

  if(test(r.bg2.oneEnable, one ^ r.bg2.oneInvert, r.bg2.twoEnable, two ^ r.bg2.twoInvert, r.bg2.mask)) {
    if(r.bg2.aboveEnable) ppu.bg2.output.above.priority = 0;
    if(r.bg2.belowEnable) ppu.bg2.output.below.priority = 0;
  }

  if(test(r.bg3.oneEnable, one ^ r.bg3.oneInvert, r.bg3.twoEnable, two ^ r.bg3.twoInvert, r.bg3.mask)) {
    if(r.bg3.aboveEnable) ppu.bg3.output.above.priority = 0;
    if(r.bg3.belowEnable) ppu.bg3.output.below.priority = 0;
  }

  if(test(r.bg4.oneEnable, one ^ r.bg4.oneInvert, r.bg4.twoEnable, two ^ r.bg4.twoInvert, r.bg4.mask)) {
    if(r.bg4.aboveEnable) ppu.bg4.output.above.priority = 0;
    if(r.bg4.belowEnable) ppu.bg4.output.below.priority = 0;
  }

  if(test(r.obj.oneEnable, one ^ r.obj.oneInvert, r.obj.twoEnable, two ^ r.obj.twoInvert, r.obj.mask)) {
    if(r.obj.aboveEnable) ppu.obj.output.above.priority = 0;
    if(r.obj.belowEnable) ppu.obj.output.below.priority = 0;
  }

  bool value = test(r.col.oneEnable, one ^ r.col.oneInvert, r.col.twoEnable, two ^ r.col.twoInvert, r.col.mask);
  bool array[] = {true, value, !value, false};
  output.above.colorEnable = array[r.col.aboveMask];
  output.below.colorEnable = array[r.col.belowMask];
}

auto PPU::Window::test(bool oneEnable, bool one, bool twoEnable, bool two, uint mask) -> bool {
  if(!oneEnable) return two && twoEnable;
  if(!twoEnable) return one;
  if(mask == 0) return (one | two);
  if(mask == 1) return (one & two);
                return (one ^ two) == 3 - mask;
}

auto PPU::Window::reset() -> void {
  r.bg1.oneEnable = random(false);
  r.bg1.oneInvert = random(false);
  r.bg1.twoEnable = random(false);
  r.bg1.twoInvert = random(false);
  r.bg1.mask = random(0);
  r.bg1.aboveEnable = random(false);
  r.bg1.belowEnable = random(false);

  r.bg2.oneEnable = random(false);
  r.bg2.oneInvert = random(false);
  r.bg2.twoEnable = random(false);
  r.bg2.twoInvert = random(false);
  r.bg2.mask = random(0);
  r.bg2.aboveEnable = random(false);
  r.bg2.belowEnable = random(false);

  r.bg3.oneEnable = random(false);
  r.bg3.oneInvert = random(false);
  r.bg3.twoEnable = random(false);
  r.bg3.twoInvert = random(false);
  r.bg3.mask = random(0);
  r.bg3.aboveEnable = random(false);
  r.bg3.belowEnable = random(false);

  r.bg4.oneEnable = random(false);
  r.bg4.oneInvert = random(false);
  r.bg4.twoEnable = random(false);
  r.bg4.twoInvert = random(false);
  r.bg4.mask = random(0);
  r.bg4.aboveEnable = random(false);
  r.bg4.belowEnable = random(false);

  r.obj.oneEnable = random(false);
  r.obj.oneInvert = random(false);
  r.obj.twoEnable = random(false);
  r.obj.twoInvert = random(false);
  r.obj.mask = random(0);
  r.obj.aboveEnable = random(false);
  r.obj.belowEnable = random(false);

  r.col.oneEnable = random(false);
  r.col.oneInvert = random(false);
  r.col.twoEnable = random(false);
  r.col.twoInvert = random(false);
  r.col.mask = random(0);
  r.col.aboveMask = random(0);
  r.col.belowMask = random(0);

  r.oneLeft = random(0x00);
  r.oneRight = random(0x00);
  r.twoLeft = random(0x00);
  r.twoRight = random(0x00);

  output.above.colorEnable = 0;
  output.below.colorEnable = 0;

  x = 0;
}

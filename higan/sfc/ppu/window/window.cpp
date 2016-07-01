auto PPU::Window::scanline() -> void {
  x = 0;
}

auto PPU::Window::run() -> void {
  bool one = (x >= io.oneLeft && x <= io.oneRight);
  bool two = (x >= io.twoLeft && x <= io.twoRight);
  x++;

  if(test(io.bg1.oneEnable, one ^ io.bg1.oneInvert, io.bg1.twoEnable, two ^ io.bg1.twoInvert, io.bg1.mask)) {
    if(io.bg1.aboveEnable) ppu.bg1.output.above.priority = 0;
    if(io.bg1.belowEnable) ppu.bg1.output.below.priority = 0;
  }

  if(test(io.bg2.oneEnable, one ^ io.bg2.oneInvert, io.bg2.twoEnable, two ^ io.bg2.twoInvert, io.bg2.mask)) {
    if(io.bg2.aboveEnable) ppu.bg2.output.above.priority = 0;
    if(io.bg2.belowEnable) ppu.bg2.output.below.priority = 0;
  }

  if(test(io.bg3.oneEnable, one ^ io.bg3.oneInvert, io.bg3.twoEnable, two ^ io.bg3.twoInvert, io.bg3.mask)) {
    if(io.bg3.aboveEnable) ppu.bg3.output.above.priority = 0;
    if(io.bg3.belowEnable) ppu.bg3.output.below.priority = 0;
  }

  if(test(io.bg4.oneEnable, one ^ io.bg4.oneInvert, io.bg4.twoEnable, two ^ io.bg4.twoInvert, io.bg4.mask)) {
    if(io.bg4.aboveEnable) ppu.bg4.output.above.priority = 0;
    if(io.bg4.belowEnable) ppu.bg4.output.below.priority = 0;
  }

  if(test(io.obj.oneEnable, one ^ io.obj.oneInvert, io.obj.twoEnable, two ^ io.obj.twoInvert, io.obj.mask)) {
    if(io.obj.aboveEnable) ppu.obj.output.above.priority = 0;
    if(io.obj.belowEnable) ppu.obj.output.below.priority = 0;
  }

  bool value = test(io.col.oneEnable, one ^ io.col.oneInvert, io.col.twoEnable, two ^ io.col.twoInvert, io.col.mask);
  bool array[] = {true, value, !value, false};
  output.above.colorEnable = array[io.col.aboveMask];
  output.below.colorEnable = array[io.col.belowMask];
}

auto PPU::Window::test(bool oneEnable, bool one, bool twoEnable, bool two, uint mask) -> bool {
  if(!oneEnable) return two && twoEnable;
  if(!twoEnable) return one;
  if(mask == 0) return (one | two);
  if(mask == 1) return (one & two);
                return (one ^ two) == 3 - mask;
}

auto PPU::Window::reset() -> void {
  io.bg1.oneEnable = random(false);
  io.bg1.oneInvert = random(false);
  io.bg1.twoEnable = random(false);
  io.bg1.twoInvert = random(false);
  io.bg1.mask = random(0);
  io.bg1.aboveEnable = random(false);
  io.bg1.belowEnable = random(false);

  io.bg2.oneEnable = random(false);
  io.bg2.oneInvert = random(false);
  io.bg2.twoEnable = random(false);
  io.bg2.twoInvert = random(false);
  io.bg2.mask = random(0);
  io.bg2.aboveEnable = random(false);
  io.bg2.belowEnable = random(false);

  io.bg3.oneEnable = random(false);
  io.bg3.oneInvert = random(false);
  io.bg3.twoEnable = random(false);
  io.bg3.twoInvert = random(false);
  io.bg3.mask = random(0);
  io.bg3.aboveEnable = random(false);
  io.bg3.belowEnable = random(false);

  io.bg4.oneEnable = random(false);
  io.bg4.oneInvert = random(false);
  io.bg4.twoEnable = random(false);
  io.bg4.twoInvert = random(false);
  io.bg4.mask = random(0);
  io.bg4.aboveEnable = random(false);
  io.bg4.belowEnable = random(false);

  io.obj.oneEnable = random(false);
  io.obj.oneInvert = random(false);
  io.obj.twoEnable = random(false);
  io.obj.twoInvert = random(false);
  io.obj.mask = random(0);
  io.obj.aboveEnable = random(false);
  io.obj.belowEnable = random(false);

  io.col.oneEnable = random(false);
  io.col.oneInvert = random(false);
  io.col.twoEnable = random(false);
  io.col.twoInvert = random(false);
  io.col.mask = random(0);
  io.col.aboveMask = random(0);
  io.col.belowMask = random(0);

  io.oneLeft = random(0x00);
  io.oneRight = random(0x00);
  io.twoLeft = random(0x00);
  io.twoRight = random(0x00);

  output.above.colorEnable = 0;
  output.below.colorEnable = 0;

  x = 0;
}

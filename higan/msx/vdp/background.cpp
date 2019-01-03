auto VDP::background(uint8 hoffset, uint8 voffset) -> void {
  if(!io.displayEnable) {
    output.color = io.colorBackground;
    return;
  }

  switch(io.videoMode) {
  case 0: return graphics1(hoffset, voffset);
//case 1: return text1(hoffset, voffset);
  case 2: return graphics2(hoffset, voffset);
  case 3: return multicolor(hoffset, voffset);
  default: output.color = 8; return;  //medium red color to identify unimplemented modes
  }
}

auto VDP::text1(uint8 hoffset, uint8 voffset) -> void {
}

auto VDP::graphics1(uint8 hoffset, uint8 voffset) -> void {
  uint14 nameAddress;
  nameAddress.bits( 0, 4) = hoffset.bits(3,7);
  nameAddress.bits( 5, 9) = voffset.bits(3,7);
  nameAddress.bits(10,13) = io.nameTableAddress;

  uint8 pattern = vram.read(nameAddress);

  uint14 patternAddress;
  patternAddress.bits( 0, 2) = voffset.bits(0,2);
  patternAddress.bits( 3,10) = pattern;
  patternAddress.bits(11,13) = io.patternTableAddress;

  uint14 colorAddress;  //d5 = 0
  colorAddress.bits(0, 4) = pattern.bits(3,7);
  colorAddress.bits(6,13) = io.colorTableAddress;

  uint8 color = vram.read(colorAddress);
  uint3 index = hoffset ^ 7;
  if(!vram.read(patternAddress).bit(index)) {
    output.color = color.bits(0,3);
  } else {
    output.color = color.bits(4,7);
  }
}

auto VDP::graphics2(uint8 hoffset, uint8 voffset) -> void {
  uint14 nameAddress;
  nameAddress.bits( 0, 4) = hoffset.bits(3,7);
  nameAddress.bits( 5, 9) = voffset.bits(3,7);
  nameAddress.bits(10,13) = io.nameTableAddress;

  uint8 pattern = vram.read(nameAddress);

  uint14 patternAddress;
  patternAddress.bits(0, 2) = voffset.bits(0,2);
  patternAddress.bits(3,10) = pattern;
  if(voffset >=  64 && voffset <= 127) patternAddress.bit(11) = io.patternTableAddress.bit(0);
  if(voffset >= 128 && voffset <= 191) patternAddress.bit(12) = io.patternTableAddress.bit(1);
  uint14 colorAddress = patternAddress;
  patternAddress.bit(13) = io.patternTableAddress.bit(2);
  colorAddress.bit(13) = io.colorTableAddress.bit(7);

  uint8 colorMask = io.colorTableAddress.bits(0,6) << 1 | 1;
  uint8 color = vram.read(colorAddress);
  uint3 index = hoffset ^ 7;
  if(!vram.read(patternAddress).bit(index)) {
    output.color = color.bits(0,3);
  } else {
    output.color = color.bits(4,7);
  }
}

auto VDP::multicolor(uint8 hoffset, uint8 voffset) -> void {
}

auto VDP::Background::run(uint8 hoffset, uint9 voffset) -> void {
  output = {};
  switch(vdp.io.mode) {
  case 0b0000: return graphics1(hoffset, voffset);
  case 0b0001: return;
  case 0b0010: return graphics2(hoffset, voffset);
  case 0b0011: return;
  case 0b0100: return;
  case 0b0101: return;
  case 0b0110: return;
  case 0b0111: return;
  case 0b1000: return graphics3(hoffset, voffset, 192);
  case 0b1001: return;
  case 0b1010: return graphics3(hoffset, voffset, 192);
  case 0b1011: return graphics3(hoffset, voffset, 224);
  case 0b1100: return graphics3(hoffset, voffset, 192);
  case 0b1101: return;
  case 0b1110: return graphics3(hoffset, voffset, 240);
  case 0b1111: return graphics3(hoffset, voffset, 192);
  }
}

auto VDP::Background::graphics1(uint8 hoffset, uint9 voffset) -> void {
  uint14 nameTableAddress;
  nameTableAddress.bits( 0, 4) = hoffset.bits(3,7);
  nameTableAddress.bits( 5, 9) = voffset.bits(3,7);
  nameTableAddress.bits(10,13) = vdp.io.nameTableAddress;
  uint8 pattern = vdp.vram[nameTableAddress];

  uint14 patternAddress;
  patternAddress.bits( 0, 2) = voffset.bits(0,2);
  patternAddress.bits( 3,10) = pattern;
  patternAddress.bits(11,13) = vdp.io.patternTableAddress;

  uint14 colorAddress;  //d5 = 0
  colorAddress.bits(0, 4) = pattern.bits(3,7);
  colorAddress.bits(6,13) = vdp.io.colorTableAddress;

  uint8 color = vdp.vram[colorAddress];
  uint3 index = hoffset ^ 7;
  if(!vdp.vram[patternAddress].bit(index)) {
    output.color = color.bits(0,3);
  } else {
    output.color = color.bits(4,7);
  }
}

auto VDP::Background::graphics2(uint8 hoffset, uint9 voffset) -> void {
  uint14 nameTableAddress;
  nameTableAddress.bits( 0, 4) = hoffset.bits(3,7);
  nameTableAddress.bits( 5, 9) = voffset.bits(3,7);
  nameTableAddress.bits(10,13) = vdp.io.nameTableAddress;
  uint8 pattern = vdp.vram[nameTableAddress];

  uint14 patternAddress;
  patternAddress.bits(0, 2) = voffset.bits(0,2);
  patternAddress.bits(3,10) = pattern;
  if(voffset >=  64 && voffset <= 127) patternAddress.bit(11) = vdp.io.patternTableAddress.bit(0);
  if(voffset >= 128 && voffset <= 191) patternAddress.bit(12) = vdp.io.patternTableAddress.bit(1);
  patternAddress.bit(13) = vdp.io.patternTableAddress.bit(2);

  uint14 colorAddress;
  colorAddress.bits(0, 2) = voffset.bits(0,2);
  colorAddress.bits(3,10) = pattern;
  if(voffset >=  64 && voffset <= 127) colorAddress.bit(11) = vdp.io.patternTableAddress.bit(0);
  if(voffset >= 128 && voffset <= 191) colorAddress.bit(12) = vdp.io.patternTableAddress.bit(1);
  colorAddress.bit(13) = vdp.io.colorTableAddress.bit(7);

  uint8 colorMask = vdp.io.colorTableAddress.bits(0,6) << 1 | 1;
  uint8 color = vdp.vram[colorAddress];
  uint3 index = hoffset ^ 7;
  if(!vdp.vram[patternAddress].bit(index)) {
    output.color = color.bits(0,3);
  } else {
    output.color = color.bits(4,7);
  }
}

auto VDP::Background::graphics3(uint8 hoffset, uint9 voffset, uint vlines) -> void {
  if(hoffset < vdp.io.hscroll.bits(0,2)) return;

  if(!vdp.io.horizontalScrollLock || voffset >=  16) hoffset -= vdp.io.hscroll;
  if(!vdp.io.verticalScrollLock   || hoffset <= 191) voffset += vdp.io.vscroll;

  uint14 nameTableAddress;
  if(vlines == 192) {
    if(voffset >= 224) voffset -= 224;
    nameTableAddress.bits( 1, 5) = hoffset.bits(3,7);
    nameTableAddress.bits( 6,10) = voffset.bits(3,7);
    nameTableAddress.bits(11,13) = vdp.io.nameTableAddress.bits(1,3);
  } else {
    voffset += 224;
    nameTableAddress.bits( 1, 5) = hoffset.bits(3,7);
    nameTableAddress.bits( 6,11) = voffset.bits(3,8);
    nameTableAddress.bits(12,13) = vdp.io.nameTableAddress.bits(2,3);
  }

  uint16 pattern;
  pattern.byte(0) = vdp.vram[nameTableAddress | 0];
  pattern.byte(1) = vdp.vram[nameTableAddress | 1];

  if(pattern.bit( 9)) hoffset ^= 7;  //hflip
  if(pattern.bit(10)) voffset ^= 7;  //vflip
  output.palette  = pattern.bit(11);
  output.priority = pattern.bit(12);

  uint14 patternAddress;
  patternAddress.bits(2, 4) = voffset.bits(0,2);
  patternAddress.bits(5,13) = pattern.bits(0,8);

  uint3 index = hoffset ^ 7;
  output.color.bit(0) = vdp.vram[patternAddress | 0].bit(index);
  output.color.bit(1) = vdp.vram[patternAddress | 1].bit(index);
  output.color.bit(2) = vdp.vram[patternAddress | 2].bit(index);
  output.color.bit(3) = vdp.vram[patternAddress | 3].bit(index);

  if(output.color == 0) output.priority = 0;
}

auto VDP::Background::power() -> void {
  output = {};
}

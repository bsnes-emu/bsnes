auto VDP::Background::scanline() -> void {
  state.x = 0;
  state.y = vdp.io.vcounter;
}

auto VDP::Background::run() -> void {
  uint8 hoffset = state.x++;
  uint9 voffset = state.y;

  if(voffset >= vdp.vlines()
  || hoffset < (vdp.io.hscroll & 7)
  ) {
    output.color = 0;
    output.palette = 0;
    output.priority = 0;
    return;
  }

  bool hscroll = !vdp.io.horizontalScrollLock || voffset >= 16;
  bool vscroll = !vdp.io.verticalScrollLock || hoffset <= 191;

  if(hscroll) hoffset -= vdp.io.hscroll;
  if(vscroll) voffset += vdp.io.vscroll;

  uint14 nameTableAddress;
  if(vdp.vlines() == 192) {
    if(voffset >= 224) voffset -= 224;
    nameTableAddress = vdp.io.nameTableAddress << 11;
  } else {
    voffset &= 255;
    nameTableAddress = (vdp.io.nameTableAddress & ~1) << 11 | 0x700;
  }
  nameTableAddress += ((voffset >> 3) << 6) + ((hoffset >> 3) << 1);

  uint16 tiledata;
  tiledata  = vdp.vram[nameTableAddress + 0] << 0;
  tiledata |= vdp.vram[nameTableAddress + 1] << 8;

  uint14 patternAddress = tiledata.bits(0,8) << 5;
  if(tiledata.bit(9)) hoffset ^= 7;
  if(tiledata.bit(10)) voffset ^= 7;
  output.palette = tiledata.bit(11);
  output.priority = tiledata.bit(12);

  auto index = 7 - (hoffset & 7);
  patternAddress += (voffset & 7) << 2;
  output.color.bit(0) = vdp.vram[patternAddress + 0].bit(index);
  output.color.bit(1) = vdp.vram[patternAddress + 1].bit(index);
  output.color.bit(2) = vdp.vram[patternAddress + 2].bit(index);
  output.color.bit(3) = vdp.vram[patternAddress + 3].bit(index);

  if(output.color == 0) output.priority = 0;
}

auto VDP::Background::power() -> void {
  memory::fill(&state, sizeof(State));
  memory::fill(&output, sizeof(Output));
}

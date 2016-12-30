auto VDP::Background::scanline() -> void {
  state.x = 0;
  state.y = vdp.io.vcounter;
}

auto VDP::Background::run() -> void {
  if(state.y >= vdp.vlines()) {
    output.color = 0;
    output.palette = 0;
    output.priority = 0;
    return;
  }

  uint8 hoffset = state.x;
  if(!vdp.io.horizontalScrollLock || hoffset >= 16) hoffset += vdp.io.hscroll;

  uint8 voffset = state.y;
  if(!vdp.io.verticalScrollLock || hoffset <= 191) voffset += vdp.io.vscroll;

  uint14 nameTableAddress;
  if(vdp.vlines() == 192) {
    nameTableAddress = vdp.io.nameTableAddress << 11;
  } else {
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

  auto index = hoffset & 7;
  patternAddress += (voffset & 7) << 2;
  output.color.bit(0) = vdp.vram[patternAddress + 0].bit(index);
  output.color.bit(1) = vdp.vram[patternAddress + 1].bit(index);
  output.color.bit(2) = vdp.vram[patternAddress + 2].bit(index);
  output.color.bit(3) = vdp.vram[patternAddress + 3].bit(index);

  state.x++;
}

auto VDP::Background::power() -> void {
}

auto VDP::Background::reset() -> void {
  memory::fill(&state, sizeof(State));
  memory::fill(&output, sizeof(Output));
}

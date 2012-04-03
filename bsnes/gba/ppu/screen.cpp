uint15 PPU::palette(uint9 index) {
  uint15 result = 0;
  result |= pram[index * 2 + 0] << 0;
  result |= pram[index * 2 + 1] << 8;
  return result;
}

void PPU::render_screen() {
  uint16 *line = output + regs.vcounter * 240;

  for(unsigned x = 0; x < 240; x++) {
    if(pixel[x].exists) line[x] = pixel[x].color;
    else line[x] = palette(0) & 0x7fff;
  }
}

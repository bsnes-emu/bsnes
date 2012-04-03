uint15 PPU::palette(uint9 index) {
  uint15 result = 0;
  result |= pram[index * 2 + 0] << 0;
  result |= pram[index * 2 + 1] << 8;
  return result;
}

void PPU::render_screen() {
  uint16 *line = output + regs.vcounter * 240;

  for(unsigned x = 0; x < 240; x++) {
    uint15 color = palette(0) & 0x7fff;
    for(signed p = 3; p >= 0; p--) {
      if(pixel[3][x].exists && pixel[3][x].priority == p) color = pixel[3][x].color;
      if(pixel[2][x].exists && pixel[2][x].priority == p) color = pixel[2][x].color;
      if(pixel[1][x].exists && pixel[1][x].priority == p) color = pixel[1][x].color;
      if(pixel[0][x].exists && pixel[0][x].priority == p) color = pixel[0][x].color;
      if(pixel[4][x].exists && pixel[4][x].priority == p) color = pixel[4][x].color;
    }
    line[x] = color;
  }
}

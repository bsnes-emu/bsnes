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
    if(layer[3][x].exists) color = layer[3][x].color;
    if(layer[2][x].exists) color = layer[2][x].color;
    if(layer[1][x].exists) color = layer[1][x].color;
    if(layer[0][x].exists) color = layer[0][x].color;
    line[x] = color;
  }
}

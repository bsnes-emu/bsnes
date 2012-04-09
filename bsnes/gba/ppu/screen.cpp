void PPU::render_forceblank() {
  uint16 *line = output + regs.vcounter * 240;
  for(unsigned x = 0; x < 240; x++) line[x] = 0x7fff;
}

void PPU::render_screen() {
  uint16 *line = output + regs.vcounter * 240;
  for(unsigned x = 0; x < 240; x++) {
    auto color = pram[0];
    if(layer[3][x].exists) color = layer[3][x].color;
    if(layer[2][x].exists) color = layer[2][x].color;
    if(layer[1][x].exists) color = layer[1][x].color;
    if(layer[0][x].exists) color = layer[0][x].color;
    line[x] = color;
  }
}

void PPU::render_forceblank() {
  uint16 *line = output + regs.vcounter * 240;
  uint16 *last = blur + regs.vcounter * 240;
  for(unsigned x = 0; x < 240; x++) {
    line[x] = ((last[x] >> 1) & 0x3def) + ((0x7fff >> 1) & 0x3def);
    last[x] = 0x7fff;
  }
}

void PPU::render_screen() {
  uint16 *line = output + regs.vcounter * 240;
  uint16 *last = blur + regs.vcounter * 240;

  for(unsigned x = 0; x < 240; x++) {
    uint16 color = above[x].color;

    switch(regs.blend.control.mode) { default:
    case 0:  //none
      break;
    case 1:  //blend
      if(regs.blend.control.above[above[x].priority & 7] && regs.blend.control.below[below[x].priority & 7]) {
        color = blend(above[x].color, regs.blend.eva, below[x].color, regs.blend.evb);
      }
      break;
    case 2:  //brighten
      if(regs.blend.control.above[above[x].priority & 7]) {
        color = blend(above[x].color, 16 - regs.blend.evy, 0x7fff, regs.blend.evy);
      }
      break;
    case 3:  //darken
      if(regs.blend.control.above[above[x].priority & 7]) {
        color = blend(above[x].color, 16 - regs.blend.evy, 0x0000, regs.blend.evy);
      }
    }

    line[x] = ((last[x] >> 1) & 0x3def) + ((color >> 1) & 0x3def);
    last[x] = color;
  }
}

void PPU::draw(unsigned x, unsigned layer, unsigned priority, unsigned color) {
  priority = (priority << 3) | layer;

  if(priority <= above[x].priority) {
    below[x] = above[x];
    above[x] = { priority, color };
    return;
  }

  if(priority <= below[x].priority) {
    below[x] = { priority, color };
    return;
  }
}

unsigned PPU::blend(unsigned above, unsigned eva, unsigned below, unsigned evb) {
  uint5 ar = above >> 0, ag = above >> 5, ab = above >> 10;
  uint5 br = below >> 0, bg = below >> 6, bb = below >> 10;

  unsigned r = ((ar * eva) + (br * evb)) >> 4;
  unsigned g = ((ag * eva) + (bg * evb)) >> 4;
  unsigned b = ((ab * eva) + (bb * evb)) >> 4;

  return min(31, r) << 0 | min(31, g) << 5 | min(31, b) << 10;
}

#ifdef PPU_CPP

void PPU::screen_render() {
  uint16 *data = output + vcounter() * 1024;
  if(regs.display_disabled) {
    memset(data, 0x00, regs.width << 1);
    return;
  }

  uint16 *table = light_table[regs.display_brightness];
  uint16 color = memory::cgram[0];
  color |= memory::cgram[1] << 8;
  color = table[color];
  for(unsigned i = 0; i < 256; i++) data[i] = color;
}

#endif

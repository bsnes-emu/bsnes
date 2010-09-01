#ifdef PPU_CPP

void PPU::Screen::render_black() {
  uint16 *data = self.output + self.vcounter() * 1024;
  memset(data, 0, self.display.width << 1);
}

void PPU::Screen::render() {
  uint16 *data = self.output + self.vcounter() * 1024;
  uint16 color = memory::cgram[0];
  color |= memory::cgram[1] << 8;
  color = light_table[color];
  for(unsigned i = 0; i < 256; i++) {
    data[i] = color;
  }
}

PPU::Screen::Screen(PPU &self) : self(self) {
  light_tables = new uint16*[16];
  for(unsigned l = 0; l < 16; l++) {
    light_tables[l] = new uint16[32768];
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          light_tables[l][(r << 10) + (g << 5) + (b << 0)] = (ab << 10) + (ag << 5) + (ar << 0);
        }
      }
    }
  }
}

PPU::Screen::~Screen() {
  for(unsigned l = 0; l < 16; l++) delete[] light_tables[l];
  delete[] light_tables;
}

#endif

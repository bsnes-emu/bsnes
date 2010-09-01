#ifdef PPU_CPP

unsigned PPU::Screen::get_palette(unsigned color) {
  color <<= 1;
  return (memory::cgram[color + 0] << 0) + (memory::cgram[color + 1] << 8);
}

void PPU::Screen::scanline() {
  for(unsigned x = 0; x < 256; x++) {
    output.main[x].priority = 0;
    output.main[x].color = 0;
    output.sub[x].priority = 0;
    output.sub[x].color = 0;
  }
}

void PPU::Screen::render_black() {
  uint16 *data = self.output + self.vcounter() * 1024;
  memset(data, 0, self.display.width << 1);
}

void PPU::Screen::render() {
  uint16 *data = self.output + self.vcounter() * 1024;
  for(unsigned i = 0; i < 256; i++) {
    data[i] = light_table[output.main[i].color];
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

void PPU::Screen::Output::plot_main(unsigned x, unsigned color, unsigned priority) {
  if(x & 256) return;
  if(priority > main[x].priority) {
    main[x].priority = priority;
    main[x].color = color;
  }
}

void PPU::Screen::Output::plot_sub(unsigned x, unsigned color, unsigned priority) {
  if(x & 256) return;
  if(priority > sub[x].priority) {
    sub[x].priority = priority;
    sub[x].color = color;
  }
}

#endif

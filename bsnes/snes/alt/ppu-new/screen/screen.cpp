#ifdef PPU_CPP

unsigned PPU::Screen::get_palette(unsigned color) {
  #if defined(ARCH_LSB)
  static uint16 *cgram = (uint16*)memory::cgram.data();
  return cgram[color];
  #else
  color <<= 1;
  return (memory::cgram[color + 0] << 0) + (memory::cgram[color + 1] << 8);
  #endif
}

uint16 PPU::Screen::addsub(unsigned x, unsigned y, bool halve) {
  if(!regs.color_mode) {
    if(!halve) {
      unsigned sum = x + y;
      unsigned carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {
    unsigned diff = x - y + 0x8420;
    unsigned borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

void PPU::Screen::scanline() {
  unsigned color = get_palette(0);

  for(unsigned x = 0; x < 256; x++) {
    output.main[x].color = color;
    output.main[x].priority = 0;
    output.main[x].source = 5;

    output.sub[x].color = regs.color;
    output.sub[x].priority = 0;
    output.sub[x].source = 5;
  }

  window.render(0);
  window.render(1);
}

void PPU::Screen::render_black() {
  uint16 *data = self.output + self.vcounter() * 1024;
  memset(data, 0, self.display.width << 1);
}

uint16 PPU::Screen::get_pixel_main(unsigned x) {
  auto &main = output.main[x];
  auto &sub = output.sub[x];

  if(!regs.addsub_mode) {
    sub.source = 5;
    sub.color = regs.color;
  }

  if(!window.main[x]) {
    if(!window.sub[x]) {
      return 0x0000;
    }
    main.color = 0x0000;
  }

  if(regs.color_enable[main.source] && window.sub[x]) {
    bool halve = false;
    if(regs.color_halve && window.main[x]) {
      if(!regs.addsub_mode || sub.source != 5) halve = true;
    }
    return addsub(main.color, sub.color, halve);
  }

  return main.color;
}

void PPU::Screen::render() {
  uint16 *data = self.output + self.vcounter() * 1024;
  uint16 *light = light_table[self.regs.display_brightness];
  for(unsigned i = 0; i < 256; i++) {
    data[i] = light[get_pixel_main(i)];
  }
}

PPU::Screen::Screen(PPU &self) : self(self) {
  light_table = new uint16*[16];
  for(unsigned l = 0; l < 16; l++) {
    light_table[l] = new uint16[32768];
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          light_table[l][(r << 10) + (g << 5) + (b << 0)] = (ab << 10) + (ag << 5) + (ar << 0);
        }
      }
    }
  }
}

PPU::Screen::~Screen() {
  for(unsigned l = 0; l < 16; l++) delete[] light_table[l];
  delete[] light_table;
}

void PPU::Screen::Output::plot_main(unsigned x, unsigned color, unsigned priority, unsigned source) {
  if(priority > main[x].priority) {
    main[x].color = color;
    main[x].priority = priority;
    main[x].source = source;
  }
}

void PPU::Screen::Output::plot_sub(unsigned x, unsigned color, unsigned priority, unsigned source) {
  if(priority > sub[x].priority) {
    sub[x].color = color;
    sub[x].priority = priority;
    sub[x].source = source;
  }
}

#endif

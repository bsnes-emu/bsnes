#ifdef SPPU_CPP

void sPPU::Screen::scanline() {
  output = self.output + self.vcounter() * 1024;
  if(self.display.interlace && self.field() == 1) output += 512;
}

void sPPU::Screen::run() {
  uint16 color;
  if(self.regs.pseudo_hires == false && self.regs.bgmode != 5 && self.regs.bgmode != 6) {
    color = get_pixel(false);
    *output++ = color;
    *output++ = color;
  } else {
    color = get_pixel(true);
    *output++ = color;
    color = get_pixel(false);
    *output++ = color;
  }
}

uint16 sPPU::Screen::get_pixel(bool swap) {
  enum source_t { BG1, BG2, BG3, BG4, OAM, BACK };
  bool color_enable[] = { regs.bg1_color_enable, regs.bg2_color_enable, regs.bg3_color_enable, regs.bg4_color_enable, regs.oam_color_enable, regs.back_color_enable };

  //===========
  //main screen
  //===========

  unsigned priority_main = 0;
  unsigned color_main;
  unsigned source_main;

  if(self.bg1.output.main.valid) {
    priority_main = self.bg1.output.main.priority;
    if(regs.direct_color && (self.regs.bgmode == 3 || self.regs.bgmode == 4 || self.regs.bgmode == 7)) {
      color_main = get_direct_color(self.bg1.output.main.palette, self.bg1.output.main.palette_number);
    } else {
      color_main = get_color(self.bg1.output.main.palette);
    }
    source_main = BG1;
  }
  if(self.bg2.output.main.valid && self.bg2.output.main.priority > priority_main) {
    priority_main = self.bg2.output.main.priority;
    color_main = get_color(self.bg2.output.main.palette);
    source_main = BG2;
  }
  if(self.bg3.output.main.valid && self.bg3.output.main.priority > priority_main) {
    priority_main = self.bg3.output.main.priority;
    color_main = get_color(self.bg3.output.main.palette);
    source_main = BG3;
  }
  if(self.bg4.output.main.valid && self.bg4.output.main.priority > priority_main) {
    priority_main = self.bg4.output.main.priority;
    color_main = get_color(self.bg4.output.main.palette);
    source_main = BG4;
  }
  if(self.oam.output.main.valid && self.oam.output.main.priority > priority_main) {
    priority_main = self.oam.output.main.priority;
    color_main = get_color(self.oam.output.main.palette);
    source_main = OAM;
  }
  if(priority_main == 0) {
    color_main = get_color(0);
    source_main = BACK;
  }

  //==========
  //sub screen
  //==========

  unsigned priority_sub = 0;
  unsigned color_sub;
  unsigned source_sub;

  if(self.bg1.output.sub.valid) {
    priority_sub = self.bg1.output.sub.priority;
    if(regs.direct_color && (self.regs.bgmode == 3 || self.regs.bgmode == 4 || self.regs.bgmode == 7)) {
      color_sub = get_direct_color(self.bg1.output.sub.palette, self.bg1.output.sub.palette_number);
    } else {
      color_sub = get_color(self.bg1.output.sub.palette);
    }
    source_sub = BG1;
  }
  if(self.bg2.output.sub.valid && self.bg2.output.sub.priority > priority_sub) {
    priority_sub = self.bg2.output.sub.priority;
    color_sub = get_color(self.bg2.output.sub.palette);
    source_sub = BG2;
  }
  if(self.bg3.output.sub.valid && self.bg3.output.sub.priority > priority_sub) {
    priority_sub = self.bg3.output.sub.priority;
    color_sub = get_color(self.bg3.output.sub.palette);
    source_sub = BG3;
  }
  if(self.bg4.output.sub.valid && self.bg4.output.sub.priority > priority_sub) {
    priority_sub = self.bg4.output.sub.priority;
    color_sub = get_color(self.bg4.output.sub.palette);
    source_sub = BG4;
  }
  if(self.oam.output.sub.valid && self.oam.output.sub.priority > priority_sub) {
    priority_sub = self.oam.output.sub.priority;
    color_sub = get_color(self.oam.output.sub.palette);
    source_sub = OAM;
  }
  if(priority_sub == 0) {
    if(self.regs.bgmode == 5 || self.regs.bgmode == 6) {
      color_sub = get_color(0);
    } else {
      color_sub = regs.color_rgb;
    }
    source_sub = BACK;
  }

  if(swap == true) {
    nall::swap(priority_main, priority_sub);
    nall::swap(color_main, color_sub);
    nall::swap(source_main, source_sub);
  }

  uint16 output;
  if(!regs.addsub_mode) {
    source_sub = BACK;
    color_sub = regs.color_rgb;
  }

  if(self.window.output.main.color_enable == false) {
    if(self.window.output.sub.color_enable == false) {
      return 0x0000;
    }
    color_main = 0x0000;
  }

  bool color_exempt = (source_main == OAM && self.oam.output.main.palette < 192);
  if(!color_exempt && color_enable[source_main] && self.window.output.sub.color_enable) {
    bool halve = false;
    if(regs.color_halve && self.window.output.main.color_enable) {
      if(regs.addsub_mode && source_sub == BACK);
      else halve = true;
    }
    output = addsub(color_main, color_sub, halve);
  } else {
    output = color_main;
  }

  //========
  //lighting
  //========

  output = light_table[self.regs.display_brightness][output];
  if(self.regs.display_disabled) output = 0x0000;
  return output;
}

uint16 sPPU::Screen::addsub(unsigned x, unsigned y, bool halve) {
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
      return   (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

uint16 sPPU::Screen::get_color(uint8 n) {
  return memory::cgram[(n << 1) + 0] + (memory::cgram[(n << 1) + 1] << 8);
}

//color   = BBBGGGRR (from tiledata)
//palette = 00000bgr (from tilemap)
//result  = 0BBb00GGGg0RRRr0
uint16 sPPU::Screen::get_direct_color(uint8 color, uint8 palette) {
  return ((color & 7) << 2) | ((palette & 1) << 1)
       | (((color >> 3) & 7) << 7) | (((palette >> 1) & 1) << 6)
       | ((color >> 6) << 13) | ((palette >> 2) << 12);
}

void sPPU::Screen::reset() {
  regs.addsub_mode = 0;
  regs.direct_color = 0;
  regs.color_mode = 0;
  regs.color_halve = 0;
  regs.bg1_color_enable = 0;
  regs.bg2_color_enable = 0;
  regs.bg3_color_enable = 0;
  regs.bg4_color_enable = 0;
  regs.oam_color_enable = 0;
  regs.back_color_enable = 0;
  regs.color_r = 0;
  regs.color_g = 0;
  regs.color_b = 0;
  regs.color_rgb = 0;
}

sPPU::Screen::Screen(sPPU &self) : self(self) {
  //generate light table
  for(unsigned l = 0; l < 16; l++) {
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          light_table[l][(b << 10) + (g << 5) + r] = (ab << 10) + (ag << 5) + ar;
        }
      }
    }
  }
}

#endif

#ifdef SPPU_CPP

void sPPU::Screen::scanline() {
  output = self.output + self.vcounter() * 1024;
}

void sPPU::Screen::run() {
  enum source_t { BG1, BG2, BG3, BG4, OAM, BACK };

  unsigned priority_sub = 0;
  uint16 color_sub;
  source_t source_sub;

  if(self.bg1.output.sub.valid) {
    priority_sub = self.bg1.output.sub.priority;
    color_sub = self.bg1.output.sub.color;
    source_sub = BG1;
  }
  if(self.bg2.output.sub.valid && self.bg2.output.sub.priority > priority_sub) {
    priority_sub = self.bg2.output.sub.priority;
    color_sub = self.bg2.output.sub.color;
    source_sub = BG2;
  }
  if(self.bg3.output.sub.valid && self.bg3.output.sub.priority > priority_sub) {
    priority_sub = self.bg3.output.sub.priority;
    color_sub = self.bg3.output.sub.color;
    source_sub = BG3;
  }
  if(self.bg4.output.sub.valid && self.bg4.output.sub.priority > priority_sub) {
    priority_sub = self.bg4.output.sub.priority;
    color_sub = self.bg4.output.sub.color;
    source_sub = BG4;
  }
  if(self.oam.output.sub.valid && self.oam.output.sub.priority > priority_sub) {
    priority_sub = self.oam.output.sub.priority;
    color_sub = self.oam.output.sub.color;
    source_sub = OAM;
  }
  if(priority_sub == 0) {
    if(self.regs.bgmode == 5 || self.regs.bgmode == 6) {
      color_sub = memory::cgram[0] + (memory::cgram[1] << 8);
    } else {
      color_sub = regs.color_rgb;
    }
    source_sub = BACK;
  }

  unsigned priority_main = 0;
  uint16 color_main;
  source_t source_main;

  if(self.bg1.output.main.valid) {
    priority_main = self.bg1.output.main.priority;
    color_main = self.bg1.output.main.color;
    source_main = BG1;
    if(regs.bg1_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub);
  }
  if(self.bg2.output.main.valid && self.bg2.output.main.priority > priority_main) {
    priority_main = self.bg2.output.main.priority;
    color_main = self.bg2.output.main.color;
    source_main = BG2;
    if(regs.bg2_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub);
  }
  if(self.bg3.output.main.valid && self.bg3.output.main.priority > priority_main) {
    priority_main = self.bg3.output.main.priority;
    color_main = self.bg3.output.main.color;
    source_main = BG3;
    if(regs.bg3_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub);
  }
  if(self.bg4.output.main.valid && self.bg4.output.main.priority > priority_main) {
    priority_main = self.bg4.output.main.priority;
    color_main = self.bg4.output.main.color;
    source_main = BG4;
    if(regs.bg4_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub);
  }
  if(self.oam.output.main.valid && self.oam.output.main.priority > priority_main) {
    priority_main = self.oam.output.main.priority;
    color_main = self.oam.output.main.color;
    source_main = OAM;
    if(self.oam.output.main.palette >= 192) {
      if(regs.oam_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub);
    }
  }
  if(priority_main == 0) {
    color_main = memory::cgram[0] + (memory::cgram[1] << 8);
    source_main = BACK;
    if(regs.back_color_enable && self.window.output.sub.color_enable) color_main = addsub(color_main, color_sub, !regs.addsub_mode || source_sub != BACK);
  }

  if(self.window.output.main.color_enable == false) {
    if(self.window.output.sub.color_enable == false) {
      color_main = 0x0000;
      goto plot;
    }
    color_main = 0x0000;
  }

  color_main = light_table[self.regs.display_brightness][color_main];
  if(self.regs.display_disabled) color_main = 0;
  plot:
  *output++ = color_main;
  *output++ = color_main;
}

uint16 sPPU::Screen::addsub(unsigned x, unsigned y, bool allow_halve) {
  bool halve = allow_halve && regs.color_halve && self.window.output.main.color_enable;

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

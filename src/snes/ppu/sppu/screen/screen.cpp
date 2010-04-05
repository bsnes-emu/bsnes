#ifdef SPPU_CPP

void sPPU::Screen::scanline() {
  output = self.output + self.vcounter() * 1024;
}

void sPPU::Screen::run() {
  unsigned priority = 0;
  uint16 color;

  if(self.bg1.output.main.valid) {
    priority = self.bg1.output.main.priority;
    color = self.bg1.output.main.color;
  }
  if(self.bg2.output.main.valid && self.bg2.output.main.priority > priority) {
    priority = self.bg2.output.main.priority;
    color = self.bg2.output.main.color;
  }
  if(self.bg3.output.main.valid && self.bg3.output.main.priority > priority) {
    priority = self.bg3.output.main.priority;
    color = self.bg3.output.main.color;
  }
  if(self.bg4.output.main.valid && self.bg4.output.main.priority > priority) {
    priority = self.bg4.output.main.priority;
    color = self.bg4.output.main.color;
  }
  if(priority == 0) {
    color = (memory::cgram[1] << 8) + (memory::cgram[0] << 0);
  }

  color = light_table[self.regs.display_brightness][color];
  if(self.regs.display_disabled) color = 0;
  *output++ = color;
  *output++ = color;
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

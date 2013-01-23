#ifdef PPU_CPP

void PPU::Screen::scanline() {
  output = self.output + self.vcounter() * 1024;
  if(self.display.interlace && self.field()) output += 512;

  //the first hires pixel of each scanline is transparent
  //note: exact value initializations are not confirmed on hardware
  math.main.color = get_color(0);
  math.sub.color  = math.main.color;

  math.main.color_enable = !(self.window.regs.col_main_mask & 1);
  math.sub.color_enable  = !(self.window.regs.col_sub_mask  & 1) && regs.back_color_enable;

  math.transparent = true;
  math.addsub_mode = false;
  math.color_halve = regs.color_halve && !regs.addsub_mode && math.main.color_enable;
}

void PPU::Screen::run() {
  if(ppu.vcounter() == 0) return;

  auto palette = &video.palette[self.regs.display_brightness << 15];
  bool hires   = self.regs.pseudo_hires || self.regs.bgmode == 5 || self.regs.bgmode == 6;
  auto sscolor = get_pixel_sub(hires);
  auto mscolor = get_pixel_main();

  *output++ = palette[hires ? sscolor : mscolor];
  *output++ = palette[mscolor];
}

uint16 PPU::Screen::get_pixel_sub(bool hires) {
  if(self.regs.display_disable || (!self.regs.overscan && self.vcounter() >= 225)) return 0;

  unsigned priority = 0;
  if(self.bg1.output.sub.priority) {
    priority = self.bg1.output.sub.priority;
    if(regs.direct_color && (self.regs.bgmode == 3 || self.regs.bgmode == 4 || self.regs.bgmode == 7)) {
      math.sub.color = get_direct_color(self.bg1.output.sub.palette, self.bg1.output.sub.tile);
    } else {
      math.sub.color = get_color(self.bg1.output.sub.palette);
    }
  }
  if(self.bg2.output.sub.priority > priority) {
    priority = self.bg2.output.sub.priority;
    math.sub.color = get_color(self.bg2.output.sub.palette);
  }
  if(self.bg3.output.sub.priority > priority) {
    priority = self.bg3.output.sub.priority;
    math.sub.color = get_color(self.bg3.output.sub.palette);
  }
  if(self.bg4.output.sub.priority > priority) {
    priority = self.bg4.output.sub.priority;
    math.sub.color = get_color(self.bg4.output.sub.palette);
  }
  if(self.sprite.output.sub.priority > priority) {
    priority = self.sprite.output.sub.priority;
    math.sub.color = get_color(self.sprite.output.sub.palette);
  }
  if(math.transparent = (priority == 0)) math.sub.color = get_color(0);

  if(!hires) return 0;
  if(!math.sub.color_enable) return math.main.color_enable ? math.sub.color : 0;

  return addsub(
    math.main.color_enable ? math.sub.color : 0,
    math.addsub_mode ? math.main.color : fixed_color()
  );
}

uint16 PPU::Screen::get_pixel_main() {
  if(self.regs.display_disable || (!self.regs.overscan && self.vcounter() >= 225)) return 0;

  unsigned priority = 0;
  if(self.bg1.output.main.priority) {
    priority = self.bg1.output.main.priority;
    if(regs.direct_color && (self.regs.bgmode == 3 || self.regs.bgmode == 4 || self.regs.bgmode == 7)) {
      math.main.color = get_direct_color(self.bg1.output.main.palette, self.bg1.output.main.tile);
    } else {
      math.main.color = get_color(self.bg1.output.main.palette);
    }
    math.sub.color_enable = regs.bg1_color_enable;
  }
  if(self.bg2.output.main.priority > priority) {
    priority = self.bg2.output.main.priority;
    math.main.color = get_color(self.bg2.output.main.palette);
    math.sub.color_enable = regs.bg2_color_enable;
  }
  if(self.bg3.output.main.priority > priority) {
    priority = self.bg3.output.main.priority;
    math.main.color = get_color(self.bg3.output.main.palette);
    math.sub.color_enable = regs.bg3_color_enable;
  }
  if(self.bg4.output.main.priority > priority) {
    priority = self.bg4.output.main.priority;
    math.main.color = get_color(self.bg4.output.main.palette);
    math.sub.color_enable = regs.bg4_color_enable;
  }
  if(self.sprite.output.main.priority > priority) {
    priority = self.sprite.output.main.priority;
    math.main.color = get_color(self.sprite.output.main.palette);
    math.sub.color_enable = regs.oam_color_enable && self.sprite.output.main.palette >= 192;
  }
  if(priority == 0) {
    math.main.color = get_color(0);
    math.sub.color_enable = regs.back_color_enable;
  }

  if(!self.window.output.sub.color_enable) math.sub.color_enable = false;
  math.main.color_enable = self.window.output.main.color_enable;
  if(!math.sub.color_enable) return math.main.color_enable ? math.main.color : 0;

  if(regs.addsub_mode && math.transparent) {
    math.addsub_mode = false;
    math.color_halve = false;
  } else {
    math.addsub_mode = regs.addsub_mode;
    math.color_halve = regs.color_halve && math.main.color_enable;
  }

  return addsub(
    math.main.color_enable ? math.main.color : 0,
    math.addsub_mode ? math.sub.color : fixed_color()
  );
}

uint16 PPU::Screen::addsub(unsigned x, unsigned y) {
  if(!regs.color_mode) {
    if(!math.color_halve) {
      unsigned sum = x + y;
      unsigned carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {
    unsigned diff = x - y + 0x8420;
    unsigned borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!math.color_halve) {
      return   (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

uint16 PPU::Screen::get_color(unsigned palette) {
  palette <<= 1;
  self.regs.cgram_iaddr = palette;
  return ppu.cgram[palette + 0] + (ppu.cgram[palette + 1] << 8);
}

uint16 PPU::Screen::get_direct_color(unsigned palette, unsigned tile) {
  //palette = -------- BBGGGRRR
  //tile    = ---bgr-- --------
  //output  = 0BBb00GG Gg0RRRr0
  return ((palette << 7) & 0x6000) + ((tile >> 0) & 0x1000)
       + ((palette << 4) & 0x0380) + ((tile >> 5) & 0x0040)
       + ((palette << 2) & 0x001c) + ((tile >> 9) & 0x0002);
}

uint16 PPU::Screen::fixed_color() const {
  return (regs.color_b << 10) | (regs.color_g << 5) | (regs.color_r << 0);
}

void PPU::Screen::reset() {
  regs.addsub_mode = random(false);
  regs.direct_color = random(false);
  regs.color_mode = random(false);
  regs.color_halve = random(false);
  regs.bg1_color_enable = random(false);
  regs.bg2_color_enable = random(false);
  regs.bg3_color_enable = random(false);
  regs.bg4_color_enable = random(false);
  regs.oam_color_enable = random(false);
  regs.back_color_enable = random(false);
  regs.color_r = random(0);
  regs.color_g = random(0);
  regs.color_b = random(0);
}

PPU::Screen::Screen(PPU &self) : self(self) {
}

#endif

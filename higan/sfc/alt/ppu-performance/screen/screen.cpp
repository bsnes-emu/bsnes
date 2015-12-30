PPU::Screen::Screen(PPU& self) : self(self) {
}

auto PPU::Screen::get_palette(uint color) -> uint {
  #if defined(ENDIAN_LSB)
  return ((uint16*)ppu.cgram)[color];
  #else
  color <<= 1;
  return (ppu.cgram[color + 0] << 0) + (ppu.cgram[color + 1] << 8);
  #endif
}

auto PPU::Screen::get_direct_color(uint p, uint t) -> uint {
  return ((t & 7) << 2) | ((p & 1) << 1) |
         (((t >> 3) & 7) << 7) | (((p >> 1) & 1) << 6) |
         ((t >> 6) << 13) | ((p >> 2) << 12);
}

auto PPU::Screen::addsub(uint x, uint y, bool halve) -> uint16 {
  if(!regs.color_mode) {
    if(!halve) {
      uint sum = x + y;
      uint carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {
    uint diff = x - y + 0x8420;
    uint borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

auto PPU::Screen::scanline() -> void {
  uint main_color = get_palette(0);
  uint sub_color = (self.regs.pseudo_hires == false && self.regs.bgmode != 5 && self.regs.bgmode != 6)
                 ? regs.color : main_color;

  for(uint x = 0; x < 256; x++) {
    output.main[x].color = main_color;
    output.main[x].priority = 0;
    output.main[x].source = 6;

    output.sub[x].color = sub_color;
    output.sub[x].priority = 0;
    output.sub[x].source = 6;
  }

  window.render(0);
  window.render(1);
}

auto PPU::Screen::render_black() -> void {
  uint32* data = self.output + self.vcounter() * 1024;
  if(self.interlace() && self.field()) data += 512;
  memset(data, 0, self.display.width << 2);
}

auto PPU::Screen::get_pixel_main(uint x) -> uint16 {
  auto main = output.main[x];
  auto sub = output.sub[x];

  if(!regs.addsub_mode) {
    sub.source = 6;
    sub.color = regs.color;
  }

  if(!window.main[x]) {
    if(!window.sub[x]) {
      return 0x0000;
    }
    main.color = 0x0000;
  }

  if(main.source != 5 && regs.color_enable[main.source] && window.sub[x]) {
    bool halve = false;
    if(regs.color_halve && window.main[x]) {
      if(!regs.addsub_mode || sub.source != 6) halve = true;
    }
    return addsub(main.color, sub.color, halve);
  }

  return main.color;
}

auto PPU::Screen::get_pixel_sub(uint x) -> uint16 {
  auto main = output.sub[x];
  auto sub = output.main[x];

  if(!regs.addsub_mode) {
    sub.source = 6;
    sub.color = regs.color;
  }

  if(!window.main[x]) {
    if(!window.sub[x]) {
      return 0x0000;
    }
    main.color = 0x0000;
  }

  if(main.source != 5 && regs.color_enable[main.source] && window.sub[x]) {
    bool halve = false;
    if(regs.color_halve && window.main[x]) {
      if(!regs.addsub_mode || sub.source != 6) halve = true;
    }
    return addsub(main.color, sub.color, halve);
  }

  return main.color;
}

auto PPU::Screen::render() -> void {
  uint32* data = self.output + self.vcounter() * 1024;
  if(self.interlace() && self.field()) data += 512;

  if(!self.regs.pseudo_hires && self.regs.bgmode != 5 && self.regs.bgmode != 6) {
    for(uint i = 0; i < 256; i++) {
      data[i] = self.regs.display_brightness << 15 | get_pixel_main(i);
    }
  } else {
    for(uint i = 0; i < 256; i++) {
      *data++ = self.regs.display_brightness << 15 | get_pixel_sub(i);
      *data++ = self.regs.display_brightness << 15 | get_pixel_main(i);
    }
  }
}

auto PPU::Screen::Output::plot_main(uint x, uint color, uint priority, uint source) -> void {
  if(priority > main[x].priority) {
    main[x].color = color;
    main[x].priority = priority;
    main[x].source = source;
  }
}

auto PPU::Screen::Output::plot_sub(uint x, uint color, uint priority, uint source) -> void {
  if(priority > sub[x].priority) {
    sub[x].color = color;
    sub[x].priority = priority;
    sub[x].source = source;
  }
}

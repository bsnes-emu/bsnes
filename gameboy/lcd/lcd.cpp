#include <gameboy/gameboy.hpp>

#define LCD_CPP
namespace GameBoy {

#include "mmio/mmio.cpp"
LCD lcd;

void LCD::Main() {
  lcd.main();
}

void LCD::main() {
  while(true) {
    add_clocks(4);

    if(status.lx == 320) {
      if(status.interrupt_hblank) cpu.interrupt_raise(CPU::Interrupt::Stat);
    }
  }
}

void LCD::add_clocks(unsigned clocks) {
  status.lx += clocks;
  if(status.lx >= 456) scanline();

  cpu.clock -= clocks;
  if(cpu.clock <= 0) co_switch(scheduler.active_thread = cpu.thread);
}

void LCD::scanline() {
  status.lx -= 456;
  status.ly++;

  if(status.interrupt_lyc == true) {
    if(status.ly == status.lyc) cpu.interrupt_raise(CPU::Interrupt::Stat);
  }

  if(status.ly == 144) {
    cpu.interrupt_raise(CPU::Interrupt::Vblank);
    if(status.interrupt_vblank) cpu.interrupt_raise(CPU::Interrupt::Stat);
  }
  if(status.ly == 154) frame();

  if(status.ly < 144) render();
}

void LCD::frame() {
  system.interface->video_refresh(screen);
  system.interface->input_poll();
  cpu.mmio_joyp_poll();

  status.ly = 0;
  scheduler.exit();
}

void LCD::render() {
  for(unsigned n = 0; n < 160; n++) line[n] = 0x00;

  if(status.display_enable == true) {
    if(status.bg_enable == true) render_bg();
    if(status.window_display_enable == true) render_window();
    if(status.obj_enable == true) render_obj();
  }

  uint8_t *output = screen + status.ly * 160;
  for(unsigned n = 0; n < 160; n++) output[n] = (3 - line[n]) * 0x55;
}

uint16 LCD::read_tile(bool select, unsigned x, unsigned y) {
  unsigned tmaddr = 0x1800 + (select << 10), tdaddr;
  tmaddr += (((y >> 3) << 5) + (x >> 3)) & 0x03ff;
  if(status.bg_tiledata_select == 0) {
    tdaddr = 0x1000 + ((int8)vram[tmaddr] << 4);
  } else {
    tdaddr = 0x0000 + (vram[tmaddr] << 4);
  }
  tdaddr += (y & 7) << 1;
  return (vram[tdaddr + 0] << 0) | (vram[tdaddr + 1] << 8);
}

void LCD::render_bg() {
  unsigned iy = (status.ly + status.scy) & 255;
  unsigned ix = status.scx, tx = ix & 7;
  uint8 mask = 0x80 >> tx;
  unsigned data = read_tile(status.bg_tilemap_select, ix, iy), palette;

  for(unsigned ox = 0; ox < 160; ox++) {
    palette  = ((data & (mask << 0)) ? 1 : 0);
    palette |= ((data & (mask << 8)) ? 2 : 0);
    mask = (mask >> 1) | (mask << 7);
    line[ox] = status.bgp[palette];

    ix = (ix + 1) & 255;
    tx = (tx + 1) & 7;

    if(tx == 0) data = read_tile(status.bg_tilemap_select, ix, iy);
  }
}

void LCD::render_window() {
  if(status.ly - status.wy >= 144U) return;
  unsigned iy = status.ly - status.wy;
  unsigned ix = (status.wx - 7) & 255, tx = ix & 7;
  uint8 mask = 0x80 >> tx;
  unsigned data = read_tile(status.window_tilemap_select, ix, iy), palette;

  for(unsigned ox = 0; ox < 160; ox++) {
    palette  = ((data & (mask << 0)) ? 1 : 0);
    palette |= ((data & (mask << 8)) ? 2 : 0);
    mask = (mask >> 1) | (mask << 7);
    if(ox - (status.wx - 7) < 160U) line[ox] = status.bgp[palette];

    ix = (ix + 1) & 255;
    tx = (tx + 1) & 7;

    if(tx == 0) data = read_tile(status.window_tilemap_select, ix, iy);
  }
}

void LCD::render_obj() {
  unsigned obj_size = (status.obj_size == 0 ? 8 : 16);

  for(unsigned s = 0; s < 40; s++) {
    unsigned sy = oam[(s << 2) + 0] - 16;
    unsigned sx = oam[(s << 2) + 1] -  8;
    unsigned tile = oam[(s << 2) + 2];
    unsigned attribute = oam[(s << 2) + 3];

    sy = status.ly - sy;
    if(sy >= obj_size) continue;
    if(attribute & 0x40) sy ^= (obj_size - 1);

    unsigned tdaddr = tile * 16 + sy * 2;

    uint8 d0 = vram[tdaddr + 0];
    uint8 d1 = vram[tdaddr + 1];
    unsigned xflip = attribute & 0x20 ? 7 : 0;

    for(unsigned tx = 0; tx < 8; tx++) {
      uint8 palette = ((d0 & 0x80) >> 7) + ((d1 & 0x80) >> 6);
      d0 <<= 1, d1 <<= 1;
      if(palette == 0) continue;

      palette = status.obp[(bool)(attribute & 0x10)][palette];
      unsigned ox = sx + (tx ^ xflip);

      if(ox <= 159) {
        if(attribute & 0x80) {
          if(line[ox] > 0) continue;
        }
        line[ox] = palette;
      }
    }
  }
}

void LCD::power() {
  create(Main, 4 * 1024 * 1024);

  for(unsigned n = 0x8000; n <= 0x9fff; n++) bus.mmio[n] = this;  //VRAM
  for(unsigned n = 0xff40; n <= 0xff4b; n++) bus.mmio[n] = this;  //MMIO
  for(unsigned n = 0xfe00; n <= 0xfe9f; n++) bus.mmio[n] = this;  //OAM

  for(unsigned n = 0; n < 8192; n++) vram[n] = 0x00;
  for(unsigned n = 0; n <  160; n++) oam [n] = 0x00;

  for(unsigned n = 0; n < 160 * 144; n++) screen[n] = 0x00;

  status.lx = 0;

  status.display_enable = 0;
  status.window_tilemap_select = 0;
  status.window_display_enable = 0;
  status.bg_tiledata_select = 0;
  status.bg_tilemap_select = 0;
  status.obj_size = 0;
  status.obj_enable = 0;
  status.bg_enable = 0;

  status.interrupt_lyc = 0;
  status.interrupt_oam = 0;
  status.interrupt_vblank = 0;
  status.interrupt_hblank = 0;

  status.scy = 0;
  status.scx = 0;
  status.ly = 0;
  status.lyc = 0;

  for(unsigned n = 0; n < 4; n++) {
    status.bgp[n] = n;
    status.obp[0][n] = n;
    status.obp[1][n] = n;
  }

  status.wy = 0;
  status.wx = 0;
}

}

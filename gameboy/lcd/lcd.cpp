#include <gameboy.hpp>

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

  if(status.ly == 144) cpu.status.interrupt_request_vblank = 1;
//print("Vblank - ", cpu.status.ime, " - ", cpu.status.interrupt_enable_vblank, "\n"); }
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
  uint8_t *output = screen + status.ly * 160;
  uint8 y = status.ly + status.scy;
  uint16 tmaddr = (status.bg_tilemap_select == 0 ? 0x1800 : 0x1c00);
  tmaddr += (y >> 3) * 32;
  tmaddr += (status.scx >> 3);

  for(unsigned t = 0; t < 20; t++) {
    unsigned tdaddr;
    if(status.bg_tiledata_select == 0) {
      tdaddr = 0x1000 + (int8)vram[tmaddr + t] * 16;
    } else {
      tdaddr = 0x0000 + vram[tmaddr + t] * 16;
    }
    tdaddr += (status.ly & 7) * 2;

    uint8 d0 = vram[tdaddr + 0];
    uint8 d1 = vram[tdaddr + 1];

    for(unsigned x = 0; x < 8; x++) {
      uint8 palette = ((d0 & 0x80) >> 7) + ((d1 & 0x80) >> 6);
      d0 <<= 1, d1 <<= 1;
      *output++ = (3 - status.bgp[palette]) * 0x55;
    }
  }

  output = screen + status.ly * 160;
  for(unsigned s = 0; s < 40; s++) {
    unsigned sy = oam[(s << 2) + 0] - 9;
    unsigned sx = oam[(s << 2) + 1] - 8;
    unsigned tile = oam[(s << 2) + 2];
    unsigned attribute = oam[(s << 2) + 3];

    sy -= status.ly;
    if(sy >= 8) continue;
    if(attribute & 0x40||1) sy ^= 7;

    unsigned addr = tile * 16 + sy * 2;

    uint8 d0 = vram[addr + 0];
    uint8 d1 = vram[addr + 1];
    unsigned xflip = attribute & 0x20 ? -7 : 0;

    for(unsigned x = 0; x < 8; x++) {
      uint8 palette = ((d0 & 0x80) >> 7) + ((d1 & 0x80) >> 6);
      d0 <<= 1, d1 <<= 1;
      if(palette == 0) continue;
      palette = status.obp[(bool)(attribute & 0x10)][palette];
      output[sx + (x ^ xflip)] = (3 - palette) * 0x55;
    }
  }
}

void LCD::power() {
  for(unsigned n = 0x8000; n <= 0x9fff; n++) bus.mmio[n] = this;  //VRAM
  for(unsigned n = 0xff40; n <= 0xff4b; n++) bus.mmio[n] = this;  //MMIO
  for(unsigned n = 0xfe00; n <= 0xfe9f; n++) bus.mmio[n] = this;  //OAM

  for(unsigned n = 0; n < 8192; n++) vram[n] = 0x00;
  for(unsigned n = 0; n <  160; n++) oam [n] = 0x00;

  reset();
}

void LCD::reset() {
  create(Main, 4 * 1024 * 1024);
  for(unsigned n = 0; n < 160 * 144; n++) screen[n] = 0x00;

  status.lx = 0;

  status.display_enable = 0;
  status.window_tilemap_select = 0;
  status.window_display_enable = 0;
  status.bg_tiledata_select = 0;
  status.bg_tilemap_select = 0;
  status.obj_size = 0;
  status.obj_enable = 0;
  status.bg_display = 0;

  status.interrupt_lyc = 0;
  status.interrupt_oam = 0;
  status.interrupt_vblank = 0;
  status.interrupt_hblank = 0;
  status.coincidence = 0;
  status.mode = 0;

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

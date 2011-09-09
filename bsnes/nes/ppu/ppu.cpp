#include <nes/nes.hpp>

namespace NES {

PPU ppu;

void PPU::Main() {
  ppu.main();
}

void PPU::main() {
  unsigned x = 0, y = 0;
  while(true) {
    add_clocks(4);

    status.lx++;

    if(status.ly >= 0 && status.ly <= 239) {
      if(status.lx == 257) {
        if(raster_enable()) {
          status.vaddr = (status.vaddr & 0x7be0) | (status.taddr & 0x041f);
        }
      }
    }

    if(status.ly == 261 && status.lx == 304) {
      if(raster_enable()) {
        status.vaddr = status.taddr;
      }
    }

    if(status.lx == 341) {
      status.lx = 0;
      status.ly++;

      if(status.ly == 262) {
        status.ly = 0;
        status.nmi = 0;
        status.sprite_zero_hit = 0;
        system.interface->video_refresh(buffer);
        scheduler.exit();
      }

      if(status.ly >= 0 && status.ly <= 239) {
        render_scanline();
      }

      if(status.ly == 240) {
        status.nmi = 1;
        if(status.nmi_enable) cpu.status.nmi_line = true;
      }
    }
  }
}

void PPU::add_clocks(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

void PPU::power() {
  paletteRGB = {
    0x7c7c7c, 0x0000fc, 0x0000bc, 0x4428bc,
    0x940084, 0xa80020, 0xa81000, 0x881400,
    0x503000, 0x007800, 0x006800, 0x005800,
    0x004058, 0x000000, 0x000000, 0x000000,
    0xbcbcbc, 0x0078f8, 0x0058f8, 0x6844fc,
    0xd800cc, 0xe40058, 0xf83800, 0xe45c10,
    0xac7c00, 0x00b800, 0x00a800, 0x00a844,
    0x008888, 0x000000, 0x000000, 0x000000,
    0xf8f8f8, 0x3cbcfc, 0x6888fc, 0x9878f8,
    0xf878f8, 0xf85898, 0xf87858, 0xfca044,
    0xf8b800, 0xb8f818, 0x58d854, 0x58f898,
    0x00e8d8, 0x787878, 0x000000, 0x000000,
    0xfcfcfc, 0xa4e4fc, 0xb8b8b8, 0xd8d8f8,
    0xf8b8f8, 0xf8a4c0, 0xf0d0b0, 0xfce0a8,
    0xf8d878, 0xd8f878, 0xb8f8b8, 0xb8f8d8,
    0x00fcfc, 0xf8d8f8, 0x000000, 0x000000,
  };

  reset();
}

void PPU::reset() {
  create(PPU::Main, 21477272);

  status.mdr = 0x00;
  status.ly = 0;
  status.lx = 0;
  status.bus_data = 0x00;
  status.address_latch = 0;

  status.vaddr = 0x0000;
  status.taddr = 0x0000;
  status.xaddr = 0x00;

  //$2000
  status.nmi_enable = false;
  status.master_select = 0;
  status.sprite_size = 0;
  status.bg_addr = 0x0000;
  status.sprite_addr = 0x0000;
  status.vram_increment = 1;

  //$2001
  status.intensify_blue = false;
  status.intensify_green = false;
  status.intensify_red = false;
  status.sprite_enable = false;
  status.bg_enable = false;
  status.sprite_edge_enable = false;
  status.bg_edge_enable = false;
  status.grayscale = false;

  //$2002
  status.nmi = false;
  status.sprite_zero_hit = false;
  status.sprite_overflow = false;

  //$2003
  status.oam_addr = 0x00;

  memset(buffer, 0, sizeof buffer);

  memset(nram, 0, sizeof nram);
  memset(pram, 0, sizeof pram);
  memset(sram, 0, sizeof sram);
}

uint8 PPU::read(uint16 addr) {
  uint8 result = 0x00;

  switch(addr & 7) {
  case 2:  //PPUSTATUS
    result |= status.nmi << 7;
    result |= status.sprite_zero_hit << 6;
    result |= status.sprite_overflow << 5;
    result |= status.mdr & 0x1f;
    status.nmi = 0;
    status.address_latch = 0;
    break;
  case 4:  //OAMDATA
    result = sram[status.oam_addr++];
    break;
  case 7:  //PPUDATA
    if(addr >= 0x3f00) {
      result = bus_read(status.vaddr);
    } else {
      result = status.bus_data;
      status.bus_data = bus_read(status.vaddr);
    }
    status.vaddr += status.vram_increment;
    break;
  }

  return result;
}

void PPU::write(uint16 addr, uint8 data) {
  status.mdr = data;

  switch(addr & 7) {
  case 0:  //PPUCTRL
    status.nmi_enable = data & 0x80;
    status.master_select = data & 0x40;
    status.sprite_size = data & 0x20;
    status.bg_addr = (data & 0x10) ? 0x1000 : 0x0000;
    status.sprite_addr = (data & 0x08) ? 0x1000 : 0x0000;
    status.vram_increment = (data & 0x04) ? 32 : 1;
    status.taddr = (status.taddr & 0x73ff) | ((data & 0x03) << 10);
    return;
  case 1:  //PPUMASK
    status.intensify_blue = data & 0x80;
    status.intensify_green = data & 0x40;
    status.intensify_red = data & 0x20;
    status.sprite_enable = data & 0x10;
    status.bg_enable = data & 0x08;
    status.sprite_edge_enable = data & 0x04;
    status.bg_edge_enable = data & 0x02;
    status.grayscale = data & 0x01;
    return;
  case 2:  //PPUSTATUS
    return;
  case 3:  //OAMADDR
    status.oam_addr = data;
    return;
  case 4:  //OAMDATA
    sram[status.oam_addr++] = data;
    return;
  case 5:  //PPUSCROLL
    if(status.address_latch == 0) {
      status.xaddr = data & 0x07;
      status.taddr = (status.taddr & 0x7fe0) | (data >> 3);
    } else {
      status.taddr = (status.taddr & 0x0c1f) | ((data & 0x07) << 12) | ((data >> 3) << 5);
    }
    status.address_latch ^= 1;
    return;
  case 6:  //PPUADDR
    if(status.address_latch == 0) {
      status.taddr = (status.taddr & 0x00ff) | ((data & 0x3f) << 8);
    } else {
      status.taddr = (status.taddr & 0x7f00) | data;
      status.vaddr = status.taddr;
    }
    status.address_latch ^= 1;
    return;
  case 7:  //PPUDATA
    bus_write(status.vaddr, data);
    status.vaddr += status.vram_increment;
    return;
  }
}

uint8 PPU::bus_read(uint16 addr) {
  if(addr <= 0x1fff) return cartridge.chr_read(addr);
  if(addr <= 0x3eff) return nram[addr & 0x07ff];
  if(addr <= 0x3fff) return pram[addr & 0x001f];
}

void PPU::bus_write(uint16 addr, uint8 data) {
  if(addr <= 0x1fff) {
    return cartridge.chr_write(addr, data);
  }

  if(addr <= 0x3eff) {
    nram[addr & 0x07ff] = data;
    return;
  }

  if(addr <= 0x3fff) {
    addr &= 0x1f;
    if(addr == 0x10) addr = 0x00;
    if(addr == 0x14) addr = 0x04;
    if(addr == 0x18) addr = 0x08;
    if(addr == 0x1c) addr = 0x0c;
    pram[addr] = data;
    return;
  }
}

bool PPU::raster_enable() const {
  return status.bg_enable || status.sprite_enable;
}

unsigned PPU::nametable_addr() const {
  return 0x2000 + (status.vaddr & 0x0c00);
}

unsigned PPU::scrollx() const {
  return ((status.vaddr & 0x1f) << 3) | status.xaddr;
}

unsigned PPU::scrolly() const {
  return (((status.vaddr >> 5) & 0x1f) << 3) | ((status.vaddr >> 12) & 7);
}

void PPU::render_scanline() {
  uint32 *line = buffer + status.ly * 256;

  uint8 oam[8][5];
  unsigned oamc = 0;

  unsigned sprite_height = status.sprite_size ? 16 : 8;
  for(unsigned n = 0; n < 64; n++) {
    unsigned y = sram[(n * 4) + 0] + 1;
    if(status.ly < y || status.ly >= y + sprite_height) continue;
    oam[oamc][0] = y;
    oam[oamc][1] = sram[(n * 4) + 1];
    oam[oamc][2] = sram[(n * 4) + 2];
    oam[oamc][3] = sram[(n * 4) + 3];
    oam[oamc][4] = n;
    if(++oamc >= 8) break;
  }

  for(unsigned x = 0; x < 256; x++) {
    unsigned offsetx = x + scrollx();
    unsigned offsety = status.ly + scrolly();

    bool screenx = offsetx & 256;
    bool screeny = offsety & 256;

    offsetx &= 255;
    offsety &= 255;

    unsigned base = nametable_addr() + (screenx * 0x0400) + (screeny * 0x0800);

    uint8 tile = bus_read(base + (offsety / 8) * 32 + (offsetx / 8));
    uint8 attr = bus_read(base + 0x03c0 + (offsety / 32) * 8 + (offsetx / 32));

    if((offsety / 16) & 1) attr >>= 4;
    if((offsetx / 16) & 1) attr >>= 2;

    unsigned tilex = offsetx & 7;
    unsigned tiley = offsety & 7;

    uint8 tdlo = bus_read(status.bg_addr + tile * 16 + 0 + tiley);
    uint8 tdhi = bus_read(status.bg_addr + tile * 16 + 8 + tiley);

    unsigned mask = 0x80 >> tilex;
    unsigned palette = 0;
    if(status.bg_enable) {
      if(status.bg_edge_enable == true || x >= 8) {
        palette |= (tdlo & mask) ? 1 : 0;
        palette |= (tdhi & mask) ? 2 : 0;
        if(palette) palette |= (attr & 3) << 2;
      }
    }

    for(unsigned n = 0; n < oamc; n++) {
      if(x < oam[n][3] || x >= oam[n][3] + 8) continue;
      if(status.sprite_enable == false) continue;
      if(status.sprite_edge_enable == false && x < 8) continue;

      unsigned spritex = x - oam[n][3];
      unsigned spritey = status.ly - oam[n][0];

      unsigned addr = (sprite_height == 8)
      ? status.sprite_addr + oam[n][1] * 16
      : ((oam[n][1] & ~1) * 16) + ((oam[n][1] & 1) * 0x1000);

      if(oam[n][2] & 0x80) spritey ^= (sprite_height - 1);
      if(oam[n][2] & 0x40) spritex ^= 7;
      if(spritey & 8) spritey += 8;

      tdlo = bus_read(addr + 0 + spritey);
      tdhi = bus_read(addr + 8 + spritey);

      mask = 0x80 >> spritex;
      unsigned sprite_palette = 0;
      sprite_palette |= (tdlo & mask) ? 1 : 0;
      sprite_palette |= (tdhi & mask) ? 2 : 0;
      if(sprite_palette == 0) continue;
      sprite_palette |= (oam[n][2] & 3) << 2;

      bool priority = oam[n][2] & 0x20;
      if(priority == 0 || palette == 0) {
        if(oam[n][4] == 0) status.sprite_zero_hit = 1;
        palette = 16 + sprite_palette;
      }
      break;
    }

    *line++ = paletteRGB[pram[palette]];
  }
}

}

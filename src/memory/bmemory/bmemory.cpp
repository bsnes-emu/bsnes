#include "../../base.h"
#include "bmemory_rw.cpp"
#include "bmemory_mapper_generic.cpp"

void bMemBus::load_cart() {
  if(rom_loaded == true)return;

uint8 *rom   = cartridge.rom;
uint16 index = cartridge.cart.header_index;
  dprintf("* Image Name      : \"%s\"", cartridge.cart.name);
  dprintf("* Region          : %s", (cartridge.cart.region == Cartridge::NTSC) ? "NTSC" : "PAL");
  dprintf("* Address Decoder : %0.2x", cartridge.cart.mapper);
  dprintf("* SRAM Size       : %dkb", cartridge.cart.sram_size / 1024);
char t[256];
  strcpy(t, "");
  if(cartridge.cart.srtc)strcat(t, "S-RTC, ");
  if(cartridge.cart.sdd1)strcat(t, "S-DD1, ");
  if(cartridge.cart.c4)  strcat(t, "Cx4, ");
  if(cartridge.cart.dsp2)strcat(t, "DSP-2, ");
  if(cartridge.cart.obc1)strcat(t, "OBC-1, ");
  strrtrim(t, ", ");
  dprintf("* Coprocessor(s)  : %s", (strlen(t) == 0) ? "None" : t);
  dprintf("* Reset:%0.4x NMI:%0.4x IRQ:%0.4x BRK[n]:%0.4x COP[n]:%0.4x BRK[e]:%0.4x COP[e]:%0.4x",
    rom[index + 0x3c] | (rom[index + 0x3d] << 8), //Reset
    rom[index + 0x2a] | (rom[index + 0x2b] << 8), //NMI
    rom[index + 0x2e] | (rom[index + 0x2f] << 8), //IRQ
    rom[index + 0x26] | (rom[index + 0x27] << 8), //BRK[n]
    rom[index + 0x24] | (rom[index + 0x25] << 8), //COP[n]
    rom[index + 0x3e] | (rom[index + 0x3f] << 8), //BRK[e]
    rom[index + 0x34] | (rom[index + 0x35] << 8)  //COP[e]
  );
  dprintf("");

  cart_map_reset();

  switch(cartridge.cart.mapper) {
  case Cartridge::LOROM:
  case Cartridge::HIROM:
  case Cartridge::EXLOROM:
  case Cartridge::EXHIROM:
    cart_map_generic(cartridge.cart.mapper);
    break;
  default:
    return;
  }

  if(cartridge.cart.sdd1)cart_map_sdd1();
  if(cartridge.cart.c4)  cart_map_c4();
  if(cartridge.cart.dsp2)cart_map_dsp2();
  if(cartridge.cart.obc1)cart_map_obc1();

  cart_map_system();

  if(cartridge.cart.region == Cartridge::NTSC) {
    snes->set_region(SNES::NTSC);
  } else {
    snes->set_region(SNES::PAL);
  }

  rom_loaded = true;
}

void bMemBus::unload_cart() {
  if(rom_loaded == false)return;

  rom_loaded = false;
}

/***********************
 *** SNES Memory Map ***
 *****************************************
 *** 00-3f  0000-1fff  First 8k WRAM   ***
 ***        2000-5fff  MMIO            ***
 ***        6000-7fff  /Cart Expansion ***
 ***        8000-ffff  /Cart           ***
 *** 40-7d  0000-ffff  /Cart           ***
 *** 7e-7f  0000-ffff  128k WRAM       ***
 *** 80-bf  0000-1fff  First 8k WRAM   ***
 ***        2000-5fff  MMIO            ***
 ***        6000-7fff  /Cart Expansion ***
 ***        8000-ffff  /Cart           ***
 *** c0-ff  0000-ffff  /Cart           ***
 *****************************************/

uint8 bMemBus::memory_type(uint32 addr) {
uint8 bank;
  bank  = addr >> 16;
  addr &= 0xffff;

  if((bank >= 0x00 && bank <= 0x3f) || (bank >= 0x80 && bank <= 0xbf)) {
    if(addr >= 0x0000 && addr <= 0x1fff)return TYPE_WRAM;
    if(addr >= 0x2000 && addr <= 0x5fff)return TYPE_MMIO;
    return TYPE_CART;
  }

  if((bank >= 0x40 && bank <= 0x7d) || (bank >= 0xc0 && bank <= 0xff)) {
    return TYPE_CART;
  }

//(bank >= 0x7e && bank <= 0x7f)
  return TYPE_WRAM;
}

uint8 bMemBus::read(uint32 addr) {
uint8 r;
#ifdef CHEAT_SYSTEM
  if(cheat.enabled()) {
    if(cheat.exists(addr)) {
      return cheat.read(addr);
    }
  }
#endif

  r = (this->*page_read[addr >> 8])(addr);
#ifdef DEBUGGER
  snes->notify(SNES::MEM_READ, addr, r);
#endif
  return r;
}

void bMemBus::write(uint32 addr, uint8 data) {
  (this->*page_write[addr >> 8])(addr, data);
#ifdef DEBUGGER
  snes->notify(SNES::MEM_WRITE, addr, data);
#endif
}

void bMemBus::cart_map_reset() {
  for(int i = 0x0000; i <= 0xffff; i++) {
    page_handle[i] = 0;
    page_read  [i] = &bMemBus::read_unmapped;
    page_write [i] = &bMemBus::write_unmapped;
  }
}

void bMemBus::cart_map_system() {
//map $[00-3f|80-bf]:[0000-1fff] to WRAM
  for(int b = 0x00; b <= 0x3f; b++) {
    for(int p = 0x00; p <= 0x1f; p++) {
      page_handle[0x0000 + (b << 8) + p] = wram + (p << 8);
      page_handle[0x8000 + (b << 8) + p] = wram + (p << 8);
      page_read  [0x0000 + (b << 8) + p] = &bMemBus::read_ram;
      page_read  [0x8000 + (b << 8) + p] = &bMemBus::read_ram;
      page_write [0x0000 + (b << 8) + p] = &bMemBus::write_ram;
      page_write [0x8000 + (b << 8) + p] = &bMemBus::write_ram;
    }
  }

//map $[7e-7f]:[0000-ffff] to WRAM
  for(int b = 0x7e; b <= 0x7f; b++) {
    for(int p = 0x00; p <= 0xff; p++) {
      page_handle[(b << 8) + p] = wram + ((b & 1) << 16) + (p << 8);
      page_read  [(b << 8) + p] = &bMemBus::read_ram;
      page_write [(b << 8) + p] = &bMemBus::write_ram;
    }
  }

//map $[00-3f|80-bf]:[2000-5fff] to MMIO
  for(int b = 0x00; b <= 0x3f; b++) {
    for(int p = 0x20; p <= 0x5f; p++) {
      page_read [0x0000 + (b << 8) + p] = &bMemBus::read_mmio;
      page_read [0x8000 + (b << 8) + p] = &bMemBus::read_mmio;
      page_write[0x0000 + (b << 8) + p] = &bMemBus::write_mmio;
      page_write[0x8000 + (b << 8) + p] = &bMemBus::write_mmio;
    }
  }
}

void bMemBus::power() {
  cart_write_protect(true);
  memset(wram, 0, 0x020000);
  reset();
}

void bMemBus::reset() {
  set_speed(false);
}

bMemBus::bMemBus() {
  wram = (uint8*)malloc(0x020000);
  rom_loaded = false;
}

bMemBus::~bMemBus() {
  SafeFree(wram);
}

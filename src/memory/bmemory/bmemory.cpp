#include "../../base.h"
#include "mapper/mapper.cpp"
#include "bmemory_rw.cpp"
#include "bmemory_mapper_generic.cpp"

void bMemBus::load_cart() {
  if(rom_loaded == true)return;

  cart_map_reset();

  switch(cartridge.info.mapper) {
  case Cartridge::PCB:
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3B-01")) { cart_map_shvc_1a3b_13(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3B-11")) { cart_map_shvc_1a3b_13(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3B-12")) { cart_map_shvc_1a3b_13(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3B-13")) { cart_map_shvc_1a3b_13(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3B-20")) { cart_map_shvc_1a3b_20(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3M-10")) { cart_map_shvc_1a3m_30(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3M-20")) { cart_map_shvc_1a3m_30(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3M-21")) { cart_map_shvc_1a3m_30(); break; }
    if(!strcmp(cartridge.info.pcb, "SHVC-1A3M-30")) { cart_map_shvc_1a3m_30(); break; }
    if(!strcmp(cartridge.info.pcb, "BSC-1A5M-01"))  { cart_map_bsc_1a5m_01();  break; }
    if(!strcmp(cartridge.info.pcb, "BSC-1A7M-01"))  { cart_map_bsc_1a7m_01();  break; }
    if(!strcmp(cartridge.info.pcb, "BSC-1A7M-10"))  { cart_map_bsc_1a7m_10();  break; }
    dprintf("* PCB mapper not found");
    return;

  case Cartridge::LOROM:
  case Cartridge::HIROM:
  case Cartridge::EXLOROM:
  case Cartridge::EXHIROM:
    cart_map_generic(cartridge.info.mapper);
    break;
  default:
    dprintf("* generic mapper not found");
    return;
  }

  if(cartridge.info.sdd1)cart_map_sdd1();
  if(cartridge.info.c4)  cart_map_c4();
  if(cartridge.info.dsp1)cart_map_dsp1();
  if(cartridge.info.dsp2)cart_map_dsp2();
  if(cartridge.info.obc1)cart_map_obc1();

  cart_map_system();

uint region = read(0xffd9) & 0x7f;
  cartridge.info.region = (region == 0 || region == 1 || region == 13) ? Cartridge::NTSC : Cartridge::PAL;
  if(cartridge.info.region == Cartridge::NTSC) {
    snes->set_region(SNES::NTSC);
  } else {
    snes->set_region(SNES::PAL);
  }

  rom_loaded = true;

//print cartridge info to debug console

  dprintf("* CRC32    : %0.8x", cartridge.info.crc32);
  dprintf("* Name     : \"%s\"", cartridge.info.name);
  dprintf("* PCB      : %s", cartridge.info.pcb);
  dprintf("* ROM Size : %dmbit", cartridge.info.rom_size / 1024 / 1024 * 8);
  dprintf("* RAM Size : %dkbit", cartridge.info.ram_size / 1024 * 8);
  dprintf("* Region   : %s", (cartridge.info.region == Cartridge::NTSC) ? "NTSC" : "PAL");
char t[256];
  strcpy(t, "");
  if(cartridge.info.srtc)strcat(t, "S-RTC, ");
  if(cartridge.info.sdd1)strcat(t, "S-DD1, ");
  if(cartridge.info.c4)  strcat(t, "Cx4, ");
  if(cartridge.info.dsp1)strcat(t, "DSP-1, ");
  if(cartridge.info.dsp2)strcat(t, "DSP-2, ");
  if(cartridge.info.obc1)strcat(t, "OBC-1, ");
  strrtrim(t, ", ");
  dprintf("* Coprocessor(s) : %s", (strlen(t) == 0) ? "None" : t);
  dprintf("* Reset:%0.4x NMI[n]:%0.4x IRQ[n]:%0.4x BRK[n]:%0.4x COP[n]:%0.4x",
    (read(0xfffc) << 0) | (read(0xfffd) << 8), //Reset
    (read(0xffea) << 0) | (read(0xffeb) << 8), //NMI
    (read(0xffee) << 0) | (read(0xffef) << 8), //IRQ
    (read(0xffe6) << 0) | (read(0xffe7) << 8), //BRK
    (read(0xffe4) << 0) | (read(0xffe5) << 8)  //COP
  );
//BRK[e] should be $fff6, however emulation mode brk is technically not supported.
//this needs verification, but I believe brk jumps to IRQ[e] vector.
  dprintf("*            NMI[e]:%0.4x IRQ[e]:%0.4x BRK[e]:%0.4x COP[e]:%0.4x",
    (read(0xfffa) << 0) | (read(0xfffb) << 8), //NMI
    (read(0xfffe) << 0) | (read(0xffff) << 8), //IRQ
    (read(0xfffe) << 0) | (read(0xffff) << 8), //BRK
    (read(0xfff4) << 0) | (read(0xfff5) << 8)  //COP
  );
  dprintf("");
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
      if(cheat.read(addr, r) == true)return r;
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
  memset(wram, 0xff, 0x020000);
  reset();
}

void bMemBus::reset() {
  set_speed(0);
}

bMemBus::bMemBus() {
  wram = (uint8*)malloc(0x020000);
  rom_loaded = false;
}

bMemBus::~bMemBus() {
  SafeFree(wram);
}

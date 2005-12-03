#include "../../base.h"
#include "bcart_lorom.cpp"
#include "bcart_hirom.cpp"
#include "bcart_exlorom.cpp"
#include "bcart_exhirom.cpp"

void bMemBus::load_cart() {
  if(rom_loaded == true)return;

uint8 *rom   = cartridge.rom;
uint16 index = cartridge.cart.header_index;
  dprintf("* Image Name : \"%s\"", cartridge.cart.name);
  dprintf("* Region     : %s", (cartridge.cart.region == Cartridge::NTSC) ? "NTSC" : "PAL");
  dprintf("* MAD        : %0.2x", cartridge.cart.mapper);
  dprintf("* SRAM Size  : %dkb", cartridge.cart.sram_size / 1024);
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

CartInfo ci;
  ci.rom       = cartridge.rom;
  ci.sram      = cartridge.sram;
  ci.rom_size  = cartridge.cart.rom_size;
  ci.sram_size = cartridge.cart.sram_size;

  switch(cartridge.cart.mapper) {
  case Cartridge::LOROM:  cart = new bCartLoROM();  break;
  case Cartridge::HIROM:  cart = new bCartHiROM();  break;
  case Cartridge::EXLOROM:cart = new bCartExLoROM();break;
  case Cartridge::EXHIROM:cart = new bCartExHiROM();break;
  default:return;
  }

  cart->set_cartinfo(&ci);

  if(cartridge.cart.region == Cartridge::NTSC) {
    snes->set_region(SNES::NTSC);
  } else {
    snes->set_region(SNES::PAL);
  }

  rom_loaded = true;
}

void bMemBus::unload_cart() {
  if(rom_loaded == false)return;

  delete(cart);
  rom_loaded = false;
}

void bMemBus::get_cartinfo(CartInfo *ci) {
  ci->rom       = cartridge.rom;
  ci->sram      = cartridge.sram;
  ci->rom_size  = cartridge.cart.rom_size;
  ci->sram_size = cartridge.cart.sram_size;
}

/***********************
 *** SNES Memory Map ***
 ***************************************
 *** 00-3f  0000-1fff  First 8k WRAM ***
 ***        2000-5fff  MMIO          ***
 ***        6000-7fff  Expansion RAM ***
 ***        8000-ffff  Cartridge ROM ***
 *** 40-7d  0000-ffff  Cartridge ROM ***
 *** 7e-7f  0000-ffff  128k WRAM     ***
 *** 80-bf  0000-1fff  First 8k WRAM ***
 ***        2000-5fff  MMIO          ***
 ***        6000-7fff  Expansion RAM ***
 ***        8000-ffff  Cartridge ROM ***
 *** c0-ff  0000-ffff  Cartridge ROM ***
 ***************************************/

uint8 bMemBus::read(uint32 addr) {
uint8 r;
  switch(addr & 0xc00000) {
  case 0x400000:
    if((addr & 0xfe0000) == 0x7e0000) {
      r = wram[addr & 0x01ffff];
      break;
    }
  //fallthrough
  case 0xc00000:
    r = cart->read(addr);
    break;

//case 0x000000:
//case 0x800000:
  default:
    switch(addr & 0x00e000) {
    case 0x0000:
      r = wram[addr & 0x1fff];
      break;
    case 0x2000:
    case 0x4000:
      r = mmio[(addr - 0x2000) & 0x3fff]->read(addr & 0x7fff);
      break;
//  case 0x6000:
//  case 0x8000:
//  case 0xa000:
//  case 0xc000:
//  case 0xe000:
    default:
      r = cart->read(addr);
      break;
    }
    break;
  }

#ifdef DEBUGGER
  snes->notify(SNES::MEM_READ, addr, r);
#endif
  return r;
}

void bMemBus::write(uint32 addr, uint8 value) {
  switch(addr & 0xc00000) {
  case 0x400000:
    if((addr & 0xfe0000) == 0x7e0000) {
      wram[addr & 0x01ffff] = value;
      break;
    }
  //fallthrough
  case 0xc00000:
    cart->write(addr, value);
    break;

//case 0x000000:
//case 0x800000:
  default:
    switch(addr & 0x00e000) {
    case 0x0000:
      wram[addr & 0x1fff] = value;
      break;
    case 0x2000:
    case 0x4000:
      mmio[(addr - 0x2000) & 0x3fff]->write(addr & 0x7fff, value);
      break;
//  case 0x6000:
//  case 0x8000:
//  case 0xa000:
//  case 0xc000:
//  case 0xe000:
    default:
      cart->write(addr, value);
      break;
    }
    break;
  }

#ifdef DEBUGGER
  snes->notify(SNES::MEM_WRITE, addr, value);
#endif
}

void bMemBus::power() {
  memset(wram, 0, 0x020000);
  reset();
}

void bMemBus::reset() {
  set_speed(false);
}

bMemBus::bMemBus() {
  wram = (byte*)malloc(0x020000);
  memset(wram, 0, 0x020000);
  rom_loaded = false;
}

bMemBus::~bMemBus() {
  zerofree(wram);
}

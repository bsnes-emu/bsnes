#include "../../base.h"
#include "bcart_lorom.cpp"
#include "bcart_hirom.cpp"
#include "bcart_exhirom.cpp"

bool bMemBus::load_cart(Reader *rf) {
uint32 cksum, icksum, index;
char   cart_title[24];
uint8  mapper;
  if(rom_loaded == true)return false;

  rf->read(&rom);

  rom_size = rf->size();

  if(rom_size < 32768)return false;

  if(rom_size >= 0x410000) {
    mapper = EXHIROM;
    goto end;
  }

  if(rom_size < 65536) {
    mapper = LOROM;
    goto end;
  }

  cksum  = rom[0xffdc] | (rom[0xffdd] << 8);
  icksum = rom[0xffde] | (rom[0xffdf] << 8);
  if(cksum + icksum == 0xffff) {
    mapper = HIROM;
  } else {
    mapper = LOROM;
  }

end:
  switch(mapper) {
  case LOROM:  index = 0x007fc0;break;
  case HIROM:  index = 0x00ffc0;break;
  case EXHIROM:index = 0x40ffc0;break;
  }
  memcpy(cart_title, (char*)rom + index, 21);
  cart_title[21] = 0;

  switch(rom[index + 0x18] & 7) {
  case 0:sram_size =   0;       break;
  case 1:sram_size =   2 * 1024;break;
  case 2:sram_size =   4 * 1024;break;
  case 3:sram_size =   8 * 1024;break;
  case 4:sram_size =  16 * 1024;break;
  case 5:sram_size =  32 * 1024;break;
  case 6:sram_size =  64 * 1024;break;
  case 7:sram_size = 128 * 1024;break;
  }

  dprintf("* Image Name : \"%s\"", cart_title);
  dprintf("* MAD        : %0.2x", mapper);
  dprintf("* SRAM Size  : %dkb", sram_size / 1024);
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
  ci.rom       = rom;
  ci.sram      = sram;
  ci.rom_size  = rom_size;
  ci.sram_size = sram_size;

  switch(mapper) {
  case LOROM:  cart = new bCartLoROM();  break;
  case HIROM:  cart = new bCartHiROM();  break;
  case EXHIROM:cart = new bCartExHiROM();break;
  default:return false;
  }

  cart->set_cartinfo(&ci);
  rom_loaded = true;
  return true;
}

bool bMemBus::load_sram(Reader *rf) {
  if(rom_loaded == false || sram_size == 0)return false;
  rf->read(&sram, sram_size);

CartInfo ci;
  ci.rom       = rom;
  ci.sram      = sram;
  ci.rom_size  = rom_size;
  ci.sram_size = sram_size;
  cart->set_cartinfo(&ci);

  return true;
}

bool bMemBus::save_sram(Writer *wf) {
  if(rom_loaded == false || sram_size == 0)return false;
  wf->write(sram, sram_size);
  return true;
}

void bMemBus::unload_cart() {
  if(rom_loaded == false)return;

  if(rom) free(rom);
  if(sram)free(sram);
  delete(cart);

  rom_loaded = false;
}

void bMemBus::get_cartinfo(CartInfo *ci) {
  ci->rom       = rom;
  ci->sram      = sram;
  ci->rom_size  = rom_size;
  ci->sram_size = sram_size;
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
uint32 b, w, r;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

  if(b <= 0x3f) {
    if(w <= 0x1fff) {
      r = wram[w];
    } else if(w <= 0x5fff) {
      r = mmio[w - 0x2000]->read(w);
    } else {
      r = cart->read(addr);
    }
  } else if(b <= 0x7d) {
    r = cart->read(addr);
  } else if(b <= 0x7f) {
    r = wram[addr & 0x01ffff];
  } else if(b <= 0xbf) {
    if(w <= 0x1fff) {
      r = wram[w];
    } else if(w <= 0x5fff) {
      r = mmio[w - 0x2000]->read(w);
    } else {
      r = cart->read(addr);
    }
  } else {
    r = cart->read(addr);
  }

  snes->notify(SNES::MEM_READ, addr, r);
  return r;
}

void bMemBus::write(uint32 addr, uint8 value) {
uint32 b, w;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

  if(b <= 0x3f) {
    if(w <= 0x1fff) {
      wram[w] = value;
    } else if(w <= 0x5fff) {
      mmio[w - 0x2000]->write(w, value);
    } else {
      cart->write(addr, value);
    }
  } else if(b <= 0x7d) {
    cart->write(addr, value);
  } else if(b <= 0x7f) {
    wram[addr & 0x01ffff] = value;
  } else if(b <= 0xbf) {
    if(w <= 0x1fff) {
      wram[w] = value;
    } else if(w <= 0x5fff) {
      mmio[w - 0x2000]->write(w, value);
    } else {
      cart->write(addr, value);
    }
  } else {
    cart->write(addr, value);
  }

  snes->notify(SNES::MEM_WRITE, addr, value);
}

void bMemBus::power() {
  memset(wram, 0, 0x020000);
  reset();
}

void bMemBus::reset() {
  fastROM = false;
}

bMemBus::bMemBus() {
  wram = (byte*)malloc(0x020000);
  memset(wram, 0, 0x020000);
  rom_loaded = false;
}

bMemBus::~bMemBus() {
  if(wram)free(wram);
}

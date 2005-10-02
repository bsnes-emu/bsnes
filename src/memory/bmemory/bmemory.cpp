#include "../../base.h"
#include "bcart_lorom.cpp"
#include "bcart_hirom.cpp"
#include "bcart_exlorom.cpp"
#include "bcart_exhirom.cpp"

bool bMemBus::load_cart(Reader *rf) {
uint32 cksum, icksum, index;
char   cart_title[24];
uint8  mapper, region;
  if(rom_loaded == true)return false;

  rf->read(&rom_image);
  rom = rom_image;
  rom_size = rf->size();

  if(rom_size < 32768) {
    free(rom_image);
    return false;
  }

//check for ROM header (currently unused)
  if((rom_size & 0x1fff) == 0x0200) {
    rom_size -= 512;
    rom += 512;
  }

  if(rom_size >= 0x410000) {
    if(rom[0x7fd5] == 0x32) {
      mapper = EXLOROM;
    } else {
      mapper = EXHIROM;
    }
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

  if(rom[0x7fd5] == 0x32 && rom[0xffd5] == 0x32) {
  //SFA2 detected
    mapper = EXLOROM;
    goto end;
  }

end:
  switch(mapper) {
  case LOROM:  index = 0x007fc0;break;
  case HIROM:  index = 0x00ffc0;break;
  case EXLOROM:index = 0x007fc0;break;
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

  region = rom[index + 0x19];

  dprintf("* Image Name : \"%s\"", cart_title);
  dprintf("* Region     : %s", (region <= 1)?"NTSC":"PAL");
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
  case EXLOROM:cart = new bCartExLoROM();break;
  case EXHIROM:cart = new bCartExHiROM();break;
  default:return false;
  }

  cart->set_cartinfo(&ci);
  rom_loaded = true;

  if(region == 0 || region == 1) {
    snes->set_region(SNES::NTSC);
  } else {
    snes->set_region(SNES::PAL);
  }

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

  if(rom_image)free(rom_image);
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
static uint32 r;
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

  snes->notify(SNES::MEM_READ, addr, r);
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

  snes->notify(SNES::MEM_WRITE, addr, value);
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

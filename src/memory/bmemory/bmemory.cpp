#include "../../base.h"

uint8 bROM::read(uint32 addr) {
uint32 b, w;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

/* SRAM Region A */
  if((b & 0x7f) >= 0x30 && (b & 0x7f) <= 0x3f && (w & 0xe000) == 0x6000) {
    b &= 0x7f;
    if(b >= 0x30 && b <= 0x3f) {
      if(sram_size) {
        addr = (b - 0x30) * 0x2000 + (w - 0x6000);
        addr &= (sram_size - 1);
        return sram_data[addr];
      } else {
        return 0x00; //no SRAM available
      }
    } else {
      return 0x00; //unmapped
    }
  }

/* SRAM Region B */
  if(b >= 0x70 && b <= 0x7d) {
    if(sram_size) {
      addr = (addr & 0xffffff) - 0x700000;
      addr &= (sram_size - 1);
      return sram_data[addr];
    } else {
      return 0x00; //no SRAM available
    }
  }

  switch(mapper) {
  case LOROM:
    if(w & 0x8000) {
      b &= 0x7f;
      b %= 0x60;
      addr = (b << 15) | (addr & 0x7fff);
    } else {
      b &= 0x7f;
      b %= 0x60;
      if(b == 0x00)b = 0x60;
      addr = (((b << 15) | (addr & 0x7fff)) - 0x8000);
    }
    if(addr < size)return data[addr];
    break;
  case HIROM:
    addr &= 0x3fffff;
    if(addr < size)return data[addr];
    break;
  }
  return 0x00;
}

void bROM::write(uint32 addr, uint8 value) {
uint32 b, w;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

/* SRAM Region A */
  if((b & 0x7f) >= 0x30 && (b & 0x7f) <= 0x3f && (w & 0xe000) == 0x6000) {
    b &= 0x7f;
    if(b >= 0x30 && b <= 0x3f) {
      if(sram_size) {
        addr = (b - 0x30) * 0x2000 + (w - 0x6000);
        addr &= (sram_size - 1);
        sram_data[addr] = value;
        return;
      } else {
        return; //no SRAM available
      }
    } else {
      return; //unmapped
    }
  }

/* SRAM Region B */
  if(b >= 0x70 && b <= 0x7d) {
    if(sram_size) {
      addr = (addr & 0xffffff) - 0x700000;
      addr &= (sram_size - 1);
      sram_data[addr] = value;
    } else {
      return; //no SRAM available
    }
  }

/* Block write attempts to ROM by default, debugger can override this */
  if(write_protection == true)return;

  switch(mapper) {
  case LOROM:
    if(w & 0x8000) {
      b &= 0x7f;
      b %= 0x60;
      addr = (b << 15) | (addr & 0x7fff);
    } else {
      b &= 0x7f;
      b %= 0x60;
      if(b == 0x00)b = 0x60;
      addr = (((b << 15) | (addr & 0x7fff)) - 0x8000);
    }
    if(addr < size)data[addr] = value;
    break;
  case HIROM:
    addr &= 0x3fffff;
    if(addr < size)data[addr] = value;
    break;
  }
}

void bROM::load_sram(Reader *rf) {
  if(rom_loaded == false)return;

  rf->read(&sram_data, sram_size);
}

void bROM::save_sram(Writer *wf) {
  if(rom_loaded == false)return;
  if(!sram_size)return;

  wf->write(sram_data, sram_size);
}

void bROM::load_rom(Reader *rf) {
uint16 cksum, icksum, index;
char cart_title[24];
  if(rom_loaded == true)return;

  rf->read(&data);

  size = rf->size();
  if(size < 65536) {
    mapper = LOROM;
    strcpy(cart_title, "");
    goto end;
  }

  cksum  = data[0xffdc] | (data[0xffdd] << 8);
  icksum = data[0xffde] | (data[0xffdf] << 8);
  if(cksum + icksum == 0xffff) {
    mapper = HIROM;
  } else {
    mapper = LOROM;
  }

end:
  if(size >= 32768) {
    memcpy(cart_title, (char*)data + ((mapper == LOROM)?0x7fc0:0xffc0), 21);
  } else *cart_title = 0;
  cart_title[21] = 0;
  index = (mapper == HIROM)?0x8000:0x0000;

  switch(data[index + 0x7fd8] & 7) {
  case 0:sram_size =   0;       break;
  case 1:sram_size =   2 * 1024;break;
  case 2:sram_size =   4 * 1024;break;
  case 3:sram_size =   8 * 1024;break;
  case 4:sram_size =  16 * 1024;break;
  case 5:sram_size =  32 * 1024;break;
  case 6:sram_size =  64 * 1024;break;
  case 7:sram_size = 128 * 1024;break;
  }

  dprintf("Image Name : \"%s\"", cart_title);
  dprintf("Image Type : %s", (mapper == LOROM)?"LoROM":"HiROM");
  dprintf("SRAM Size  : %dkb", sram_size / 1024);
  dprintf("Reset:%0.4x NMI:%0.4x IRQ:%0.4x BRK[n]:%0.4x COP[n]:%0.4x BRK[e]:%0.4x COP[e]:%0.4x",
    data[index + 0x7ffc] | (data[index + 0x7ffd] << 8), //Reset
    data[index + 0x7fea] | (data[index + 0x7feb] << 8), //NMI
    data[index + 0x7fee] | (data[index + 0x7fef] << 8), //IRQ
    data[index + 0x7fe6] | (data[index + 0x7fe7] << 8), //BRK[n]
    data[index + 0x7fe4] | (data[index + 0x7fe5] << 8), //COP[n]
    data[index + 0x7ffe] | (data[index + 0x7fff] << 8), //BRK[e]
    data[index + 0x7ff4] | (data[index + 0x7ff5] << 8)  //COP[e]
  );
  dprintf("");

  rom_loaded = true;
}

void bROM::unload() {
  if(rom_loaded == false)return;

  if(data)memfree(data, "bROM::data");
  if(sram_data)memfree(sram_data, "bROM::sram_data");

  rom_loaded = false;
}

void bROM::write_protect(bool yn) {
  write_protection = yn;
}

bROM::bROM() {
  sram_size = 0;
  sram_data = 0;
  write_protection = true;

  rom_loaded = false;
}

bROM::~bROM() {
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
      r = rom->read(addr);
    }
  } else if(b <= 0x7d) {
    r = rom->read(addr);
  } else if(b <= 0x7f) {
    r = wram[addr & 0x01ffff];
  } else if(b <= 0xbf) {
    if(w <= 0x1fff) {
      r = wram[w];
    } else if(w <= 0x5fff) {
      r = mmio[w - 0x2000]->read(w);
    } else {
      r = rom->read(addr);
    }
  } else {
    r = rom->read(addr);
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
      rom->write(addr, value);
    }
  } else if(b <= 0x7d) {
    rom->write(addr, value);
  } else if(b <= 0x7f) {
    wram[addr & 0x01ffff] = value;
  } else if(b <= 0xbf) {
    if(w <= 0x1fff) {
      wram[w] = value;
    } else if(w <= 0x5fff) {
      mmio[w - 0x2000]->write(w, value);
    } else {
      rom->write(addr, value);
    }
  } else {
    rom->write(addr, value);
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
  rom = new bROM();

  wram = (byte*)memalloc(0x020000, "bMemBus::wram");
  memset(wram, 0, 0x020000);
}

bMemBus::~bMemBus() {
  delete(rom);

  if(wram)memfree(wram, "bMemBus::wram");
}

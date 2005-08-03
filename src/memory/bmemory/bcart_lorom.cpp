void bCartLoROM::write_protect(bool r) { write_protected = r; }

uint8 bCartLoROM::read(uint32 addr) {
uint32 b, w;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

//SRAM Region A
  if((b & 0x7f) >= 0x30 && (b & 0x7f) <= 0x3f && (w & 0xe000) == 0x6000) {
    b &= 0x7f;
    if(b >= 0x30 && b <= 0x3f) {
      if(sram_size) {
        addr = (b - 0x30) * 0x2000 + (w - 0x6000);
        addr &= (sram_size - 1);
        return sram[addr];
      } else {
        return 0x00; //no SRAM available
      }
    } else {
      return 0x00; //unmapped
    }
  }

//SRAM Region B
  if(b >= 0x70 && b <= 0x7d) {
    if(sram_size) {
      addr = (addr & 0xffffff) - 0x700000;
      addr &= (sram_size - 1);
      return sram[addr];
    } else {
      return 0x00; //no SRAM available
    }
  }

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
  if(addr < rom_size)return rom[addr];

  return 0x00;
}

void bCartLoROM::write(uint32 addr, uint8 value) {
uint32 b, w;
  addr &= 0xffffff;
  b = (addr >> 16);
  w = (addr & 0xffff);

//SRAM Region A
  if((b & 0x7f) >= 0x30 && (b & 0x7f) <= 0x3f && (w & 0xe000) == 0x6000) {
    b &= 0x7f;
    if(b >= 0x30 && b <= 0x3f) {
      if(sram_size) {
        addr = (b - 0x30) * 0x2000 + (w - 0x6000);
        addr &= (sram_size - 1);
        sram[addr] = value;
        return;
      } else {
        return; //no SRAM available
      }
    } else {
      return; //unmapped
    }
  }

//SRAM Region B
  if(b >= 0x70 && b <= 0x7d) {
    if(sram_size) {
      addr = (addr & 0xffffff) - 0x700000;
      addr &= (sram_size - 1);
      sram[addr] = value;
    } else {
      return; //no SRAM available
    }
  }

  if(write_protected == true)return;
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
  if(addr < rom_size)rom[addr] = value;
}

void bCartLoROM::set_cartinfo(CartInfo *ci) {
  rom       = ci->rom;
  sram      = ci->sram;
  rom_size  = ci->rom_size;
  sram_size = ci->sram_size;
}

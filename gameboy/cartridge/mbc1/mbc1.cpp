#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC1::mmio_read(uint16 addr) {
  if(addr >= 0x0000 && addr <= 0x3fff) {
    return cartridge.rom_read(addr);
  }

  if(addr >= 0x4000 && addr <= 0x7fff) {
    return cartridge.rom_read(rom_bank | (addr & 0x3fff));
  }

  if(addr >= 0xa000 && addr <= 0xbfff) {
    if(ram_enable) return cartridge.ram_read(ram_bank | (addr & 0x1fff));
    return 0x00;
  }
}

void Cartridge::MBC1::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0x0000 && addr <= 0x1fff) {
    ram_enable = (data & 0x0f) == 0x0a;
  }

  if(addr >= 0x2000 && addr <= 0x3fff) {
    rom_select = data & 0x1f;
    if(rom_select == 0) rom_select = 1;
  }

  if(addr >= 0x4000 && addr <= 0x5fff) {
    ram_select = data & 0x03;
  }

  if(addr >= 0x6000 && addr <= 0x7fff) {
    mode_select = data & 0x01;
  }

  if(mode_select == 0) {
    rom_bank = (ram_select << 19) | (rom_select << 14);
    ram_bank = 0x00;
  } else {
    rom_bank = (rom_select << 14);
    ram_bank = (ram_select << 13);
  }

  if(addr >= 0xa000 && addr <= 0xbfff) {
    if(ram_enable) cartridge.ram_write(ram_bank | (addr & 0x1fff), data);
  }
}

void Cartridge::MBC1::power() {
}

void Cartridge::MBC1::reset() {
  ram_enable = false;
  rom_select = 0x01;
  ram_select = 0x00;
  mode_select = 0;

  rom_bank = 0x4000;
  ram_bank = 0x0000;
}

#endif

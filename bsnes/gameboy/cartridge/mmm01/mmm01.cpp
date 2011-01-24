#ifdef CARTRIDGE_CPP

uint8 Cartridge::MMM01::mmio_read(uint16 addr) {
  if(within<0x0000, 0x7fff>(addr)) {
    if(rom_mode == 0) return cartridge.rom_read(addr);
  }

  if(within<0x0000, 0x3fff>(addr)) {
    return cartridge.rom_read(0x8000 + (rom_base << 14) + (addr & 0x3fff));
  }

  if(within<0x4000, 0x7fff>(addr)) {
    return cartridge.rom_read(0x8000 + (rom_base << 14) + (rom_select << 14) + (addr & 0x3fff));
  }

  if(within<0xa000, 0xbfff>(addr)) {
    if(ram_enable) return cartridge.ram_read((ram_select << 13) + (addr & 0x1fff));
    return 0x00;
  }

  return 0x00;
}

void Cartridge::MMM01::mmio_write(uint16 addr, uint8 data) {
  if(within<0x0000, 0x1fff>(addr)) {
    if(rom_mode == 0) {
      rom_mode = 1;
    } else {
      ram_enable = (data & 0x0f) == 0x0a;
    }
  }

  if(within<0x2000, 0x3fff>(addr)) {
    if(rom_mode == 0) {
      rom_base = data & 0x3f;
    } else {
      rom_select = data;
    }
  }

  if(within<0x4000, 0x5fff>(addr)) {
    if(rom_mode == 1) {
      ram_select = data;
    }
  }

  if(within<0x6000, 0x7fff>(addr)) {
    //unknown purpose
  }

  if(within<0xa000, 0xbfff>(addr)) {
    if(ram_enable) cartridge.ram_write((ram_select << 13) + (addr & 0x1fff), data);
  }
}

void Cartridge::MMM01::power() {
  rom_mode = 0;
  rom_base = 0x00;

  ram_enable = false;
  rom_select = 0x01;
  ram_select = 0x00;
}

#endif

auto Cartridge::MBC1::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    if(mode == 0) {
      return cartridge.readROM(ram.select << 19 | rom.select << 14 | (uint14)addr);
    } else {
      return cartridge.readROM(rom.select << 14 | (uint14)addr);
    }
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) {
      if(mode == 0) {
        return cartridge.readRAM((uint13)addr);
      } else {
        return cartridge.readRAM(ram.select << 13 | (uint13)addr);
      }
    }
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC1::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    ram.enable = (data & 0x0f) == 0x0a;
    return;
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    rom.select = (data & 0x1f) + ((data & 0x1f) == 0);
    return;
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    ram.select = data & 0x03;
    return;
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    mode = data & 0x01;
    return;
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) {
      if(mode == 0) {
        cartridge.writeRAM(addr & 0x1fff, data);
      } else {
        cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
      }
    }
    return;
  }
}

auto Cartridge::MBC1::power() -> void {
  rom.select = 0x01;
  ram.enable = false;
  ram.select = 0x00;
  mode = 0;
}

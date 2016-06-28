auto Cartridge::HuC1::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.select << 14 | (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.readRAM(ram.select << 13 | (uint13)addr);
  }

  return 0xff;
}

auto Cartridge::HuC1::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    ram.writable = data.bits(0,3) == 0x0a;
    return;
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    rom.select = data + (data == 0);
    return;
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    ram.select = data;
    return;
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    model = data.bit(0);
    return;
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(!ram.writable) return;
    return cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
  }
}

auto Cartridge::HuC1::power() -> void {
  rom.select = 0x01;
  ram.writable = false;
  ram.select = 0x00;
  model = 0;
}

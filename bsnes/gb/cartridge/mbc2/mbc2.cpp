auto Cartridge::MBC2::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(bits(address,0-13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | bits(address,0-13));
  }

  if((address & 0xee01) == 0xa000) {  //$a000-a1ff (even)
    if(!io.ram.enable) return 0xff;
    auto ram = cartridge.ram.read(bits(address,1-8));
    return 0xf0 | bits(ram,0-3);
  }

  if((address & 0xee01) == 0xa001) {  //$a000-a1ff (odd)
    if(!io.ram.enable) return 0xff;
    auto ram = cartridge.ram.read(bits(address,1-8));
    return 0xf0 | bits(ram,4-7);
  }

  return 0xff;
}

auto Cartridge::MBC2::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    if(!bit1(address,8)) io.ram.enable = bits(data,0-3) == 0x0a;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    if(bit1(address,8)) io.rom.bank = bits(data,0-3);
    if(!io.rom.bank) io.rom.bank = 0x01;
    return;
  }

  if((address & 0xee01) == 0xa000) {  //$a000-a1ff (even)
    if(!io.ram.enable) return;
    auto ram = cartridge.ram.read(bits(address,1-8));
    bits(ram,0-3) = (uint)bits(data,0-3);
    cartridge.ram.write(bits(address,1-8), ram);
    return;
  }

  if((address & 0xee01) == 0xa001) {  //$a000-a1ff (odd)
    if(!io.ram.enable) return;
    auto ram = cartridge.ram.read(bits(address,1-8));
    bits(ram,4-7) = (uint)bits(data,0-3);
    cartridge.ram.write(bits(address,1-8), ram);
    return;
  }
}

auto Cartridge::MBC2::power() -> void {
  io = {};
}

auto Cartridge::MBC2::serialize(serializer& s) -> void {
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
}

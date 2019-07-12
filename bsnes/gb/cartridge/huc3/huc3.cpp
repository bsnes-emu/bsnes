auto Cartridge::HuC3::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(bits(address,0-13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | bits(address,0-13));
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return 0x01;  //does not return open collection
    return cartridge.ram.read(io.ram.bank << 13 | bits(address,0-12));
  }

  return 0xff;
}

auto Cartridge::HuC3::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.enable = bits(data,0-3) == 0x0a;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank = data;
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    io.ram.bank = data;
    return;
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return;
    return cartridge.ram.write(io.ram.bank << 13 | bits(address,0-12), data);
  }
}

auto Cartridge::HuC3::power() -> void {
  io = {};
}

auto Cartridge::HuC3::serialize(serializer& s) -> void {
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank);
}

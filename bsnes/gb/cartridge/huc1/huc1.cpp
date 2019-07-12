auto Cartridge::HuC1::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(bits(address,0-13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | bits(address,0-13));
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.ram.read(io.ram.bank << 13 | bits(address,0-12));
  }

  return 0xff;
}

auto Cartridge::HuC1::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.writable = bits(data,0-3) == 0x0a;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank = data;
    if(!io.rom.bank) io.rom.bank = 0x01;
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    io.ram.bank = data;
    return;
  }

  if((address & 0xe000) == 0x6000) {  //$6000-7fff
    io.model = data & 1;
    return;
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.writable) return;
    return cartridge.ram.write(io.ram.bank << 13 | bits(address,0-12), data);
  }
}

auto Cartridge::HuC1::power() -> void {
  io = {};
}

auto Cartridge::HuC1::serialize(serializer& s) -> void {
  s.integer(io.model);
  s.integer(io.rom.bank);
  s.integer(io.ram.writable);
  s.integer(io.ram.bank);
}

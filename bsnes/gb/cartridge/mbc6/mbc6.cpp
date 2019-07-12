auto Cartridge::MBC6::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(bits(address,0-13));
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    return cartridge.rom.read(io.rom.bank[0] << 13 | bits(address,0-12));
  }

  if((address & 0xe000) == 0x6000) {  //$6000-7fff
    return cartridge.rom.read(io.rom.bank[1] << 13 | bits(address,0-12));
  }

  if((address & 0xf000) == 0xa000) {  //$a000-afff
    if(!io.ram.enable) return 0xff;
    return cartridge.ram.read(io.ram.bank[0] << 12 | bits(address,0-11));
  }

  if((address & 0xf000) == 0xb000) {  //$b000-bfff
    if(!io.ram.enable) return 0xff;
    return cartridge.ram.read(io.ram.bank[1] << 12 | bits(address,0-11));
  }

  return 0xff;
}

auto Cartridge::MBC6::write(uint16 address, uint8 data) -> void {
  if((address & 0xfc00) == 0x0000) {
    io.ram.enable = bits(data,0-3) == 0xa;
    return;
  }

  if((address & 0xfc00) == 0x0400) {
    io.ram.bank[0] = data;
    return;
  }

  if((address & 0xfc00) == 0x0800) {
    io.ram.bank[1] = data;
    return;
  }

  if((address & 0xf800) == 0x2000) {
    io.rom.bank[0] = data;
    return;
  }

  if((address & 0xf800) == 0x3000) {
    io.rom.bank[1] = data;
    return;
  }

  if((address & 0xf000) == 0xa000) {  //$a000-afff
    if(!io.ram.enable) return;
    return cartridge.ram.write(io.ram.bank[0] << 12 | bits(address,0-11), data);
  }

  if((address & 0xf000) == 0xb000) {  //$b000-bfff
    if(!io.ram.enable) return;
    return cartridge.ram.write(io.ram.bank[1] << 12 | bits(address,0-11), data);
  }
}

auto Cartridge::MBC6::power() -> void {
  io = {};
}

auto Cartridge::MBC6::serialize(serializer& s) -> void {
  s.integer(io.rom.bank[0]);
  s.integer(io.rom.bank[1]);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank[0]);
  s.integer(io.ram.bank[1]);
}

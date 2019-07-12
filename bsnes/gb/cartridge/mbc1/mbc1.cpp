auto Cartridge::MBC1::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(bits(address,0-13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    if(io.mode == 0) {
      return cartridge.rom.read(io.ram.bank << 19 | io.rom.bank << 14 | bits(address,0-13));
    } else {
      return cartridge.rom.read(io.rom.bank << 14 | bits(address,0-13));
    }
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return 0xff;
    if(io.mode == 0) {
      return cartridge.ram.read(bits(address,0-12));
    } else {
      return cartridge.ram.read(io.ram.bank << 13 | bits(address,0-12));
    }
  }

  return 0xff;
}

auto Cartridge::MBC1::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.enable = bits(data,0-3) == 0x0a;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank = bits(data,0-4);
    if(!io.rom.bank) io.rom.bank = 0x01;
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    io.ram.bank = bits(data,0-1);
    return;
  }

  if((address & 0xe000) == 0x6000) {  //$6000-7fff
    io.mode = bit1(data,0);
    return;
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return;
    if(io.mode == 0) {
      return cartridge.ram.write(bits(address,0-12), data);
    } else {
      return cartridge.ram.write(io.ram.bank << 13 | bits(address,0-12), data);
    }
  }
}

auto Cartridge::MBC1::power() -> void {
  io = {};
}

auto Cartridge::MBC1::serialize(serializer& s) -> void {
  s.integer(io.mode);
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank);
}

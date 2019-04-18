auto Cartridge::MBC1M::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    if(io.mode == 0) return cartridge.rom.read(address.bits(0,13));
    return cartridge.rom.read(io.rom.bank.bits(4,5) << 18 | address.bits(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address.bits(0,13));
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.ram.read(address.bits(0,12));
  }

  return 0xff;
}

auto Cartridge::MBC1M::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank.bits(0,3) = data.bits(0,3);
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    io.rom.bank.bits(4,5) = data.bits(0,1);
  }

  if((address & 0xe000) == 0x6000) {  //$6000-7fff
    io.mode = data.bit(0);
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    cartridge.ram.write(address.bits(0,13), data);
  }
}

auto Cartridge::MBC1M::power() -> void {
  io = {};
}

auto Cartridge::MBC1M::serialize(serializer& s) -> void {
  s.integer(io.mode);
  s.integer(io.rom.bank);
}

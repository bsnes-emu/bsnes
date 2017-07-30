auto Cartridge::MBC6::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(address.bits(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address.bits(0,13));
  }

  return 0xff;
}

auto Cartridge::MBC6::write(uint16 address, uint8 data) -> void {
  if((address & 0xf000) == 0x2000) {  //$2000-2fff
    io.rom.bank = data;
    return;
  }
}

auto Cartridge::MBC6::power() -> void {
  io = {};
}

auto Cartridge::MBC6::serialize(serializer& s) -> void {
}

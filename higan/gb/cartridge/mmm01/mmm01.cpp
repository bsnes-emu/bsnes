auto Cartridge::MMM01::read(uint16 address) -> uint8 {
  if(io.mode == 0) {
    if((address & 0x8000) == 0x0000) {  //$0000-7fff
      return cartridge.rom.read(cartridge.rom.size - 0x8000 + address.bits(0,14));
    }

    return 0xff;
  } else {
    if((address & 0xc000) == 0x0000) {  //$0000-3fff
      return cartridge.rom.read((io.rom.base << 14) + address.bits(0,13));
    }

    if((address & 0xc000) == 0x4000) {  //$4000-7fff
      return cartridge.rom.read((io.rom.base << 14) + (io.rom.bank << 14) + address.bits(0,13));
    }

    if((address & 0xe000) == 0xa000) {  //$a000-bfff
      if(!io.ram.enable) return 0xff;
      return cartridge.ram.read(io.ram.bank << 13 | address.bits(0,12));
    }

    return 0xff;
  }
}

auto Cartridge::MMM01::write(uint16 address, uint8 data) -> void {
  if(io.mode == 0) {
    if((address & 0xe000) == 0x0000) {  //$0000-1fff
      io.mode = 1;
    }

    if((address & 0xe000) == 0x2000) {  //$2000-3fff
      io.rom.base = data.bits(0,5);
    }
  } else {
    if((address & 0xe000) == 0x0000) {  //$0000-1fff
      io.ram.enable = data.bits(0,3) == 0x0a;
    }

    if((address & 0xe000) == 0x2000) {  //$2000-3fff
      io.rom.bank = data;
    }

    if((address & 0xe000) == 0x4000) {  //$4000-5fff
      io.ram.bank = data;
    }

    if((address & 0xe000) == 0xa000) {  //$a000-bfff
      if(!io.ram.enable) return;
      cartridge.ram.write(io.ram.bank << 13 | address.bits(0,12), data);
    }
  }
}

auto Cartridge::MMM01::power() -> void {
  io = {};
}

auto Cartridge::MMM01::serialize(serializer& s) -> void {
  s.integer(io.mode);
  s.integer(io.rom.base);
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank);
}

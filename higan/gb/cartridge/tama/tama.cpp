//U1: TAMA7: Mask ROM (512KB)
//U2: TAMA5: Game Boy cartridge connector interface
//U3: TAMA6: Toshiba TMP47C243M (4-bit MCU)
//U4: RTC: Toshiba TC8521AM

//note: the TMP47C243M's 2048 x 8-bit program ROM is currently undumped
//as such, high level emulation is used as a necessary evil

auto Cartridge::TAMA::second() -> void {
}

auto Cartridge::TAMA::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(address.bits(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address.bits(0,13));
  }

  if((address & 0xe001) == 0xa000) {  //$a000-bfff (even)
    if(io.select == 0x0a) {
      return 0xf0 | io.ready;
    }

    if(io.select == 0x0c) {
      return 0xf0 | io.output.bits(0,3);
    }

    if(io.select == 0x0d) {
      return 0xf0 | io.output.bits(4,7);
    }

    return 0xff;
  }

  if((address & 0xe001) == 0xa001) {  //$a000-bfff (odd)
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::TAMA::write(uint16 address, uint8 data) -> void {
  if((address & 0xe001) == 0xa000) {  //$a000-bfff (even)
    if(io.select == 0x00) {
      io.rom.bank.bits(0,3) = data.bits(0,3);
    }

    if(io.select == 0x01) {
      io.rom.bank.bit(4) = data.bit(0);
    }

    if(io.select == 0x04) {
      io.input.bits(0,3) = data.bits(0,3);
    }

    if(io.select == 0x05) {
      io.input.bits(4,7) = data.bits(0,3);
    }

    if(io.select == 0x06) {
      io.index.bit(4) = data.bit(0);
      io.mode = data.bits(1,3);
    }

    if(io.select == 0x07) {
      io.index.bits(0,3) = data.bits(0,3);

      if(io.mode == 0) {
        cartridge.ram.write(io.index, io.input);
      }

      if(io.mode == 1) {
        io.output = cartridge.ram.read(io.index);
      }
    }

    return;
  }

  if((address & 0xe001) == 0xa001) {  //$a000-bfff (odd)
    io.select = data.bits(0,3);

    if(io.select == 0x0a) {
      io.ready = true;
    }

    return;
  }
}

auto Cartridge::TAMA::readRTC(uint1 page, uint4 address) -> uint4 {
  if(address >= 13) return 0xf;
  auto ram = cartridge.rtc.read(page * 13 + address.bits(1,3));
  if(!address.bit(0)) {
    return ram.bits(0,3);
  } else {
    return ram.bits(4,7);
  }
}

auto Cartridge::TAMA::writeRTC(uint1 page, uint4 address, uint4 data) -> void {
  if(address >= 13) return;
  auto ram = cartridge.rtc.read(page * 13 + address.bits(1,3));
  if(!address.bit(0)) {
    ram.bits(0,3) = data;
  } else {
    ram.bits(4,7) = data;
  }
  cartridge.rtc.write(page * 13 + address.bits(1,3), ram);
}

auto Cartridge::TAMA::power() -> void {
  io = {};
}

auto Cartridge::TAMA::serialize(serializer& s) -> void {
  s.integer(io.ready);
  s.integer(io.select);
  s.integer(io.mode);
  s.integer(io.index);
  s.integer(io.input);
  s.integer(io.output);
  s.integer(io.rom.bank);
}

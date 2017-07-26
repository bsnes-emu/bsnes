auto Cartridge::MBC3::second() -> void {
  if(io.rtc.halt) return;
  if(++io.rtc.second >= 60) {
    io.rtc.second = 0;
    if(++io.rtc.minute >= 60) {
      io.rtc.minute = 0;
      if(++io.rtc.hour >= 24) {
        io.rtc.hour = 0;
        if(++io.rtc.day == 0) {
          io.rtc.dayCarry = true;
        }
      }
    }
  }
}

auto Cartridge::MBC3::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(address.bits(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address.bits(0,13));
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return 0xff;
    if(io.ram.bank <= 0x03) return cartridge.ram.read(io.ram.bank << 13 | address.bits(0,12));
    if(io.ram.bank == 0x08) return io.rtc.latchSecond;
    if(io.ram.bank == 0x09) return io.rtc.latchMinute;
    if(io.ram.bank == 0x0a) return io.rtc.latchHour;
    if(io.ram.bank == 0x0b) return io.rtc.latchDay;
    if(io.ram.bank == 0x0c) return io.rtc.latchDayCarry << 7 | io.rtc.latchDay >> 8;
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC3::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.enable = data.bits(0,3) == 0x0a;
    return;
  }

  if((address & 0xe000) == 0x2000) {  //$2000-3fff
    io.rom.bank = data.bits(0,6);
    if(!io.rom.bank) io.rom.bank = 0x01;
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    io.ram.bank = data;
    return;
  }

  if((address & 0xe000) == 0x6000) {  //$6000-7fff
    if(io.rtc.latch == 0 && data == 1) {
      io.rtc.latchSecond = io.rtc.second;
      io.rtc.latchMinute = io.rtc.minute;
      io.rtc.latchHour = io.rtc.hour;
      io.rtc.latchDay = io.rtc.day;
      io.rtc.latchDayCarry = io.rtc.dayCarry;
    }
    io.rtc.latch = data;
    return;
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return;
    if(io.ram.bank <= 0x03) {
      cartridge.ram.write(io.ram.bank << 13 | address.bits(0,12), data);
    } else if(io.ram.bank == 0x08) {
      if(data >= 60) data = 0;
      io.rtc.second = data;
    } else if(io.ram.bank == 0x09) {
      if(data >= 60) data = 0;
      io.rtc.minute = data;
    } else if(io.ram.bank == 0x0a) {
      if(data >= 24) data = 0;
      io.rtc.hour = data;
    } else if(io.ram.bank == 0x0b) {
      io.rtc.day.bits(0,7) = data.bits(0,7);
    } else if(io.ram.bank == 0x0c) {
      io.rtc.day.bit(8) = data.bit(0);
      io.rtc.halt = data.bit(6);
      io.rtc.dayCarry = data.bit(7);
    }
    return;
  }
}

auto Cartridge::MBC3::power() -> void {
  io = {};
}

auto Cartridge::MBC3::serialize(serializer& s) -> void {
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank);
  s.integer(io.rtc.halt);
  s.integer(io.rtc.latch);
  s.integer(io.rtc.second);
  s.integer(io.rtc.minute);
  s.integer(io.rtc.hour);
  s.integer(io.rtc.day);
  s.integer(io.rtc.dayCarry);
  s.integer(io.rtc.latchSecond);
  s.integer(io.rtc.latchMinute);
  s.integer(io.rtc.latchHour);
  s.integer(io.rtc.latchDay);
  s.integer(io.rtc.latchDayCarry);
}

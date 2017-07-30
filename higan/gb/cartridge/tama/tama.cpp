//U1: TAMA7: Mask ROM (512KB)
//U2: TAMA5: Game Boy cartridge connector interface
//U3: TAMA6: Toshiba TMP47C243M (4-bit MCU)
//U4: RTC: Toshiba TC8521AM

//note: the TMP47C243M's 2048 x 8-bit program ROM is currently undumped
//as such, high level emulation is used as a necessary evil

auto Cartridge::TAMA::second() -> void {
  if(++rtc.second >= 60) {
    rtc.second = 0;

    if(++rtc.minute >= 60) {
      rtc.minute = 0;

      if(rtc.hourMode == 0 && ++rtc.hour >= 12) {
        rtc.hour = 0;
        rtc.meridian++;
      }

      if(rtc.hourMode == 1 && ++rtc.hour >= 24) {
        rtc.hour = 0;
        rtc.meridian = rtc.hour >= 12;
      }

      if((rtc.hourMode == 0 && rtc.hour == 0 && rtc.meridian == 0)
      || (rtc.hourMode == 1 && rtc.hour == 0)
      ) {
        uint days[12] = {31, 28, 31, 30, 31, 30, 30, 31, 30, 31, 30, 31};
        if(rtc.leapYear == 0) days[1] = 29;  //extra day in February for leap years

        if(++rtc.day > days[(rtc.month - 1) % 12]) {
          rtc.day = 1;

          if(++rtc.month > 12) {
            rtc.month = 1;
            rtc.leapYear++;

            if(++rtc.year >= 100) {
              rtc.year = 0;
            }
          }
        }
      }
    }
  }
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

    if(io.mode == 0 || io.mode == 1) {
      if(io.select == 0x0c) {
        return 0xf0 | io.output.bits(0,3);
      }

      if(io.select == 0x0d) {
        return 0xf0 | io.output.bits(4,7);
      }
    }

    if(io.mode == 2 || io.mode == 4) {
      if(io.select == 0x0c || io.select == 0x0d) {
        uint4 data;
        if(rtc.index == 0) data = rtc.minute % 10;
        if(rtc.index == 1) data = rtc.minute / 10;
        if(rtc.index == 2) data = rtc.hour % 10;
        if(rtc.index == 3) data = rtc.hour / 10;
        if(rtc.index == 4) data = rtc.day / 10;
        if(rtc.index == 5) data = rtc.day % 10;
        if(rtc.index == 6) data = rtc.month / 10;
        if(rtc.index == 7) data = rtc.month % 10;
        rtc.index++;
        return 0xf0 | data;
      }
    }

    return 0xff;
  }

  if((address & 0xe001) == 0xa001) {  //$a000-bfff (odd)
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::TAMA::write(uint16 address, uint8 data) -> void {
  auto toBCD = [](uint8 data) -> uint8 { return (data / 10) * 16 + (data % 10); };
  auto fromBCD = [](uint8 data) -> uint8 { return (data / 16) * 10 + (data % 16); };

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

      if(io.mode == 2 && io.index == 0x04) {
        rtc.minute = fromBCD(io.input);
      }

      if(io.mode == 2 && io.index == 0x05) {
        rtc.hour = fromBCD(io.input);
        rtc.meridian = rtc.hour >= 12;
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0x7) {
        uint8 day = toBCD(rtc.day);
        day.bits(0,3) = io.input.bits(4,7);
        rtc.day = fromBCD(day);
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0x8) {
        uint8 day = toBCD(rtc.day);
        day.bits(4,7) = io.input.bits(4,7);
        rtc.day = fromBCD(day);
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0x9) {
        uint8 month = toBCD(rtc.month);
        month.bits(0,3) = io.input.bits(4,7);
        rtc.month = fromBCD(month);
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0xa) {
        uint8 month = toBCD(rtc.month);
        month.bits(4,7) = io.input.bits(4,7);
        rtc.month = fromBCD(month);
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0xb) {
        uint8 year = toBCD(rtc.year);
        year.bits(0,3) = io.input.bits(4,7);
        rtc.year = fromBCD(year);
      }

      if(io.mode == 4 && io.index == 0x00 && io.input.bits(0,3) == 0xc) {
        uint8 year = toBCD(rtc.year);
        year.bits(4,7) = io.input.bits(4,7);
        rtc.year = fromBCD(year);
      }

      if(io.mode == 4 && io.index == 0x02 && io.input.bits(0,3) == 0xa) {
        rtc.hourMode = io.input.bit(4);
        rtc.second = 0;  //hack: unclear where this is really being set (if it is at all)
      }

      if(io.mode == 4 && io.index == 0x02 && io.input.bits(0,3) == 0xb) {
        rtc.leapYear = data.bits(4,5);
      }

      if(io.mode == 4 && io.index == 0x02 && io.input.bits(0,3) == 0xe) {
        rtc.test = io.input.bits(4,7);
      }

      if(io.mode == 2 && io.index == 0x06) {
        rtc.index = 0;
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

  s.integer(rtc.year);
  s.integer(rtc.month);
  s.integer(rtc.day);
  s.integer(rtc.hour);
  s.integer(rtc.minute);
  s.integer(rtc.second);
  s.integer(rtc.meridian);
  s.integer(rtc.leapYear);
  s.integer(rtc.hourMode);
  s.integer(rtc.test);
}

auto Cartridge::MBC3::second() -> void {
  if(!rtc.halt) {
    if(++rtc.second >= 60) {
      rtc.second = 0;
      if(++rtc.minute >= 60) {
        rtc.minute = 0;
        if(++rtc.hour >= 24) {
          rtc.hour = 0;
          if(++rtc.day >= 512) {
            rtc.day = 0;
            rtc.dayCarry = true;
          }
        }
      }
    }
  }
}

auto Cartridge::MBC3::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.select<< 14 | (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) {
      if(ram.select <= 0x03) {
        return cartridge.readRAM(ram.select << 13 | (uint13)addr);
      }
      if(ram.select == 0x08) return rtc.latchSecond;
      if(ram.select == 0x09) return rtc.latchMinute;
      if(ram.select == 0x0a) return rtc.latchHour;
      if(ram.select == 0x0b) return rtc.latchDay;
      if(ram.select == 0x0c) return rtc.latchDayCarry << 7 | rtc.latchDay >> 8;
    }
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC3::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    ram.enable = (data & 0x0f) == 0x0a;
    return;
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    rom.select = (data & 0x7f) + ((data & 0x7f) == 0);
    return;
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    ram.select = data;
    return;
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    if(rtc.latch == 0 && data == 1) {
      rtc.latchSecond = rtc.second;
      rtc.latchMinute = rtc.minute;
      rtc.latchHour = rtc.hour;
      rtc.latchDay = rtc.day;
      rtc.latchDayCarry = rtc.dayCarry;
    }
    rtc.latch = data;
    return;
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) {
      if(ram.select <= 0x03) {
        cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
      } else if(ram.select == 0x08) {
        if(data >= 60) data = 0;
        rtc.second = data;
      } else if(ram.select == 0x09) {
        if(data >= 60) data = 0;
        rtc.minute = data;
      } else if(ram.select == 0x0a) {
        if(data >= 24) data = 0;
        rtc.hour = data;
      } else if(ram.select == 0x0b) {
        rtc.day = (rtc.day & 0x0100) | data;
      } else if(ram.select == 0x0c) {
        rtc.day = ((data & 1) << 8) | (rtc.day & 0xff);
        rtc.halt = data & 0x40;
        rtc.dayCarry = data & 0x80;
      }
    }
    return;
  }
}

auto Cartridge::MBC3::power() -> void {
  rom.select = 0x01;

  ram.enable = false;
  ram.select = 0x00;

  rtc.latch = 0;

  rtc.halt = true;
  rtc.second = 0;
  rtc.minute = 0;
  rtc.hour = 0;
  rtc.day = 0;
  rtc.dayCarry = false;

  rtc.latchSecond = 0;
  rtc.latchMinute = 0;
  rtc.latchHour = 0;
  rtc.latchDay = 0;
  rtc.latchDayCarry = false;
}

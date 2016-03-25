//calculate time between last play of game and current time;
//increment RTC by said amount of seconds
auto Cartridge::rtcLoad() -> void {
  uint64 timestamp = 0;
  for(auto n : range(8)) timestamp.byte(n) = rtc.data[8 + n];
  if(!timestamp) return;  //new save file

  timestamp = time(0) - timestamp;
  while(timestamp--) rtcTickSecond();
}

//save time when game is unloaded
auto Cartridge::rtcSave() -> void {
  uint64 timestamp = time(0);
  for(auto n : range(8)) rtc.data[8 + n] = timestamp.byte(n);
}

auto Cartridge::rtcTickSecond() -> void {
  if(++rtc.second() < 60) return;
  rtc.second() = 0;

  if(++rtc.minute() < 60) return;
  rtc.minute() = 0;

  if(++rtc.hour() < 60) return;
  rtc.hour() = 0;

  rtc.weekday() += 1;
  rtc.weekday() %= 7;

  uint daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(rtc.year() && (rtc.year() % 100) && !(rtc.year() % 4)) daysInMonth[1]++;

  if(++rtc.day() < daysInMonth[rtc.month()]) return;
  rtc.day() = 0;

  if(++rtc.month() < 12) return;
  rtc.month() = 0;

  ++rtc.year();
}

auto Cartridge::rtcCheckAlarm() -> void {
  if(!rtc.alarm.bit(5)) return;

  if(rtc.hour() == rtc.alarmHour && rtc.minute() == rtc.alarmMinute) {
    cpu.raise(CPU::Interrupt::Cartridge);
  } else {
    cpu.lower(CPU::Interrupt::Cartridge);
  }
}

auto Cartridge::rtcStatus() -> uint8 {
  return 0x80;
}

auto Cartridge::rtcCommand(uint8 data) -> void {
  rtc.command = data;

  //RESET
  if(rtc.command == 0x10) {
    rtc.year() = 0;
    rtc.month() = 0;
    rtc.day() = 0;
    rtc.weekday() = 0;
    rtc.hour() = 0;
    rtc.minute() = 0;
    rtc.second() = 0;
  }

  //ALARM_FLAG
  if(rtc.command == 0x12) {
    rtc.index = 0;
  }

  //SET_DATETIME
  if(rtc.command == 0x14) {
    rtc.index = 0;
  }

  //GET_DATETIME
  if(rtc.command == 0x15) {
    rtc.index = 0;
  }

  //SET_ALARM
  if(rtc.command == 0x18) {
    rtc.index = 0;
  }
}

auto Cartridge::rtcRead() -> uint8 {
  uint8 data = 0;

  static auto encode = [](uint8 data) -> uint8 {
    return ((data / 10) << 4) + (data % 10);
  };

  //GET_DATETIME
  if(rtc.command == 0x15) {
    switch(rtc.index) {
    case 0: data = encode(rtc.year()); break;
    case 1: data = encode(rtc.month() + 1); break;
    case 2: data = encode(rtc.day() + 1); break;
    case 3: data = encode(rtc.weekday()); break;
    case 4: data = encode(rtc.hour()); break;
    case 5: data = encode(rtc.minute()); break;
    case 6: data = encode(rtc.second()); break;
    }
    if(++rtc.index >= 7) rtc.command = 0;
  }

  return data;
}

auto Cartridge::rtcWrite(uint8 data) -> void {
  static auto decode = [](uint8 data) -> uint8 {
    return (data >> 4) * 10 + (data & 0x0f);
  };

  //ALARM_FLAG
  if(rtc.command == 0x12) {
    if(data.bit(6)) rtc.alarm = data;  //todo: is bit6 really required to be set?
    rtc.command = 0;
  }

  //SET_DATETIME
  if(rtc.command == 0x14) {
    switch(rtc.index) {
    case 0: rtc.year() = decode(data); break;
    case 1: rtc.month() = decode(data) - 1; break;
    case 2: rtc.day() = decode(data) - 1; break;
    case 3: rtc.weekday() = decode(data); break;
    case 4: rtc.hour() = decode(data); break;
    case 5: rtc.minute() = decode(data); break;
    case 6: rtc.second() = decode(data); break;
    }
    if(++rtc.index >= 7) rtc.command = 0;
  }

  //SET_ALRM
  if(rtc.command == 0x18) {
    switch(rtc.index) {
    case 0: rtc.alarmHour = decode(data.bits(0,6)); break;
    case 1: rtc.alarmMinute = decode(data); break;
    }
    if(++rtc.index >= 2) rtc.command = 0;
  }
}

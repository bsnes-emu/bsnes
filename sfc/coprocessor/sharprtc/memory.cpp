auto SharpRTC::rtc_read(uint4 addr) -> uint4 {
  switch(addr) {
  case  0: return second % 10;
  case  1: return second / 10;
  case  2: return minute % 10;
  case  3: return minute / 10;
  case  4: return hour % 10;
  case  5: return hour / 10;
  case  6: return day % 10;
  case  7: return day / 10;
  case  8: return month;
  case  9: return year % 10;
  case 10: return year / 10 % 10;
  case 11: return year / 100;
  case 12: return weekday;
  default: return 0;
  }
}

auto SharpRTC::rtc_write(uint4 addr, uint4 data) -> void {
  switch(addr) {
  case  0: second = second / 10 * 10 + data; break;
  case  1: second = data * 10 + second % 10; break;
  case  2: minute = minute / 10 * 10 + data; break;
  case  3: minute = data * 10 + minute % 10; break;
  case  4: hour = hour / 10 * 10 + data; break;
  case  5: hour = data * 10 + hour % 10; break;
  case  6: day = day / 10 * 10 + data; break;
  case  7: day = data * 10 + day % 10; break;
  case  8: month = data; break;
  case  9: year = year / 10 * 10 + data; break;
  case 10: year = year / 100 * 100 + data * 10 + year % 10; break;
  case 11: year = data * 100 + year % 100; break;
  case 12: weekday = data; break;
  }
}

auto SharpRTC::load(const uint8* data) -> void {
  for(auto byte : range(8)) {
    rtc_write(byte * 2 + 0, data[byte] >> 0);
    rtc_write(byte * 2 + 1, data[byte] >> 4);
  }

  uint64 timestamp = 0;
  for(auto byte : range(8)) {
    timestamp |= data[8 + byte] << (byte * 8);
  }

  uint64 diff = (uint64)time(0) - timestamp;
  while(diff >= 60 * 60 * 24) { tick_day(); diff -= 60 * 60 * 24; }
  while(diff >= 60 * 60) { tick_hour(); diff -= 60 * 60; }
  while(diff >= 60) { tick_minute(); diff -= 60; }
  while(diff--) tick_second();
}

auto SharpRTC::save(uint8* data) -> void {
  for(auto byte : range(8)) {
    data[byte]  = rtc_read(byte * 2 + 0) << 0;
    data[byte] |= rtc_read(byte * 2 + 1) << 4;
  }

  uint64 timestamp = (uint64)time(nullptr);
  for(auto byte : range(8)) {
    data[8 + byte] = timestamp;
    timestamp >>= 8;
  }
}

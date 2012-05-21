#ifdef RTC4513_CPP

unsigned RTC4513::second() { return secondlo + secondhi * 10; }
unsigned RTC4513::minute() { return minutelo + minutehi * 10; }
unsigned RTC4513::hour  () { return hourlo   + hourhi   * 10; }
unsigned RTC4513::day   () { return daylo    + dayhi    * 10; }
unsigned RTC4513::month () { return monthlo  + monthhi  * 10; }
unsigned RTC4513::year  () { return yearlo   + yearhi   * 10; }

void RTC4513::second(unsigned data) { secondlo = data % 10; secondhi = (data / 10) % 10; }
void RTC4513::minute(unsigned data) { minutelo = data % 10; minutehi = (data / 10) % 10; }
void RTC4513::hour  (unsigned data) { hourlo   = data % 10; hourhi   = (data / 10) % 10; }
void RTC4513::day   (unsigned data) { daylo    = data % 10; dayhi    = (data / 10) % 10; }
void RTC4513::month (unsigned data) { monthlo  = data % 10; monthhi  = (data / 10) % 10; }
void RTC4513::year  (unsigned data) { yearlo   = data % 10; yearhi   = (data / 10) % 10; }

void RTC4513::rtc_reset() {
  state = State::Mode;
  offset = 0;

  pause = 0;
  test = 0;
  minutecarry = 0;
}

uint4 RTC4513::rtc_read(uint4 addr) {
  switch(addr) {
  case  0: return secondlo;
  case  1: return secondhi | batteryfailure << 3;
  case  2: return minutelo;
  case  3: return minutehi | minutecarry << 3;
  case  4: return hourlo;
  case  5: return hourhi | meridian << 2 | hourcarry << 3;
  case  6: return daylo;
  case  7: return dayhi | dayram << 2 | daycarry << 3;
  case  8: return monthlo;
  case  9: return monthhi | monthram << 1 | monthcarry << 3;
  case 10: return yearlo;
  case 11: return yearhi;
  case 12: return weekday | weekdaycarry << 3;
  case 13: {
    uint1 readflag = irqflag & irqmask;
    irqflag = 0;
    return hold | calendar << 1 | readflag << 2 | roundseconds << 3;
  }
  case 14: return irqmask | irqduty << 1 | irqperiod << 2;
  case 15: return pause | stop << 1 | atime << 2 | test << 3;
  }
}

void RTC4513::rtc_write(uint4 addr, uint4 data) {
  switch(addr) {
  case 0:
    secondlo = data;
    break;
  case 1:
    secondhi = data;
    batteryfailure = data >> 3;
    break;
  case 2:
    minutelo = data;
    break;
  case 3:
    minutehi = data;
    minutecarry = data >> 3;
    break;
  case 4:
    hourlo = data;
    break;
  case 5:
    hourhi = data;
    meridian = data >> 2;
    hourcarry = data >> 3;
    if(atime == 1) meridian = 0;
    if(atime == 0) hourhi &= 1;
    break;
  case 6:
    daylo = data;
    break;
  case 7:
    dayhi = data;
    dayram = data >> 2;
    daycarry = data >> 3;
    break;
  case 8:
    monthlo = data;
    break;
  case 9:
    monthhi = data;
    monthram = data >> 1;
    monthcarry = data >> 3;
    break;
  case 10:
    yearlo = data;
    break;
  case 11:
    yearhi = data;
    break;
  case 12:
    weekday = data;
    weekdaycarry = data >> 3;
    break;
  case 13:
    hold = data;
    calendar = data >> 1;
  //irqflag cannot be set manually
    roundseconds = data >> 3;
    if(roundseconds) {
      roundseconds = 0;
      if(second() >= 30) tick_minute();
      second(0);
    }
    break;
  case 14:
    irqmask = data;
    irqduty = data >> 1;
    irqperiod = data >> 2;
    break;
  case 15:
    pause = data;
    stop = data >> 1;
    atime = data >> 2;
    test = data >> 3;
    if(atime == 1) meridian = 0;
    if(atime == 0) hourhi &= 1;
    if(pause) second(0);
    break;
  }
}

void RTC4513::load(const uint8 *data) {
  for(unsigned byte = 0; byte < 8; byte++) {
    rtc_write(byte * 2 + 0, data[byte] >> 0);
    rtc_write(byte * 2 + 1, data[byte] >> 4);
  }

  uint64 timestamp = 0;
  for(unsigned byte = 0; byte < 8; byte++) {
    timestamp |= data[8 + byte] << (byte * 8);
  }

  uint64 diff = (uint64)time(0) - timestamp;
  while(diff >= 60 * 60 * 24) { tick_day(); diff -= 60 * 60 * 24; }
  while(diff >= 60 * 60) { tick_hour(); diff -= 60 * 60; }
  while(diff >= 60) { tick_minute(); diff -= 60; }
  while(diff--) tick_second();
}

void RTC4513::save(uint8 *data) {
  for(unsigned byte = 0; byte < 8; byte++) {
    data[byte]  = rtc_read(byte * 2 + 0) << 0;
    data[byte] |= rtc_read(byte * 2 + 1) << 4;
  }

  uint64 timestamp = (uint64)time(0);
  for(unsigned byte = 0; byte < 8; byte++) {
    data[8 + byte] = timestamp;
    timestamp >>= 8;
  }
}

#endif

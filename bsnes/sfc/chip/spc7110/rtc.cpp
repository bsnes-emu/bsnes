#ifdef SPC7110_CPP

//OFS  NAME  BIT:3  BIT:2  BIT:1  BIT:0
//---  ----  -----  -----  -----  -----
//0x0  S01   SEC3   SEC2   SEC1   SEC0
//0x1  S10   LOST   SEC6   SEC5   SEC4
//0x2  M01   MIN3   MIN2   MIN1   MIN0
//0x3  M10   WRAP   MIN6   MIN5   MIN4
//0x4  H01   HOUR3  HOUR2  HOUR1  HOUR0
//0x5  H10   WRAP   AM/PM  HOUR5  HOUR4
//0x6  D01   DAY3   DAY2   DAY1   DAY0
//0x7  D10   WRAP   RAM0   DAY5   DAY4
//0x8  MO01  MON3   MON2   MON1   MON0
//0x9  MO10  WRAP   RAM2   RAM1   MON4
//0xa  Y01   YEAR3  YEAR2  YEAR1  YEAR0
//0xb  Y10   YEAR7  YEAR6  YEAR5  YEAR4
//0xc  WDAY  WRAP   WEEK2  WEEK1  WEEK0
//0xd  CD    30ADJ  IRQF   CAL    HOLD
//0xe  CE    RATE1  RATE0  DUTY   MASK
//0xf  CF    TEST   24/12  STOP   RESET

void SPC7110::rtc_reset() {
  rtc_mode = 0;
  rtc_addr = 0;

  rtcram[0xf] &= ~1;  //clear reset
  rtcram[0xf] &= ~8;  //clear test

  rtcram[0x3] &= ~8;  //clear wrap
//rtcram[0x5] &= ~8;  //clear wrap
//if((rtcram[0xd] & 2) == 0) return;  //calendar mode disabled (bits are RAM)

//rtcram[0x7] &= ~8;  //clear wrap
//rtcram[0x9] &= ~8;  //clear wrap
//rtcram[0xc] &= ~8;  //clear wrap
}

void SPC7110::rtc_duty() {
  if(rtcram[0xe] & 2) rtcram[0xd] &= ~4;
}

void SPC7110::rtc_irq(uint2 frequency) {
  uint2 rate = rtcram[0xe] >> 2;
  if(frequency != rate) return;
  rtcram[0xd] |= 4;
}

uint4 SPC7110::rtc_read(uint4 addr) {
  switch(addr) { default:
  case 0x0: return rtcram[0x0];
  case 0x1: return rtcram[0x1];
  case 0x2: return rtcram[0x2];
  case 0x3: return rtcram[0x3];
  case 0x4: return rtcram[0x4];
  case 0x5: return rtcram[0x5];
  case 0x6: return rtcram[0x6];
  case 0x7: return rtcram[0x7];
  case 0x8: return rtcram[0x8];
  case 0x9: return rtcram[0x9];
  case 0xa: return rtcram[0xa];
  case 0xb: return rtcram[0xb];
  case 0xc: return rtcram[0xc];
  case 0xd: {
    uint4 data = rtcram[0xd];
    if(rtcram[0xe] & 1) data &= ~4;  //force irq flag clear if mask is set
    rtcram[0xd] &= ~4;  //always clear irq flag on read (acknowledge pending IRQ)
    return data;
  }
  case 0xe: return rtcram[0xe];
  case 0xf: return rtcram[0xf];
  }
}

void SPC7110::rtc_write(uint4 addr, uint4 data) {
  switch(addr) {
  case 0x0: rtcram[0x0] = data; break;
  case 0x1: rtcram[0x1] = data; break;
  case 0x2: rtcram[0x2] = data; break;
  case 0x3: rtcram[0x3] = data; break;
  case 0x4: rtcram[0x4] = data; break;
  case 0x5:
    if((rtcram[0xf] & 4) == 0) rtcram[0x5] = data & ~2;  //12-hour mode cannot set D5
    if((rtcram[0xf] & 4) != 0) rtcram[0x5] = data & ~4;  //24-hour mode cannot set AM/PM
    break;
  case 0x6: rtcram[0x6] = data; break;
  case 0x7: rtcram[0x7] = data; break;
  case 0x8: rtcram[0x8] = data; break;
  case 0x9: rtcram[0x9] = data; break;
  case 0xa: rtcram[0xa] = data; break;
  case 0xb: rtcram[0xb] = data; break;
  case 0xc: rtcram[0xc] = data; break;
  case 0xd: rtcram[0xd] = (rtcram[0xd] & 4) | (data & ~4);  //irq flag is read-only
    if(data & 8) {  //round to nearest minute
      unsigned second = (rtcram[0x1] & 7) * 10 + rtcram[0x0];
      rtcram[0x0] &= 0;
      rtcram[0x1] &= 8;
      if(second >= 30) rtc_minute();
    }
    break;
  case 0xe: rtcram[0xe] = data; break;
  case 0xf: rtcram[0xf] = data;
    if(data & 1) {
      //clear seconds
      rtcram[0x0] &= 0;
      rtcram[0x1] &= 8;
    }
    break;
  }
}

void SPC7110::rtc_pulse() {
  if(rtcram[0xd] & 1) return;  //clock hold
  if(rtcram[0xf] & 1) return;  //clock reset
  if(rtcram[0xf] & 2) return;  //clock stop

  //set wrap flags (time changed since last select)
  rtcram[0x3] |= 8;
//rtcram[0x5] |= 8;
//if(rtcram[0xd] & 2) {
//  rtcram[0x7] |= 8;
//  rtcram[0x9] |= 8;
//  rtcram[0xc] |= 8;
//}

  rtc_second();
}

void SPC7110::rtc_second() {
  unsigned second = (rtcram[0x1] & 0x7) * 10 + rtcram[0x0];

  if(++second > 59) second = 0;
  rtcram[0x0] = second % 10;
  rtcram[0x1] = (rtcram[0x1] & 8) | ((second / 10) & 7);

  if(second == 0) rtc_minute();
}

void SPC7110::rtc_minute() {
  unsigned minute = (rtcram[0x3] & 0x7) * 10 + rtcram[0x2];

  if(++minute > 59) minute = 0;
  rtcram[0x2] = minute % 10;
  rtcram[0x3] = (rtcram[0x3] & 8) | ((minute / 10) & 7);

  if(minute == 0) rtc_hour();
}

void SPC7110::rtc_hour() {
  unsigned hour = (rtcram[0x5] & 3) * 10 + rtcram[0x4];

  if(rtcram[0xf] & 4) {
    //24-hour mode (00-23)
    if(++hour > 23) hour = 0;
    rtcram[0x4] = hour % 10;
    rtcram[0x5] = (rtcram[0x5] & 12) | ((hour / 10) & 3);
    if(hour == 0) rtc_day();
  } else {
    //12-hour mode (01-12)
    if(++hour > 12) hour = 1;
    rtcram[0x4] = hour % 10;
    rtcram[0x5] = (rtcram[0x5] & 12) | ((hour / 10) & 3);
    if(hour == 12) {
      rtcram[0x5] ^= 4;  //toggle meridian
      if((rtcram[0x5] & 4) == 0) rtc_day();
    }
  }
}

void SPC7110::rtc_day() {
  //calendar disable
  if((rtcram[0xd] & 2) == 0) return;

  //calendar
  static const unsigned daysinmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  unsigned year = rtcram[0xb] * 10 + rtcram[0xa];
  unsigned month = (rtcram[0x9] & 1) * 10 + rtcram[0x8];
  unsigned day = (rtcram[0x7] & 3) * 10 + rtcram[0x6];
  unsigned weekday = rtcram[0xc] & 7;
  unsigned days = daysinmonth[month];

  //add leap year day if necessary
  //range is ~199x-209x; so year %400 -> year % 100 rules are unnecessary
  if(year % 4 == 0 && month == 2) days++;

  //day (01-31)
  if(++day > days) day = 1;
  rtcram[0x6] = day % 10;
  rtcram[0x7] = (rtcram[0x7] & 12) | ((day / 10) & 3);

  if(++weekday > 6) weekday = 0;
  rtcram[0xc] = (rtcram[0xc] & 8) | (weekday & 7);

  if(day == 1) rtc_month();
}

void SPC7110::rtc_month() {
  //month (01-12)
  unsigned month = (rtcram[0x9] & 1) * 10 + rtcram[0x8];

  if(++month > 12) month = 1;
  rtcram[0x8] = month % 10;
  rtcram[0x9] = (rtcram[0x9] & 14) | ((month / 12) & 1);

  if(month == 1) rtc_year();
}

void SPC7110::rtc_year() {
  //year (00-99)
  unsigned year = rtcram[0xb] * 10 + rtcram[0xa];

  if(++year > 99) year = 0;
  rtcram[0xa] = year % 10;
  rtcram[0xb] = year / 10;
}

void SPC7110::rtcram_load(const uint8 *data) {
  uint64 timestamp = 0;

  for(unsigned n = 0; n < 8; n++) {
    rtcram[n * 2 + 0] = data[n] >> 0;
    rtcram[n * 2 + 1] = data[n] >> 4;
  }

  for(unsigned n = 0; n < 8; n++) {
    timestamp |= data[8 + n] << (n * 8);
  }

  //determine the number of seconds that have passed since the last time the
  //RTC state was saved ... and add that many seconds to the saved RTC time.
  uint64 diff = (uint64)time(0) - timestamp;
  while(diff >= 60 * 60 * 24) { rtc_day(); diff -= 60 * 60 * 24; }
  while(diff >= 60 * 60) { rtc_hour(); diff -= 60 * 60; }
  while(diff >= 60) { rtc_minute(); diff -= 60; }
  while(diff--) rtc_second();
}

void SPC7110::rtcram_save(uint8 *data) {
  uint64 timestamp = (uint64)time(0);

  for(unsigned n = 0; n < 8; n++) {
    data[n]  = rtcram[n * 2 + 0] << 0;
    data[n] |= rtcram[n * 2 + 1] << 4;
  }

  for(unsigned n = 0; n < 8; n++) {
    data[8 + n] = timestamp;
    timestamp >>= 8;
  }
}

#endif

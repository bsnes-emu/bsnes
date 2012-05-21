#ifdef RTC4513_CPP

void RTC4513::irq(uint2 period) {
  if(period == irqperiod) irqflag = 1;
}

void RTC4513::duty() {
  if(irqduty) irqflag = 0;
}

void RTC4513::tick() {
  if(hold) return;
  if(pause) return;
  if(stop) return;

  minutecarry = true;
  tick_second();
}

void RTC4513::tick_second() {
  if(second() < 59) return second(second() + 1);
  second(0);
  tick_minute();
}

void RTC4513::tick_minute() {
  if(minute() < 59) return minute(minute() + 1);
  minute(0);
  tick_hour();
}

void RTC4513::tick_hour() {
  if(atime) {
    if(hour() < 23) return hour(hour() + 1);
    hour(0);
    tick_day();
  }

  else {
    if(hour() < 12) return hour(hour() + 1);
    hour(1);
    meridian ^= 1;
    if(meridian == 0) tick_day();
  }
}

void RTC4513::tick_day() {
  if(calendar == false) return;

  static const unsigned daysinmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  unsigned days = daysinmonth[month() - 1];
  if(year() % 4 == 0 && month() == 2) days++;

  if(day() < days) return day(day() + 1);
  day(1);

  if(weekday < 6) weekday++;
  else weekday = 0;

  tick_month();
}

void RTC4513::tick_month() {
  if(month() < 12) return month(month() + 1);
  month(1);
  tick_year();
}

void RTC4513::tick_year() {
  year(year() + 1);
}

#endif

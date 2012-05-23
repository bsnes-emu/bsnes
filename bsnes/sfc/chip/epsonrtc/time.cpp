#ifdef EPSONRTC_CPP

void EpsonRTC::irq(uint2 period) {
  if(stop) return;
  if(pause) return;

  if(period == irqperiod) irqflag = 1;
}

void EpsonRTC::duty() {
  if(irqduty) irqflag = 0;
}

void EpsonRTC::tick() {
  if(stop) return;
  if(pause) return;

  if(hold) {
    holdtick = true;
    return;
  }

  resync = true;
  tick_second();
}

//below code provides bit-perfect emulation of invalid BCD values on the RTC-4513

void EpsonRTC::tick_second() {
  if(secondlo <= 8 || secondlo == 12) {
    secondlo++;
  } else {
    secondlo = 0;
    if(secondhi < 5) {
      secondhi++;
    } else {
      secondhi = 0;
      tick_minute();
    }
  }
}

void EpsonRTC::tick_minute() {
  if(minutelo <= 8 || minutelo == 12) {
    minutelo++;
  } else {
    minutelo = 0;
    if(minutehi < 5) {
      minutehi++;
    } else {
      minutehi = 0;
      tick_hour();
    }
  }
}

void EpsonRTC::tick_hour() {
  if(atime) {
    if(hourhi < 2) {
      if(hourlo <= 8 || hourlo == 12) {
        hourlo++;
      } else {
        hourlo = !(hourlo & 1);
        hourhi++;
      }
    } else {
      if(hourlo != 3 && !(hourlo & 4)) {
        if(hourlo <= 8 || hourlo >= 12) {
          hourlo++;
        } else {
          hourlo = !(hourlo & 1);
          hourhi++;
        }
      } else {
        hourlo = !(hourlo & 1);
        hourhi = 0;
        tick_day();
      }
    }
  } else {
    if(hourhi == 0) {
      if(hourlo <= 8 || hourlo == 12) {
        hourlo++;
      } else {
        hourlo = !(hourlo & 1);
        hourhi ^= 1;
      }
    } else {
      if(hourlo & 1) meridian ^= 1;
      if(hourlo < 2 || hourlo == 4 || hourlo == 5 || hourlo == 8 || hourlo == 12) {
        hourlo++;
      } else {
        hourlo = !(hourlo & 1);
        hourhi ^= 1;
      }
      if(meridian == 0 && !(hourlo & 1)) tick_day();
    }
  }
}

void EpsonRTC::tick_day() {
  if(calendar == false) return;
  weekday = (weekday + 1) + (weekday == 6);

  //January - December = 0x01 - 0x09; 0x10 - 0x12
  static const unsigned daysinmonth[32] = {
    30, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 30, 31, 30,
    31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
  };

  unsigned days = daysinmonth[monthhi << 4 | monthlo];
  if(days == 28) {
    //add one day for leap years
    if((yearhi & 1) == 0 && ((yearlo - 0) & 3) == 0) days++;
    if((yearhi & 1) == 1 && ((yearlo - 2) & 3) == 0) days++;
  }

  if(days == 28 && (dayhi == 3 || (dayhi == 2 && daylo >= 8))) {
    daylo = 1;
    dayhi = 0;
    return tick_month();
  }

  if(days == 29 && (dayhi == 3 || (dayhi == 2 && (daylo > 8 && daylo != 12)))) {
    daylo = 1;
    dayhi = 0;
    return tick_month();
  }

  if(days == 30 && (dayhi == 3 || (dayhi == 2 && (daylo == 10 || daylo == 14)))) {
    daylo = 1;
    dayhi = 0;
    return tick_month();
  }

  if(days == 31 && (dayhi == 3 && (daylo & 3))) {
    daylo = 1;
    dayhi = 0;
    return tick_month();
  }

  if(daylo <= 8 || daylo == 12) {
    daylo++;
  } else {
    daylo = !(daylo & 1);
    dayhi++;
  }
}

void EpsonRTC::tick_month() {
  if(monthhi == 0 || !(monthlo & 2)) {
    if(monthlo <= 8 || monthlo == 12) {
      monthlo++;
    } else {
      monthlo = !(monthlo & 1);
      monthhi ^= 1;
    }
  } else {
    monthlo = !(monthlo & 1);
    monthhi = 0;
    tick_year();
  }
}

void EpsonRTC::tick_year() {
  if(yearlo <= 8 || yearlo == 12) {
    yearlo++;
  } else {
    yearlo = !(yearlo & 1);
    if(yearhi <= 8 || yearhi == 12) {
      yearhi++;
    } else {
      yearhi = !(yearhi & 1);
    }
  }
}

#endif

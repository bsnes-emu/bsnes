const unsigned SharpRTC::daysinmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void SharpRTC::tick_second() {
  if(++second < 60) return;
  second = 0;
  tick_minute();
}

void SharpRTC::tick_minute() {
  if(++minute < 60) return;
  minute = 0;
  tick_hour();
}

void SharpRTC::tick_hour() {
  if(++hour < 24) return;
  hour = 0;
  tick_day();
}

void SharpRTC::tick_day() {
  unsigned days = daysinmonth[month % 12];
  if(day++ < days) return;
  day = 0;
  tick_month();
}

void SharpRTC::tick_month() {
  if(month++ < 12) return;
  month = 0;
  tick_year();
}

void SharpRTC::tick_year() {
  year++;
  year = (uint12)year;
}

//returns day of week for specified date
//eg 0 = Sunday, 1 = Monday, ... 6 = Saturday
//usage: weekday(2008, 1, 1) returns weekday of January 1st, 2008
unsigned SharpRTC::calculate_weekday(unsigned year, unsigned month, unsigned day) {
  unsigned y = 1900, m = 1;  //epoch is 1900-01-01
  unsigned sum = 0;          //number of days passed since epoch

  year = max(1900, year);
  month = max(1, min(12, month));
  day = max(1, min(31, day));

  while(y < year) {
    bool leapyear = false;
    if((y % 4) == 0) {
      leapyear = true;
      if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
    }
    sum += leapyear ? 366 : 365;
    y++;
  }

  while(m < month) {
    unsigned days = daysinmonth[m - 1];
    if(days == 28) {
      bool leapyear = false;
      if((y % 4) == 0) {
        leapyear = true;
        if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
      }
      if(leapyear) days++;
    }
    sum += days;
    m++;
  }

  sum += day - 1;
  return (sum + 1) % 7;  //1900-01-01 was a Monday
}

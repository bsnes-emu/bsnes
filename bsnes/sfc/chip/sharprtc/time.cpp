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

  //add one day for leap years
       if(year % 400 == 0) days++;
  else if(year % 100 == 0);
  else if(year %   4 == 0) days++;

  if(day++ < days) return;
  day = 1;
  tick_month();
}

void SharpRTC::tick_month() {
  if(month++ < 12) return;
  month = 1;
  tick_year();
}

void SharpRTC::tick_year() {
  year++;
  year = (uint12)year;
}

//returns day of week for specified date
//eg 0 = Sunday, 1 = Monday, ... 6 = Saturday
//usage: calculate_weekday(2008, 1, 1) returns weekday of January 1st, 2008
unsigned SharpRTC::calculate_weekday(unsigned year, unsigned month, unsigned day) {
  unsigned y = 1000, m = 1;  //SharpRTC epoch is 1000-01-01
  unsigned sum = 0;          //number of days passed since epoch

  year = max(1000, year);
  month = max(1, min(12, month));
  day = max(1, min(31, day));

  while(y < year) {
    bool leapyear = false;
    if(y % 4 == 0) {
      leapyear = true;
      if(y % 100 == 0 && y % 400 != 0) leapyear = false;
    }
    sum += 365 + leapyear;
    y++;
  }

  while(m < month) {
    unsigned days = daysinmonth[m - 1];
    bool leapyearmonth = false;
    if(days == 28) {
      if(y % 4 == 0) {
        leapyearmonth = true;
        if(y % 100 == 0 && y % 400 != 0) leapyearmonth = false;
      }
    }
    sum += days + leapyearmonth;
    m++;
  }

  sum += day - 1;
  return (sum + 3) % 7;  //1000-01-01 was a Wednesday
}

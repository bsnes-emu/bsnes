
Clock::~Clock() { }

Clock::Clock() {
  stoppedSec = 0;
  stoppedUSec = 0;
}

void Clock::reset() {
  status1  = 0;  year     = 0;  hour      = 0;
  status2  = 0;  month    = 1;  minute    = 0;
  adjust   = 0;  day      = 1;  second    = 0;
  userByte = 0;  weekday  = 0;
  
  for(int i = 0; i < 2; i++)
    for(int j = 0; j < 3; j++)
      alarm[i].setting[j] = 0;
}

void Clock::power() {
  secondsTickPending = hold = false;
  intr[0] = intr[1] = lag       = 0;
  lastClk = lastCs  = output    = false;
  command = buffer  = nbits     = dataOut = 0;
  
  // The clock is 32768 Hz but we need twice that, because serial
  // interrupts are edge sensitive.
  event.action = [&]() {
    tick();
    arm7.event.queue.add(2*33513982/(2*32768), event);
  };
  arm7.event.queue.add(2*33513982/(2*32768), event);
}

uint4 Clock::io(uint4 pins) {
  uint1 dataIn = pins>>0;
  uint1 clk    = pins>>1;
  uint1 cs     = pins>>2;
  
  if(cs != lastCs && !(lastCs = cs)) {
    command = nbits = 0;
    output = false;
    return 0xf;
  }
  // Wait for falling CLK edge with CS high..
  if(clk != lastClk && !(lastClk = clk)) {
    // Rotate data through the buffer
    dataOut = buffer;
    buffer  = dataIn<<7 | buffer>>1;
    
    if(++nbits == 0) {
      // Have we got a full byte to act on?
      
      // Accept a new command. They're MSB first, unlike data:
      if(!command && (buffer & 0x0f) == 6) {  //     0110
        command = buffer & 0x7f;              //  ccc
        output  = buffer & 0x80;              // D
        index   = 0;
        
        // Prevent the time changing while read
        if(command == 0x26 || command == 0x66)
          hold = true;
        
        if(output == false) return 0xf;
        else /*fall through and latch output byte for reading*/;
      }
      if(command == 0x06) field(status1, resetBit|hourMode);
      if(command == 0x46) field(status2, int1Mode|int2Mode|testMode);
      if(command == 0x26) dateTime();
      if(command == 0x66) dateTime();
      if(command == 0x16) alarmTime(0);
      if(command == 0x56) alarmTime(1);
      if(command == 0x36) field(adjust,   0xff);
      if(command == 0x76) field(userByte, 0xff);
    }
  }
  return output? dataOut | 0xe : 0xf;
}



void Clock::field(uint8& src, uint8 mask) {
  if(command == 0x06 && output == 0 && (buffer & resetBit)) {
    reset(); buffer &= ~resetBit;
  }
  if(output) buffer = src;
  else       src ^= (src ^ buffer) & mask;
}

void Clock::dateTime() {
  if(index >= 7) return;
  if(command == 0x66)  // start from time field
    index = max(4, index);

  bool fixHour = !(status1 & hourMode) && index == 4;
  
  if(output) {
    buffer = time[index++];
    if(fixHour && buffer >= 0x12)
      buffer += 0x40 - 0x12;  // Convert back to 12-hour + AM/PM
    return;
  }
  time[index] = buffer;  // Should correct these properly..
  if(fixHour && (buffer & 0x40))
    time[index] += 0x12 - 0x40;  // Convert from 12-hour mode
  index++;
}

void Clock::alarmTime(unsigned no) {
  if(index >= 3) return;
  if(output) buffer = alarm[no].setting[index++];
  else       alarm[no].setting[index++] = buffer;
}



bool Clock::incBCD(uint8& val, uint8 first, uint8 last) {
  val++;
  if((val & 0x0f) == 0x0a) val += 0x06;
  if((val & 0xf0) == 0xa0) val += 0x60;
  if(val > last)           val = first;
  return val == first;  // carry out
}

void Clock::tick() {
  bool previously = intr[0] | intr[1];
  
  if(++counter == 0)
    secondsTickPending = true;
  
  // Ticks can be held for up to 500 ms while reading the time.
  if(secondsTickPending && (hold == false || counter >= 2*16384)) {
    lag = counter;
    tickSecond();
  }
  
  switch(status2 & int2Mode /*0..1*/) {
  case 0:  intr[1] = 0; break;
  default: break;  // Alarm 2 - checked in tickSecond
  }
  
  switch(status2 & int1Mode /*0..15*/) {
  case 0:  intr[0] = 0; break;
  case 4:  break;  // Alarm 1 - checked in tickSecond
  
  case 1:  /**/;
  case 5:         // Selectable frequency - often used as seconds interrupt.
    intr[0] = 0;  // /IRQ output is the AND of the selected divisors.
    for(unsigned divisor = 0; divisor <= 4; divisor++)  // 1..16 Hz
      intr[0] |= (~counter >> 15-divisor) & (alarm[0].minute >> divisor) & 1;
    break;
    
  case 2:  /**/;  // Start-of-minute interrupts
  case 6:  intr[0] |= !second && counter-lag < 2*4u; break; // latched
  case 3:  intr[0]  = second < 0x30; break;                 // 30 sec duty
  case 7:  intr[0]  = counter-lag < lag+2*128u; break;      // 128 ticks duty
  
  default: intr[0]  = !(counter & 1<<0); break;             // 32KHz clock
  }
  
  if(intr[0]) status1 |= int1Flag;
  if(intr[1]) status1 |= int2Flag;
  
  bool interrupt = intr[0] | intr[1];
  
  if(arm7.sio.irq && interrupt && previously == false) {
    //arm7.raiseIrq(CPUCore::irqClock);
    arm7.interrupt.flags |= CPUCore::irqClock;
  }
}

void Clock::tickSecond() {
  secondsTickPending = false;
  
  if(  incBCD(second, 0, 0x59)
    && incBCD(minute, 0, 0x59)
    && incBCD(hour,   0, 0x23))
      tickDay();
  
  // Check alarms - not sure how this is supposed to work yet.
  // The datasheet implies this is checked each minute, something like:
  //   d:hh:mm  weekly           d:**:mm  hourly for one day a week?
  //   *:hh:mm  daily            d:**:**  every minute for one day a week?
  //   *:**:mm  hourly?          d:hh:**  every minute for one hour on one day?
  //   *:**:**  every minute?    *:hh:**  every minute for one hour a day
  if(second == 0x00) {
    bool enable[2] = {
      (status2 & int1Mode) == 4,  // Alarm 1
      (status2 & int2Mode) != 0,  // Alarm 2
    };
    // Triggered at start of minute; cleared by disabling alarm
    for(unsigned i = 0; i < 2; i++)
      if(enable[i]
        && (weekday == (alarm[i].weekday & 0x07) || alarm[i].weekday < 0x80)
        && (hour    == (alarm[i].hour    & 0x1f) || alarm[i].hour    < 0x80)
        && (minute  == (alarm[i].minute  & 0x3f) || alarm[i].minute  < 0x80))
          intr[i] = 1;
    
  }
}

void Clock::tickDay() {
  if(++weekday == 7) weekday = 0;
  
  unsigned y = 10*(year>>4) + (year & 15);
  unsigned daysIn[32] = {  0,  // (invalid)
    0x31,0x28+!(y % 4),        // Jan..Feb
    0x31,0x30,0x31,0x30,0x31,  // Mar..Jul
    0x31,0x30,   0,0,0,0,0,0,  // Aug..Sep (+ invalid months)
    0x31,0x30,0x31             // Oct..Dec
  };
     incBCD(day, 1, daysIn[month & 31])
  && incBCD(month, 1, 0x12)
  && incBCD(year, 0, 0x99);
}


void Clock::thaw(int64 curSec, int64 curUSec) {
  // Calculate time elapsed while the emulation wasn't running..
  // Then advance RTC to make up for it.
  int64 secs  = curSec - stoppedSec;
  int64 usecs = curUSec - stoppedUSec;
  if(usecs < 0) secs -= 1, usecs += 1000000;
  
  int32 ticks = counter + usecs * 32768/1000000;
  counter = ticks;
  if(ticks >= 32768) secs += 1;
  
  while(secs > 86400) { secs -= 86400; tickDay(); }
  while(secs > 0)     { secs -= 1;     tickSecond(); }
  
  stoppedSec = curSec;
  stoppedUSec = curUSec;
}

void Clock::freeze(int64 curSec, int64 curUSec) {
  stoppedSec = curSec;
  stoppedUSec = curUSec;
}

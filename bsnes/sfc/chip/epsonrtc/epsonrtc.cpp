#include <sfc/sfc.hpp>

#define EPSONRTC_CPP
namespace SuperFamicom {

#include "memory.cpp"
#include "time.cpp"
#include "serialization.cpp"
EpsonRTC epsonrtc;

void EpsonRTC::Enter() {
  epsonrtc.enter();
}

void EpsonRTC::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(wait) { if(--wait == 0) ready = true; }

    clocks++;
    if((clocks & ~0x03fff) == 0) duty();            //1/128th second (frequency / 128 - 1)
    if((clocks & ~0x07fff) == 0) irq(0);            //1/ 64th second (frequency /  64 - 1)
    if((clocks & ~0x3ffff) == 0) roundseconds = 0;  //1/  8th second (frequency /   8 - 1)
    if(clocks == 0) {  //1 second
      seconds++;
      irq(1);
      if(seconds %   60 == 0) irq(2);  //1 minute
      if(seconds % 1440 == 0) irq(3);  //1 hour
      if(seconds == 1440) seconds = 0;
      tick();
    }

    step(1);
    synchronize_cpu();
  }
}

void EpsonRTC::init() {
}

void EpsonRTC::load() {
  if(cartridge.has_epsonrtc()) interface->memory.append({ID::EpsonRTC, "rtc.ram"});

  secondlo = 0;
  secondhi = 0;
  batteryfailure = 1;

  minutelo = 0;
  minutehi = 0;
  resync = 0;

  hourlo = 0;
  hourhi = 0;
  meridian = 0;

  daylo = 0;
  dayhi = 0;
  dayram = 0;

  monthlo = 0;
  monthhi = 0;
  monthram = 0;

  yearlo = 0;
  yearhi = 0;

  weekday = 0;

  hold = 0;
  calendar = 0;
  irqflag = 0;
  roundseconds = 0;

  irqmask = 0;
  irqduty = 0;
  irqperiod = 0;

  pause = 0;
  stop = 0;
  atime = 0;
  test = 0;
}

void EpsonRTC::unload() {
}

void EpsonRTC::power() {
}

void EpsonRTC::reset() {
  create(EpsonRTC::Enter, 32768 * 64);

  clocks = 0;
  seconds = 0;

  chipselect = 0;
  state = State::Mode;
  offset = 0;
  wait = 0;
  ready = false;
  holdtick = false;
}

void EpsonRTC::sync() {
  time_t systime = time(0);
  tm *timeinfo = localtime(&systime);

  unsigned second = min(59, timeinfo->tm_sec);  //round down leap second
  secondlo = second % 10;
  secondhi = second / 10;

  unsigned minute = timeinfo->tm_min;
  minutelo = minute % 10;
  minutehi = minute / 10;

  unsigned hour = timeinfo->tm_hour;
  if(atime) {
    hourlo = hour % 10;
    hourhi = hour / 10;
  } else {
    meridian = hour >= 12;
    hour %= 12;
    if(hour == 0) {
      hourlo = 2;
      hourhi = 1;
    } else {
      hourlo = hour % 10;
      hourhi = hour / 10;
    }
  }

  unsigned day = timeinfo->tm_mday;
  daylo = day % 10;
  dayhi = day / 10;

  unsigned month = 1 + timeinfo->tm_mon;
  monthlo = month % 10;
  monthhi = month / 10;

  unsigned year = timeinfo->tm_year % 100;
  yearlo = year % 10;
  yearhi = year / 10;

  weekday = timeinfo->tm_wday;

  resync = true;  //alert RTC that time has changed
}

uint8 EpsonRTC::read(unsigned addr) {
  cpu.synchronize_coprocessors();
  addr &= 3;

  if(addr == 0) {
    return chipselect;
  }

  if(addr == 1) {
    if(chipselect != 1) return 0;
    if(ready == false) return 0;
    if(state == State::Write) return mdr;
    if(state != State::Read) return 0;
    ready = false;
    wait = 8;
    return rtc_read(offset++);
  }

  if(addr == 2) {
    return ready << 7;
  }
}

void EpsonRTC::write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessors();
  addr &= 3, data &= 15;

  if(addr == 0) {
    chipselect = data;
    if(chipselect != 1) rtc_reset();
    ready = true;
  }

  if(addr == 1) {
    if(chipselect != 1) return;
    if(ready == false) return;

    if(state == State::Mode) {
      if(data != 0x03 && data != 0x0c) return;
      state = State::Seek;
      ready = false;
      wait = 8;
      mdr = data;
    }

    else if(state == State::Seek) {
      if(mdr == 0x03) state = State::Write;
      if(mdr == 0x0c) state = State::Read;

      offset = data;
      ready = false;
      wait = 8;
      mdr = data;
    }

    else if(state == State::Write) {
      rtc_write(offset++, data);
      ready = false;
      wait = 8;
      mdr = data;
    }
  }
}

}

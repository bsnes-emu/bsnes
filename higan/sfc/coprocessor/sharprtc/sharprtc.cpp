#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "time.cpp"
#include "serialization.cpp"
SharpRTC sharprtc;

auto SharpRTC::Enter() -> void {
  sharprtc.enter();
}

auto SharpRTC::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    tick_second();

    step(1);
    synchronizeCPU();
  }
}

auto SharpRTC::init() -> void {
}

auto SharpRTC::load() -> void {
  return;

  second = 0;
  minute = 0;
  hour = 0;
  day = 0;
  month = 0;
  year = 0;
  weekday = 0;
}

auto SharpRTC::unload() -> void {
}

auto SharpRTC::power() -> void {
}

auto SharpRTC::reset() -> void {
  create(SharpRTC::Enter, 1);

  rtc_state = State::Read;
  rtc_index = -1;
}

auto SharpRTC::sync() -> void {
  time_t systime = time(0);
  tm* timeinfo = localtime(&systime);

  second = min(59, timeinfo->tm_sec);
  minute = timeinfo->tm_min;
  hour = timeinfo->tm_hour;
  day = timeinfo->tm_mday;
  month = 1 + timeinfo->tm_mon;
  year = 900 + timeinfo->tm_year;
  weekday = timeinfo->tm_wday;
}

auto SharpRTC::read(uint addr, uint8 data) -> uint8 {
  addr &= 1;

  if(addr == 0) {
    if(rtc_state != State::Read) return 0;

    if(rtc_index < 0) {
      rtc_index++;
      return 15;
    } else if(rtc_index > 12) {
      rtc_index = -1;
      return 15;
    } else {
      return rtc_read(rtc_index++);
    }
  }

  return data;
}

auto SharpRTC::write(uint addr, uint8 data) -> void {
  addr &= 1, data &= 15;

  if(addr == 1) {
    if(data == 0x0d) {
      rtc_state = State::Read;
      rtc_index = -1;
      return;
    }

    if(data == 0x0e) {
      rtc_state = State::Command;
      return;
    }

    if(data == 0x0f) return;  //unknown behavior

    if(rtc_state == State::Command) {
      if(data == 0) {
        rtc_state = State::Write;
        rtc_index = 0;
      } else if(data == 4) {
        rtc_state = State::Ready;
        rtc_index = -1;
        //reset time
        second = 0;
        minute = 0;
        hour = 0;
        day = 0;
        month = 0;
        year = 0;
        weekday = 0;
      } else {
        //unknown behavior
        rtc_state = State::Ready;
      }
      return;
    }

    if(rtc_state == State::Write) {
      if(rtc_index >= 0 && rtc_index < 12) {
        rtc_write(rtc_index++, data);
        if(rtc_index == 12) {
          //day of week is automatically calculated and written
          weekday = calculate_weekday(1000 + year, month, day);
        }
      }
      return;
    }
  }
}

}

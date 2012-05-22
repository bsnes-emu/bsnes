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
    if((clocks & ~255) == 0) duty();  //1/128th second
    if((clocks & ~511) == 0) irq(0);  //1/ 64th second
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
  batteryfailure = 1;
}

void EpsonRTC::unload() {
}

void EpsonRTC::power() {
}

void EpsonRTC::reset() {
  create(EpsonRTC::Enter, 32768);

  clocks = 0;
  seconds = 0;

  chipselect = 0;
  state = State::Mode;
  offset = 0;
  wait = 0;
  ready = false;
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
    wait = 1;
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
      wait = 1;
      mdr = data;
    }

    else if(state == State::Seek) {
      if(mdr == 0x03) state = State::Write;
      if(mdr == 0x0c) state = State::Read;

      offset = data;
      ready = false;
      wait = 1;
      mdr = data;
    }

    else if(state == State::Write) {
      rtc_write(offset++, data);
      ready = false;
      wait = 1;
      mdr = data;
    }
  }
}

}

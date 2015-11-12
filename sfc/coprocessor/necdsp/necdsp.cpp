#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
NECDSP necdsp;

void NECDSP::Enter() { necdsp.enter(); }

void NECDSP::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    exec();
    step(1);
    synchronize_cpu();
  }
}

uint8 NECDSP::read(unsigned addr) {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::sr_read();
  } else {
    return uPD96050::dr_read();
  }
}

void NECDSP::write(unsigned addr, uint8 data) {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::sr_write(data);
  } else {
    return uPD96050::dr_write(data);
  }
}

uint8 NECDSP::ram_read(unsigned addr) {
  cpu.synchronizeCoprocessors();
  return uPD96050::dp_read(addr);
}

void NECDSP::ram_write(unsigned addr, uint8 data) {
  cpu.synchronizeCoprocessors();
  return uPD96050::dp_write(addr, data);
}

void NECDSP::init() {
}

void NECDSP::load() {
}

void NECDSP::unload() {
}

void NECDSP::power() {
}

void NECDSP::reset() {
  create(NECDSP::Enter, frequency);
  uPD96050::power();
}

}

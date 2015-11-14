#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
NECDSP necdsp;

auto NECDSP::Enter() -> void { necdsp.enter(); }

auto NECDSP::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    exec();
    step(1);
    synchronize_cpu();
  }
}

auto NECDSP::read(uint addr) -> uint8 {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::sr_read();
  } else {
    return uPD96050::dr_read();
  }
}

auto NECDSP::write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::sr_write(data);
  } else {
    return uPD96050::dr_write(data);
  }
}

auto NECDSP::ram_read(uint addr) -> uint8 {
  cpu.synchronizeCoprocessors();
  return uPD96050::dp_read(addr);
}

auto NECDSP::ram_write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  return uPD96050::dp_write(addr, data);
}

auto NECDSP::init() -> void {
}

auto NECDSP::load() -> void {
}

auto NECDSP::unload() -> void {
}

auto NECDSP::power() -> void {
}

auto NECDSP::reset() -> void {
  create(NECDSP::Enter, frequency);
  uPD96050::power();
}

}

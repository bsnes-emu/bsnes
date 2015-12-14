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
    synchronizeCPU();
  }
}

auto NECDSP::read(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::readSR();
  } else {
    return uPD96050::readDR();
  }
}

auto NECDSP::write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  if(addr & Select) {
    return uPD96050::writeSR(data);
  } else {
    return uPD96050::writeDR(data);
  }
}

auto NECDSP::readRAM(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  return uPD96050::readDP(addr);
}

auto NECDSP::writeRAM(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  return uPD96050::writeDP(addr, data);
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

#include <ws/ws.hpp>

namespace WonderSwan {

CPU cpu;
#include "memory.cpp"

auto CPU::Enter() -> void {
  cpu.main();
}

auto CPU::main() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::CPU) {
      scheduler.sync = Scheduler::SynchronizeMode::All;
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    exec();
  }
}

auto CPU::step(uint clocks) -> void {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

auto CPU::read(uint32 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::write(uint32 addr, uint8 data) -> void {
  return bus.write(addr, data);
}

auto CPU::power() -> void {
  V30MZ::power();
  create(CPU::Enter, 3072000);
}

}

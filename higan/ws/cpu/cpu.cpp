#include <ws/ws.hpp>

namespace WonderSwan {

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "dma.cpp"

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

auto CPU::wait(uint clocks) -> void {
  step(clocks);
}

auto CPU::read(uint20 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::write(uint20 addr, uint8 data) -> void {
  return bus.write(addr, data);
}

auto CPU::in(uint16 port) -> uint8 {
  return iomap[port]->portRead(port);
}

auto CPU::out(uint16 port, uint8 data) -> void {
  return iomap[port]->portWrite(port, data);
}

auto CPU::power() -> void {
  V30MZ::power();
  create(CPU::Enter, 3072000);

  iomap[0x00a0] = this;

  if(WSC() || SC()) {
    for(uint p = 0x0040; p <= 0x0049; p++) iomap[p] = this;
    iomap[0x0062] = this;
  }

  s.dmaSource = 0x00000;
  s.dmaTarget = 0x0000;
  s.dmaLength = 0x0000;
  s.dmaEnable = false;
  s.dmaMode   = 0;
}

}

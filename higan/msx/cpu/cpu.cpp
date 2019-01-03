#include <msx/msx.hpp>

namespace MSX {

CPU cpu;
#include "memory.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(io.irqLine) irq(1, 0x0038, 0xff);
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
}

auto CPU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto CPU::power() -> void {
  Z80::bus = this;
  Z80::power();
  create(CPU::Enter, system.colorburst());

  r.pc = 0x0000;  //reset vector address

  ram.allocate(0x10000);

  io = {};
}

auto CPU::setIRQ(bool line) -> void {
  io.irqLine = line;
}

}

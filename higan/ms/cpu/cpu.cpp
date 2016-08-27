#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
}

auto CPU::wait() -> void {
  step(1);
}

auto CPU::read(uint16 addr) -> uint8 {
  step(1);
  return bus.read(addr);
}

auto CPU::write(uint16 addr, uint8 data) -> void {
  step(1);
  return bus.write(addr, data);
}

auto CPU::in(uint8 addr) -> uint8 {
  step(1);
  return bus.in(addr);
}

auto CPU::out(uint8 addr, uint8 data) -> void {
  step(1);
  return bus.out(addr, data);
}

auto CPU::power() -> void {
}

auto CPU::reset() -> void {
  create(CPU::Enter, system.colorburst());
}

}

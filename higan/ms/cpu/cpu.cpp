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

auto CPU::power() -> void {
  Processor::Z80::power(&MasterSystem::bus);
}

auto CPU::reset() -> void {
  Processor::Z80::reset();
  create(CPU::Enter, system.colorburst());
}

}

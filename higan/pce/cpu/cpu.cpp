#include <pce/pce.hpp>

namespace PCEngine {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdc);
  synchronize(psg);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::power() -> void {
  HuC6280::power();
  create(CPU::Enter, system.colorburst() * 6.0);
}

}

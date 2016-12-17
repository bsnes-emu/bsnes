#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  #if 1
  if(instructionsExecuted < 20)
  print(disassemble(r.pc), "\n");
  instructionsExecuted++;
  #endif

  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
}

auto CPU::power() -> void {
  Z80::bus = &MasterSystem::bus;
  Z80::power();
}

auto CPU::reset() -> void {
  Z80::reset();
  create(CPU::Enter, system.colorburst());

  instructionsExecuted = 0;
}

}

#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  #if 1
  static uint64 instructionsExecuted = 0;
  if(instructionsExecuted < 20)
  print(disassemble(r.pc), "\n");
  instructionsExecuted++;
  #endif

  //note: SMS1 extbus value is random; SMS2+ is pulled high ($ff)

  if(state.nmiLine) {
    state.nmiLine = 0;  //edge-sensitive
    irq(0, 0x0066, 0xff);
  }

  if(state.intLine) {
    //level-sensitive
    irq(1, 0x0038, 0xff);
  }

  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
}

auto CPU::setNMI(bool value) -> void {
  state.nmiLine = value;
}

auto CPU::setINT(bool value) -> void {
  state.intLine = value;
}

auto CPU::power() -> void {
  Z80::bus = &MasterSystem::bus;
  Z80::power();
}

auto CPU::reset() -> void {
  Z80::reset();
  create(CPU::Enter, system.colorburst());

  memory::fill(&state, sizeof(State));
}

}

#include <pce/pce.hpp>

namespace PCEngine {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  #if 1
  static uint counter = 0;
  if(++counter < 10) {
    print(disassemble(r.pc), "\n");
  }
  #endif

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

auto CPU::read(uint16 addr) -> uint8 {
  step(3);
  return 0xea;
}

auto CPU::write(uint16 addr, uint8 data) -> void {
  step(3);
}

auto CPU::lastCycle() -> void {
}

auto CPU::disassembleRead(uint16 pc) -> uint8 {
  return 0xea;
}

}

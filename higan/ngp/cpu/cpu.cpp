#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

CPU cpu;
#include "memory.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
static uint ctr=0;
if(++ctr < 200) print(disassemble(), "\n");
else return step(1);
  instruction();
  step(1);
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vpu);
  synchronize(apu);
  synchronize(psg);
}

auto CPU::power() -> void {
  TLCS900H::power();
  create(CPU::Enter, system.frequency());
  ram.allocate(0x3000);
  r.pc.l.l0 = 0xff1800;
}

}

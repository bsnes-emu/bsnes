#include <md/md.hpp>

namespace MegaDrive {

APU apu;

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  if(!state.enabled) return step(1);

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

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto APU::setNMI(bool value) -> void {
  state.nmiLine = value;
}

auto APU::setINT(bool value) -> void {
  state.intLine = value;
}

auto APU::enable(bool value) -> void {
  if(state.enabled && !value) power();
  state.enabled = value;
}

auto APU::power() -> void {
  Z80::bus = &busAPU;
  Z80::power();
  create(APU::Enter, system.colorburst());
  memory::fill(&state, sizeof(State));

  r.pc = 0x0000;
  r.im = 0;
  r.iff1 = 0;
  r.iff2 = 0;
  r.ir = {};
}

}

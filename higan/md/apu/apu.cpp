#include <md/md.hpp>

namespace MegaDrive {

APU apu;
#include "serialization.cpp"

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  if(!state.enabled) {
    return step(1);
  }

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
  //68K cannot disable the Z80 without bus access
  if(!bus->granted() && !value) return;
  if(state.enabled && !value) reset();
  state.enabled = value;
}

auto APU::power() -> void {
  Z80::bus = &busAPU;
  Z80::power();
  create(APU::Enter, system.colorburst());
  memory::fill(&state, sizeof(State));
}

auto APU::reset() -> void {
  create(APU::Enter, system.colorburst());
  memory::fill(&r, sizeof(Registers));
  memory::fill(&state, sizeof(State));
}

}

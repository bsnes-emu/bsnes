#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

APU apu;
#include "serialization.cpp"

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  step(1);
}

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto APU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto APU::power() -> void {
  Z80::bus = this;
  Z80::power();
  bus->grant(false);
  create(APU::Enter, system.frequency() / 2.0);
  ram.allocate(0x1000);
}

}

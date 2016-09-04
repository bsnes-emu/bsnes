#include <md/md.hpp>

namespace MegaDrive {

APU apu;

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

auto APU::power() -> void {
  Z80::bus = &busAPU;
  Z80::power();
}

auto APU::reset() -> void {
  Z80::reset();
  create(APU::Enter, system.colorburst());
}

}

#include <md/md.hpp>

namespace MegaDrive {

APU apu;

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  step(system.colorburst());
}

auto APU::step(uint clocks) -> void {
}

auto APU::power() -> void {
}

auto APU::reset() -> void {
  create(APU::Enter, system.colorburst());
}

}

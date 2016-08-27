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

auto APU::wait() -> void {
  step(1);
}

auto APU::read(uint16 addr) -> uint8 {
  step(1);
  return 0x00;
}

auto APU::write(uint16 addr, uint8 data) -> void {
  step(1);
}

auto APU::in(uint8 addr) -> uint8 {
  return 0x00;
}

auto APU::out(uint8 addr, uint8 data) -> void {
}

auto APU::power() -> void {
}

auto APU::reset() -> void {
  create(APU::Enter, system.colorburst());
}

}

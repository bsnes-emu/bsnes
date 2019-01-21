#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

VPU vpu;
#include "serialization.cpp"

auto VPU::Enter() -> void {
  while(true) scheduler.synchronize(), vpu.main();
}

auto VPU::main() -> void {
  step(system.frequency() / 60.0);
  scheduler.exit(Scheduler::Event::Frame);
}

auto VPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VPU::refresh() -> void {
  Emulator::video.refresh(buffer, 160 * sizeof(uint32), 160, 152);
}

auto VPU::power() -> void {
  create(VPU::Enter, system.frequency());
  ram.allocate(0x4000);
}

}

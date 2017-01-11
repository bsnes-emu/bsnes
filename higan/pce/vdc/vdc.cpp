#include <pce/pce.hpp>

namespace PCEngine {

VDC vdc;

auto VDC::Enter() -> void {
  while(true) scheduler.synchronize(), vdc.main();
}

auto VDC::main() -> void {
  step(system.colorburst() * 6.0 / 60.0);
  scheduler.exit(Scheduler::Event::Frame);
}

auto VDC::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VDC::refresh() -> void {
  Emulator::video.refresh(buffer, 512 * sizeof(uint32), 512, 484);
}

auto VDC::power() -> void {
  create(VDC::Enter, system.colorburst() * 6.0);
  for(auto& pixel : buffer) pixel = 0;
}

}

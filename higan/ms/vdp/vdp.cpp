#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  for(uint y : range(262)) {
    for(uint x : range(342)) {
      step(1);
    }
    if(y == 240) scheduler.exit(Scheduler::Event::Frame);
  }
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
}

auto VDP::in(uint8 addr) -> uint8 {
  switch(addr) {
  }

  return 0xb0;
}

auto VDP::out(uint8 addr, uint8 data) -> void {
  switch(addr) {
  }
}

auto VDP::power() -> void {
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst());
}

}

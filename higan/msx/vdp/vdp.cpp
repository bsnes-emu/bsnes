#include <msx/msx.hpp>

//228 clocks/scanline

namespace MSX {

VDP vdp;
#include "serialization.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  step(1);
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 192);
}

auto VDP::power() -> void {
  create(VDP::Enter, system.colorburst());
}

}

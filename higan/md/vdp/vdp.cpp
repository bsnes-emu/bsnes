#include <md/md.hpp>

//256-width = colorburst * 15 / 10
//320-width = colorburst * 15 /  8

namespace MegaDrive {

VDP vdp;

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  step(system.colorburst() * 15.0 / 10.0);
}

auto VDP::step(uint clocks) -> void {
}

auto VDP::power() -> void {
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 10.0);
}

}

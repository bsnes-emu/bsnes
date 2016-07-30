#include <md/md.hpp>

namespace MegaDrive {

YM2612 ym2612;

auto YM2612::Enter() -> void {
  while(true) scheduler.synchronize(), ym2612.main();
}

auto YM2612::main() -> void {
  step(system.colorburst() * 15.0 / 7.0);
}

auto YM2612::step(uint clocks) -> void {
}

auto YM2612::power() -> void {
}

auto YM2612::reset() -> void {
  create(YM2612::Enter, system.colorburst() * 15.0 / 7.0);
}

}
